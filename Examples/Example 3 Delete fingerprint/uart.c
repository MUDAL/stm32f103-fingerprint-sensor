#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include "uart.h"

void USART_Init(USART_TypeDef* uartPort,
								uint32_t baud,
								uint8_t dmaMode,
								uint8_t enable)
{
	uartPort->CR1 |= USART_CR1_UE;
	switch(dmaMode)
	{
		case TX_DMA_ENABLE:
			uartPort->CR3 |= USART_CR3_DMAT;
			break;
		case RX_DMA_ENABLE:
			uartPort->CR3 |= USART_CR3_DMAR;
			break;
		case BOTH_DMA_ENABLE:
			uartPort->CR3 |= (USART_CR3_DMAT | USART_CR3_DMAR);
			break;
		case BOTH_DMA_DISABLE:
			uartPort->CR3 &= ~(USART_CR3_DMAT | USART_CR3_DMAR);
			break;
	}
	uartPort->BRR = baud;
	uartPort->CR1 |= enable;
}

void USART_TransmitByte(USART_TypeDef* uartPort, uint8_t byte)
{	
	while((uartPort->SR & USART_SR_TXE) != USART_SR_TXE);
	uartPort->DR = byte;
}

void USART_TransmitBytes(USART_TypeDef* uartPort, uint8_t* bytes, uint8_t len)
{
	uint8_t i = 0;
	while (i < len)
	{
		while((uartPort->SR & USART_SR_TXE) != USART_SR_TXE);
		uartPort->DR = bytes[i];
		i++;
	}
}

bool USART_RxBufferFull(USART_TypeDef* uartPort)
{
	if((uartPort->SR & USART_SR_RXNE) == USART_SR_RXNE)
	{
		return true;
	}
	return false;
}

bool USART_RxIdleLineDetected(USART_TypeDef* uartPort)
{
	bool idleLineDetected = false;
	if((uartPort->SR & USART_SR_IDLE) == USART_SR_IDLE)
	{
		idleLineDetected = true;
		volatile uint8_t readDR = uartPort->DR;
	}
	return idleLineDetected;
}
