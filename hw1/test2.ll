; ModuleID = 'test2.ll'
source_filename = "test2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
entry:
  %A = alloca [40 x i32], align 16
  %C = alloca [40 x i32], align 16
  %D = alloca [40 x i32], align 16
  br label %for.body

for.body:                                         ; preds = %entry, %for.inc
  %i.01 = phi i32 [ 2, %entry ], [ %inc, %for.inc ]
  %idxprom = sext i32 %i.01 to i64
  %arrayidx = getelementptr inbounds [40 x i32], ptr %C, i64 0, i64 %idxprom
  %0 = load i32, ptr %arrayidx, align 4
  %idxprom1 = sext i32 %i.01 to i64
  %arrayidx2 = getelementptr inbounds [40 x i32], ptr %A, i64 0, i64 %idxprom1
  store i32 %0, ptr %arrayidx2, align 4
  %mul = mul nsw i32 3, %i.01
  %sub = sub nsw i32 %mul, 4
  %idxprom3 = sext i32 %sub to i64
  %arrayidx4 = getelementptr inbounds [40 x i32], ptr %A, i64 0, i64 %idxprom3
  %1 = load i32, ptr %arrayidx4, align 4
  %idxprom5 = sext i32 %i.01 to i64
  %arrayidx6 = getelementptr inbounds [40 x i32], ptr %D, i64 0, i64 %idxprom5
  store i32 %1, ptr %arrayidx6, align 4
  %mul7 = mul nsw i32 2, %i.01
  %idxprom8 = sext i32 %mul7 to i64
  %arrayidx9 = getelementptr inbounds [40 x i32], ptr %C, i64 0, i64 %idxprom8
  %2 = load i32, ptr %arrayidx9, align 4
  %sub10 = sub nsw i32 %i.01, 1
  %idxprom11 = sext i32 %sub10 to i64
  %arrayidx12 = getelementptr inbounds [40 x i32], ptr %D, i64 0, i64 %idxprom11
  store i32 %2, ptr %arrayidx12, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.01, 1
  %cmp = icmp slt i32 %inc, 20
  br i1 %cmp, label %for.body, label %for.end, !llvm.loop !6

for.end:                                          ; preds = %for.inc
  ret i32 0
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
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
