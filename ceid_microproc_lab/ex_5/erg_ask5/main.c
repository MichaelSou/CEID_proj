#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define ped 20


int thermos = 0; //μεταβλητη για να γνωρίζουμε αν ειναι ανοιχτός ο θερμοσίφωνας
int anemist = 0; //μεταβλητη για να γνωρίζουμε αν ειναι ανοιχτός ο ανεμιστήρας
int leak = 0; //μεταβλητη για να γνωρίζουμε αν υπαρχει διαροή
int lock = 0; //μεταβλητη για να γνωρίζουμε αν ειναι κλειδωμένο το σπίτι
int menu_select = 0; // ματαβλητή για επιλογές μενού
int pass_atempts = 0;

void LED_init(void)//αρχικοποιηση LED
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
void LED0(int status) //LED θερμοσίφωνα
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
void LED1(int status)// LED anemisthra
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
void LED2(int status) // LED leak
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
void LED3(int status) //LED LOCK
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


//συναρτηση για τον θερμοσίφωνα αν ειναι κλειστός ανοιγει και ενεργοποιει τον timer
//αν ειναι ανοιχτός τοτε τον κλείνει και απενεργοποιει τον timer
void thermo()
{
	if (thermos = 0)
	{
		LED0(1);
		TCA0.SINGLE.CTRLA |=1;//Enable
		TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm; //Interrupt Enable (=0x10)
	}
	else
	{
		LED0(0);
		TCA0.SINGLE.CTRLA |=0;//Disable
	}
}


//interrupt timer  αν το σπιτι ειναι ξεκλείδωτο τοτε ελέγχει αν πατη8ηκε το switch 5 και συνεχίζει να αναβει ο θερμοσύφωνας
// αν το σπιτι ειναι κλειδωμενο τοτε απενεργοποιει τον timer και τον κλειδωνει τελειως
ISR(TCA0_CMP0_vect){
	LED0(0);
	TCA0.SINGLE.CTRLA = 0; //Disable
	//clear flag
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS=intflags;
	int switflags = PORTF.INTFLAGS;

	if (lock == 0)
	{
		if (switflags == 32)
		{
			LED0(1);
			TCA0.SINGLE.CTRLA |=1;//Enable
			TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm; //Interrupt Enable (=0x10)
		}
	}
	else
	{
		TCA0.SINGLE.CTRLA &= (~TCA_SINGLE_ENABLE_bm);
		LED3(1);
		menu_select = 4;
	}


}



//συνάρτηση για τον ανεμμηστίρα αν ειναι κλειστος ενεργοποιει τον PWM και αυτος αναβοσβήνει το λαμπάκι
//αν ειναι ανοιχτός τον κλείνει και απενεργοποιει το interrupt του PWM.
void anemisthras()
{
	if (anemist == 0)
	{
		//enable interrupt COMP1
		TCA0.SINGLE.INTCTRL |= TCA_SINGLE_CMP1_bm;
		TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm; //Enable
	}
	else
	{
		LED1(0);
		TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm; //Enable
	}
}


//interupt του PWM οταν εκτελεστεί το interrupt τοτε αναβοσβήνει το LED για τον ανεμιστήρα.
ISR(TCA0_CMP1_vect){
	//clear the interrupt flag
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS = intflags;
	LED1(1);
	_delay_ms(1);
	LED1(0);
}

//συνρατηση που καλείται οταν το χρήστης παταει το switch 6 και υπαρχει διαροη
// η συναρτηση σβήνει το led2 το οποιο ειναι του leak.
void waterleak()
{
	if (leak == 1)
	{	
		LED2(0);
	}
}


//Το interrupt του ADC αν το σπιτι ειναι κλειδωμενο αναβει το led3 ενεργοποιει τον timer και τρεχει την house_lock()
// αν το σπιτι δεν ειναι κλειδωμενο τοτε αναβει το led 2 και κανει την μεταβλητη leak=1 γιατι εχει βρει διαροή
ISR(ADC0_WCOMP_vect){
	int intflags = ADC0.INTFLAGS;
	ADC0.INTFLAGS = intflags;
	if (lock == 0)
	{
		LED2(1);
		leak = 1;
	}
	else
	{
		LED3(1);
		TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm; //enable
		house_lock();
	}
}

//η συναρτηση house_lock() αν το σπιτι ειναι ξεκλειδωτο το κλειδωνει και απενεργοποιει το σύστημα
//και να ειναι κλειδωμενο τοτε καλει την switch_password() για να εισαγει τον κωδικο ο χρήστης και αναλογα με το αποτελεσμα
//είτε κλειδώνει τελειως το σπίτι (σε περιπτωση λαθος κωδικού) ή ξεκλειδώνει το σπιτι (σε περιπτωση σωστού κωδικού)
void house_lock()
{
	if (lock == 0)
	{
		lock = 1; // κλειδώνει το σύστημα.
		ADC0.CTRLA &= ~ADC_ENABLE_bm;//disable adc
		TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm; //disable timer

	}
	else
	{
		int temp = switch_password(); //τρέχει η συνάρτηση για την εισαγωγή κωδικού
		if (temp == 0)//αν ο κωδικός ειναι λάθος κλειδώνει το σύστημα και δεν μπορει να γινει καμια αλλη ενέργεια.
		{
			LED3(1);
			menu_select = 4;
		}
		else//αν ο κωδικός ειναι σωστος ξεκλειδωνει και ενεργοποιει το σύστημα.
		{
			lock = 0;
			ADC0.CTRLA |= ADC_ENABLE_bm;
			TCA0.SINGLE.CTRLA &= (~TCA_SINGLE_ENABLE_bm);//disable timer
		}
	}

}


