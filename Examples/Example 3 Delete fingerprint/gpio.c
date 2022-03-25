#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include "gpio.h"

/**
	* Sets GPIO port configuration registers to a default state
	* (i.e. an Analog mode)
	* PA13,PA14,PA15,PB2,PB3 and PB4 are left in their reset states.
*/
void GPIO_Reset(void)
{
	GPIOA->CRL &= ~0xCCCCCCCC;
	GPIOA->CRH &= ~0x000CCCCC;
	GPIOB->CRL &= ~0xCCC000CC;
	GPIOB->CRH &= ~0xCCCCCCCC;
	GPIOC->CRL &= ~0xCCCCCCCC;
	GPIOC->CRH &= ~0xCCCCCCCC;
}

void GPIO_InputInit(GPIO_TypeDef* gpioPort, 
										uint8_t portLevel, 
										uint16_t gpioPins,
										uint32_t config,  
										bool pullupEn)
											 
{
	if(portLevel == GPIO_PORT_REG_HIGH)
	{
		gpioPort->CRH |= config;
	}
	else
	{
		gpioPort->CRL |= config;
	}
	if(pullupEn)
	{
		gpioPort->ODR |= (gpioPins);
	}
}

void GPIO_OutputInit(GPIO_TypeDef* gpioPort,
										 uint8_t portLevel,
										 uint32_t mode,
										 uint32_t config)

{
	if(portLevel == GPIO_PORT_REG_HIGH)
	{
		gpioPort->CRH |= mode;
		gpioPort->CRH |= config;
	}
	else
	{
		gpioPort->CRL |= mode;
		gpioPort->CRL |= config;
	}
}

void GPIO_OutputWrite(GPIO_TypeDef* gpioPort,
											uint16_t gpioPins,
											bool gpioPinLogic)
																
{
	gpioPort->ODR &= ~gpioPins;
	if(gpioPinLogic)
	{
		gpioPort->ODR |= gpioPins;
	}
}

bool GPIO_InputRead(GPIO_TypeDef* gpioPort, uint16_t gpioPin)
{
	if((gpioPort->IDR & gpioPin) == gpioPin)
	{
		return true;
	}
	return false;
}

bool GPIO_OutputRead(GPIO_TypeDef* gpioPort, uint16_t gpioPin)
{
	if((gpioPort->ODR & gpioPin) == gpioPin)
	{
		return true;
	}
	return false;	
}
	
