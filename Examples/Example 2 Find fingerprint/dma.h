#ifndef _DMA_H
#define _DMA_H

#define DMA_CHANNEL1											1
#define DMA_CHANNEL2											2
#define DMA_CHANNEL3											3
#define DMA_CHANNEL4											4
#define DMA_CHANNEL5											5
#define DMA_CHANNEL6											6
#define DMA_CHANNEL7											7

//Generic DMA Channel enable
#define DMA_CCR_EN												((uint16_t)0x0001)
#define DMA_CHANNEL_ENABLE								DMA_CCR_EN

#define DMA_CHANNEL3_MEMORY_INC_MODE			DMA_CCR3_MINC
#define DMA_CHANNEL3_CIRCULAR_BUFFER			DMA_CCR3_CIRC

#define DMA_CHANNEL5_MEMORY_INC_MODE			DMA_CCR5_MINC
#define DMA_CHANNEL5_CIRCULAR_BUFFER			DMA_CCR5_CIRC

#define DMA_CHANNEL6_MEMORY_INC_MODE			DMA_CCR6_MINC
#define DMA_CHANNEL6_CIRCULAR_BUFFER			DMA_CCR6_CIRC

extern void DMA_USART_Rx_Init(DMA_Channel_TypeDef* dmaChannel, 
															USART_TypeDef* uartPort, 
															uint8_t* uartRxBuffer, 
															uint8_t bufferSize, 
															uint32_t dmaConfig);																	
extern bool DMA_Rx_BufferFull(DMA_TypeDef* dmaPort, uint8_t dmaChannel);
extern uint8_t DMA_Rx_CNDTR(DMA_Channel_TypeDef* dmaChannel);
															
#endif //_DMA_H
