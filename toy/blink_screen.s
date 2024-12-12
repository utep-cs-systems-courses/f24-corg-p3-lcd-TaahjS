	.arch msp430g2553
	.p2align 1,0
	.text

	.global blink_screen
	.extern P1OUT

blink_screen:
	call #led_flip
	cmp #1, r12
	jc zero
	mov #1, r12
	pop r0
zero:
	call #display_time
	call #game_over
	mov #0, r12
	pop r0
