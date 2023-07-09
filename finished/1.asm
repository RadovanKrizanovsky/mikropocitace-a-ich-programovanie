
;Radovan Križanovský
;Cvicenie 1.
;Mikropocitace a ich programovanie
;Stvrtok 10:00

;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            								; Uklada do FLASHky (Na parne adresy napr 0xC000, 0xC002...)
            								; (napr. .bss by bolo do RAM)

            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Inicializacia stackpointera

;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

StopWDT	mov.w   #WDTPW+WDTHOLD,&WDTCTL	; Zastavenie watchdog-u
										; Moze byt + alebo OR (Nemaju ziadne log. 1ky na tom istom mieste)

SetupP1	bis.b   #01000001b,&P1DIR		; Nastave vystupu pre zelenu(1) a cervenu(7) LEDku (P1.0)

			; SUBEZNE
			;mov.b   #01000001b,&P1OUT	; Zapneme obe LEDky aby menili stav (on/off) subezne

			; STRIEDAVO
			mov.b   #01000000b,&P1OUT	; Zapneme iba jednu LEDku z dvoch aby menili stav (on/off) striedavo

Mainloop	xor.b   #01000001b,&P1OUT	; XORovanie port 1 outputu s bin. hodnotou 01000001 sposobi striedanie hodnot na 1 a 7 bite

			; Na register R15 nastavime desiatkovu hodnotu poctu instrukcii (kvoli zdrzaniu)
Wait		mov.w   #65000,R15 => ; Povodna hodnota
;Wait		mov.w   #32500,R15	=> 2x mensia hodnota = 2x kratsie intervaly (2x rychlesie blikanie) (za predpokladu ze nepouzijeme R12 z nasledujucej ulohy)

			mov.w	#2,R12		; Vzdy ked R15 dojdu instrukcie dekrementujeme R12 a znova "naplinime" R15 (kolko krat chceme zopakovat cely proces => kolko nasobne chceme spomalit)
								; Toto je potrebne pretoze register je len 16 bitovy

L1			dec.w   R15			; Dekrementacia R15
			jnz     L1			; Test: R15 = 0?

			mov.w	#65000,R15	; Opatovne zapisanie hodnoty na R15
			dec.w	R12			; Dekrementacia R12
			jnz		L1			; Navrat k opatovnemu dekrementovaniu R15

			jmp     Mainloop	; Ak R15 > 0, opakujeme cyklus

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Vektor resetu
;-------------------------------------------------------------------------------
											; Procesor po starte zacina vektorom resetu
            .sect   ".reset"                ; Volime sekciu
            .short  RESET					; Zapiseme do nej konstantu ktora ma hodnotu 0xC000

;Zaver:
;1) Prikazmi sme procesoru zadavali ukony pre pracu s LED
;2) Pomocou prednastaveni sme nastavili ktore LED zacinaju ako zasvietene a ktore nie
;3) Vdaka porovnavaniu, dekrementacii a prikazu jmp sme vytvorili jednoduchy cyklus
;4) Binarnou hodnotou na registroch a jej naslednou dekrementaciou sme vedeli regulovat frekvenciu rozsvecovania a zhasinania LED
;5) Podla zadania sme upravovali assemblersky kod a tak docielili rozne konfiguracie blikania LED

            
