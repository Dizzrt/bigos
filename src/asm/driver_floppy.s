.file "floppy_driver_s"
.global __floppy_wait_RQM__
.global __floppy_command__
.global __floppy_sense_int__
.global __floppy_init__
.global __floppy_recalibrate__
.global __floppy_seek__
.global __floppy_read__
.global __floppy_DMA_init__
.global __floppy_write__
.extern io_delay
.text
.code64

__floppy_wait_RQM__:
    mov $0x3f4,%dx
    inb %dx,%al
    and $0x80,%al
    cmpb $0x80,%al
    jne  __floppy_wait_RQM__
    retq

__floppy_command__:
    call __floppy_wait_RQM__
    mov 8(%rsp),%rax
    mov $0x3f5,%dx
    out %al,%dx
    retq

__floppy_sense_int__:
    push $0x08
    call __floppy_command__
    add $8,%esp
    call __floppy_wait_RQM__
    retq

__floppy_init__:
    #reset and open one driver motor
    mov $0x3f2,%dx
    mov $0,%al
    out %al,%dx
    call io_delay
    mov $0x1c,%al #TODO driver 0 is selected for now
    out %al,%dx
    call __floppy_wait_RQM__

    #set the transfer speed
    mov $0x3f7,%dx
    mov $0,%al 
    out %al,%dx

    #fd specify
    push $0x03
    call __floppy_command__
    add $8,%rsp

    push $0xcf
    call __floppy_command__
    add $8,%rsp

    push $0x06
    call __floppy_command__    
    add $8,%rsp

    push $0
    call __floppy_recalibrate__
    add $8,%rsp
    ret

__floppy_recalibrate__:
    push $0x07
    call __floppy_command__
    add $8,%rsp
    push 8(%rsp)  #recalibrate target driver
    call __floppy_command__
    add $8,%rsp
    retq

__floppy_seek__:
    push $0x0f
    call __floppy_command__
    add $8,%rsp
    mov 8(%rsp),%rax #AH-target track  AL-head and driver
    push %rax
    call __floppy_command__
    add $8,%rsp
    shr $8,%rax
    push %rax
    call __floppy_command__
    add $8,%rsp
    retq

__floppy_read__:
    push $0xe6
    call __floppy_command__
    add $8,%rsp

    mov 8(%rsp),%rbx #DTL,GPL,EOT,N,R,H,C,D

    push %rbx
    call __floppy_command__
    add $8,%rsp
    shr $8,%rbx

    push %rbx
    call __floppy_command__
    add $8,%rsp
    shr $8,%rbx

    cmpb $0,%bl
    jne llll
GG:
    push %rbx
    call __floppy_command__
    add $8,%rsp
    shr $8,%rbx
    
    push %rbx
    call __floppy_command__
    add $8,%rsp
    shr $8,%rbx

    push %rbx
    call __floppy_command__
    add $8,%rsp
    shr $8,%rbx

    push %rbx
    call __floppy_command__
    add $8,%rsp
    shr $8,%rbx

    push %rbx
    call __floppy_command__
    add $8,%rsp
    shr $8,%rbx

    push %rbx
    call __floppy_command__
    add $8,%rsp
    retq
__floppy_DMA_init__:
    mov $0x0a,%dx
    mov $0x06,%al
    out %al,%dx

    mov $0x0c,%dx
    mov $0xff,%al
    out %al,%dx

    mov 8(%rsp),%rax #high count,low count,external addr,high addr,low addr
    mov $0x4,%dx
    out %al,%dx
    call io_delay
    shr $8,%rax
    out %al,%dx
    shr $8,%rax
    mov $0x81,%dx
    out %al,%dx

    push %rax
    mov $0x0c,%dx
    mov $0xff,%al
    out %al,%dx
    pop %rax

    mov $0x05,%dx
    shr $8,%rax
    out %al,%dx
    call io_delay
    shr $8,%rax
    out %al,%dx
    
    mov $0x0a,%dx
    mov $0x02,%al
    out %al,%dx
    retq
__floppy_write__:
    retq
llll:
    cmpb $1,%bl
    jne lll
    jmp GG

lll:
    jmp .
    