//η switch_password() δεχεται εναν κωδικο (μοτιβο) απο του διακόπτες και αν ειναι σωστό επιστρέφει 1
// αν ειναι λάθος η μεταβλητη pass_atmepts αυξάνεται κατα ένα και οταν πάρει την τιμή 3 τοτε η συνάρτηση επιστρέφει την τιμη 0.
int switch_password()
{
	_delay_ms(1);
	int pass1 = PORTF.INTFLAGS;
	if (pass1 == 32)
	{
		_delay_ms(1);
		int pass2 = PORTF.INTFLAGS;
		if (pass2 == 64)
		{
			_delay_ms(1);
			int pass3 = PORTF.INTFLAGS;
			if (pass3 == 32)
			{
				return 1;
			}
		}
	}
	if (pass_atempts < 3)
	{
		pass_atempts++;
		switch_password();
	}
	else
	{
		return 0;
	}

}



//στην main αρχικοποιουμε αρκετα απο τα πραγματα που θα χρησιμοποιήσουμε.
//Οπως επίσης εχουμε φτιάξει με while και if το βασικό μενού για τις επιλογέ του χρήστη.

int main()
{
	LED_init();// αρχικοποίηση των LED
	

	//αρχικοποίηση του ADC για Free-Running mode
	ADC0.CTRLA |= ADC_RESSEL_10BIT_gc; //10-bit resolution
	ADC0.CTRLA |= ADC_FREERUN_bm; //Free-Running mode enabled
	ADC0.CTRLA |= ADC_ENABLE_bm; //Enable ADC
	ADC0.MUXPOS |= ADC_MUXPOS_AIN7_gc; //The bit
	//Debug Mode
	ADC0.DBGCTRL |= ADC_DBGRUN_bm;
	//Window Comparator Mode
	ADC0.WINLT = 3;
	ADC0.WINHT |= 10; //Set threshold
	ADC0.INTCTRL |= ADC_WCMP_bm; //ενεργοποίηση Interrupts για WCM
	ADC0.CTRLE |= ADC_WINCM0_bm; //Interrupt οταν RESULT < WINLT


	TCA0.SINGLE.CNT = 0; //clear counter
	TCA0.SINGLE.CTRLB = 0; //Normal Mode (TCA_SINGLE_WGMODE_NORMAL_gc σελ 207)
	TCA0.SINGLE.CMP0 = ped; //When reaches this value -> interrupt CLOCK FREQUENCY/1024
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc; 


	//prescaler=1024
	TCA0.SINGLE.CTRLA=TCA_SINGLE_CLKSEL_DIV1024_gc;
	TCA0.SINGLE.PER = 254; //select the resolution
	TCA0.SINGLE.CMP1 = 127; //select the duty cycle
	//select Single_Slope_PWM
	TCA0.SINGLE.CTRLB |= TCA_SINGLE_WGMODE_SINGLESLOPE_gc;


	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	PORTF.PIN6CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	sei();
	ADC0.COMMAND |= ADC_STCONV_bm; //Start Conversion
    while (1) 
    {
		while(menu_select == 0)
		{
			int intflags = PORTF.INTFLAGS;
			PORTF.INTFLAGS = intflags;
			if (intflags == 32)
			{
				menu_select++;
			}
			if (intflags == 64)
			{
				thermo();
			}
		}
		while(menu_select == 1)
		{
			int intflags = PORTF.INTFLAGS;
			PORTF.INTFLAGS = intflags;
			if (intflags == 32)
			{
				menu_select++;
			}
			if (intflags == 64)
			{
				anemisthras();
			}
		}
		while(menu_select == 2)
		{
			int intflags = PORTF.INTFLAGS;
			PORTF.INTFLAGS = intflags;
			if (intflags == 32)
			{
				menu_select++;
			}
			if (intflags == 64)
			{
				waterleak();
			}
		}
		while(menu_select == 3)
		{
			int intflags = PORTF.INTFLAGS;
			PORTF.INTFLAGS = intflags;
			if (intflags == 32)
			{
				menu_select = 0;
			}
			if (intflags == 64)
			{
				house_lock();
			}
		}
		while (menu_select == 4)
		{
			//αν ο χρήστης εισαγει 3 φορες λάθος κωδικό "κλειδώνετε σε αυτό το άδειο loop
		}
    }
}

