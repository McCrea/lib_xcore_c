<main_pipe>:
             0x000102b0: 4b 77:       entsp (u6)      0xb
             0x000102b2: 0a 54:       stw (ru6)       r0, sp[0xa]
             0x000102b4: 49 54:       stw (ru6)       r1, sp[0x9]
             0x000102b6: d1 a6:       mkmsk (rus)     r0, 0x1
             0x000102b8: 08 54:       stw (ru6)       r0, sp[0x8]
             0x000102ba: 40 68:       ldc (ru6)       r1, 0x0
             0x000102bc: 47 54:       stw (ru6)       r1, sp[0x7]
             0x000102be: 33 70:       bt (ru6)        r0, 0x33 <.label63>
             0x000102c0: 33 73:       bu (u6)         0x33 <.label64>
.label88     0x000102c2: 00 68:       ldc (ru6)       r0, 0x0
             0x000102c4: 2c 70:       bt (ru6)        r0, 0x2c <.label65>
             0x000102c6: 00 73:       bu (u6)         0x0 <.label66>
.label66     0x000102c8: 00 f0 be d1: bl (lu10)       0x1be <__xcore_select_disable_trigger_all>
             0x000102cc: 00 73:       bu (u6)         0x0 <.label67>
.label67     0x000102ce: 07 5c:       ldw (ru6)       r0, sp[0x7]
             0x000102d0: 55 68:       ldc (ru6)       r1, 0x15
             0x000102d2: 01 c0:       lss (3r)        r0, r0, r1
             0x000102d4: 05 54:       stw (ru6)       r0, sp[0x5]
             0x000102d6: 09 5c:       ldw (ru6)       r0, sp[0x9]
             0x000102d8: 45 5c:       ldw (ru6)       r1, sp[0x5]
             0x000102da: 00 f0 0f d1: bl (lu10)       0x10f <__xcore_resource_event_enable_if_true>
             0x000102de: 41 7b:       setsr (u6)      0x1
             0x000102e0: 01 7b:       clrsr (u6)      0x1
             0x000102e2: 00 73:       bu (u6)         0x0 <.label68>
.label68     0x000102e4: 00 73:       bu (u6)         0x0 <.label69>
.label69     0x000102e6: 0a 5c:       ldw (ru6)       r0, sp[0xa]
             0x000102e8: 00 f0 f4 d0: bl (lu10)       0xf4 <__xcore_resource_event_enable_unconditional>
             0x000102ec: 41 7b:       setsr (u6)      0x1
             0x000102ee: 01 7b:       clrsr (u6)      0x1
             0x000102f0: 00 73:       bu (u6)         0x0 <.label70>
.label70     0x000102f2: 00 73:       bu (u6)         0x0 <.label71>
.label71     0x000102f4: 08 5c:       ldw (ru6)       r0, sp[0x8]
             0x000102f6: 04 54:       stw (ru6)       r0, sp[0x4]
             0x000102f8: d1 a6:       mkmsk (rus)     r0, 0x1
             0x000102fa: 0a 70:       bt (ru6)        r0, 0xa <.label72>
             0x000102fc: 00 73:       bu (u6)         0x0 <.label73>
.label73     0x000102fe: 04 5c:       ldw (ru6)       r0, sp[0x4]
             0x00010300: 03 78:       bf (ru6)        r0, 0x3 <.label74>
             0x00010302: 00 73:       bu (u6)         0x0 <.label75>
.label75     0x00010304: ec 07:       waiteu (0r)     
             0x00010306: 00 73:       bu (u6)         0x0 <.label74>
.label74     0x00010308: 41 7b:       setsr (u6)      0x1
             0x0001030a: 01 7b:       clrsr (u6)      0x1
             0x0001030c: 00 73:       bu (u6)         0x0 <.label76>
.label76     0x0001030e: 1b 73:       bu (u6)         0x1b <.label77>
.label72     0x00010310: 41 7b:       setsr (u6)      0x1
             0x00010312: 01 7b:       clrsr (u6)      0x1
             0x00010314: 00 73:       bu (u6)         0x0 <.label78>
.label78     0x00010316: 04 5c:       ldw (ru6)       r0, sp[0x4]
             0x00010318: e0 0f:       waitet (1r)     r0
             0x0001031a: 00 73:       bu (u6)         0x0 <.label79>
