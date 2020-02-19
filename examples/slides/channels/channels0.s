	.text
	.file	"channels0.c"

	.weak       _fptrgroup._Par2.group
	.weak       _fptrgroup._Par2.nstackwords.group
	.weak       _fptrgroup._Par2.maxcores.group
	.weak       _fptrgroup._Par2.maxtimers.group
	.weak       _fptrgroup._Par2.maxchanends.group
	.max_reduce _fptrgroup._Par2.nstackwords, _fptrgroup._Par2.nstackwords.group, 0
	.max_reduce _fptrgroup._Par2.maxcores, _fptrgroup._Par2.maxcores.group, 0
	.max_reduce _fptrgroup._Par2.maxtimers, _fptrgroup._Par2.maxtimers.group, 0
	.max_reduce _fptrgroup._Par2.maxchanends, _fptrgroup._Par2.maxchanends.group, 0


	.globl	sends_first
	.align	4
	.type	sends_first,@function
	.cc_top sends_first.function,sends_first
sends_first:
	retsp 0
	# RETURN_REG_HOLDER
	.cc_bottom sends_first.function
	.set	sends_first.nstackwords,0
	.globl	sends_first.nstackwords
	.set	sends_first.maxcores,1
	.globl	sends_first.maxcores
	.set	sends_first.maxtimers,0
	.globl	sends_first.maxtimers
	.set	sends_first.maxchanends,0
	.globl	sends_first.maxchanends
.Ltmp0:
	.size	sends_first, .Ltmp0-sends_first

	.globl	receives_first
	.align	4
	.type	receives_first,@function
	.cc_top receives_first.function,receives_first
receives_first:
	retsp 0
	# RETURN_REG_HOLDER
	.cc_bottom receives_first.function
	.set	receives_first.nstackwords,0
	.globl	receives_first.nstackwords
	.set	receives_first.maxcores,1
	.globl	receives_first.maxcores
	.set	receives_first.maxtimers,0
	.globl	receives_first.maxtimers
	.set	receives_first.maxchanends,0
	.globl	receives_first.maxchanends
.Ltmp1:
	.size	receives_first, .Ltmp1-receives_first

	.globl	main
	.align	4
	.type	main,@function
	.cc_top main.function,main
main:
	entsp 4
	stw r4, sp[3]
	#APP
	getr r0, 2
	#NO_APP
	ldc r2, 0
	bf r0, .LBB2_4
	#APP
	getr r1, 2
	#NO_APP
	bf r1, .LBB2_3
	#APP
	setd res[r0], r1
	#NO_APP
	#APP
	setd res[r1], r0
	#NO_APP
	bu .LBB2_5
.LBB2_3:
	#APP
	freer res[r0]
	#NO_APP
.LBB2_4:
	mov r1, r2
	mov r0, r2
