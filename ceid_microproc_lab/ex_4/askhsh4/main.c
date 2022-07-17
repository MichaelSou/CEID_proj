#include <avr/io.h>
#include <avr/interrupt.h>


int memor[32][4]; //RAM δυσδιαστατος πινακας
int last_input = -1; //μεταβλητη για να γνωριζουμε την τελευταια τιμη που προσθεσαμε.


void LED_init(void)
{
	PORTD.DIR |= PIN0_bm; //PIN is output
	PORTD.OUT |= PIN0_bm; //LED is off
	PORTD.DIR |= PIN1_bm; //PIN is output
	PORTD.OUT |= PIN1_bm; //LED is off
	PORTD.DIR |= PIN2_bm; //PIN is output
	PORTD.OUT |= PIN2_bm; //LED is off
	PORTD.DIR |= PIN3_bm; //PIN is output
	PORTD.OUT |= PIN3_bm; //LED is off
}
void LED0(int status)
{
	if (status == 1)
	{
		/* LED(ON) */
		PORTD.OUT &= ~PIN0_bm;
	}
	if (status == 0)
	{
		/* LED(OFF) */
		PORTD.OUT |= PIN0_bm;
	}
}
void LED1(int status)
{
	if (status == 1)
	{
		/* LED(ON) */
		PORTD.OUT &= ~PIN1_bm;
	}
	if (status == 0)
	{
		/* LED(OFF) */
		PORTD.OUT |= PIN1_bm;
	}
}
void LED2(int status)
{
	if (status == 1)
	{
		/* LED(ON) */
		PORTD.OUT &= ~PIN2_bm;
	}
	if (status == 0)
	{
		/* LED(OFF) */
		PORTD.OUT |= PIN2_bm;
	}
}
void LED3(int status)
{
	if (status == 1)
	{
		/* LED(ON) */
		PORTD.OUT &= ~PIN3_bm;
	}
	if (status == 0)
	{
		/* LED(OFF) */
		PORTD.OUT |= PIN3_bm;
	}
}


//διαβαζουμε απο την μνήμη
void read()
{
	LED0(memor[last_input][0]);
	LED1(memor[last_input][1]);
	LED2(memor[last_input][2]);
	LED3(memor[last_input][3]);
}
//γραφουμε στην μνήμη με μια for και γραφουμε 0 ή 1 καθε θέση
void write()
{
	last_input++;
	for (int i = 0; i < 4; i++)
	{
		memor[last_input][i] = rand() % 2; //η rand μας δινει ή 0 ή 1
	}
}
//cmp0 για το switch5 για να γραψουμε στη μνήμη
ISR(TCA0_CMP0_vect)
{
	//clear the interrupt flag
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS = intflags;
	write();
}

//cmp1 για το switch6 για να διαβασουμε απο την μνήμη
ISR(TCA0_CMP1_vect)
{
	//clear the interrupt flag
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS = intflags;
	read();
}

ISR(PORTF_PORT_vect)
{
	//clear the interrupt flag
	int intflags = PORTF.INTFLAGS;
	PORTF.INTFLAGS=intflags;
	//το 5ο pin εχει την τιμη 0x20 η οποια ειναι το 32 και ελεγχουμε ετσι
	//αν ο χρηστης πατησε το 5ο η το 6ο κουμπι και το interupt εκτελει τον αναλογο κωδικα
	//οταν παταμε το switch του pinctrl5 τοτε κανει write
	if (intflags == 32)
	{
		
		//enable interrupt CMP0
		TCA0.SINGLE.INTCTRL |= TCA_SINGLE_CMP0_bm;
		TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm; //Enable
	}
	//οταν παταμε το switch του pinctrl6 τοτε κανει read
	else{
		
		//enable interrupt CMP1
		TCA0.SINGLE.INTCTRL |= TCA_SINGLE_CMP1_bm;
		TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm; //Enable
	}

}



int main()
{
	LED_init();

	//prescaler=1024
	TCA0.SINGLE.CTRLA=TCA_SINGLE_CLKSEL_DIV1024_gc;
	TCA0.SINGLE.PER = 254; //select the resolution
	TCA0.SINGLE.CMP0 = 127; //select the duty cycle
	TCA0.SINGLE.CMP1 = 60; //select the duty cycle
	//select Single_Slope_PWM
	TCA0.SINGLE.CTRLB |= TCA_SINGLE_WGMODE_SINGLESLOPE_gc;

	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	PORTF.PIN6CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	sei();
	while (1){ }
}

