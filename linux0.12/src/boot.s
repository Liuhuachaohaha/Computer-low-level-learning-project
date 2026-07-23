;
; 最简单的 Bootloader
; 功能：在屏幕上打印 "Hello, OS!" 然后死机
;

org 0x7C00          ; BIOS 会把我们加载到 0x7C00 处

start:
    ; 设置段寄存器
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00  ; 栈从 0x7C00 向下生长

    ; 清屏 (用 BIOS 中断 INT 0x10, AH=0x06)
    mov ax, 0x0600  ; AH=06 滚动窗口, AL=0 全屏
    mov bh, 0x07    ; 黑底白字
    xor cx, cx      ; 左上角 (0,0)
    mov dx, 0x184F  ; 右下角 (24,79)
    int 0x10

    ; 设置光标位置到 (0,0)
    mov ah, 0x02
    xor bh, bh      ; 第 0 页
    xor dx, dx      ; dh=行, dl=列
    int 0x10

    ; 打印字符串 "Hello, OS!"
    mov si, hello_msg
    call print_string

    ; 打印提示信息
    mov si, prompt_msg
    call print_string

    ; 键盘输入循环：读一个键，回显到屏幕
key_loop:
    call read_key        ; 等待按键
    cmp al, 0x0D         ; 如果是回车
    je key_newline
    call put_char        ; 回显字符
    jmp key_loop

key_newline:
    mov al, 0x0D
    call put_char
    mov al, 0x0A
    call put_char
    jmp key_loop

; ============ 读键盘函数 ============
; 返回: AL = ASCII 码, AH = 扫描码
read_key:
    mov ah, 0x00        ; INT 0x16, AH=00: 等待按键
    int 0x16
    ret

; ============ 打印单个字符函数 ============
; AL = 要打印的字符
put_char:
    push ax
    push bx
    mov ah, 0x0E
    xor bh, bh
    mov bl, 0x07
    int 0x10
    pop bx
    pop ax
    ret

; ============ 打印字符串函数 ============
; DS:SI 指向以 0 结尾的字符串
print_string:
    push ax
    push bx
.loop:
    lodsb           ; 从 [si] 读一个字节到 al, si++
    or al, al       ; 判断是不是 0（字符串结束）
    jz .done
    mov ah, 0x0E    ; BIOS 打印字符功能
    mov bh, 0x00    ; 第 0 页
    mov bl, 0x07    ; 黑底白字
    int 0x10
    jmp .loop
.done:
    pop bx
    pop ax
    ret

; ============ 数据 ============
hello_msg  db 'Hello, OS!', 0xD, 0xA, 0
prompt_msg db 'Type something: ', 0

; ============ 填充到 512 字节，最后加启动标志 ============
times 510 - ($ - $$) db 0   ; 填充 0 到 510 字节
dw 0xAA55                   ; 启动标志 (小端序，所以是 0xAA55)