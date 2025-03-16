@format_str = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define float @sum(i32 %count, ...) {
entry:
  %va_list = alloca ptr, align 8
  call void @llvm.va_start(ptr %va_list)
  %sum_ptr = alloca float, align 4
  store float 0.000000e+00, ptr %sum_ptr, align 4
  %count_ptr = alloca i32, align 4
  store i32 %count, ptr %count_ptr, align 4
  br label %loop

loop:                                             ; preds = %loop, %entry
  %current_count = load i32, ptr %count_ptr, align 4
  %next_arg = va_arg ptr %va_list, double
  %truncated = fptrunc double %next_arg to float
  %current_sum = load float, ptr %sum_ptr, align 4
  %new_sum = fadd float %current_sum, %truncated
  store float %new_sum, ptr %sum_ptr, align 4
  %0 = sub i32 %current_count, 1
  store i32 %0, ptr %count_ptr, align 4
  %1 = icmp eq i32 %0, 0
  br i1 %1, label %end, label %loop

end:                                              ; preds = %loop
  call void @llvm.va_end(ptr %va_list)
  %final_sum = load float, ptr %sum_ptr, align 4
  ret float %final_sum
}

define i32 @main() {
entry:
  %result = call float (i32, ...) @sum(i32 3, double 3.0, double 2.0,double 7.0)
  %result_double = fpext float %result to double
  %format_str_ptr = getelementptr inbounds [4 x i8], ptr @format_str, i32 0, i32 0
  call i32 (ptr, ...) @printf(ptr %format_str_ptr, double %result_double)
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.va_start(ptr) #1
declare void @llvm.va_end(ptr) #1
attributes #1 = { nounwind }
