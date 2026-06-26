.global _Z15userThreadStartPv
.align 4
.type _Z15userThreadStartPv @function

_Z15userThreadStartPv:
    # s0 = body, s1 = userSP, s2 = arg, s3 = kernelSP

    la ra, userThreadExit

    csrw sepc, s0
    csrw sscratch, s3
    
    csrr t0, sstatus
    li t1, 1<<8
    not t1, t1
    and t0, t0, t1
    ori t0, t0, 1<<5
    csrw sstatus, t0

    mv a0, s2
    mv sp, s1

    sret