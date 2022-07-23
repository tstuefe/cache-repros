.globl FOO

.text

.type FOO,@function

FOO:
# rdi: start address of array
# rsi: end address
# rdx: stride len
# return : sum

xor %rax,%rax

LOOP:

cmpq %rsi, %rdi
ja END

mov (%rdi), %r11
addq %r11, %rax

addq %rdx, %rdi  # stride

jmp LOOP

END:

ret


