//�������� �����������.
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#define ped 100
#define del 2 //������ ������������ ������ ��� ��������� ��� ��������.
#define time_to_pass 10 //�������� �������� ������ ������ ����� ������ �����.
int pezos = 0;//��������� ��� �� ������� �� �� ������ ��� ����� ����� �������.

ISR(PORTF_PORT_vect){
	//����������� interrupt flag.
	int intflags = PORTF.INTFLAGS;
	PORTF.INTFLAGS=intflags;
	
	PORTD.OUT= PIN0_bm; //������ ������� �� ������ ��� ������� ������.
	_delay_ms(del);//��������� ����������� ������ ��� ��������.
	PORTD.OUTCLR= PIN1_bm; //������ ������� �� ������ ��� �����.
	PORTD.OUTCLR= PIN2_bm; //������ ������� �� ������ ��� ������ ������.
	pezos = 1;//������ 1 � ��������� ��� �� �� ����� ������� �� ������ ��� �����.
}
ISR(TCA0_CMP0_vect){
	TCA0.SINGLE.CTRLA = 0;
	//����������� �� flag.
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS=intflags;

	PORTD.OUTCLR= PIN1_bm; //������ ������� �� ������ ��� �����.
	PORTD.OUTCLR= PIN2_bm; //������ ������� �� ������ ��� ������ ������.
	_delay_ms(del);//��������� ����������� ������ ��� ��������.
	PORTD.OUT= PIN0_bm; //������ ������� �� ������ ��� ������� ������.

}



int main() {
	PORTD.DIR |= PIN0_bm; //������ ������� ������.
	PORTD.OUTCLR= PIN0_bm; //�� ������ ����� �������.	PORTD.DIR |= PIN1_bm; //������ �����.
	PORTD.OUT= PIN1_bm; //�� ������ ����� �������.	PORTD.DIR |= PIN2_bm; //������ ������ ������.
	PORTD.OUT= PIN2_bm; //�� ������ ����� �������.	//pullup enable and Interrupt enabled with sense on both edges
	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;	sei(); //�������� �� ��������� interrupt signals.

	int car;//��������� ��� ����������� ���������.
	while (1) {
		if (pezos == 1)
		{
			TCA0.SINGLE.CNT = 0; //����������� ��� COUNTER.
			TCA0.SINGLE.CTRLB = 0; //Normal Mode.
			TCA0.SINGLE.CMP0 = ped; //When reaches this value -> interrupt CLOCK FREQUENCY/1024.
			TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc; //(= 0x7<<1 ��� 224 )
			TCA0.SINGLE.CTRLA |=1;//Enable
			TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm; //Interrupt Enable (=0x10).
			pezos = 0;
		}
		//������� ��� ����� ���� ����� �����.
		car = rand(); //� RAND �������� ��� ���������� ��� �� �� ������� �����.
		if(car%10 == 0 || car%10 == 8 || car%10 == 5)//��������� �� � ������� ��������� �� 0, 5, 8.
		{
			PORTD.OUT= PIN0_bm; //������� �� ������ ���� ������ ����� �������.
			_delay_ms(del);//��������� ����������� ������ ��� �������� ��� 2 ��������.
			PORTD.OUTCLR= PIN2_bm; //������� ������� �� ������ ���� ����� �����.
			_delay_ms(time_to_pass); // ��������� ��� ����������� ��� �� �������� �� �������.
			PORTD.OUT= PIN2_bm; //������� ������� �� ������ ���� ����� �����.
			_delay_ms(del);//��������� ����������� ������ ��� �������� ��� 2 ��������.
			PORTD.OUTCLR= PIN0_bm; //������� �� ������ ���� ������ ����� �������.
		}
	}
	cli();//��������� �� ��������� inter�upt signals.
}