.label79     0x0001031c: 14 73:       bu (u6)         0x14 <.label77>
.label65     0x0001031e: d1 a6:       mkmsk (rus)     r0, 0x1
             0x00010320: 11 70:       bt (ru6)        r0, 0x11 <.label80>
             0x00010322: 00 73:       bu (u6)         0x0 <.label81>
.label81     0x00010324: 00 73:       bu (u6)         0x0 <.label63>
.label63     0x00010326: 00 73:       bu (u6)         0x0 <.label64>
.label64     0x00010328: 02 d8:       ldap (u10)      r11, 0x2 <.label82>
             0x0001032a: c6 56:       stw (ru6)       r11, sp[0x6]
             0x0001032c: 00 73:       bu (u6)         0x0 <.label82>
.label82     0x0001032e: 09 5c:       ldw (ru6)       r0, sp[0x9]
             0x00010330: 13 d8:       ldap (u10)      r11, 0x13 <.label83>
             0x00010332: 9c 91:       add (2rus)      r1, r11, 0x0
             0x00010334: 00 f0 fa d0: bl (lu10)       0xfa <__xcore_resource_register_event_vector>
             0x00010338: 0a 5c:       ldw (ru6)       r0, sp[0xa]
             0x0001033a: 1d d8:       ldap (u10)      r11, 0x1d <.label84>
             0x0001033c: 9c 91:       add (2rus)      r1, r11, 0x0
             0x0001033e: 00 f0 f5 d0: bl (lu10)       0xf5 <__xcore_resource_register_event_vector>
             0x00010342: 23 73:       bu (u6)         0x23 <.label85>
.label80     0x00010344: 00 73:       bu (u6)         0x0 <.label77>
.label77     0x00010346: 00 f0 4b 7f: ldaw (lu6)      r11, cp[0xb]
             0x0001034a: 8c 91:       add (2rus)      r0, r11, 0x0
             0x0001034c: 00 f0 7e d1: bl (lu10)       0x17e <puts>
             0x00010350: 00 73:       bu (u6)         0x0 <.label86>
.label86     0x00010352: 06 5c:       ldw (ru6)       r0, sp[0x6]
             0x00010354: 01 54:       stw (ru6)       r0, sp[0x1]
             0x00010356: 1c 73:       bu (u6)         0x1c <.label87>
.label83     0x00010358: 09 5c:       ldw (ru6)       r0, sp[0x9]
             0x0001035a: 00 f0 21 d1: bl (lu10)       0x121 <chan_in_word>
             0x0001035e: 03 54:       stw (ru6)       r0, sp[0x3]
             0x00010360: 0a 5c:       ldw (ru6)       r0, sp[0xa]
             0x00010362: 43 5c:       ldw (ru6)       r1, sp[0x3]
             0x00010364: 00 f0 08 d1: bl (lu10)       0x108 <chan_out_word>
             0x00010368: 03 5c:       ldw (ru6)       r0, sp[0x3]
             0x0001036a: 08 54:       stw (ru6)       r0, sp[0x8]
             0x0001036c: 07 5c:       ldw (ru6)       r0, sp[0x7]
             0x0001036e: 01 90:       add (2rus)      r0, r0, 0x1
             0x00010370: 07 54:       stw (ru6)       r0, sp[0x7]
             0x00010372: 01 f0 1a 77: bu (lu6)        -0x5a <.label88>
.label84     0x00010376: 0a 5c:       ldw (ru6)       r0, sp[0xa]
             0x00010378: 00 f0 12 d1: bl (lu10)       0x112 <chan_in_word>
             0x0001037c: 02 54:       stw (ru6)       r0, sp[0x2]
             0x0001037e: 09 5c:       ldw (ru6)       r0, sp[0x9]
             0x00010380: 42 5c:       ldw (ru6)       r1, sp[0x2]
             0x00010382: 00 f0 f9 d0: bl (lu10)       0xf9 <chan_out_word>
             0x00010386: 01 f0 24 77: bu (lu6)        -0x64 <.label88>
.label85     0x0001038a: 00 73:       bu (u6)         0x0 <.label89>
.label89     0x0001038c: 01 f0 27 77: bu (lu6)        -0x67 <.label88>
.label87     0x00010390: 01 5c:       ldw (ru6)       r0, sp[0x1]
             0x00010392: f0 27:       bau (1r)        r0

