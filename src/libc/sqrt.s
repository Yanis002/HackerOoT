#include "ultra64/asm.h"

#if DEBUG_FEATURES
.set noreorder
#endif

.section .text

.balign 16

LEAF(sqrt)
    sqrt.d      $f0, $f12
    jr          $ra
     nop
END(sqrt)