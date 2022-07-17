#include <avr/io.h>
#include <avr/interrupt.h>


int memor[32][4]; //RAM ������������ �������
int last_input = -1; //��������� ��� �� ���������� ��� ��������� ���� ��� ����������.


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


//���������� ��� ��� �����
void read()
{
	LED0(memor[last_input][0]);
	LED1(memor[last_input][1]);
	LED2(memor[last_input][2]);
	LED3(memor[last_input][3]);
}
//�������� ���� ����� �� ��� for ��� �������� 0 � 1 ���� ����
void write()
{
	last_input++;
	for (int i = 0; i < 4; i++)
	{
		memor[last_input][i] = rand() % 2; //� rand ��� ����� � 0 � 1
	}
}
//cmp0 ��� �� switch5 ��� �� �������� ��� �����
ISR(TCA0_CMP0_vect)
{
	//clear the interrupt flag
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS = intflags;
	write();
}

//cmp1 ��� �� switch6 ��� �� ���������� ��� ��� �����
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
	//�� 5� pin ���� ��� ���� 0x20 � ����� ����� �� 32 ��� ��������� ����
	//�� � ������� ������ �� 5� � �� 6� ������ ��� �� interupt ������� ��� ������� ������
	//���� ������ �� switch ��� pinctrl5 ���� ����� write
	if (intflags == 32)
	{
		
		//enable interrupt CMP0
		TCA0.SINGLE.INTCTRL |= TCA_SINGLE_CMP0_bm;
		TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm; //Enable
	}
	//���� ������ �� switch ��� pinctrl6 ���� ����� read
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

