; ModuleID = 'my_module'
source_filename = "my_module"

@format_str = private constant [3 x i8] c"%f\00"

define float @arthimatic(i32 %count, i32 %SwitchArg, ...) {
entry:
  %va_list = alloca ptr, align 8
  call void @llvm.va_start(ptr %va_list)
  %result_ptr = alloca float, align 4
  store float 0.000000e+00, ptr %result_ptr, align 4
  %count_ptr = alloca i32, align 4
  store i32 %count, ptr %count_ptr, align 4
  %Switch_ptr = alloca ptr, align 8
  store i32 %SwitchArg, ptr %Switch_ptr, align 4
  br label %loop

loop:                                             ; preds = %end1, %entry
  %current_count = load i32, ptr %count_ptr, align 4
  %next_arg = va_arg ptr %va_list, double
  %truncated = fptrunc double %next_arg to float
  %current_result = load float, ptr %result_ptr, align 4
  switch i32 %SwitchArg, label %end [
    i32 0, label %Add
    i32 1, label %Sub
    i32 2, label %Mul
    i32 3, label %Div
  ]

end:                                              ; preds = %end1, %loop
  call void @llvm.va_end(ptr %va_list)
  %final_sum = load float, ptr %result_ptr, align 4
  ret float %final_sum

Add:                                              ; preds = %loop
  %new_result = fadd float %current_result, %truncated
  store float %new_result, ptr %result_ptr, align 4
  br label %end1

Sub:                                              ; preds = %loop
  %new_result2 = fsub float %current_result, %truncated
  store float %new_result2, ptr %result_ptr, align 4
  br label %end1

Mul:                                              ; preds = %loop
  %cond_mul = fcmp oeq float %current_result, 0.000000e+00
  br i1 %cond_mul, label %ResultOne, label %SkipResultOne

Div:                                              ; preds = %loop
  %cond_div = fcmp oeq float %current_result, 0.000000e+00
  br i1 %cond_div, label %DivIntialize, label %SkipDivIntialize

ResultOne:                                        ; preds = %Mul
  store float 1.000000e+00, ptr %result_ptr, align 4
  br label %SkipResultOne

SkipResultOne:                                    ; preds = %ResultOne, %Mul
  %updated_current_result = load float, ptr %result_ptr, align 4
  %new_result3 = fmul float %updated_current_result, %truncated
  store float %new_result3, ptr %result_ptr, align 4
  br label %end1

DivIntialize:                                     ; preds = %Div
  store float %truncated, ptr %result_ptr, align 4
  br label %end1

SkipDivIntialize:                                 ; preds = %Div
  %new_result4 = fdiv float %current_result, %truncated
  store float %new_result4, ptr %result_ptr, align 4
  br label %end1

end1:                                             ; preds = %SkipDivIntialize, %DivIntialize, %SkipResultOne, %Sub, %Add
  %0 = sub i32 %current_count, 1
  store i32 %0, ptr %count_ptr, align 4
  %1 = icmp eq i32 %0, 0
  br i1 %1, label %end, label %loop
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start(ptr) #0

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end(ptr) #0

define i32 @main() {
entry:
  %result = call float (i32, i32, ...) @arthimatic(i32 3, i32 3, double 1.000000e+00, double 2.000000e+00, double 7.000000e+00)
  %result_double = fpext float %result to double
  %0 = call i32 (ptr, double, ...) @printf(ptr @format_str, double %result_double)
  ret i32 %0
}

declare i32 @printf(ptr, double, ...)

attributes #0 = { nocallback nofree nosync nounwind willreturn }
