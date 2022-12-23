.text
.globl user_intr
.type user_intr,"function"

user_intr:
    pusha
    call kernel_handler
    popa
    iret   