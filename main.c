/*
 * main.c
 *
 *  Created on: 12 Sept 2022
 *   Author : Omar Elsherif
 *      
 */

#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>


typedef struct{
	unsigned char sec0;
	unsigned char sec1;
	unsigned char min0;
	unsigned char min1;
	unsigned char hr0;
	unsigned char hr1;
}Time;

Time time={0,0,0,0,0,0};


/*
 * Description:
 * Timer1 will be used with pre-scaler F_CPU/64
 * F_Timer = 15.625 KHz, Ttimer Period = 0.064 ms
 * so we need 15625 counts to get 1sec
 */

#define COMPARE_VALUE 15625

void Timer1_CTC_Init()
{

	/* Configure timer control register TCCR1A
	 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
	 * 2. FOC1A=1 FOC1B=0
	 * 3. CTC Mode WGM10=0 WGM11=0 (Mode Number 4)
	 */
	TCCR1A = (1<<FOC1A);
	TCNT1 = 0;							// Initially 0
	OCR1A = COMPARE_VALUE;				//Set the Compare value
	TIMSK |= (1<<OCIE1A);				 /* Enable Timer1 Compare A Interrupt */

	/* Configure timer control register TCCR1B
	 * 1. CTC Mode WGM12=1 WGM13=0 (Mode Number 4)
	 * 2. Prescaler = F_CPU/64 CS12=0 CS11=1 CS10=1
	 */
	TCCR1B = (1<<WGM12) | (1<<CS11)|(1<<CS10);

}



ISR(TIMER1_COMPA_vect)
{
	time.sec0++;

	if(time.sec0>9)
	{
		time.sec0=0;
		time.sec1++;
	}
	if(time.sec1>5)
	{
		time.sec1=0;
		time.min0++;
	}
	if(time.min0>9)
	{
		time.min0=0;
		time.min1++;
	}
	if(time.min1>5)
	{
		time.min1=0;
		time.hr0++;
	}
	if(time.hr0>9)
	{
		time.hr0=0;
		time.hr1++;
	}

}

void Display_7_segment(unsigned char number,unsigned char enable_id)
{
	PORTC = (PORTC&0xF0) | (number&0x0F);

	PORTA |= (1<<enable_id);		// activate the chosen 7 seg

	_delay_ms(2);
	PORTA &= ~(1<<enable_id);		//disable that 7-seg
}


/*Stop watch time reset*/
ISR(INT0_vect)
{
	time.sec0=0;
	time.sec1=0;
	time.min0=0;
	time.min1=0;
	time.hr0=0;
	time.hr1=0;

	TCNT1 = 0;			// timer begin from 0 again
}

/*Stop watch time pause*/
ISR(INT1_vect)
{
	TCCR1B &= ~(1<<CS11);  		/* stop timer by removing clock source from it */
	TCCR1B &=~(1<<CS12);		// Prescaler = F_CPU/64 CS12=0 CS11=1 CS10=1
	TCCR1B &=~(1<<CS10);
}

/*Stop watch time resume*/
ISR(INT2_vect)
{
	TCCR1B |= (1<<CS11) | (1<<CS10);  		// resume timer by providing clock source to it
}											// CS11=1 , CS10=1



int main()
{
	DDRC|=0x0F;				// first 4 pins of PORTC are o/p
	DDRA|=0x3F;				// first 6-pins in PORTA as the enable/disable pins for the six 7-segments.

	DDRD &=~(1<<PD3);		// stop watch pause button

	DDRD &=~(1<<PD2);		// stop watch reset button
	PORTD |=(1<<PD2);		// INT0 button internal pull up activated

	DDRB &=~(1<<PB2);		// stop watch resume button
	PORTB |=(1<<PB2);		// INT2 button internal pull up activated

	GICR |=(1<<INT1)|(1<<INT0)|(1<<INT2);		// enable external interrupts

	MCUCR |=(1<<ISC01);					// INT0 falling edge
	MCUCR |=(1<<ISC11)|(1<<ISC10);		// INT1 rising edge
	MCUCSR &=~(1<<ISC2);				// INT2 falling edge

	SREG |= (1<<7); 		/* Enable global interrupts in MC */
	Timer1_CTC_Init();

	while(1)
	{
		Display_7_segment(time.sec0,PA0);
		Display_7_segment(time.sec1,PA1);
		Display_7_segment(time.min0,PA2);
		Display_7_segment(time.min1,PA3);
		Display_7_segment(time.hr0,PA4);
		Display_7_segment(time.hr1,PA5);
	}

	return 0;
}


