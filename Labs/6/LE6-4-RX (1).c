/*==================================================
AUTHORS				:		CRAIG JOSEPH C. GOC-ONG
DESCRIPTION			:		A program that will
							read the character sent 
							by MCU1 and the character 
							will be displayed on the 
							7-segment display.
DATE				:		APRIL 17, 2024    
==================================================*/

/*==================================================
HEADER FILE
==================================================*/
#include <xc.h>

/*==================================================
MACROS
==================================================*/
#define _XTAL_FREQ 20000000

/*==================================================
CONFIGURATION BITS
==================================================*/
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

/*==================================================
MAIN FUNCTION
==================================================*/
void main(void)
{
	SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
	SYNC = 0; // asynchronous mode (TXSTA reg)
	SPEN = 1; // enable serial port (RCSTA reg)
	RX9 = 0; // 8-bit reception (TXSTA reg)
	BRGH = 1; // asynchronous high-speed (TXSTA reg)
	CREN = 1; // enable continuous receive (RCSTA reg)
	TRISB = 0x00; // set all ports in PORTB to output

	for(;;) // foreground routine
	{
		while(!RCIF); // wait until receive buffer is full
		PORTB = RCREG; // read the receive register
	}
}