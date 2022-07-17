#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define ped 20 //τιμή για τον timer

int reverse = 0; // οταν γίνει 1 αρχίζει την ανάποδη πορεια για να φτάσει στην αρχη
int corners = 0; //μεταβλητή που μετράει τις γωνίες που εχει κάνει

//η μεταβλητή αυτη θα αυξάνεται κατα 1 οταν στρίβει δεξιά και θα μειώνεται κατα 1 οταν στρίβει αριστερά
//έτσι οταν φτάσει στην τιμή +4 ή στην τιμή -4 θα ξερουμε οτι εχει κανει ολο το περιγραμμα του δωματίου και ειναι πάλι στην αρχή
//γιατί για να κανει το περίγραμμα του δωματίου πρεπει να γυρίσει 360 μοίρες και αμα στρίψει 4 φορές απο 90 μοίρες θα εχει κανει όλο το περίγραμμα
int room_outline_corners= 0;

//οι παρακάτω συναρτήσεις ειναι για την αρχικοποίηση των led 
//και για ευκολία στον προγραμματισμο πχ η LED0(1) αναβει το LED στο pin1

void LED_init(void)
{
	PORTD.DIR |= PIN1_bm; //PIN is output ευθεία
	PORTD.OUT |= PIN1_bm; //LED is off
	PORTD.DIR |= PIN2_bm; //PIN is output αριστερά
	PORTD.OUT |= PIN2_bm; //LED is off
	PORTD.DIR |= PIN3_bm; //PIN is output δεξιά
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
		corners++;//αυξάνουμε τις γωνίες κατα 1
		room_outline_corners--;//στρίβει αριστερά οπότε αφαιρεί γωνιες για να φτασει στο -4

	}
	else
	{
		LED2(1);
		corners--;//μειώνουμε τις γωνίες κατα 1 γιατι κάνει την ανάποδη πορεία

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
	//το 5ο pin εχει την τιμη 0x20 η οποια ειναι το 32 και ελεγχουμε ετσι
	//αν ο χρηστης πατησε το 5ο η το 6ο κουμπι και το interupt εκτελει τον αναλογο κωδικα
	if (intflags == 32)
	{
		if (reverse == 0)
		{
			LED2(1);
			corners++;//αυξάνουμε τις γωνίες κατα 1 μεχρι να φτασουμε τις 8
			room_outline_corners++;//προσθέτουμε γωνίες για να φτασει στο +4
		}
		else
		{
			LED1(1);
			corners--;//μειώνουμε τις γωνίες κατα 1 γιατι κάνει την ανάποδη πορεία
		}
	}
	//οταν παταμε το switch του pinctrl6 τοτε μπαινει στην else και αναβοσβηνει τα λαμπακια και κανει στοφη 180 μοιρες
	else{
		LED0(1);
		LED1(1);
		LED2(1);
		//στρίψε 180 μοίρες
		reverse=1;
		LED0(0);
		LED1(0);
		LED2(0);

	}
	//timer ο οποιος θα κανει interupt για να σταματησει να στρίβει
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc;
	TCA0.SINGLE.CTRLA |=1;//Enable
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm; //Interrupt Enable
}

//interupt του timer για να σταματησει να στριβει (δεξια ή αριστερα)
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
	//αρχικοποίηση των LED
	LED_init();

	//αρχικοποίηση του timer 
	TCA0.SINGLE.CNT = 0; //clear counter
	TCA0.SINGLE.CTRLB = 0; //Normal Mode (TCA_SINGLE_WGMODE_NORMAL_gc σελ 207)
	TCA0.SINGLE.CMP0 = ped; //When reaches this value -> interrupt CLOCK FREQUENCY/1024


	//αρχικοποίηση του ADC για Free-Running mode
	ADC0.CTRLA |= ADC_RESSEL_10BIT_gc; //10-bit resolution
	ADC0.CTRLA |= ADC_FREERUN_bm; //Free-Running mode enabled
	ADC0.CTRLA |= ADC_ENABLE_bm; //Enable ADC
	ADC0.MUXPOS |= ADC_MUXPOS_AIN7_gc; //The bit
	//Debug Mode
	ADC0.DBGCTRL |= ADC_DBGRUN_bm;
	//Window Comparator Mode
	ADC0.WINLT |= 10; //Set threshold
	ADC0.INTCTRL |= ADC_WCMP_bm; //ενεργοποίηση Interrupts για WCM
	ADC0.CTRLE |= ADC_WINCM0_bm; //Interrupt οταν RESULT < WINLT

	//pullup enable and Interrupt enabled with sense on both edges
	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;//switch να στρίψει
	PORTF.PIN6CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;//switch για να κανει στροφή 180 μοιρών και να κανει αντίθετη πορεια
	sei();
	ADC0.COMMAND |= ADC_STCONV_bm; //Start Conversion

	LED0(1);//ξεκινάει να κινείται ευθεία


	//το πρόγραμμα τρεχει μεχρι να κανει 8 γωνίες ή αν ειναι σε αναποδη πορεία αν εφτασε πάλι στην αρχη (δηλαδή αφαιρέι απο την μεταβλητή corners μεχρι να γινει 0)
	if (reverse == 0 && corners < 8)
	{
		while(1){}
	}
	if (reverse == 1 && corners > 0)
	{
		while (1){}
	}
}

