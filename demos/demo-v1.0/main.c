#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "throttle.h"
#include "Motor.h"

uint16_t ThrottleVal;

int main(void)
{
	OLED_Init();
	Motor_Init();
	Throttle_ADC_Init();
	
	OLED_ShowString(1,1,"throttle:000%");
	
	while(1)
	{
		ThrottleVal=Get_ThrottleValue();
			OLED_ShowNum(1,10,ThrottleVal,3);
			Motor_SetSpeed(ThrottleVal);
	}
}
