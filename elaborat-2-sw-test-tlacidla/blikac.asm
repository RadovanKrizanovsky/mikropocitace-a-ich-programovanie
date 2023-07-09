
			.cdecls C,LIST,  "msp430g2231.h"

			.text				; zaciatok programu v pamati

RESET		mov.w   #0280h,SP			; inicializacia ukazovatela zasobnika
StopWDT		mov.w   #WDTPW+WDTHOLD,&WDTCTL		; zastavenie watchdog-u
SetupP1		bis.b   #001h,&P1DIR			; P1.0 nastaveny ako vystup

Mainloop	xor.b   #001h,&P1OUT			; softverovy prepinac P1.0
Wait		mov.w   #65000,R15			; register R15 - pocitadlo
L1			dec.w   R15			; dekrementacia R15
			jnz     L1			; test: R15 = 0?
			jmp     Mainloop		; ak R15 > 0, opakujeme cyklus

;------------------------------------------------------------------------------
;			Vektor resetu
;------------------------------------------------------------------------------
			.sect   ".reset"		; vektor resetu MSP430
			.short  RESET
			.end
