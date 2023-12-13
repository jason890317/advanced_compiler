; ModuleID = 'test4.c'
source_filename = "test4.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @icpp4() #0 {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  %e = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %f = alloca i32, align 4
  %g = alloca i32, align 4
  %h = alloca i32, align 4
  %i = alloca i32, align 4
  %p = alloca ptr, align 8
  %pp = alloca ptr, align 8
  %0 = load i32, ptr %b, align 4
  %1 = load i32, ptr %c, align 4
  %add = add nsw i32 %0, %1
  store i32 %add, ptr %a, align 4
  store ptr %y, ptr %p, align 8
  %2 = load i32, ptr %b, align 4
  %3 = load i32, ptr %c, align 4
  %add1 = add nsw i32 %2, %3
  store i32 %add1, ptr %d, align 4
  %4 = load i32, ptr %a, align 4
  %5 = load i32, ptr %d, align 4
  %add2 = add nsw i32 %4, %5
  %6 = load i32, ptr %x, align 4
  %add3 = add nsw i32 %add2, %6
  %7 = load i32, ptr %y, align 4
  %add4 = add nsw i32 %add3, %7
  store i32 %add4, ptr %f, align 4
  %8 = load i32, ptr %a, align 4
  %9 = load i32, ptr %d, align 4
  %add5 = add nsw i32 %8, %9
  %10 = load i32, ptr %x, align 4
  %add6 = add nsw i32 %add5, %10
  %11 = load i32, ptr %y, align 4
  %add7 = add nsw i32 %add6, %11
  store i32 %add7, ptr %g, align 4
  %12 = load i32, ptr %i, align 4
  %add8 = add nsw i32 %12, 2
  %13 = load ptr, ptr %p, align 8
  store i32 %add8, ptr %13, align 4
  %14 = load i32, ptr %a, align 4
  %15 = load i32, ptr %d, align 4
  %add9 = add nsw i32 %14, %15
  %16 = load i32, ptr %x, align 4
  %add10 = add nsw i32 %add9, %16
  %17 = load i32, ptr %y, align 4
  %add11 = add nsw i32 %add10, %17
  store i32 %add11, ptr %h, align 4
  %18 = load i32, ptr %y, align 4
  %add12 = add nsw i32 %18, 1
  store i32 %add12, ptr %f, align 4
  %19 = load i32, ptr %b, align 4
  %20 = load ptr, ptr %pp, align 8
  %21 = load ptr, ptr %20, align 8
  store i32 %19, ptr %21, align 4
  %22 = load i32, ptr %c, align 4
  store i32 %22, ptr %b, align 4
  %23 = load ptr, ptr %p, align 8
  %24 = load ptr, ptr %pp, align 8
  store ptr %23, ptr %24, align 8
  %25 = load ptr, ptr %pp, align 8
  store ptr %a, ptr %25, align 8
  store ptr %p, ptr %pp, align 8
  %26 = load ptr, ptr %p, align 8
  %27 = load i32, ptr %26, align 4
  %28 = load ptr, ptr %pp, align 8
  %29 = load ptr, ptr %28, align 8
  store i32 %27, ptr %29, align 4
  %30 = load ptr, ptr %pp, align 8
  %31 = load ptr, ptr %30, align 8
  store ptr %31, ptr %p, align 8
  %32 = load ptr, ptr %pp, align 8
  %33 = load ptr, ptr %32, align 8
  %34 = load i32, ptr %33, align 4
  store i32 %34, ptr %a, align 4
  ret void
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 17.0.2"}
