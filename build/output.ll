; ModuleID = 'my_module'
source_filename = "my_module"

@format_str = private constant [3 x i8] c"%f\00"
@hello_world = private constant [15 x i8] c"Hello, World!\0A\00"

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
  switch i32 %SwitchArg, label %AddOrSubIntialize [
    i32 2, label %MulIntialize
    i32 3, label %DivIntialize
  ]

loop:                                             ; preds = %end1, %AddOrSubIntialize
  %current_count2 = load i32, ptr %count_ptr, align 4
  %next_arg3 = va_arg ptr %va_list, double
  %truncated4 = fptrunc double %next_arg3 to float
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
  %new_result = fadd float %current_result, %truncated4
  store float %new_result, ptr %result_ptr, align 4
  br label %end1

Sub:                                              ; preds = %loop
  %new_result5 = fsub float %current_result, %truncated4
  store float %new_result5, ptr %result_ptr, align 4
  br label %end1

Mul:                                              ; preds = %loop
  %new_result6 = fmul float %current_result, %truncated4
  store float %new_result6, ptr %result_ptr, align 4
  br label %end1

Div:                                              ; preds = %loop
  %new_result7 = fdiv float %current_result, %truncated4
  store float %new_result7, ptr %result_ptr, align 4
  br label %end1

MulIntialize:                                     ; preds = %entry
  store float 1.000000e+00, ptr %result_ptr, align 4
  br label %AddOrSubIntialize

DivIntialize:                                     ; preds = %entry
  %next_arg = va_arg ptr %va_list, double
  %truncated = fptrunc double %next_arg to float
  %current_count = load i32, ptr %count_ptr, align 4
  %0 = sub i32 %current_count, 1
  store i32 %0, ptr %count_ptr, align 4
  store float %truncated, ptr %result_ptr, align 4
  br label %AddOrSubIntialize

AddOrSubIntialize:                                ; preds = %DivIntialize, %MulIntialize, %entry
  br label %loop

end1:                                             ; preds = %Div, %Mul, %Sub, %Add
  %1 = sub i32 %current_count2, 1
  store i32 %1, ptr %count_ptr, align 4
  %2 = icmp eq i32 %1, 0
  br i1 %2, label %end, label %loop
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start(ptr) #0

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end(ptr) #0

define i32 @main() {
entry:
  %0 = call i32 (ptr, ...) @printf(ptr @hello_world)
  %result = call float (i32, i32, ...) @arthimatic(i32 4, i32 0, double 3.000000e+00, double 4.000000e+00, double 7.000000e+00, double 1.000000e+00)
  %result_double = fpext float %result to double
  %1 = call i32 (ptr, ...) @printf(ptr @format_str, double %result_double)
  ret i32 %1
}

declare i32 @printf(ptr, ...)

attributes #0 = { nocallback nofree nosync nounwind willreturn }
