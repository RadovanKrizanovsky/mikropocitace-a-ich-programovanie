
//Radovan Križanovský

//Mikropocitace a ich programovanie
//******************************************************************************
// Priklad nastavenia modulu generovania hodinovych signalov
//******************************************************************************

#include <msp430.h>

short int i;

void blink(char n, unsigned int cykly);
void delay(unsigned int j);

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; // zastavenie casovaca watchdog

	P1DIR |= 0x41;           //nastav piny portu P1.0 and P1.6 ako vystupne,
							//funkcia ostatnych pinov portu sa nemeni

	P1OUT = 0x01;		// log.1 na P1.0 a log.0 na vsetky ostatne piny, 
						//teda aj na P1.6

// Kalibracia oscilatora DCO na DCO_FREQ = 1MHz a jeho zapnutie

	BCSCTL1 = CALBC1_1MHZ;		 
	DCOCTL = CALDCO_1MHZ;		
	__bic_SR_register(SCG0);    //zapnutie DCO
								//intrizicka funkcia
	

// Test rozbehu krystalom riadeneho oscilatora LFXT1OSC

	__bic_SR_register(OSCOFF); // zapnutie LFXT1 , pre istotu
	do {
	   IFG1 &= ~OFIFG;
	   __delay_cycles(50);
	   } while((IFG1&OFIFG)); // cakanie na rozbeh LFXT1


while(1){
		
//PRVE nastvenie zdroja hodinoveho signalu DCO->MCLK
		

        //1Mhz
		BCSCTL3 &= ~(LFXT1S1|LFXT1S0);	//prepnutie medzi VLO a LFXT1
										//prepnutie na LFXT1
		BCSCTL2 &= ~(SELM1|SELM0);		//medzi DCOCLK a VLOorLFXT1 pre MCLK
										// DCOCLK -> MCLK
		BCSCTL2 &= ~(DIVM1|DIVM0);		// delicka MCLK 1:1 00
		


		blink(5, 60000); // 5-krat preblikni LED - cerv-zel-cerv-zel-cerv-zel-cerv-zel-cerv-zel
							//cislo 20000 nevyjadruje cas ale pocet vykonani funkcie "void delay(int j);"

		delay(60000);	// svit LED sa chvilu nemeni, (ostava svietit zelena)
		delay(60000);
// DRUHE nastavenie zdroja hodinoveho signalu (DCO/2)->MCLK
		
		//500Khz
		// delenie na 2 (f/2)
		BCSCTL2 |= (DIVM0);  //nastavi 0ty na 1
		BCSCTL2 &= ~(DIVM1); //nastavi 1vy na 0


        blink(5, 30000); // 5-krat preblikni LED - cerv-zel-cerv-zel-cerv-zel-cerv-zel-cerv-zel
                            //cislo 20000 nevyjadruje cas ale pocet vykonani funkcie "void delay(int j);"

        delay(30000);   // svit LED sa chvilu nemeni, (ostava svietit zelena)
        delay(30000);

        IFG1 &= ~OFIFG; // vynuluje nulty flag aby to slo aj pri druhom+ cykle

		// vydelime DCOCLK dvoma, ostatne nastavenia su
		//rovnake z predosleho	
		
// TRETIE nastavenie zdroja hodinoveho signalu LFXT1->MCLK


       //32768Hz
        BCSCTL2 &= ~(SELM0); //nastavi 0ty na 0
        BCSCTL2 |= (SELM1); //nastavi 1vy na 1
        BCSCTL2 &= ~(DIVM1|DIVM0);      // delicka MCLK 1:1 00
		
        blink(5, 1966); // 5-krat preblikni LED - cerv-zel-cerv-zel-cerv-zel-cerv-zel-cerv-zel
                            //cislo 20000 nevyjadruje cas ale pocet vykonani funkcie "void delay(int j);"

        delay(1966);   // svit LED sa chvilu nemeni, (ostava svietit zelena)
        delay(1966);

// STVRTE nastavenie zdroja hodinoveho signalu VLO->MCLK

            //12Khz
            BCSCTL3 &= ~(LFXT1S0); //nastavi 0ty na 0
            BCSCTL3 |= (LFXT1S1); //nastavi 1vy na 1

            blink(5, 720); // 5-krat preblikni LED - cerv-zel-cerv-zel-cerv-zel-cerv-zel-cerv-zel
                            //cislo 20000 nevyjadruje cas ale pocet vykonani funkcie "void delay(int j);"

            delay(720);   // svit LED sa chvilu nemeni, (ostava svietit zelena)
            delay(720);

			P1OUT = 0x00; // zhasni obe LED
			delay(720); //@ fMCLK = 1000kHz
						  // a nechaj chvilu zhasnute (aby sme videli,
						  //ze program sa nachadza prave tu)
			delay(720);
			P1OUT = 0x01; // zapni zelenu LED
	}
}

void blink(char n, unsigned int cykly)
{
	for(i=0;i<n;i++){
	    delay(cykly);
		P1OUT ^= 0x41; //bitova operacia exclusive OR.
						//0x41=0b01000001, instrukcia precita stav vsetkych pinov portu P1
						//nacitane hodnoty bit po bite X-OR -uje s 0x41, vysledok zapise na vsetky piny portu
						//co svietilo - zhasne, co bolo zhasnute zasvieti. Ale len na bitovej pozicii .0 a .6
						//teda tam, kde operand 0x41 obsahuje log. 1
		delay(cykly);
		P1OUT ^= 0x41;
}
	
}

void delay(unsigned int j)
{
	do {(j--);
	asm(" nop");	//funkcia musi obsahovat nejaku zmysluplnu instrukciu - staci aj assemblerovska " nop"
				//inak prekladac funkciu "void delay(long int j)" vobec neprelozi a program ju nebude
				//nikdy volat
	}
	while (j != 0);
}


/*
 * Zaver:
 * 0) Kód sme písali v programovacom jazyku C
 * 1) Boli sme oboznámeni s rôznymi zdrojmi hodinového signálu s rôznymi frekvenciami
 * 2) Používali sme funkciu blink() na blikanie lediek n krát a funkciu delay() kvôli spomaleniu chodu programu a blikania
 * 3) Pomocou BCSCTL (basic clock system control) sme menili hodnoty tak, aby zodpovedali potrebnej binárnej hodnote pridelenej konkretnej frekvencií
 * 4) Testovali sme beh programu s rôznymi zdrojmi a frekveciami CLK signálu
 * 5) Pri rôznych frekvenciách sme zmenili delay tak aby aj s rozlièními osciláciami èasti programu bežali zhruba rovnako
 * 6) Na záver sme zistili že program dôsledkom nestabilného prechodu aktivuje bit na nultom flagu a ten musíme znova vynulovat
 */
