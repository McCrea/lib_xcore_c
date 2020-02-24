<main_pipe>:
             0x00010280: 48 77:       entsp (u6)      0x8
             0x00010282: 07 55:       stw (ru6)       r4, sp[0x7]
             0x00010284: 46 55:       stw (ru6)       r5, sp[0x6]
             0x00010286: 85 55:       stw (ru6)       r6, sp[0x5]
             0x00010288: c4 55:       stw (ru6)       r7, sp[0x4]
             0x0001028a: 03 56:       stw (ru6)       r8, sp[0x3]
             0x0001028c: 42 56:       stw (ru6)       r9, sp[0x2]
             0x0001028e: 81 56:       stw (ru6)       r10, sp[0x1]
             0x00010290: 44 90:       add (2rus)      r4, r1, 0x0
             0x00010292: 50 90:       add (2rus)      r5, r0, 0x0
             0x00010294: 19 a7:       mkmsk (rus)     r6, 0x1
             0x00010296: 00 6a:       ldc (ru6)       r8, 0x0
             0x00010298: 55 6a:       ldc (ru6)       r9, 0x15
             0x0001029a: 20 92:       add (2rus)      r10, r8, 0x0
.label63     0x0001029c: 0a d8:       ldap (u10)      r11, 0xa <.label55>
             0x0001029e: 9c 91:       add (2rus)      r1, r11, 0x0
             0x000102a0: c0 90:       add (2rus)      r0, r4, 0x0
             0x000102a2: 00 f0 cb d0: bl (lu10)       0xcb <__xcore_resource_register_event_vector>
             0x000102a6: 10 d8:       ldap (u10)      r11, 0x10 <.label56>
             0x000102a8: 9c 91:       add (2rus)      r1, r11, 0x0
             0x000102aa: c4 90:       add (2rus)      r0, r5, 0x0
             0x000102ac: 00 f0 c6 d0: bl (lu10)       0xc6 <__xcore_resource_register_event_vector>
             0x000102b0: 09 73:       bu (u6)         0x9 <.label57>
.label55     0x000102b2: c0 90:       add (2rus)      r0, r4, 0x0
             0x000102b4: 00 f0 e6 d0: bl (lu10)       0xe6 <chan_in_word>
             0x000102b8: 60 90:       add (2rus)      r6, r0, 0x0
             0x000102ba: c4 90:       add (2rus)      r0, r5, 0x0
             0x000102bc: d8 90:       add (2rus)      r1, r6, 0x0
             0x000102be: 00 f0 c9 d0: bl (lu10)       0xc9 <chan_out_word>
             0x000102c2: 29 92:       add (2rus)      r10, r10, 0x1
.label57     0x000102c4: 79 c6:       lss (3r)        r7, r10, r9
             0x000102c6: 07 73:       bu (u6)         0x7 <.label58>
.label56     0x000102c8: c4 90:       add (2rus)      r0, r5, 0x0
             0x000102ca: 00 f0 db d0: bl (lu10)       0xdb <chan_in_word>
             0x000102ce: 10 90:       add (2rus)      r1, r0, 0x0
             0x000102d0: c0 90:       add (2rus)      r0, r4, 0x0
             0x000102d2: 00 f0 bf d0: bl (lu10)       0xbf <chan_out_word>
.label58     0x000102d6: 00 f0 09 d1: bl (lu10)       0x109 <__xcore_select_disable_trigger_all>
             0x000102da: c0 90:       add (2rus)      r0, r4, 0x0
             0x000102dc: dc 90:       add (2rus)      r1, r7, 0x0
             0x000102de: 00 f0 a9 d0: bl (lu10)       0xa9 <__xcore_resource_event_enable_if_true>
             0x000102e2: 41 7b:       setsr (u6)      0x1
             0x000102e4: 01 7b:       clrsr (u6)      0x1
             0x000102e6: 00 73:       bu (u6)         0x0 <.label59>
.label59     0x000102e8: c4 90:       add (2rus)      r0, r5, 0x0
             0x000102ea: 00 f0 9f d0: bl (lu10)       0x9f <__xcore_resource_event_enable_unconditional>
             0x000102ee: 41 7b:       setsr (u6)      0x1
             0x000102f0: 01 7b:       clrsr (u6)      0x1
             0x000102f2: 00 73:       bu (u6)         0x0 <.label60>
.label60     0x000102f4: 41 7b:       setsr (u6)      0x1
             0x000102f6: 01 7b:       clrsr (u6)      0x1
             0x000102f8: 00 73:       bu (u6)         0x0 <.label61>
.label61     0x000102fa: e6 0f:       waitet (1r)     r6
             0x000102fc: 00 73:       bu (u6)         0x0 <.label62>
.label62     0x000102fe: 00 f0 4b 7f: ldaw (lu6)      r11, cp[0xb]
             0x00010302: 8c 91:       add (2rus)      r0, r11, 0x0
             0x00010304: 00 f0 f4 d0: bl (lu10)       0xf4 <puts>
             0x00010308: e0 91:       add (2rus)      r6, r8, 0x0
             0x0001030a: 38 77:       bu (u6)         -0x38 <.label63>
