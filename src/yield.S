.global _ZN3TCB5yieldEP7ContextS1_
.type _ZN3TCB5yieldEP7ContextS1_, @function

_ZN3TCB5yieldEP7ContextS1_:
    
    sd ra, 0(a0)
    sd sp, 8(a0)
    

    ld ra, 0(a1)
    ld sp, 8(a1)
    
    ret