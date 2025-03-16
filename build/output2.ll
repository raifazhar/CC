; ModuleID = 'HelloModule'
source_filename = "HelloModule"

@hello_world = private constant [15 x i8] c"Hello, World!\0A\00"

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %0 = call i32 (ptr, ...) @printf(ptr @hello_world)
  ret i32 0
}
