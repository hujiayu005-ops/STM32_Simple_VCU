#include "stm32f10x.h"                  // Device header

void Throttle_ADC_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//对72mhz进行分频，一般ADCCLK最大14mhz，所以说只能用6分频(12mhz)以上的
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//选择软件触发
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//独立工作
	ADC_InitStructure.ADC_NbrOfChannel=1;//多少个通道
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;//选择单通道
	
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);
	
//BB1.	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//AA2.我们只需要开启连续转换且直接使能一次即可
//	
//BB1.	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);//选择通道	
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1)==1);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)==1);
	
//	问题B:我的使能采样顺序搞错了    使能ADC → 校准 → 配通道 → 启动转换
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);//BB2
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//BB2
}

uint16_t Get_ThrottleValue(void)
{
	uint16_t Value;
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//AA1.这里因为在mian函数里需要调用这个Getval函数获取值
//	//就相当于再次给软件触发使能了，所以我们直接使用连续转换就行了
//	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//等待软件结束标志位EOC，确保读到的是这一轮的数据
//	//问题C:这个等待标志位EOC没能很好的处理		ADC一直在转换	EOC会不断被置位
	Value=ADC_GetConversionValue(ADC1);//CC1:等待EOC标志位结束后赋最近得到的值给value，然后再clear eoc
//	ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//手动清除更保险
	//问题D:事实上没必要等待标志位
	return Value*100/4095;//返回最近一次ad转换结束值
}
