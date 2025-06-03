/*==================================================
AUTHORS				:		CRAIG JOSEPH C. GOC-ONG
DESCRIPTION			:		A program that will send a 
							string to the the virtual 
							terminal when a key in 
							the keypad is pressed.
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
GLOBAL VARIABLES
==================================================*/
char data[] = "You pressed: ";

/*==================================================
FUNCTIONS
==================================================*/

//Function to getting keypad value
unsigned int keypadValue()
{
    unsigned int keypad[16] = {0x01,0x02,0x03,0x00,0x04,0x05,0x06,0x00,0x07,0x08,0x09,0x00,0x0D,0x0E,0xF};
    unsigned int input = PORTB & 0x0F;
    return keypad[input];
}

//Function to send a character to TXREG
void sendCharacter(char c) { 
    while(!TRMT); 
    TXREG = c;
}

// Function to point out each char from the array
// and send the value to sendCharacter() fucntion
void sendString(const char *str) {
    while (*str != '\0') {
        sendCharacter(*str);
        str++;
    }
}

/*==================================================
MAIN FUNCTION
==================================================*/
void main ()
{
    TRISB = 0x1F;

    SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed

    SYNC = 0; // asynchronous mode (TXSTA reg)
    SPEN = 1; // enable serial port (RCSTA reg)
    TX9 = 0; // 8-bit transmission (TXSTA reg)
    BRGH = 1; // asynchronous high-speed (TXSTA reg)
    TXEN = 1; // transmit enable (TXSTA reg)
	int i = 0;
	  while(1) {
	        unsigned int key = keypadValue(); 
	
		if(RB4 = 1)// Checking Datavlb
		{
		        if (key != 0x00) //Filtering
			{  
		            sendString(data); // Send "You pressed: "
		
		            char keyChar;
		            if (key <= 9) 
			    {
		                keyChar = key + '0';  
		            } else 
			    {
		                keyChar = key - 10 + 'A'; 
		            }
					if (key == 13)
						keyChar = '*';
					if (key == 14)
						keyChar = '0';
					if (key == 15)
						keyChar = '#';

		            sendCharacter(keyChar); 
		
		            sendCharacter('\r');   
		        }
		
		        __delay_ms(40); 
		  }
		}
}