#include"methods.h"
void SystemInit() {}
int main()
{
	portF_init();
	portA_init();
	__enable_irq();
	Timer3_init(3000000); //for timer interrupt period = 3
	ADC_init();
	function(0,2); //input switch is PF0 and output led is PF2 (blue red)
	while(1)
	{
		
		
	}
	return 0;
}
