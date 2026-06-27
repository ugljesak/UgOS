.global _ZN3TCB5yieldEP7ContextS1_
.type _ZN3TCB5yieldEP7ContextS1_, @function

_ZN3TCB5yieldEP7ContextS1_:
    
    sd ra, 0(a0)
    sd sp, 8(a0)
    
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11
    sd s\index, (16 + \index * 8)(a0)
    .endr


    ld ra, 0(a1)
    ld sp, 8(a1)

    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11
    ld s\index, (16 + \index * 8)(a1)
    .endr
    
    ret