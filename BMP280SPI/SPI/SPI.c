#include "main.h"
#include "SPI.h"
#include "stdio.h"
void delay_us(uint32_t nus)//for 延迟
{		
	uint32_t Delay = nus * 168/4;
	do
	{
		  __NOP();
	}
	while (Delay--);
}
void SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;//IO口初始化
	GPIO_InitTypeDef GPIO_Initure2;//IO口初始化

	__HAL_RCC_GPIOF_CLK_ENABLE();   //使能GPIOF时钟
	
	GPIO_Initure.Pin = GPIO_PIN_15|GPIO_PIN_13|GPIO_PIN_11;//PF15 SCL, PF13 SDA, PF11 CSB
	GPIO_Initure.Speed = GPIO_SPEED_FAST;
	GPIO_Initure.Pull=GPIO_PULLUP;         
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOF,&GPIO_Initure);
	
	GPIO_Initure2.Pin = GPIO_PIN_14;//PF14 SDO
	GPIO_Initure2.Speed = GPIO_SPEED_FAST;
	GPIO_Initure2.Pull=GPIO_PULLUP;         
	GPIO_Initure2.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOF,&GPIO_Initure2);
	//初始化SPI
	SPI_CSB = 1;
	SPI_SCL = 1;
	SPI_SDA = 1;
}
void SPI_WriteByte(uint8_t TxData)
{
	uint8_t cnt;
	
	for(cnt = 0;cnt < 8;cnt++)
	{
		SPI_SCL = 0;//时钟拉低
		HAL_Delay(1);
		if(TxData &0x80)//发送数据
		{
			printf("1\r\n");
			SPI_SDA = 1;
		}
		else
		{
			printf("0\r\n");
			SPI_SDA = 0;
		}
		TxData <<= 1;//左移一位
		
		
		SPI_SCL = 1;//时钟拉高
		HAL_Delay(1);
	}
}
uint8_t SPI_ReadByte(void)
{
	uint8_t cnt;
	uint8_t RxData = 0;
	
	for(cnt = 0;cnt <8;cnt++)
	{
		SPI_SCL = 0;//时钟拉低
		HAL_Delay(1);
		
		RxData <<=1;//读取数据
		if(SPI_SDO)
		{
			RxData |=0x01;
			printf("1\r\n");
		}else
		{
			printf("0\r\n");
		}
		
		SPI_SCL = 1;//时钟拉高
		HAL_Delay(1);
	}
	return RxData;
}