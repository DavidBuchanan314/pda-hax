.section .text.startup
.globl _start
.extern main

_start:
	ldr	sp, =0x84080000


	ldr	r0, =0xacf00004 // PSSR
	mov	r1, #0x37
	str	r1, [r0]


	ldr	r0, =0x8000e0bc // init lcd XXX: don't "cheat" by calling into bootflash
	blx	r0


	ldr	r0, =0xacc00000 // PWM registers
	mov	r1, #0x3f
	str	r1, [r0, #0] // PWM_CTRL1
	mov	r1, #0x140
	str	r1, [r0, #4] // PWM_PWDUTY1
	mov	r1, #0x144
	str	r1, [r0, #8] // PWM_PERVAL1


	ldr	r0, =0xad300000 // Clock registers
	ldr	r1, [r0, #4] // CKEN
	orr	r1, r1, #2
	orr	r1, r1, #(1<<16)
	str	r1, [r0, #4] // CKEN


	blx	main

halt:
	b	halt
