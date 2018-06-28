#include"methods.h"
int inputSwitch; // equals 0 when PF0 is pressed and equals 4 when PF4 is pressed
int outputLed;	// equals 1,2 or 3 when PF1,PF2 or PF3 is on repectively


void portF_init(void)
{
	int volatile delay;
	SYSCTL_RCGCGPIO_R |=0x20;  //enable clock for port f
	delay=0; // delay to allow port f to start as it needs 2 clock cycles
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0x1F;
	GPIO_PORTF_AMSEL_R = 0x00;
	GPIO_PORTF_PCTL_R = 0x00000000;
	GPIO_PORTF_DIR_R = 0x0E;
	GPIO_PORTF_AFSEL_R = 0x00000000;
	GPIO_PORTF_PUR_R = 0x11;
	GPIO_PORTF_DEN_R = 0x1F;
	
	//for the interrupt of port f
	GPIO_PORTF_IS_R &=~(1<<0);
	GPIO_PORTF_IBE_R &=~(1<<0);
	GPIO_PORTF_IEV_R |=(1<<0);
	GPIO_PORTF_ICR_R |=(1<<0); 
	GPIO_PORTF_IM_R |=(1<<0);
	NVIC_EN0_R |=(1<<30);
	NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF)|0x00A00000; //priority = 5

}

void portA_init(void)
{
	SYSCTL_RCGCGPIO_R |=1<<0;			
	SYSCTL_RCGCUART_R |=1<<0;  	 //enabling uart at port A
	GPIO_PORTA_AFSEL_R=0x03;  
	GPIO_PORTA_PCTL_R |=(1<<0) | (1<<4);
	GPIO_PORTA_DEN_R |=(1<<0) | (1<<1);
	//setting uart function on port A
	UART0_CTL_R=~1;		
	// baud rate = 9600 so integet part = (16*10^6)/(16*9600)  and floating part = (remainder*64)+0.5
	UART0_IBRD_R=104;
	UART0_FBRD_R=11;
	UART0_LCRH_R =(1<<5) | (1<<6);
	UART0_CTL_R =(1<<0) | (1<<8) | (1<<9);
	UART0_CC_R=0x00;
}

// reads 1 byte from the uart transmitter
char readChar(void)
{
	while((UART0_FR_R & 0x0010) !=0){}
	return (char)(UART0_DR_R & 0xFF);
}

// sends 1 byte through the uart receiver
void printChar(char c)
{
	while((UART0_FR_R & 0x0020)!=0){}
	UART0_DR_R = c;
}

//sends a sequence of characters using printChar function
void printString(char* string)
{
	while(*string)
	{
		printChar(*(string++));
	}
}

//ISR of GPIO port F
void GPIOF_Handler(void)
{
	GPIO_PORTF_ICR_R |=(1<<0);
	GPIO_PORTF_DATA_R ^= (1<<outputLed);
	if(inputSwitch==0)
	{
		printString("Switch one is pressed");
	}
	else if(inputSwitch==4)
	{
		printString("Switch two is pressed");
	}
	if(GPIO_PORTF_DATA_R&(1<<outputLed))
	{
		if(outputLed==1)
			printString(" and red led is on\n\r");
		else if(outputLed==2)
			printString(" and blue led is on\n\r");
		else
			printString(" and green led is on\n\r");
	}
	else
		{
		if(outputLed==1)
			printString(" and red led is off\n\r");
		else if(outputLed==2)
			printString(" and blue led is off\n\r");
		else
			printString(" and green led is off\n\r");
	}
}

//to initialize Timer3A in periodic mode and the input period is in microseconds
void Timer3_init(int period)
{
	int volatile delay;
	SYSCTL_RCGCTIMER_R |=0x8;
	delay=1;
	delay=0;
	TIMER3_CTL_R = 0x00;
	TIMER3_CFG_R = 0x00;
	TIMER3_TAMR_R = 0x12;
	TIMER3_TAILR_R = 16*period-1; //as the clock frequency is 16MHz
	TIMER3_TAPR_R = 0;
	TIMER3_ICR_R = 0x1;
	TIMER3_IMR_R = 0x1;
	NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|(0x20000000); //priority 1
	TIMER3_CTL_R |=0x21; //enable timer and adc interrupt
}




void ADC_init(void)
{
	int volatile delay;
	SYSCTL_RCGCADC_R |=0x1;
	delay =2;
	delay =1;
	delay =0; //to make delay to allow adc to be initialized
	ADC0_ACTSS_R &=~0x8;
	ADC0_EMUX_R = (ADC0_EMUX_R & 0x0FFF)|(0x5000);
	ADC0_SSMUX3_R = 0;
	ADC0_SSCTL3_R |= 0xE;
	ADC0_IM_R |=(1<<3);
	NVIC_EN0_R |=(1<<17);
	ADC0_ACTSS_R |=0x8;
}

//ISR for ADC
void ADC0SS3_Handler(void)
{
	char a[5];
	int temperature = (147.5-(247.5*ADC0_SSFIFO3_R)/4096.0);
	printString("temperature is : ");
	sprintf(a,"%d",temperature);
	printString(a);
	printString(" celcius\n\r");
	ADC0_ISC_R|=0x8;
}	

void function(int in,int out)
{
	inputSwitch=in;
	outputLed=out;
}