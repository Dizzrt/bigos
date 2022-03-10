msg_error: .ascii "error code: "
error_code: .word 0 

# 0 ==> not support int 0x13 extension APIs
# 1 ==> not no active partition was found
# 2 ==> Int 0x13 Read error
# 3 ==> BOOT.BIN not exists

error:
    movw $msg_error,%si
print:
    movb (%si),%al
    cmpb $0,%al
    je end
    movb $0x0e,%ah
    int $0x10
    inc %si 
    jmp print
end:
    #hlt
    jmp .
