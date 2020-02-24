<main_pipe>:
             0x0001026c: 46 77:       entsp (u6)      0x6
             0x0001026e: 05 55:       stw (ru6)       r4, sp[0x5]
             0x00010270: 44 55:       stw (ru6)       r5, sp[0x4]
             0x00010272: 83 55:       stw (ru6)       r6, sp[0x3]
             0x00010274: c2 55:       stw (ru6)       r7, sp[0x2]
             0x00010276: 01 56:       stw (ru6)       r8, sp[0x1]
             0x00010278: 44 90:       add (2rus)      r4, r1, 0x0
             0x0001027a: 50 90:       add (2rus)      r5, r0, 0x0
             0x0001027c: d1 a6:       mkmsk (rus)     r0, 0x1
             0x0001027e: 80 69:       ldc (ru6)       r6, 0x0
             0x00010280: d5 69:       ldc (ru6)       r7, 0x15
             0x00010282: 48 91:       add (2rus)      r8, r6, 0x0
.label62     0x00010284: 04 d8:       ldap (u10)      r11, 0x4 <.label54>
             0x00010286: f4 47:       setv (1r)       res[r4], r11
             0x00010288: 0f d8:       ldap (u10)      r11, 0xf <.label55>
             0x0001028a: f5 47:       setv (1r)       res[r5], r11
             0x0001028c: 0b 73:       bu (u6)         0xb <.label56>
.label54     0x0001028e: 11 cf:       chkct (rus)     res[r4], 0x1
             0x00010290: 11 4f:       outct (rus)     res[r4], 0x1
             0x00010292: 80 b7:       in (2r)         r0, res[r4]
             0x00010294: 11 cf:       chkct (rus)     res[r4], 0x1
             0x00010296: 11 4f:       outct (rus)     res[r4], 0x1
             0x00010298: 15 4f:       outct (rus)     res[r5], 0x1
             0x0001029a: 15 cf:       chkct (rus)     res[r5], 0x1
             0x0001029c: 81 af:       out (r2r)       res[r5], r0
             0x0001029e: 15 4f:       outct (rus)     res[r5], 0x1
             0x000102a0: 15 cf:       chkct (rus)     res[r5], 0x1
             0x000102a2: 01 92:       add (2rus)      r8, r8, 0x1
.label56     0x000102a4: d3 c3:       lss (3r)        r1, r8, r7
             0x000102a6: 0a 73:       bu (u6)         0xa <.label57>
.label55     0x000102a8: 15 cf:       chkct (rus)     res[r5], 0x1
             0x000102aa: 15 4f:       outct (rus)     res[r5], 0x1
             0x000102ac: 89 b7:       in (2r)         r2, res[r5]
             0x000102ae: 15 cf:       chkct (rus)     res[r5], 0x1
             0x000102b0: 15 4f:       outct (rus)     res[r5], 0x1
             0x000102b2: 11 4f:       outct (rus)     res[r4], 0x1
             0x000102b4: 11 cf:       chkct (rus)     res[r4], 0x1
             0x000102b6: 88 af:       out (r2r)       res[r4], r2
             0x000102b8: 11 4f:       outct (rus)     res[r4], 0x1
             0x000102ba: 11 cf:       chkct (rus)     res[r4], 0x1
.label57     0x000102bc: ed 07:       clre (0r)       
             0x000102be: 94 27:       eet (2r)        r1, res[r4]
             0x000102c0: 41 7b:       setsr (u6)      0x1
             0x000102c2: 01 7b:       clrsr (u6)      0x1
             0x000102c4: 00 73:       bu (u6)         0x0 <.label58>
.label58     0x000102c6: f5 07:       eeu (1r)        res[r5]
             0x000102c8: 41 7b:       setsr (u6)      0x1
             0x000102ca: 01 7b:       clrsr (u6)      0x1
             0x000102cc: 00 73:       bu (u6)         0x0 <.label59>
.label59     0x000102ce: 41 7b:       setsr (u6)      0x1
             0x000102d0: 01 7b:       clrsr (u6)      0x1
             0x000102d2: 00 73:       bu (u6)         0x0 <.label60>
.label60     0x000102d4: e0 0f:       waitet (1r)     r0
             0x000102d6: 00 73:       bu (u6)         0x0 <.label61>
.label61     0x000102d8: 00 f0 4b 7f: ldaw (lu6)      r11, cp[0xb]
             0x000102dc: 8c 91:       add (2rus)      r0, r11, 0x0
             0x000102de: 00 f0 82 d0: bl (lu10)       0x82 <puts>
             0x000102e2: c8 90:       add (2rus)      r0, r6, 0x0
             0x000102e4: 31 77:       bu (u6)         -0x31 <.label62>
             0x000102e6: 00 00:       stw (2rus)      r0, r0[0x0]
