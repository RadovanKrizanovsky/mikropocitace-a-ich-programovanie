//***********************************************************
//Radovan Krizanovsky
// Mikropocítae a ich programovanie
// Cvicenie c. 11 a 12
//***********************************************************

#include <msp430.h>
#include "evb2.h"

const unsigned char text1[] = "Mikropocitace a ich"; //"Mikropoc" "\xa0" "tace a ich";
const unsigned char text2[] = "   programovanie   ";

#define LCD_SHIFT_RIGHT 0x1c
#define LCD_SHIFT_LEFT 0x18

void main(void)
{


    char i = 0;

    evb2_io_init();

    lcd_init();
    lcd_clear();

    lcd_puts(&text1[0]);
    lcd_goto(0x40);
    lcd_puts(&text2[0]);

    LCD_SEND_INSTRUCTION; //Idem posielat instrukcie
    lcd_write(0x0c); //zapni displej, vypni blikanie kurzora


    lcd_goto(0x49); //na pozicii 0x49 dáme iný znak

    LCD_SEND_DATA;
    lcd_write(0x24); // na danu poziciu dame znak $
//  lcd_write(0xF2); na ziskanie informacie, ktoru tabulku pouziva nas mikropocitac.
/*
    LCD_SEND_DATA;

    lcd_write(0xA0);

    LCD_SEND_INSTRUCTION;
    lcd_goto(0x02);

    LCD_SEND_DATA;
    lcd_putch('T');

*/
    LCD_RS_L;
    lcd_write(0x0C);

    while(1)
    {
        for(i=0;i<16;i++) // pohyb dolava
        {
            __delay_cycles(250000);
            lcd_write(0x1C);
        }
        for(i=0;i<35;i++) // pohyb doprava
        {
            __delay_cycles(250000);
            lcd_write(0x18);
        }
        for(i=0;i<19;i++) //vratenie sa do stredu
        {
            __delay_cycles(250000);
            lcd_write(0x1C);
        }
    }
}

/*
 * 1) Pracovali sme s pripojenim LCD displayom
 * 2) Odosielane data sa odosielaju po 4 bitoch
 * 3) Kazdy jeden "prestor na znak" ma svoju unikatnu adresu
 * 4) Programovali sme v jazyku C
 * 5) Kedze sa nam text nezmestil na display tak sme ho posuvala po display-i zo strany na stranu
 * 6) Prestudovali sme akou formou su znaky reprezentovane a aj ASCII tabulku prislusneho display-a
 * 7) V header file-i sme mali definovane funkcie s ktorymi sme vedeli intuitivne nastavovat obsah display-u
 * */
