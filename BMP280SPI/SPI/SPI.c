#include "main.h"
#include "SPI.h"
#include "stdio.h"
void delay_us(uint32_t nus)//for �ӳ�
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
	GPIO_InitTypeDef GPIO_Initure;//IO�ڳ�ʼ��
	GPIO_InitTypeDef GPIO_Initure2;//IO�ڳ�ʼ��

	__HAL_RCC_GPIOF_CLK_ENABLE();   //ʹ��GPIOFʱ��
	
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
	//��ʼ��SPI
	SPI_CSB = 1;
	SPI_SCL = 1;
	SPI_SDA = 1;
}
void SPI_WriteByte(uint8_t TxData)
{
	uint8_t cnt;
	
	for(cnt = 0;cnt < 8;cnt++)
	{
		SPI_SCL = 0;//ʱ������
		HAL_Delay(1);
		if(TxData &0x80)//��������
		{
			printf("1\r\n");
			SPI_SDA = 1;
		}
		else
		{
			printf("0\r\n");
			SPI_SDA = 0;
		}
		TxData <<= 1;//����һλ
		
		
		SPI_SCL = 1;//ʱ������
		HAL_Delay(1);
	}
}
uint8_t SPI_ReadByte(void)
{
	uint8_t cnt;
	uint8_t RxData = 0;
	
	for(cnt = 0;cnt <8;cnt++)
	{
		SPI_SCL = 0;//ʱ������
		HAL_Delay(1);
		
		RxData <<=1;//��ȡ����
		if(SPI_SDO)
		{
			RxData |=0x01;
			printf("1\r\n");
		}else
		{
			printf("0\r\n");
		}
		
		SPI_SCL = 1;//ʱ������
		HAL_Delay(1);
	}
	return RxData;
}