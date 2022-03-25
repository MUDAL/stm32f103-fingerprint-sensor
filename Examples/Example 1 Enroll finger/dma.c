#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include "dma.h"
/**
@brief Initializes a USART Rx DMA channel.  
*/
void DMA_USART_Rx_Init(DMA_Channel_TypeDef* dmaChannel,
											 USART_TypeDef* uartPort,
											 uint8_t* uartRxBuffer, 
											 uint8_t bufferSize,
											 uint32_t dmaConfig)
{
	volatile uint32_t* pUart_DR = (uint32_t*)&uartPort->DR;
	dmaChannel->CCR &= ~dmaConfig;
	dmaChannel->CPAR = (uint32_t)pUart_DR;
	dmaChannel->CMAR = (uint32_t)uartRxBuffer;
	dmaChannel->CNDTR = bufferSize;
	dmaChannel->CCR |= dmaConfig;
}

/**
@brief Signals if the Rx buffer of a DMA channel is full.  
@param dmaPort: e.g. DMA1,DMA2
@param dmaChannel: The channel of a particular DMA port e.g. DMA1 channel 5.  
@return true if buffer is full and false if otherwise.  
*/
bool DMA_Rx_BufferFull(DMA_TypeDef* dmaPort, uint8_t dmaChannel)
{
	bool bufferIsFull = false;
	uint8_t bitLocation = (dmaChannel * 4) - 3;
	
	if ((dmaPort->ISR & (1<<bitLocation)) == (1<<bitLocation))
	{
		bufferIsFull = true;
		dmaPort->IFCR |= (1<<bitLocation);
	}
	return bufferIsFull;
}

/**
@brief Gets the value in CNDTR register of a DMA channel. This value is  
the number of bytes left to be received.  
@param dmaChannel: The channel of a particular DMA port e.g. DMA1 channel 5.  
@return number of bytes left to be received by the Rx buffer of a DMA channel.   
*/
uint8_t DMA_Rx_CNDTR(DMA_Channel_TypeDef* dmaChannel)
{
	return dmaChannel->CNDTR;
}
