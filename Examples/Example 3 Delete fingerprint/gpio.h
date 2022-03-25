#ifndef _GPIO_H
#define _GPIO_H

#include <stdbool.h>

enum PortRegLevel
{
	GPIO_PORT_REG_LOW = 0,
	GPIO_PORT_REG_HIGH = 1
};

#define GPIO_PULLUP_ENABLE										true
#define GPIO_GEN_PUR_OUTPUT_PUSH_PULL					0x00

#define GPIO_PIN0															1<<0
#define GPIO_PIN0_INPUT_FLOATING							GPIO_CRL_CNF0_0
#define GPIO_PIN0_INPUT_PULLUP_OR_PULLDOWN		GPIO_CRL_CNF0_1
#define GPIO_PIN0_OUTPUT_MODE_2MHZ						GPIO_CRL_MODE0_1
#define GPIO_PIN0_GEN_PUR_OPEN_DRAIN					GPIO_CRL_CNF0_0

#define GPIO_PIN1															1<<1
#define GPIO_PIN1_OUTPUT_MODE_2MHZ						GPIO_CRL_MODE1_1
#define GPIO_PIN1_INPUT_PULLUP_OR_PULLDOWN		GPIO_CRL_CNF1_1

#define GPIO_PIN2															1<<2
#define GPIO_PIN2_OUTPUT_MODE_2MHZ						GPIO_CRL_MODE2_1
#define GPIO_PIN2_ALT_FUNC_PUSH_PULL					GPIO_CRL_CNF2_1

#define GPIO_PIN3															1<<3
#define GPIO_PIN3_INPUT_PULLUP_OR_PULLDOWN		GPIO_CRL_CNF3_1

#define GPIO_PIN4															1<<4
#define GPIO_PIN4_OUTPUT_MODE_2MHZ						GPIO_CRL_MODE4_1
#define GPIO_PIN4_INPUT_PULLUP_OR_PULLDOWN		GPIO_CRL_CNF4_1

#define GPIO_PIN5					  									1<<5
#define GPIO_PIN5_OUTPUT_MODE_2MHZ						GPIO_CRL_MODE5_1
#define GPIO_PIN5_INPUT_PULLUP_OR_PULLDOWN		GPIO_CRL_CNF5_1
#define GPIO_PIN5_ALT_FUNC_PUSH_PULL					GPIO_CRL_CNF5_1

#define GPIO_PIN6					  									1<<6
#define GPIO_PIN6_OUTPUT_MODE_2MHZ						GPIO_CRL_MODE6_1
#define GPIO_PIN6_INPUT_PULLUP_OR_PULLDOWN 	  GPIO_CRL_CNF6_1
#define GPIO_PIN6_ALT_FUNC_OPEN_DRAIN					GPIO_CRL_CNF6

#define GPIO_PIN7					  									1<<7
#define GPIO_PIN7_ALT_FUNC_OPEN_DRAIN					GPIO_CRL_CNF7
#define GPIO_PIN7_OUTPUT_MODE_2MHZ						GPIO_CRL_MODE7_1
#define GPIO_PIN7_ALT_FUNC_PUSH_PULL					GPIO_CRL_CNF7_1

#define GPIO_PIN8															1<<8
#define GPIO_PIN8_INPUT_PULLUP_OR_PULLDOWN		GPIO_CRH_CNF8_1
#define GPIO_PIN8_OUTPUT_MODE_2MHZ						GPIO_CRH_MODE8_1

#define GPIO_PIN9															1<<9
#define GPIO_PIN9_OUTPUT_MODE_2MHZ						GPIO_CRH_MODE9_1
#define GPIO_PIN9_INPUT_PULLUP_OR_PULLDOWN		GPIO_CRH_CNF9_1
#define GPIO_PIN9_ALT_FUNC_PUSH_PULL					GPIO_CRH_CNF9_1

#define GPIO_PIN10														1<<10
#define GPIO_PIN10_INPUT_PULLUP_OR_PULLDOWN 	GPIO_CRH_CNF10_1
#define GPIO_PIN10_OUTPUT_MODE_2MHZ						GPIO_CRH_MODE10_1
#define GPIO_PIN10_ALT_FUNC_PUSH_PULL					GPIO_CRH_CNF10_1

#define GPIO_PIN11														1<<11
#define GPIO_PIN11_INPUT_PULLUP_OR_PULLDOWN 	GPIO_CRH_CNF11_1
#define GPIO_PIN11_OUTPUT_MODE_2MHZ						GPIO_CRH_MODE11_1

#define GPIO_PIN12														1<<12
#define GPIO_PIN12_OUTPUT_MODE_2MHZ						GPIO_CRH_MODE12_1

#define GPIO_PIN13														1<<13
#define GPIO_PIN13_OUTPUT_MODE_2MHZ						GPIO_CRH_MODE13_1

#define GPIO_PIN14														1<<14
#define GPIO_PIN14_INPUT_PULLUP_OR_PULLDOWN 	GPIO_CRH_CNF14_1
#define GPIO_PIN14_OUTPUT_MODE_2MHZ						GPIO_CRH_MODE14_1

#define GPIO_PIN15														1<<15
#define GPIO_PIN15_INPUT_PULLUP_OR_PULLDOWN 	GPIO_CRH_CNF15_1
#define GPIO_PIN15_OUTPUT_MODE_2MHZ						GPIO_CRH_MODE15_1

extern void GPIO_Reset(void);
//Initializes the GPIO to the input mode of choice 
extern void GPIO_InputInit(GPIO_TypeDef* gpioPort, 
													 uint8_t portLevel,  
													 uint16_t gpioPins,
													 uint32_t config,  
													 bool pullupEn);
//Initializes the GPIO to the output mode of choice
extern void GPIO_OutputInit(GPIO_TypeDef* gpioPort,
														uint8_t portLevel,
														uint32_t mode,
														uint32_t config);
//Writes a logic 'high' or 'low' to a GPIO pin
extern void GPIO_OutputWrite(GPIO_TypeDef* gpioPort,
														 uint16_t gpioPins,
														 bool gpioPinLogic);
//Returns the logic level (high(true) or low(false)) of a GPIO input pin
extern bool GPIO_InputRead(GPIO_TypeDef* gpioPort, uint16_t gpioPin);
//Returns the logic level (high(true) or low(false)) of a GPIO output pin
extern bool GPIO_OutputRead(GPIO_TypeDef* gpioPort, uint16_t gpioPin);

#endif //_GPIO_H
