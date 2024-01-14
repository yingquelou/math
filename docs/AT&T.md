# 下面是使用 GCC 支持的 AT&T 格式汇编写的"hello world"代码

```asm
.section .data
hello_str:
    .string "Hello, world!\n"

.section .text
.globl _start
_start:
    movl $4, %eax      # 'write' system call number
    movl $1, %ebx      # file descriptor (stdout)
    movl $hello_str, %ecx  # string to write
    movl $16, %edx     # length of string
    int $0x80          # invoke the kernel

    # exit(0)
    xorl %eax, %eax
    movl $1, %eax      # 'exit' system call number
    xorl %ebx, %ebx    # exit status code
    int $0x80          # invoke the kernel
```

这个程序将字符串“Hello, world!”打印到标准输出，并以 0 为退出状态码结束。

现在来详细解释一下这个程序的每一行代码：

- `.section .data`: 这条指令将当前段设置为数据段。在数据段中，我们将定义包含要在程序中使用的全局变量和常量。
- `hello_str: .string "Hello, world!\n"`: 这个指令定义了一个名为`hello_str`的标签，并将它指向一个以 NULL 结尾的字符串"LHello, World!"，并在结尾处包括一个换行符"\n"。这个字符串将被用于后面的输出操作。
- `.section .text`: 这个指令将当前段设置为代码段。在代码段中，我们将定义包含程序的指令和操作。
- `.globl _start`: 这个指令将`_start`标签作为程序的入口点进行导出。这说明操作系统将以此作为程序的起始位置。
- `movl $4, %eax`: 把数字 4 加载到 EAX 寄存器中，该寄存器用于告诉操作系统我们要进行的"write"系统调用。
- `movl $1, %ebx`: 将数字 1 加载到 EBX 寄存器中，该寄存器表示打印输出的目标设备，1 表示标准输出(stdout)。
- `movl $hello_str, %ecx`: 将`hello_str`标签的地址拷贝到 ECX 寄存器中，该寄存器用于告诉操作系统我们要输出的字符串的地址。
- `movl $16, %edx`: 将数字 16 加载到 EDX 寄存器中, 该寄存器用于告诉操作系统我们要打印的字符串的长度。
- `int $0x80`: 触发一个系统调用，执行输出操作，由之前设置的各种参数决定具体的操作。在 Linux 系统下，这会通过内核向终端打印输出字符串。
- `xorl %eax, %eax`: 将 EAX 寄存器中的值与自身进行异或运算，结果存储回 EAX 寄存器。这是一种快速清空寄存器的方法。
- `movl $1, %eax`: 将数字 1 加载到 EAX 寄存器中，该寄存器用于告诉操作系统我们要进行"exit"系统调用。
- `xorl %ebx, %ebx`: 将 EBX 寄存器中的值与自身进行异或运算，结果存储回 EBX 寄存器。这是一种快速清空寄存器的方法。
- `int $0x80`: 触发一个系统调用，执行程序退出操作，由之前设置的各种参数决定具体的操作。在 Linux 系统下，这会使程序停止并返回 0 作为退出状态码。
