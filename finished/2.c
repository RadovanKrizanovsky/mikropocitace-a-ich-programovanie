/*
 * Radovan Križanovský
 */


/*
 * Mikropocitace a ich programovanie
 * cvicenie c. 2 softvérový test tlaèítka na porte
 */

#include  <msp430.h>

void onesk(unsigned int i); //deklaracia funkcie onesk

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;  // Zastav casovac watchdog-u
  P1DIR |= 0x41;             // Nastav piny P1.0 and P1.6 do vystupneho modu
  P1DIR &= 0xF7;			 // Nastav pin P1.3 do vstupnej funkcie

  unsigned int j = 0;
  for (;;) //nekonecny cyklus
  {

    while(P1IN & BIT3);//1. Stlacenie
                //Stlacil som

    //Po 1. stlaceni sa nedeje nic

    onesk(2); //oneskorenie kvoli fyzickym vlastnostiam spinaca
    while(~P1IN & BIT3);//1. Pustenie
                //Pustil som

    P1OUT = 0x40; // Po 1. pusteni sa rozsviety cervena LED 0x40 = 01000000b

  	while(P1IN & BIT3);//2. Stlacenie
  				//Stlacil som
  	P1OUT = 0x01; // Zhasne cervena, rozsvieti sa zelena

  	onesk(2); //oneskorenie kvoli fyzickym vlastnostiam spinaca
  	while(~P1IN & BIT3);//2. Pustenie
  				//Pustil som

  	onesk(2);
  	while(P1IN & BIT3);//3. Stlacenie
  	           //Stlacil som
  	P1OUT = 0x00; //Obe LED zhasnu

  	onesk(2);
	while(~P1IN & BIT3);//3. Pustenie
	 	 	 	//Pustil som
				// LED su zhasnute

	onesk(2);
	while(P1IN & BIT3);//4. Stlacenie
	            // Stlacil som

  	for(j=0;j<4;j++) // 4x blikne zelena / cervena
  	{
  		P1OUT = 0x01;
  		onesk(50000);
  		P1OUT = 0x40;
  		onesk(50000);

  	}
  	P1OUT = 0x00; //Po blikani zhasnu obe

  	//onesk(2); netreba, pretoze 'for'
  	while(~P1IN & BIT3);//4. Pustenie
  	            //Pustil som

  	//Po 4. pusteni sa nedeje nic

    while(P1IN & BIT3);//5. Stlacenie
               //Stlacil som

    //Po 5. stlaceni sa nedeje nic

    onesk(2);
    while(~P1IN & BIT3);//5. Pustenie
                //Pustil som

    P1OUT = 0x40; // Zapne sa cervena LED 0x40 = 01000000b


  }
}

// Cakacia funkcia
void onesk(unsigned int i)   //definicia funkcie onesk
{
	do {(i--);
	    asm(" nop"); //musí by 1 medzera pred nop aby to chápal ako inštrukciu a nie ako návestie
	}
    while (i != 0);
}

/*
 * ZAVER:
 * 1)Kod sme pisali v programovacom jazyku C
 * 2)While loop-om sme cakali na stlacenie tlacidla
 * 3)Nasledne sme P1OUT priradzovali hexadecimalnu hodnotu
 * 4)Least significant bitom sme kontrolovali zelenu led a siestym(z prava od indexu 0) zasa cervenu
 * 5)For loop-om a oneskorenim sme docielili striedave blikanie
 * 6)Na konci resp. pred novym zaciatkom while-u zapneme cervenu LED
 */
