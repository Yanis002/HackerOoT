glabel func_80845668
/* 13458 80845668 27BDFFC0 */  addiu   $sp, $sp, 0xFFC0           ## $sp = FFFFFFC0
/* 1345C 8084566C AFBF0024 */  sw      $ra, 0x0024($sp)           
/* 13460 80845670 AFB00020 */  sw      $s0, 0x0020($sp)           
/* 13464 80845674 AFA50044 */  sw      $a1, 0x0044($sp)           
/* 13468 80845678 8C8E0680 */  lw      $t6, 0x0680($a0)           ## 00000680
/* 1346C 8084567C 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 13470 80845680 260501B4 */  addiu   $a1, $s0, 0x01B4           ## $a1 = 000001B4
/* 13474 80845684 35CF0020 */  ori     $t7, $t6, 0x0020           ## $t7 = 00000020
/* 13478 80845688 AC8F0680 */  sw      $t7, 0x0680($a0)           ## 00000680
/* 1347C 8084568C AFA5002C */  sw      $a1, 0x002C($sp)           
/* 13480 80845690 0C028EF0 */  jal     func_800A3BC0              
/* 13484 80845694 8FA40044 */  lw      $a0, 0x0044($sp)           
/* 13488 80845698 8E1901BC */  lw      $t9, 0x01BC($s0)           ## 000001BC
/* 1348C 8084569C 3C180400 */  lui     $t8, 0x0400                ## $t8 = 04000000
/* 13490 808456A0 27182D48 */  addiu   $t8, $t8, 0x2D48           ## $t8 = 04002D48
/* 13494 808456A4 1719002E */  bne     $t8, $t9, .L80845760       
/* 13498 808456A8 00401825 */  or      $v1, $v0, $zero            ## $v1 = 00000000
/* 1349C 808456AC 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 134A0 808456B0 44812000 */  mtc1    $at, $f4                   ## $f4 = 1.00
/* 134A4 808456B4 3C054100 */  lui     $a1, 0x4100                ## $a1 = 41000000
/* 134A8 808456B8 E6040838 */  swc1    $f4, 0x0838($s0)           ## 00000838
/* 134AC 808456BC 0C02914C */  jal     func_800A4530              
/* 134B0 808456C0 8FA4002C */  lw      $a0, 0x002C($sp)           
/* 134B4 808456C4 1040007D */  beq     $v0, $zero, .L808458BC     
/* 134B8 808456C8 3C0B8016 */  lui     $t3, 0x8016                ## $t3 = 80160000
/* 134BC 808456CC 8E080678 */  lw      $t0, 0x0678($s0)           ## 00000678
/* 134C0 808456D0 C6000884 */  lwc1    $f0, 0x0884($s0)           ## 00000884
/* 134C4 808456D4 3C018085 */  lui     $at, %hi(D_80855480)       ## $at = 80850000
/* 134C8 808456D8 C502000C */  lwc1    $f2, 0x000C($t0)           ## 0000000C
/* 134CC 808456DC 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 134D0 808456E0 00002825 */  or      $a1, $zero, $zero          ## $a1 = 00000000
/* 134D4 808456E4 4600103C */  c.lt.s  $f2, $f0                   
/* 134D8 808456E8 8FA70044 */  lw      $a3, 0x0044($sp)           
/* 134DC 808456EC 240C6814 */  addiu   $t4, $zero, 0x6814         ## $t4 = 00006814
/* 134E0 808456F0 45020003 */  bc1fl   .L80845700                 
/* 134E4 808456F4 8E09067C */  lw      $t1, 0x067C($s0)           ## 0000067C
/* 134E8 808456F8 46001006 */  mov.s   $f0, $f2                   
/* 134EC 808456FC 8E09067C */  lw      $t1, 0x067C($s0)           ## 0000067C
.L80845700:
/* 134F0 80845700 00095100 */  sll     $t2, $t1,  4               
/* 134F4 80845704 05410006 */  bgez    $t2, .L80845720            
/* 134F8 80845708 00000000 */  nop
/* 134FC 8084570C 3C018085 */  lui     $at, %hi(D_8085547C)       ## $at = 80850000
/* 13500 80845710 C426547C */  lwc1    $f6, %lo(D_8085547C)($at)  
/* 13504 80845714 46060002 */  mul.s   $f0, $f0, $f6              
/* 13508 80845718 10000004 */  beq     $zero, $zero, .L8084572C   
/* 1350C 8084571C 00000000 */  nop
.L80845720:
/* 13510 80845720 C4285480 */  lwc1    $f8, %lo(D_80855480)($at)  
/* 13514 80845724 46080002 */  mul.s   $f0, $f0, $f8              
/* 13518 80845728 00000000 */  nop
.L8084572C:
/* 1351C 8084572C 8D6BE664 */  lw      $t3, -0x199C($t3)          ## 8015E664
/* 13520 80845730 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 13524 80845734 51600005 */  beql    $t3, $zero, .L8084574C     
/* 13528 80845738 44060000 */  mfc1    $a2, $f0                   
/* 1352C 8084573C 44815000 */  mtc1    $at, $f10                  ## $f10 = 1.00
/* 13530 80845740 00000000 */  nop
/* 13534 80845744 460A0000 */  add.s   $f0, $f0, $f10             
/* 13538 80845748 44060000 */  mfc1    $a2, $f0                   
.L8084574C:
/* 1353C 8084574C 0C20E250 */  jal     func_80838940              
/* 13540 80845750 AFAC0010 */  sw      $t4, 0x0010($sp)           
/* 13544 80845754 240DFFFF */  addiu   $t5, $zero, 0xFFFF         ## $t5 = FFFFFFFF
/* 13548 80845758 10000058 */  beq     $zero, $zero, .L808458BC   
/* 1354C 8084575C A60D0850 */  sh      $t5, 0x0850($s0)           ## 00000850
.L80845760:
/* 13550 80845760 8FA40044 */  lw      $a0, 0x0044($sp)           
/* 13554 80845764 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 13558 80845768 8FA6002C */  lw      $a2, 0x002C($sp)           
/* 1355C 8084576C 3C074080 */  lui     $a3, 0x4080                ## $a3 = 40800000
/* 13560 80845770 0C20DD28 */  jal     func_808374A0              
/* 13564 80845774 AFA3003C */  sw      $v1, 0x003C($sp)           
/* 13568 80845778 14400007 */  bne     $v0, $zero, .L80845798     
/* 1356C 8084577C 8FA3003C */  lw      $v1, 0x003C($sp)           
/* 13570 80845780 8E0E067C */  lw      $t6, 0x067C($s0)           ## 0000067C
/* 13574 80845784 3C01FFFB */  lui     $at, 0xFFFB                ## $at = FFFB0000
/* 13578 80845788 3421BFFF */  ori     $at, $at, 0xBFFF           ## $at = FFFBBFFF
/* 1357C 8084578C 01C17824 */  and     $t7, $t6, $at              
/* 13580 80845790 1000004A */  beq     $zero, $zero, .L808458BC   
/* 13584 80845794 AE0F067C */  sw      $t7, 0x067C($s0)           ## 0000067C
.L80845798:
/* 13588 80845798 14600003 */  bne     $v1, $zero, .L808457A8     
/* 1358C 8084579C 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 13590 808457A0 18400009 */  blez    $v0, .L808457C8            
/* 13594 808457A4 3C080400 */  lui     $t0, 0x0400                ## $t0 = 04000000
.L808457A8:
/* 13598 808457A8 0C20F03A */  jal     func_8083C0E8              
/* 1359C 808457AC 8FA50044 */  lw      $a1, 0x0044($sp)           
/* 135A0 808457B0 8E18067C */  lw      $t8, 0x067C($s0)           ## 0000067C
/* 135A4 808457B4 3C01FFFB */  lui     $at, 0xFFFB                ## $at = FFFB0000
/* 135A8 808457B8 3421BFFF */  ori     $at, $at, 0xBFFF           ## $at = FFFBBFFF
/* 135AC 808457BC 0301C824 */  and     $t9, $t8, $at              
/* 135B0 808457C0 1000003E */  beq     $zero, $zero, .L808458BC   
/* 135B4 808457C4 AE19067C */  sw      $t9, 0x067C($s0)           ## 0000067C
.L808457C8:
/* 135B8 808457C8 8E0201BC */  lw      $v0, 0x01BC($s0)           ## 000001BC
/* 135BC 808457CC 44800000 */  mtc1    $zero, $f0                 ## $f0 = 0.00
/* 135C0 808457D0 250832E8 */  addiu   $t0, $t0, 0x32E8           ## $t0 = 000032E8
/* 135C4 808457D4 1502000D */  bne     $t0, $v0, .L8084580C       
/* 135C8 808457D8 3C090400 */  lui     $t1, 0x0400                ## $t1 = 04000000
/* 135CC 808457DC 8FA4002C */  lw      $a0, 0x002C($sp)           
/* 135D0 808457E0 0C02914C */  jal     func_800A4530              
/* 135D4 808457E4 3C0541F0 */  lui     $a1, 0x41F0                ## $a1 = 41F00000
/* 135D8 808457E8 10400004 */  beq     $v0, $zero, .L808457FC     
/* 135DC 808457EC 8FA40044 */  lw      $a0, 0x0044($sp)           
/* 135E0 808457F0 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 135E4 808457F4 0C20F42A */  jal     func_8083D0A8              
/* 135E8 808457F8 3C064120 */  lui     $a2, 0x4120                ## $a2 = 41200000
.L808457FC:
/* 135EC 808457FC 3C014248 */  lui     $at, 0x4248                ## $at = 42480000
/* 135F0 80845800 44810000 */  mtc1    $at, $f0                   ## $f0 = 50.00
/* 135F4 80845804 1000000E */  beq     $zero, $zero, .L80845840   
/* 135F8 80845808 44050000 */  mfc1    $a1, $f0                   
.L8084580C:
/* 135FC 8084580C 25292D40 */  addiu   $t1, $t1, 0x2D40           ## $t1 = 00002D40
/* 13600 80845810 15220005 */  bne     $t1, $v0, .L80845828       
/* 13604 80845814 3C0A0400 */  lui     $t2, 0x0400                ## $t2 = 04000000
/* 13608 80845818 3C0141F0 */  lui     $at, 0x41F0                ## $at = 41F00000
/* 1360C 8084581C 44810000 */  mtc1    $at, $f0                   ## $f0 = 30.00
/* 13610 80845820 10000007 */  beq     $zero, $zero, .L80845840   
/* 13614 80845824 44050000 */  mfc1    $a1, $f0                   
.L80845828:
/* 13618 80845828 254A2D38 */  addiu   $t2, $t2, 0x2D38           ## $t2 = 04002D38
/* 1361C 8084582C 15420003 */  bne     $t2, $v0, .L8084583C       
/* 13620 80845830 3C014180 */  lui     $at, 0x4180                ## $at = 41800000
/* 13624 80845834 44810000 */  mtc1    $at, $f0                   ## $f0 = 16.00
/* 13628 80845838 00000000 */  nop
.L8084583C:
/* 1362C 8084583C 44050000 */  mfc1    $a1, $f0                   
.L80845840:
/* 13630 80845840 0C02914C */  jal     func_800A4530              
/* 13634 80845844 8FA4002C */  lw      $a0, 0x002C($sp)           
/* 13638 80845848 50400007 */  beql    $v0, $zero, .L80845868     
/* 1363C 8084584C 8E0C01BC */  lw      $t4, 0x01BC($s0)           ## 000001BC
/* 13640 80845850 0C20CA28 */  jal     func_808328A0              
/* 13644 80845854 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 13648 80845858 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 1364C 8084585C 0C20C9A6 */  jal     func_80832698              
/* 13650 80845860 24056804 */  addiu   $a1, $zero, 0x6804         ## $a1 = 00006804
/* 13654 80845864 8E0C01BC */  lw      $t4, 0x01BC($s0)           ## 000001BC
.L80845868:
/* 13658 80845868 3C0B0400 */  lui     $t3, 0x0400                ## $t3 = 04000000
/* 1365C 8084586C 256B2D38 */  addiu   $t3, $t3, 0x2D38           ## $t3 = 04002D38
/* 13660 80845870 116C0007 */  beq     $t3, $t4, .L80845890       
/* 13664 80845874 3C0140A0 */  lui     $at, 0x40A0                ## $at = 40A00000
/* 13668 80845878 44818000 */  mtc1    $at, $f16                  ## $f16 = 5.00
/* 1366C 8084587C C61201CC */  lwc1    $f18, 0x01CC($s0)          ## 000001CC
/* 13670 80845880 4612803C */  c.lt.s  $f16, $f18                 
/* 13674 80845884 00000000 */  nop
/* 13678 80845888 4502000D */  bc1fl   .L808458C0                 
/* 1367C 8084588C 8FBF0024 */  lw      $ra, 0x0024($sp)           
.L80845890:
/* 13680 80845890 860D0850 */  lh      $t5, 0x0850($s0)           ## 00000850
/* 13684 80845894 55A00006 */  bnel    $t5, $zero, .L808458B0     
/* 13688 80845898 260400BC */  addiu   $a0, $s0, 0x00BC           ## $a0 = 000000BC
/* 1368C 8084589C 0C20CA15 */  jal     func_80832854              
/* 13690 808458A0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 13694 808458A4 240E0001 */  addiu   $t6, $zero, 0x0001         ## $t6 = 00000001
/* 13698 808458A8 A60E0850 */  sh      $t6, 0x0850($s0)           ## 00000850
/* 1369C 808458AC 260400BC */  addiu   $a0, $s0, 0x00BC           ## $a0 = 000000BC
.L808458B0:
/* 136A0 808458B0 24050000 */  addiu   $a1, $zero, 0x0000         ## $a1 = 00000000
/* 136A4 808458B4 0C01DE80 */  jal     Math_ApproxF
              
/* 136A8 808458B8 3C064316 */  lui     $a2, 0x4316                ## $a2 = 43160000
.L808458BC:
/* 136AC 808458BC 8FBF0024 */  lw      $ra, 0x0024($sp)           
.L808458C0:
/* 136B0 808458C0 8FB00020 */  lw      $s0, 0x0020($sp)           
/* 136B4 808458C4 27BD0040 */  addiu   $sp, $sp, 0x0040           ## $sp = 00000000
/* 136B8 808458C8 03E00008 */  jr      $ra                        
/* 136BC 808458CC 00000000 */  nop

