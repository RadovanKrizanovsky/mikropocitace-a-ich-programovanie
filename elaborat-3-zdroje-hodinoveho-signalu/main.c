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
		
		BCSCTL3 &= ~(LFXT1S1|LFXT1S0);	//prepnutie medzi VLO a LFXT1
										//prepnutie na LFXT1
		BCSCTL2 &= ~(SELM1|SELM0);		//medzi DCOCLK a VLOorLFXT1 pre MCLK
										// DCOCLK -> MCLK
		BCSCTL2 &= ~(DIVM1|DIVM0);		// delicka MCLK 1:1
		


		blink(5, 60000); // 5-krat preblikni LED - cerv-zel-cerv-zel-cerv-zel-cerv-zel-cerv-zel
							//cislo 20000 nevyjadruje cas ale pocet vykonani funkcie "void delay(int j);"

		delay(60000);	// svit LED sa chvilu nemeni, (ostava svietit zelena)
		delay(60000);
// DRUHE nastavenie zdroja hodinoveho signalu (DCO/2)->MCLK
		
		// vydelime DCOCLK dvoma, ostatne nastavenia su
		//rovnake z predosleho	
		
// TRETIE nastavenie zdroja hodinoveho signalu LFXT1->MCLK
		
		
// STVRTE nastavenie zdroja hodinoveho signalu VLO->MCLK


			P1OUT = 0x00; // zhasni obe LED
			delay(60000); //@ fMCLK = 1000kHz
						  // a nechaj chvilu zhasnute (aby sme videli,
						  //ze program sa nachadza prave tu)
			delay(60000);
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
