#include "stm32f10x.h"                  // Device header
#include "clock.h"

void Clock_HSI_8MHz_Init(void)
{
	while((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY); //wait for internal oscillator to be stable
	RCC->CFGR = 0; //Reset CFGR
	RCC->CR &= ~(RCC_CR_PLLON | RCC_CR_HSEON);	//disable PLL and external oscillator
	//enable clock for DMA1
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	//enable clock access for GPIOA,USART1
	RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN);
}
