.global _Z5yieldP3TCBS0_
.align 4
.type _Z5yieldP3TCBS0_ @function

_Z5yieldP3TCBS0_:
    # a0 = oldThread, a1 = newThread

    #SP offset in class: 40
    sd sp, 40(a0) 
    ld sp, 40(a1)

    ret