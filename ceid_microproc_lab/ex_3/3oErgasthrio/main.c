#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define ped 20 //���� ��� ��� timer

int reverse = 0; // ���� ����� 1 ������� ��� ������� ������ ��� �� ������ ���� ����
int corners = 0; //��������� ��� ������� ��� ������ ��� ���� �����

//� ��������� ���� �� ��������� ���� 1 ���� ������� ����� ��� �� ��������� ���� 1 ���� ������� ��������
//���� ���� ������ ���� ���� +4 � ���� ���� -4 �� ������� ��� ���� ����� ��� �� ���������� ��� �������� ��� ����� ���� ���� ����
//����� ��� �� ����� �� ���������� ��� �������� ������ �� ������� 360 ������ ��� ��� ������� 4 ����� ��� 90 ������ �� ���� ����� ��� �� ����������
int room_outline_corners= 0;

//�� �������� ����������� ����� ��� ��� ������������ ��� led 
//��� ��� ������� ���� �������������� �� � LED0(1) ������ �� LED ��� pin1

void LED_init(void)
{
	PORTD.DIR |= PIN1_bm; //PIN is output ������
	PORTD.OUT |= PIN1_bm; //LED is off
	PORTD.DIR |= PIN2_bm; //PIN is output ��������
	PORTD.OUT |= PIN2_bm; //LED is off
	PORTD.DIR |= PIN3_bm; //PIN is output �����
	PORTD.OUT |= PIN3_bm; //LED is off
}
void LED0(int status)
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
void LED1(int status)
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
void LED2(int status)
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


//interupts

ISR(ADC0_WCOMP_vect){
	int intflags = ADC0.INTFLAGS;
	ADC0.INTFLAGS = intflags;
	if (reverse == 0)
	{
		LED1(1);
		corners++;//��������� ��� ������ ���� 1
		room_outline_corners--;//������� �������� ����� ������� ������ ��� �� ������ ��� -4

	}
	else
	{
		LED2(1);
		corners--;//��������� ��� ������ ���� 1 ����� ����� ��� ������� ������

	}

	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc;
	TCA0.SINGLE.CTRLA |=1;//Enable
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm; //Interrupt Enable

}

ISR(PORTF_PORT_vect){
	//clear the interrupt flag
	int check = PORTF.INTFLAGS;
	int intflags = PORTF.INTFLAGS;
	PORTF.INTFLAGS=intflags;
	//�� 5� pin ���� ��� ���� 0x20 � ����� ����� �� 32 ��� ��������� ����
	//�� � ������� ������ �� 5� � �� 6� ������ ��� �� interupt ������� ��� ������� ������
	if (intflags == 32)
	{
		if (reverse == 0)
		{
			LED2(1);
			corners++;//��������� ��� ������ ���� 1 ����� �� �������� ��� 8
			room_outline_corners++;//����������� ������ ��� �� ������ ��� +4
		}
		else
		{
			LED1(1);
			corners--;//��������� ��� ������ ���� 1 ����� ����� ��� ������� ������
		}
	}
	//���� ������ �� switch ��� pinctrl6 ���� ������� ���� else ��� ����������� �� �������� ��� ����� ����� 180 ������
	else{
		LED0(1);
		LED1(1);
		LED2(1);
		//������ 180 ������
		reverse=1;
		LED0(0);
		LED1(0);
		LED2(0);

	}
	//timer � ������ �� ����� interupt ��� �� ���������� �� �������
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc;
	TCA0.SINGLE.CTRLA |=1;//Enable
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm; //Interrupt Enable
}

//interupt ��� timer ��� �� ���������� �� ������� (����� � ��������)
ISR(TCA0_CMP0_vect){
	TCA0.SINGLE.CTRLA = 0; //Disable
	//clear flag
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS=intflags;
	LED1(0);
	LED2(0);
}


//main
int main(){
	//������������ ��� LED
	LED_init();

	//������������ ��� timer 
	TCA0.SINGLE.CNT = 0; //clear counter
	TCA0.SINGLE.CTRLB = 0; //Normal Mode (TCA_SINGLE_WGMODE_NORMAL_gc ��� 207)
	TCA0.SINGLE.CMP0 = ped; //When reaches this value -> interrupt CLOCK FREQUENCY/1024


	//������������ ��� ADC ��� Free-Running mode
	ADC0.CTRLA |= ADC_RESSEL_10BIT_gc; //10-bit resolution
	ADC0.CTRLA |= ADC_FREERUN_bm; //Free-Running mode enabled
	ADC0.CTRLA |= ADC_ENABLE_bm; //Enable ADC
	ADC0.MUXPOS |= ADC_MUXPOS_AIN7_gc; //The bit
	//Debug Mode
	ADC0.DBGCTRL |= ADC_DBGRUN_bm;
	//Window Comparator Mode
	ADC0.WINLT |= 10; //Set threshold
	ADC0.INTCTRL |= ADC_WCMP_bm; //������������ Interrupts ��� WCM
	ADC0.CTRLE |= ADC_WINCM0_bm; //Interrupt ���� RESULT < WINLT

	//pullup enable and Interrupt enabled with sense on both edges
	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;//switch �� �������
	PORTF.PIN6CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;//switch ��� �� ����� ������ 180 ������ ��� �� ����� �������� ������
	sei();
	ADC0.COMMAND |= ADC_STCONV_bm; //Start Conversion

	LED0(1);//�������� �� �������� ������


	//�� ��������� ������ ����� �� ����� 8 ������ � �� ����� �� ������� ������ �� ������ ���� ���� ���� (������ ������� ��� ��� ��������� corners ����� �� ����� 0)
	if (reverse == 0 && corners < 8)
	{
		while(1){}
	}
	if (reverse == 1 && corners > 0)
	{
		while (1){}
	}
}

