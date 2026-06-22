.global trap_entry
.align 4

trap_entry:
    addi sp, sp, -272
    
    sd ra, 8(sp) 
    addi ra, sp, 272
    sd ra, 16(sp)

    sd gp, 24(sp)
    sd tp, 32(sp)
    
    sd t0, 40(sp)
    sd t1, 48(sp)
    sd t2, 56(sp)
    sd t3, 64(sp)
    sd t4, 72(sp)
    sd t5, 80(sp)
    sd t6, 88(sp)
    
    sd s0, 96(sp)
    sd s1, 104(sp)
    sd s2, 112(sp)
    sd s3, 120(sp)
    sd s4, 128(sp)
    sd s5, 136(sp)
    sd s6, 144(sp)
    sd s7, 152(sp)
    sd s8, 160(sp)
    sd s9, 168(sp)
    sd s10, 176(sp)
    sd s11, 184(sp)
    
    sd a0, 192(sp)
    sd a1, 200(sp)
    sd a2, 208(sp)
    sd a3, 216(sp)
    sd a4, 224(sp)
    sd a5, 232(sp)
    sd a6, 240(sp)
    sd a7, 248(sp)

    csrr t0, sepc
    sd t0, 256(sp)
    csrr t0, sstatus
    sd t0, 264(sp)


    mv a5, a4
    mv a4, a3
    mv a3, a2
    mv a2, a1
    mv a1, a0
    csrr a0, scause
    call handle_trap
    

    ld t0, 256(sp)
    addi t0, t0, 4
    csrw sepc, t0
    ld t0, 264(sp)
    csrw sstatus, t0


    ld t0, 16(sp)
    csrw sscratch, t0
    ld ra, 8(sp) 

    ld gp, 24(sp)
    ld tp, 32(sp)
    
    ld t0, 40(sp)
    ld t1, 48(sp)
    ld t2, 56(sp)
    ld t3, 64(sp)
    ld t4, 72(sp)
    ld t5, 80(sp)
    ld t6, 88(sp)
    
    ld s0, 96(sp)
    ld s1, 104(sp)
    ld s2, 112(sp)
    ld s3, 120(sp)
    ld s4, 128(sp)
    ld s5, 136(sp)
    ld s6, 144(sp)
    ld s7, 152(sp)
    ld s8, 160(sp)
    ld s9, 168(sp)
    ld s10, 176(sp)
    ld s11, 184(sp)
    
    ld a1, 200(sp)
    ld a2, 208(sp)
    ld a3, 216(sp)
    ld a4, 224(sp)
    ld a5, 232(sp)
    ld a6, 240(sp)
    ld a7, 248(sp)

    csrr sp, sscratch

    sret
