// Check that we run dsymutil properly with multiple -arch options.
//
// RUN: %clang -ccc-host-triple x86_64-apple-darwin10 -ccc-print-phases \
// RUN:   -arch i386 -arch x86_64 %s -g 2> %t
// RUN: FileCheck -check-prefix=CHECK-MULTIARCH-ACTIONS < %t %s
//
// CHECK-MULTIARCH-ACTIONS: 0: input, "{{.*}}darwin-dsymutil.c", c
// CHECK-MULTIARCH-ACTIONS: 1: preprocessor, {0}, cpp-output
// CHECK-MULTIARCH-ACTIONS: 2: compiler, {1}, assembler
// CHECK-MULTIARCH-ACTIONS: 3: assembler, {2}, object
// CHECK-MULTIARCH-ACTIONS: 4: linker, {3}, image
// CHECK-MULTIARCH-ACTIONS: 5: bind-arch, "i386", {4}, image
// CHECK-MULTIARCH-ACTIONS: 6: bind-arch, "x86_64", {4}, image
// CHECK-MULTIARCH-ACTIONS: 7: lipo, {5, 6}, image
// CHECK-MULTIARCH-ACTIONS: 8: dsymutil, {7}, dSYM
//
// RUN: %clang -ccc-host-triple x86_64-apple-darwin10 -ccc-print-bindings \
// RUN:   -arch i386 -arch x86_64 %s -g 2> %t
// RUN: FileCheck -check-prefix=CHECK-MULTIARCH-BINDINGS < %t %s
//
// CHECK-MULTIARCH-BINDINGS: "x86_64-apple-darwin10" - "darwin::Lipo", inputs: [{{.*}}, {{.*}}], output: "a.out"
// CHECK-MULTIARCH-BINDINGS: # "x86_64-apple-darwin10" - "darwin::Dsymutil", inputs: ["a.out"], output: "a.dSYM"

// Check output name derivation.
//
// RUN: %clang -ccc-host-triple x86_64-apple-darwin10 -ccc-print-bindings \
// RUN:   -o foo %s -g 2> %t
// RUN: FileCheck -check-prefix=CHECK-OUTPUT-NAME < %t %s
//
// CHECK-OUTPUT-NAME: "x86_64-apple-darwin10" - "darwin::Link", inputs: [{{.*}}], output: "foo"
// CHECK-OUTPUT-NAME: "x86_64-apple-darwin10" - "darwin::Dsymutil", inputs: ["foo"], output: "foo.dSYM"