.LBB2_5:
	#APP
	.local _fptrgroup._Par2.nstackwords 
	.local _fptrgroup._Par2.nstackwords.group 
	.set .LPar0sg,0 
	.local _fptrgroup._Par2.maxcores 
	.local _fptrgroup._Par2.maxcores.group 
	.set .LPar0cg,0 
	.local _fptrgroup._Par2.maxtimers 
	.local _fptrgroup._Par2.maxtimers.group 
	.set .LPar0tg,0 
	.local _fptrgroup._Par2.maxchanends 
	.local _fptrgroup._Par2.maxchanends.group 
	.set .LPar0eg,0 
	.add_to_set .LPar0sg,(__xcore_ugs_shim_receives_first.nstackwords + 1 
	.add_to_set .LPar0cg,__xcore_ugs_shim_receives_first.maxcores 
	.add_to_set .LPar0tg,__xcore_ugs_shim_receives_first.maxtimers 
	.add_to_set .LPar0eg,__xcore_ugs_shim_receives_first.maxchanends 
	.set .L_Par2ms,( sends_first.nstackwords $M thread_group_add.nstackwords $M __xcore_substack_advance.nstackwords $M thread_group_start.nstackwords $M __xcore_dynamically_false.nstackwords $M thread_group_wait_and_free.nstackwords ) 
	.add_to_set .LPar0sg,.L_Par2ms 
	.add_to_set .LPar0cg,sends_first.maxcores 
	.add_to_set .LPar0tg,sends_first.maxtimers 
	.add_to_set .LPar0eg,sends_first.maxchanends 
	.sum_reduce .LPar0sm,.LPar0sg,0 
	.add_to_set _fptrgroup._Par2.nstackwords.group,.LPar0sm 
	.sum_reduce .LPar0cm,.LPar0cg,0 
	.add_to_set _fptrgroup._Par2.maxcores.group,.LPar0cm 
	.sum_reduce .LPar0tm,.LPar0tg,0 
	.add_to_set _fptrgroup._Par2.maxtimers.group,.LPar0tm 
	.sum_reduce .LPar0em,.LPar0eg,0 
	.add_to_set _fptrgroup._Par2.maxchanends.group,.LPar0em 

	#NO_APP
	#APP
	ldc r2,.L_Par2ms 

	#NO_APP
	#APP
	ldaw r3, sp[0]
	#NO_APP
	add r2, r2, 1
	#APP
	ldaw r3, r3[-r2]
	#NO_APP
	#APP
	getr r2, 3
	#NO_APP
	stw r1, sp[2]
	#APP
	getst r4, res[r2]
	#NO_APP
	ldap r11, __xcore_ugs_shim_receives_first
	#APP
	init t[r4]:pc, r11
	#NO_APP
	#APP
	init t[r4]:sp, r3
	#NO_APP
	ldaw r3, sp[2]
	#APP
	set t[r4]:r0, r3
	#NO_APP
	ldap r11, __xcore_synchronised_thread_end
	#APP
	init t[r4]:lr, r11
	#NO_APP
	#APP
	msync res[r2]
	#NO_APP
	#APP
	ldc r3, 0
	#NO_APP
	bt r3, .LBB2_7
	#APP
	mjoin res[r2]
	#NO_APP
	#APP
	freer res[r2]
	#NO_APP
	#APP
	freer res[r0]
	#NO_APP
	#APP
	freer res[r1]
	#NO_APP
	ldc r0, 0
	ldw r4, sp[3]
	retsp 4
	# RETURN_REG_HOLDER
.LBB2_7:
	ldc r0, 0
	stw r0, sp[1]
	ldw r0, sp[1]
	bla r0
	.cc_bottom main.function
	.set	main.nstackwords,(_fptrgroup._Par2.nstackwords + 4)
	.globl	main.nstackwords
	.set	main.maxcores,_fptrgroup._Par2.maxcores $M 1
	.globl	main.maxcores
	.set	main.maxtimers,_fptrgroup._Par2.maxtimers $M 0
	.globl	main.maxtimers
	.set	main.maxchanends,_fptrgroup._Par2.maxchanends $M 0
	.globl	main.maxchanends
.Ltmp2:
	.size	main, .Ltmp2-main

	.align	4
	.type	__xcore_ugs_shim_receives_first,@function
	.cc_top __xcore_ugs_shim_receives_first.function,__xcore_ugs_shim_receives_first
__xcore_ugs_shim_receives_first:
	#APP
	#NO_APP
	retsp 0
	# RETURN_REG_HOLDER
	.cc_bottom __xcore_ugs_shim_receives_first.function
	.set	__xcore_ugs_shim_receives_first.nstackwords,0
	.set	__xcore_ugs_shim_receives_first.maxcores,1
	.set	__xcore_ugs_shim_receives_first.maxtimers,0
	.set	__xcore_ugs_shim_receives_first.maxchanends,0
.Ltmp3:
	.size	__xcore_ugs_shim_receives_first, .Ltmp3-__xcore_ugs_shim_receives_first


	.ident	"XMOS-324-2b25498-Jan-29-2020 clang version 3.6.0  (based on LLVM 3.6.0svn)"
	.typestring sends_first, "f{0}(ui)"
	.typestring receives_first, "f{0}(ui)"
	.typestring main, "f{si}(0)"
	.typestring __xcore_synchronised_thread_end, "f{0}(0)"
	.section	.trap_info,"",@progbits
.Ltrap_info_entries_start0:
	.long	.Ltrap_info_entries_end0-.Ltrap_info_entries_start0
	.long	1
.Ltrap_info_entries_end0:
