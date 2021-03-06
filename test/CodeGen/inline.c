// RUN: echo "GNU89 tests:"
// RUN: %clang %s -O1 -emit-llvm -S -o %t -std=gnu89
// RUN: grep "define available_externally i32 @ei()" %t
// RUN: grep "define i32 @foo()" %t
// RUN: grep "define i32 @bar()" %t
// RUN: grep "define void @unreferenced1()" %t
// RUN: not grep unreferenced2 %t
// RUN: grep "define void @gnu_inline()" %t
// RUN: grep "define available_externally void @gnu_ei_inline()" %t
// RUN: grep "define i32 @test1" %t
// RUN: grep "define i32 @test2" %t
// RUN: grep "define void @test3()" %t
// RUN: grep "define available_externally i32 @test4" %t
// RUN: grep "define available_externally i32 @test5" %t

// RUN: echo "\nC99 tests:"
// RUN: %clang %s -O1 -emit-llvm -S -o %t -std=c99
// RUN: grep "define i32 @ei()" %t
// RUN: grep "define available_externally i32 @foo()" %t
// RUN: grep "define i32 @bar()" %t
// RUN: not grep unreferenced1 %t
// RUN: grep "define void @unreferenced2()" %t
// RUN: grep "define void @gnu_inline()" %t
// RUN: grep "define available_externally void @gnu_ei_inline()" %t
// RUN: grep "define i32 @test1" %t
// RUN: grep "define i32 @test2" %t
// RUN: grep "define void @test3" %t
// RUN: grep "define available_externally i32 @test4" %t
// RUN: grep "define available_externally i32 @test5" %t

// RUN: echo "\nC++ tests:"
// RUN: %clang -x c++ %s -O1 -emit-llvm -S -o %t -std=c++98
// RUN: grep "define linkonce_odr i32 @_Z2eiv()" %t
// RUN: grep "define linkonce_odr i32 @_Z3foov()" %t
// RUN: grep "define i32 @_Z3barv()" %t
// RUN: not grep unreferenced %t
// RUN: grep "define void @_Z10gnu_inlinev()" %t
// RUN: grep "define available_externally void @_Z13gnu_ei_inlinev()" %t

extern __inline int ei() { return 123; }

__inline int foo() {
  return ei();
}

int bar() { return foo(); }


__inline void unreferenced1() {}
extern __inline void unreferenced2() {}

__inline __attribute((__gnu_inline__)) void gnu_inline() {}

// PR3988
extern __inline __attribute__((gnu_inline)) void gnu_ei_inline() {}
void (*P)() = gnu_ei_inline;

// <rdar://problem/6818429>
int test1();
__inline int test1() { return 4; }
__inline int test2() { return 5; }
__inline int test2();
int test2();

void test_test1() { test1(); }
void test_test2() { test2(); }

// PR3989
extern __inline void test3() __attribute__((gnu_inline));
__inline void __attribute__((gnu_inline)) test3() {}

extern int test4(void);
extern __inline __attribute__ ((__gnu_inline__)) int test4(void)
{
  return 0;
}

void test_test4() { test4(); }

extern __inline int test5(void)  __attribute__ ((__gnu_inline__));
extern __inline int __attribute__ ((__gnu_inline__)) test5(void)
{
  return 0;
}

void test_test5() { test5(); }
