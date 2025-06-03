/*==================================================
AUTHORS				:		CRAIG JOSEPH C. GOC-ONG
DESCRIPTION			:		A program that will read 
							the temperature and 
							relative humidity data 
							and display it on the
							LCD in real-time.
DATE				:		MAY 6, 2024    
==================================================*/

#include <xc.h>
#include <math.h>
#include <stdio.h>

#define _XTAL_FREQ 4000000

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF


void init_I2C_Master(void)
{
	TRISC3 = 1; // set RC3 (SCL) to input
	TRISC4 = 1; // set RC4 (SDA) to input
	SSPCON = 0x28; // SSP enabled, I2C master mode
	SSPCON2 = 0x00; // start condition idle, stop condition idle
	// receive idle
	SSPSTAT = 0x00; // slew rate enabled
	SSPADD = 0x09; // clock frequency at 100 KHz (FOSC = 4MHz)
}
void I2C_Wait(void)
{
	/* wait until all I2C operation are finished*/
	while((SSPCON2 & 0x1F) || (SSPSTAT & 0x04));
}
void I2C_Start(void)
{
	/* wait until all I2C operation are finished*/
	I2C_Wait();
	/* enable start condition */
	SEN = 1; // SSPCON2
}
void I2C_Repeated_Start(void)
{
	/* wait until all I2C operation are finished*/
	I2C_Wait();
	/* enable start condition */
	RSEN = 1; // SSPCON2
}
void I2C_Stop(void)
{
	/* wait until all I2C operation are finished*/
	I2C_Wait();
	/* enable stop condition */
	PEN = 1; // SSPCON2
}
void I2C_Send(unsigned char data)
{
	/* wait until all I2C operation are finished*/
	I2C_Wait();
	/* write data to buffer and transmit */
	SSPBUF = data;
}
unsigned char I2C_Receive(unsigned char ack)
{
	unsigned char temp;
	I2C_Wait(); // wait until all I2C operation are finished
	RCEN = 1; // enable receive (SSPCON2 reg)
	I2C_Wait(); // wait until all I2C operation are finished
	temp = SSPBUF; // read SSP buffer
	I2C_Wait(); // wait until all I2C operation are finished
	ACKDT = (ack)?0:1; // set acknowledge (ACK) or not acknowledge (NACK)
	ACKEN = 1; // enable acknowledge sequence
	return temp;
}
void instCtrl(unsigned int data )
{	
	PORTD = data;
	__delay_ms(50);
	PORTB = 0b00000010;
	__delay_ms(50);
	PORTB = 0b00000000;	
}
void initLCD()
{
	
	__delay_ms(50);
	instCtrl(0x38);
	instCtrl(0x08);
	instCtrl(0x01);
	instCtrl(0x06);
	instCtrl(0x0E);

}

void dataCtrl(unsigned int data )
{	

	PORTD = data;
	PORTB = 0b00000011;
	__delay_ms(50);
	PORTB = 0b00000001;	
}

void shtSensor()
{
	int resolution = (int)(pow(2,14));
	unsigned int RH_temp = 0x0000, T_temp = 0x0000;	
	unsigned char temp = 0x00;
	int RH;
	float T;
//	int resolution = (int)(pow(2,14));
	I2C_Start(); // initiate start condition
	I2C_Send(0x80); // send the slave address + write
	I2C_Send(0xE5); // send 8-bit command
	I2C_Repeated_Start(); //initiate repeated start condition
	I2C_Send(0x81); // send the slave address + write
	RH_temp = I2C_Receive(1); //receive the first 8-bit frame from slave + ACK
	temp = I2C_Receive(0); // receive the second 8-bit frame from slave + NACK
	I2C_Stop(); // initiate stop condition
	__delay_ms(200); // delay before next operation

		//Getting the right values
		RH_temp = RH_temp << 6; 	// shift MSB left by 6 bits to align for combination
		temp = temp >> 2; 			// shift LSB right by 2 bits to align for combination and remove the 2 lsb
		RH_temp = RH_temp | temp; 	// combine MSB and LSB combining 14 bit
		
		//Calculation
		RH = (int)(-6+(125*((float)RH_temp/(float)resolution)));
		
		instCtrl(0x80);
		dataCtrl('H');
		dataCtrl('U');
		dataCtrl('M');
		dataCtrl('I');
		dataCtrl('D');
		dataCtrl('I');
		dataCtrl('T');
		dataCtrl('Y');
		dataCtrl(':');
		char valRH[4] = {' ',' ',' ',' '};
		sprintf(valRH, "%d", RH);
	    dataCtrl(valRH[0]);
	    dataCtrl(valRH[1]);
	    dataCtrl(valRH[2]);
		dataCtrl('%');
		
		temp = 0x00; // resetting for next use
	
	I2C_Start(); // iniate start condition
	I2C_Send(0x80);	// send the slave address + write
	I2C_Send(0xE3); // send 8-bit command
	I2C_Repeated_Start(); // repeated start condition
	I2C_Send(0x81); // send the slave address + read
	T_temp = I2C_Receive(1);// receive the first 8-bit frame from slave + ACK
	temp = I2C_Receive(0); // receive the first 8-bit frame from slave + NACK
	I2C_Stop(); // initiate stop condition
	__delay_ms(200); // delay

		//Getting the right values
		T_temp = T_temp << 6; 	// shift MSB left by 6 bits to align for combination
		temp = temp >> 2; 			// shift LSB right by 2 bits to align for combination and remove the 2 lsb
		T_temp = T_temp | temp; 	// combine MSB and LSB combining 14 bit

		T =  (float)(-46.85+(175.72*((float)T_temp/(float)resolution)));

		instCtrl(0xC0);
		dataCtrl('T');
		dataCtrl('E');
		dataCtrl('M');
		dataCtrl('P');
		dataCtrl('E');
		dataCtrl('R');
		dataCtrl('A');
		dataCtrl('T');
		dataCtrl('U');
		dataCtrl('R');
		dataCtrl('E');
		dataCtrl(':');
		

		char valT[5] = {' ',' ',' ',' '};
		sprintf(valT, "%f", T);		    
		dataCtrl(valT[0]);
	    dataCtrl(valT[1]);
	    dataCtrl(valT[2]);
		dataCtrl(valT[3]);
		dataCtrl(valT[4]);
		dataCtrl('C');
}
/* Master Device*/
void main(void)
{
	TRISB = 0x00; // set all bits in PORTB to output
	PORTB = 0x00; // all LEDs in PORTB are off
	TRISD = 0x00; // set all bits in PORTD to input
	PORTD = 0x00; // all LEDs in PORTB are off
	init_I2C_Master(); // initialize I2C as master
	initLCD();
	for(;;)
	{
		shtSensor();
	}
}