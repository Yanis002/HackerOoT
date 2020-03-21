.include "macro.inc"

 # assembler directives
 .set noat      # allow manual use of $at
 .set noreorder # don't insert nops after branches
 .set gp=64     # allow use of 64-bit general purposee registers

.section .data

glabel En_Horse_Zelda_InitVars
 .word 0x005B0100, 0x00000010, 0x00460000, 0x000002A8
.word EnHorseZelda_Init
.word EnHorseZelda_Destroy
.word EnHorseZelda_Update
.word EnHorseZelda_Draw
glabel D_80A6E1D0
 .word 0x06007148
glabel D_80A6E1D4
 .word 0x3F2AAAAB
glabel D_80A6E1D8
 .word 0x0A000039, 0x01000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000100, 0x00280064, 0x00000000, 0x00000000
glabel D_80A6E204
 .word 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000100, 0x0D000000, 0x00000000, 0x00140064
glabel D_80A6E228
 .word 0x0A000939, 0x12000000, 0x00000001
.word D_80A6E204
glabel D_80A6E238
 .word 0x0A000023, 0x0064FE00
glabel D_80A6E240
 .byte 0xF9, 0x6E, 0xFE, 0x0C, 0x31, 0x22
glabel D_80A6E246
 .byte 0x07, 0x00, 0xF3, 0x28, 0xFE, 0x0C, 0x32, 0xD5, 0x07, 0x00, 0xEB, 0xEA, 0xFE, 0x5F, 0x2D, 0x6E, 0x07, 0x00, 0xE9, 0x5E, 0xFE, 0x27, 0x25, 0x65, 0x07, 0x00, 0xE5, 0x93, 0xFE, 0x0C, 0x20, 0xAC, 0x07, 0x00, 0xE6, 0x25, 0xFE, 0x77, 0x1B, 0x07, 0x07, 0x00, 0xEB, 0xB7, 0x00, 0x7C, 0x15, 0x39, 0x07, 0x00, 0xF4, 0x66, 0x00, 0x02, 0x11, 0xB9, 0x07, 0x00, 0xF4, 0x7B, 0xFF, 0xDD, 0x11, 0xAF, 0x07, 0x00, 0xF8, 0x8D, 0xFF, 0xD1, 0x0B, 0xA2, 0x07, 0x00, 0x09, 0xB8, 0x01, 0x26, 0x0E, 0x2C, 0x07, 0x00, 0x0C, 0x11, 0x01, 0x7A, 0x12, 0x69, 0x07, 0x00, 0x06, 0x4E, 0xFE, 0xFB, 0x1D, 0xAC, 0x07, 0x00, 0x02, 0xF2, 0xFF, 0x45, 0x24, 0x4F, 0x07, 0x00
glabel D_80A6E2B0
 .word 0x30F804B0
glabel D_80A6E2B4
 .word func_80A6DCCC
.word func_80A6DDFC
.word 0x00000000
