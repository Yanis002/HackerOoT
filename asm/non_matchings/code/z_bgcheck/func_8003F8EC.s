glabel func_8003F8EC
/* AB6A8C 8003F8EC 27BDFFD0 */  addiu $sp, $sp, -0x30
/* AB6A90 8003F8F0 AFB40028 */  sw    $s4, 0x28($sp)
/* AB6A94 8003F8F4 AFB30024 */  sw    $s3, 0x24($sp)
/* AB6A98 8003F8F8 AFB20020 */  sw    $s2, 0x20($sp)
/* AB6A9C 8003F8FC AFB1001C */  sw    $s1, 0x1c($sp)
/* AB6AA0 8003F900 AFB00018 */  sw    $s0, 0x18($sp)
/* AB6AA4 8003F904 00C09825 */  move  $s3, $a2
/* AB6AA8 8003F908 0080A025 */  move  $s4, $a0
/* AB6AAC 8003F90C AFBF002C */  sw    $ra, 0x2c($sp)
/* AB6AB0 8003F910 00008025 */  move  $s0, $zero
/* AB6AB4 8003F914 00A08825 */  move  $s1, $a1
/* AB6AB8 8003F918 24120032 */  li    $s2, 50
.L8003F91C:
/* AB6ABC 8003F91C 962E138C */  lhu   $t6, 0x138c($s1)
/* AB6AC0 8003F920 268407C0 */  addiu $a0, $s4, 0x7c0
/* AB6AC4 8003F924 31CF0001 */  andi  $t7, $t6, 1
/* AB6AC8 8003F928 51E0000C */  beql  $t7, $zero, .L8003F95C
/* AB6ACC 8003F92C 26100001 */   addiu $s0, $s0, 1
/* AB6AD0 8003F930 0C00FAE1 */  jal   DynaPolyInfo_GetActor
/* AB6AD4 8003F934 02002825 */   move  $a1, $s0
/* AB6AD8 8003F938 50400008 */  beql  $v0, $zero, .L8003F95C
/* AB6ADC 8003F93C 26100001 */   addiu $s0, $s0, 1
/* AB6AE0 8003F940 54530006 */  bnel  $v0, $s3, .L8003F95C
/* AB6AE4 8003F944 26100001 */   addiu $s0, $s0, 1
/* AB6AE8 8003F948 0C010D28 */  jal   func_800434A0
/* AB6AEC 8003F94C 02602025 */   move  $a0, $s3
/* AB6AF0 8003F950 10000005 */  b     .L8003F968
/* AB6AF4 8003F954 8FBF002C */   lw    $ra, 0x2c($sp)
/* AB6AF8 8003F958 26100001 */  addiu $s0, $s0, 1
.L8003F95C:
/* AB6AFC 8003F95C 1612FFEF */  bne   $s0, $s2, .L8003F91C
/* AB6B00 8003F960 26310002 */   addiu $s1, $s1, 2
/* AB6B04 8003F964 8FBF002C */  lw    $ra, 0x2c($sp)
.L8003F968:
/* AB6B08 8003F968 8FB00018 */  lw    $s0, 0x18($sp)
/* AB6B0C 8003F96C 8FB1001C */  lw    $s1, 0x1c($sp)
/* AB6B10 8003F970 8FB20020 */  lw    $s2, 0x20($sp)
/* AB6B14 8003F974 8FB30024 */  lw    $s3, 0x24($sp)
/* AB6B18 8003F978 8FB40028 */  lw    $s4, 0x28($sp)
/* AB6B1C 8003F97C 03E00008 */  jr    $ra
/* AB6B20 8003F980 27BD0030 */   addiu $sp, $sp, 0x30
