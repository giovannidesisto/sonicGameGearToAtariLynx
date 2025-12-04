;
; 2003-04-13, Ullrich von Bassewitz
; 2012-02-06, Greg King
;
; #include <time.h>
;
; typedef unsigned long int clock_t;
; clock_t _clk_tck(void);
; #define CLOCKS_PER_SEC _clk_tck()
; clock_t clock(void);
;
; clk_tck()'s test-values are based on the numbers in "set_tv.s".
; If you change the numbers there, then change them here, too.
;

	.export		_clock2
	.interruptor	update_clock2

	.import		sreg: zp

	.macpack	generic


	.proc	_clock2
	php
	sei			; Disable interrupts

; Read the clock counter.

	lda	clock_count2
	ldx	clock_count2+1
	ldy	clock_count2+2

	plp			; Re-enable interrupts
	sty	sreg
	stz	sreg+1		; Promote 24 bits up to 32 bits
	rts
	.endproc

;-----------------------------------------------------------------------------
; This interrupt handler increments a 24-bit counter at every video
; vertical-blanking time.
;
update_clock2:
	lda	$FD81
	and	#%00000100
	beq	@NotVBlank	; Not vertical-blank interrupt

	inc	clock_count2
	bne	@L1
	inc	clock_count2+1
	bne	@L1
	inc	clock_count2+2
@L1:	;clc			; General interrupt was not reset
@NotVBlank:
	rts

;-----------------------------------------------------------------------------
;
	.bss
clock_count2:
	.res	3