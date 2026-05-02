#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//将gpio交给TIM
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseStructure.TIM_Period=100-1;//周期	即CNT++到多少时会从来
	TIM_TimeBaseStructure.TIM_Prescaler=36-1;//预分频
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//高级才用
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_Cmd(TIM2,ENABLE);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;//空闲状态下的电平
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//选择模式 PWM1模式是CNT<CCR为低电平
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//高电平有效，这里和第一个参数有所出入
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;//CCR值
	
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);//我们选用的引脚为PA2对应TIM2_CH3所以要用OC3Init
}
void PWM_SetCompare(uint16_t Compare)
{
	TIM_SetCompare3(TIM2,Compare);
}
