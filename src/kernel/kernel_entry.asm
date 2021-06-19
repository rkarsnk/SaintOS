;main.cpp
extern kernel_main_stack
extern KernelMainNewStack

global KernelMain
KernelMain:
    ;スタック領域の移動
    mov rsp, kernel_main_stack + 1024 * 1024
    call KernelMainNewStack
.fin:
    hlt
    jmp .fin