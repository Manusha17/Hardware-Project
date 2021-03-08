/*
 * GccApplication7.c
 *
 * Created: 5/20/2018 11:42:40 PM
 * Author : Manusha
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#define F_CPU 1500000UL
/*Defines a macro for the delay.h header file. F_CPU is the microcontroller frequency value for the delay.h header file. Default value of F_CPU in delay.h header file is 1000000(1MHz)*/


/*Includes delay.h header file which defines two functions, _delay_ms (millisecond delay) and _delay_us (microsecond delay)*/

/*Timer Counter 0 Function Declarations*/
void set_timercounter0_mode(unsigned char mode);
void set_timercounter0_prescaler(unsigned char prescaler_value);
void set_timercounter0_output_mode(unsigned char output_mode);
void set_timercounter0_compare_value(unsigned char compare_value);

/*Timer Counter 2 Function Declarations*/
void set_timercounter2_mode(unsigned char mode);
void set_timercounter2_prescaler(unsigned char prescaler_value);
void set_timercounter2_output_mode(unsigned char output_mode);
void set_timercounter2_compare_value(unsigned char compare_value);



int main(void)
{
	char dataIn;
	usart_init(9600);
	int count=0;
	int countOkay=1;
	int val=5;
	int distance=0;
	char s[5];
	char f[5];
	int focal;
	int total=0;
	DDRC=0x00;
	DDRB=0x08;
	/* PB3 pin of PORTB is declared output (PWM1 pin of DC Motor Driver is connected) */
	//PORTB=0b00001000;
	DDRD=0x80;
	// DDRD=0b10000000;
	/* PD7 pin of PORTD is declared output (PWM2 pin of DC Motor Driver is connected) */
	DDRC=0x0f;
    	DDRA=0xff;
	/*All pins of PortA are declared output ( i/p1,i/p2,i/p3 and i/p4 pins of DC Motor Drivers are connected)*/
	set_timercounter0_mode(3);
	/*Timer counter 0 is set to fast pwm mode*/

	set_timercounter0_prescaler(4);
	/*Timer counter 0 frequency is set to 3.90625KHz*/

	set_timercounter0_output_mode(2);
	/*Timer counter 0 output mode is set  to non-inverting mode*/

	set_timercounter2_mode(3);
	/*Timer counter 2 is set to fast pwm mode*/

	set_timercounter2_prescaler(6);
	/*Timer counter 2 frequency is set to 3.90625KHz*/

	set_timercounter2_output_mode(2); 
	/*Timer counter 2 output mode is set to non-inverting mode*/
	
	void length(){
	while(count < 20){
		if(bit_is_set(PIND,3))  //black strip
		{
			if(countOkay==1){
				count++;
				total++;
				countOkay=0;
			}
			} else {
			if(countOkay==0){
				count++;
				total++;
				countOkay=1;
			}
		}
		
		if(count==20){
			distance += val*(count);
			focal=1/(1/600+10/distance);
			itoa(distance/10,s,10);
			itoa(focal,f,10);
			usart_string_transmit("Distance = ");
			usart_string_transmit(s);
			usart_string_transmit("cm\n");
		//	usart_string_transmit("Focal length = ");
		//	usart_string_transmit(f);
		//	usart_string_transmit("cm\n");
			count=0;
			PORTC=0x00;
			PORTA=0x00;
			break;
		}
		if(dataIn=='3'){
			PORTA=0x00;
			break;
		}
		_delay_ms(30);
    }
    }
	
	void length2(){
		while(total>=0){
			if(bit_is_set(PIND,3))  //black strip
			{
				if(countOkay==1){
					total--;
					countOkay=0;
				}
				} else {
				if(countOkay==0){
					total--;
					countOkay=1;
				}
			}
			
			if(total==2){
				usart_string_transmit("Distance = 0cm\n");
				distance=0;
				PORTC=0x00;
				PORTA=0x00;
				break;
			}
			_delay_ms(30);
		}
	}
	
	while(1)
	{
		
		dataIn=usart_data_receive();   //get input from bluetooth module
		if(dataIn == '1'){
			

			  PORTA=0b01010101;
			/*Robot will move forward in forward direction*/
			
			set_timercounter0_compare_value(80);
			/*OCR2 register value is set to 100*/
			
			set_timercounter2_compare_value(80);
			/*OCR2 register value is set to 100*/
			length();
			
			
			}else if(dataIn=='2'){
				 PORTA=0b10101010;
				 
				 set_timercounter0_compare_value(90);
				 /*OCR2 register value is set to 90*/
				 
				 set_timercounter2_compare_value(90);
				 /*OCR2 register value is set to 90*/
				 length2();
			//	 _delay_ms(2000);
				 
		
		}else{
			PORTC=0x00;
			PORTA=0x00;
		}
		
	}
}
/*End of program*/


/*Timer Counter 0 Function Definitions*/
void set_timercounter0_mode(unsigned char mode)
{
	if(mode==0)
	{
		TCCR0|=0;
	}
	else if(mode==1)
	{
		TCCR0|=_BV(WGM00);
	}
	else if(mode==2)
	{
		TCCR0|=_BV(WGM01);
	}
	else if(mode==3)
	{
		TCCR0|=_BV(WGM00)|_BV(WGM01);
	}
	else
	;

}

void set_timercounter0_prescaler(unsigned char prescaler_value)
{
	TCCR0&=~(0x07);
	TCCR0|=prescaler_value;
}

void set_timercounter0_output_mode(unsigned char output_mode)
{
	TCCR0|=output_mode<<4;
}

void enable_timercounter0_interrupt(unsigned char interrupt_type)
{
	TIMSK|=1<<interrupt_type;
}

void disable_timercounter0_interrupt(unsigned char interrupt_type)
{
	TIMSK&=~(1<<interrupt_type);
}

void set_timercounter0_compare_value(unsigned char compare_value)
{
	OCR0=compare_value;
}

/*Timer Counter 2 Function Definitions*/
void set_timercounter2_mode(unsigned char mode)
{
	if(mode==0)
	{
		TCCR2|=0;
	}
	else if(mode==1)
	{
		TCCR2|=_BV(WGM20);
	}
	else if(mode==2)
	{
		TCCR2|=_BV(WGM21);
	}
	else if(mode==3)
	{
		TCCR2|=_BV(WGM20)|_BV(WGM21);
	}
	else
	;

}

void set_timercounter2_prescaler(unsigned char prescaler_value)
{
	TCCR2&=~(0x07);
	TCCR2|=prescaler_value;
}

void set_timercounter2_output_mode(unsigned char output_mode)
{
	TCCR2|=output_mode<<4;
}

void enable_timercounter2_interrupt(unsigned char interrupt_type)
{
	TIMSK|=1<<(interrupt_type+6);
}

void disable_timercounter2_interrupt(unsigned char interrupt_type)
{
	TIMSK&=~(1<<(interrupt_type+6));
}

void set_timercounter2_compare_value(unsigned char compare_value)
{
	OCR2=compare_value;
}
