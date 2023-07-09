/*
 * Mikropocitace a ich programovanie
 * cvicenie c. 2 softv�rov� test tla��tka na porte
*/

#include  <msp430.h>

void onesk(unsigned int i); //deklaracia funkcie onesk

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;  // Zastav casovac watchdog-u
  P1DIR |= 0x41;             // Nastav piny P1.0 and P1.6 do vystupneho modu
  P1DIR &= 0xF7;			 // Nastav pin P1.3 do vstupnej funkcie
  	  	  	  	  	  	  	 //(zbytocny prikaz. Preco?)
  unsigned int j = 0; //kolko bitovy je short integer?
  for (;;) //nekonecny cyklus
  {
  	P1OUT = 0x00;

  	while(P1IN & BIT3); //test pustene tl, test na log.1 z P1.3
  				//stlacil som
  	P1OUT = 0x01;

  	onesk(2); //nasledujuci test spusti a� pochvili, preco?
  	while(~P1IN & BIT3); //test stlacene tl
  				//pustil som
  				// led stale svieti

  	onesk(2);
  	while(P1IN & BIT3); //test pustene tl
  	           //stlacil som
  	P1OUT = 0x00; //LED zhasne

  	onesk(2);
	while(~P1IN & BIT3);//test stlacene tl
	 	 	 	//pustil som
				// led stale zhasnuta

	onesk(2);
	while(P1IN & BIT3); //test pustene tl
				// stlacil som
				// led stale zhasnuta

	//onesk(2); netreba, je tam 'for'
  	
  	for(j=0;j<5;j++)
  	{
  		P1OUT = 0x41;
  		onesk(50000);
  		//50000 je na signed integer
  		// uz prilis velka
  		// na unsigned integer este nie
  		P1OUT = 0x00;
  		onesk(50000);
  	}
  	while(~P1IN & BIT3); // test stlacene
  }
}

// Cakacia funkcia
void onesk(unsigned int i)   //definicia funkcie onesk
{
	do {(i--);
	//asm(" nop"); //mus� by� 1 medzera pred nop aby to ch�pal ako in�trukciu a nie ako n�vestie
	}
    while (i != 0);
}
