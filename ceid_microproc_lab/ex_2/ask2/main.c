//пяосхгйг бибкиохгйым.
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#define ped 100
#define del 2 //вяомос йахустеягсгс летану тгс емаккацгс тым жамаяиым.
#define time_to_pass 10 //диаяйеиа жамаяиоу лийяоу дяолоу выяис жамаяи пефым.
int pezos = 0;//летабкгтг циа ма неяоуле ам то жамаяи тым пефым еимаи пяасимо.

ISR(PORTF_PORT_vect){
	//йахаяифоуле interrupt flag.
	int intflags = PORTF.INTFLAGS;
	PORTF.INTFLAGS=intflags;
	
	PORTD.OUT= PIN0_bm; //амабеи йоййимо то жамаяи тоу лецакоу дяолоу.
	_delay_ms(del);//еисацоуле йахустеягсг летану тым жамаяиым.
	PORTD.OUTCLR= PIN1_bm; //амабеи пяасимо то жамаяи тым пефым.
	PORTD.OUTCLR= PIN2_bm; //амабеи пяасимо то жамаяи тоу лийяоу дяолоу.
	pezos = 1;//цимете 1 г летабкгтг циа то ам еимаи амалемо то жамаяи тым пефым.
}
ISR(TCA0_CMP0_vect){
	TCA0.SINGLE.CTRLA = 0;
	//йахаяифоуле то flag.
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS=intflags;

	PORTD.OUTCLR= PIN1_bm; //амабеи йоййимо то жамаяи тым пефым.
	PORTD.OUTCLR= PIN2_bm; //амабеи йоййимо то жамаяи тоу лийяоу дяолоу.
	_delay_ms(del);//еисацоуле йахустеягсг летану тым жамаяиым.
	PORTD.OUT= PIN0_bm; //амабеи пяасимо то жамаяи тоу лецакоу дяолоу.

}



int main() {
	PORTD.DIR |= PIN0_bm; //жамаяи лецакоу дяолоу.
	PORTD.OUTCLR= PIN0_bm; //то жамаяи еимаи пяасимо.	PORTD.DIR |= PIN1_bm; //жамаяи пефым.
	PORTD.OUT= PIN1_bm; //то жамаяи еимаи йоййимо.	PORTD.DIR |= PIN2_bm; //жамаяи лийяоу дяолоу.
	PORTD.OUT= PIN2_bm; //то жамаяи еимаи йоййимо.	//pullup enable and Interrupt enabled with sense on both edges
	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;	sei(); //нейимале ма деволасте interrupt signals.

	int car;//летабкгтг циа пяосолоиысг аисхгтгяа.
	while (1) {
		if (pezos == 1)
		{
			TCA0.SINGLE.CNT = 0; //лгдемифоуле том COUNTER.
			TCA0.SINGLE.CTRLB = 0; //Normal Mode.
			TCA0.SINGLE.CMP0 = ped; //When reaches this value -> interrupt CLOCK FREQUENCY/1024.
			TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc; //(= 0x7<<1 СЕК 224 )
			TCA0.SINGLE.CTRLA |=1;//Enable
			TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm; //Interrupt Enable (=0x10).
			pezos = 0;
		}
		//екецвос циа алани стом лийяо дяоло.
		car = rand(); //г RAND доукеуеи сам аисхгтгяас циа то ам упаявеи алани.
		if(car%10 == 0 || car%10 == 8 || car%10 == 5)//екецвоуле ам о аяихлос текеиымеи се 0, 5, 8.
		{
			PORTD.OUT= PIN0_bm; //йамоуле то жамаяи стом лецако дяоло йоййимо.
			_delay_ms(del);//еисацоуле йахустеягсг летану тгс емакацгс тым 2 жамаяиым.
			PORTD.OUTCLR= PIN2_bm; //йамоуле пяасимо то жамаяи стом лийяо дяоло.
			_delay_ms(time_to_pass); // еисацоуле лиа йахустеягсг циа ма пеяасоум та овглата.
			PORTD.OUT= PIN2_bm; //йамоуле йоййимо то жамаяи стом лийяо дяоло.
			_delay_ms(del);//еисацоуле йахустеягсг летану тгс емакацгс тым 2 жамаяиым.
			PORTD.OUTCLR= PIN0_bm; //йамоуле то жамаяи стом лецако дяоло пяасимо.
		}
	}
	cli();//сталатале ма деволасте interЯupt signals.
}
