.global _Z15userThreadStartv
.align 4
_Z15userThreadStartv:
    # Po tvojim 'frame' indeksima iz TCB.cpp:
    # s0 = adresa body funkcije (userMain)
    # s1 = User Stek (usp)
    # s2 = argument funkcije
    # s3 = vrh Kernel Steka (ksp)

    csrw sepc, s0
    
    mv sp, s1
    
    mv a0, s2
    
    csrw sscratch, s3
    
    csrc sstatus, t0
    
    li t0, 0x20
    csrs sstatus, t0
    li t0, 0x40000
    csrs sstatus, t0
    
    sret