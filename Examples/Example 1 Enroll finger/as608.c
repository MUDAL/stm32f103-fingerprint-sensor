#include "stm32f10x.h"                  // Device header
#include "systick.h"
#include "gpio.h"
#include "uart.h"
#include "dma.h"
#include "as608.h"
#include <string.h>

//Estimated size of packet to be received from the fingerprint sensor
#define RXBUFFER_SIZE	50

typedef struct
{
	uint16_t start_code; ///< "Wakeup" code for packet detection
	uint8_t address[4]; ///< 32-bit Fingerprint sensor address
	uint8_t type; ///< Type of packet
	uint16_t length; ///< Length of packet
	uint8_t data[64]; ///< The raw buffer for packet payload
}as608Packet_t;
	
static as608Packet_t packet;
static uint8_t rxPacketBuffer[RXBUFFER_SIZE];

//Static functions
static void PacketInit(uint8_t type,uint16_t length,uint8_t* data)
{
	packet.start_code = FINGERPRINT_STARTCODE;
	packet.type = type;
	packet.length = length;
	for(uint8_t i = 0; i < 4; i++)
	{
		packet.address[i] = 0xFF;
	}
	if(length < 64)
	{
		memcpy(packet.data,data,length);
	}
	else
	{
		memcpy(packet.data,data,64);
	}
}

/**
	@brief Process a packet and send it over UART to the sensor
*/
static void WriteStructuredPacket(void)
{
	USART_TransmitByte(USART1,(packet.start_code >> 8));
	USART_TransmitByte(USART1,(packet.start_code & 0xFF));
	USART_TransmitBytes(USART1,packet.address,4);
	USART_TransmitByte(USART1,packet.type);
	
	uint16_t wireLength = packet.length + 2;
	USART_TransmitByte(USART1,wireLength >> 8);
	USART_TransmitByte(USART1,wireLength & 0xFF);
	
  uint16_t sum = (wireLength >> 8) + (wireLength & 0xFF) + packet.type;
  for (uint8_t i = 0; i < packet.length; i++)
	{
		USART_TransmitByte(USART1,packet.data[i]);
    sum += packet.data[i];
  }	
  USART_TransmitByte(USART1,sum >> 8);
  USART_TransmitByte(USART1,sum & 0xFF);	
}

/**
	@brief Receives data over UART from the sensor via DMA then processes 
	the data into a packet
	@return <code>FINGERPRINT_OK</code> on success
	@return <code>FINGERPRINT_TIMEOUT</code> or
					<code>FINGERPRINT_BADPACKET</code> on failure
*/
static uint8_t GetStructuredPacket(void)
{
	uint8_t nReceivedBytes = 0;
	while(1)
	{
		if(USART_RxIdleLineDetected(USART1))
		{
			nReceivedBytes = RXBUFFER_SIZE - DMA_Rx_CNDTR(DMA1_Channel5);
			break;
		}
	}
	if(rxPacketBuffer[0] == (FINGERPRINT_STARTCODE >> 8))
	{
		packet.start_code = (uint16_t)rxPacketBuffer[0] << 8;
	}
	packet.start_code |= rxPacketBuffer[1];
	if(packet.start_code != FINGERPRINT_STARTCODE)
	{
		//Re-initialize the rx packet buffer
		DMA_USART_Rx_Init(DMA1_Channel5,
											USART1,
											rxPacketBuffer,
											RXBUFFER_SIZE, 
											DMA_CHANNEL5_MEMORY_INC_MODE |
											DMA_CHANNEL5_CIRCULAR_BUFFER |
											DMA_CHANNEL_ENABLE);	
		memset(rxPacketBuffer,0,nReceivedBytes); //clear the rx packet buffer
		return FINGERPRINT_BADPACKET;
	}
	for(uint8_t i = 2; i <= 5; i++)
	{
		packet.address[i - 2] = rxPacketBuffer[i];
	}
	packet.type = rxPacketBuffer[6];
	packet.length = (uint16_t)rxPacketBuffer[7] << 8;
	packet.length |= rxPacketBuffer[8];
	for(uint8_t i = 9; i < nReceivedBytes; i++)
	{
		packet.data[i - 9] = rxPacketBuffer[i];
		if((i - 8) == packet.length)
		{
			//Re-initialize the rx packet buffer
			DMA_USART_Rx_Init(DMA1_Channel5,
												USART1,
												rxPacketBuffer,
												RXBUFFER_SIZE, 
												DMA_CHANNEL5_MEMORY_INC_MODE |
												DMA_CHANNEL5_CIRCULAR_BUFFER |
												DMA_CHANNEL_ENABLE);	
			memset(rxPacketBuffer,0,nReceivedBytes); //clear the rx packet buffer			
			return FINGERPRINT_OK;
		}
	}
	return FINGERPRINT_BADPACKET;
}

//Variadic macros
/**
 * @brief Gets the command packet
 */
#define GET_CMD_PACKET(...)                                                    \
  uint8_t data[] = {__VA_ARGS__};                                              \
  PacketInit(FINGERPRINT_COMMANDPACKET,sizeof(data),data);                     \
  WriteStructuredPacket();                                               			 \
																																							 \
  if (GetStructuredPacket() != FINGERPRINT_OK)                          			 \
    return FINGERPRINT_PACKETRECIEVEERR;                                       \
  if (packet.type != FINGERPRINT_ACKPACKET)                                    \
    return FINGERPRINT_PACKETRECIEVEERR;

/**
 * @brief Sends the command packet
 */
#define SEND_CMD_PACKET(...)                                                   \
  GET_CMD_PACKET(__VA_ARGS__);                                                 \
  return packet.data[0];

//External functions
/**
	@brief Initializes fingerprint module, sets baud rate to 57600
*/
void AS608_Init(void)
{
	SysTick_DelayMs(1000); //Boot-up time
	//GPIO configuration for USART1 Tx
	GPIO_OutputInit(GPIOA,
									GPIO_PORT_REG_HIGH,
									GPIO_PIN9_OUTPUT_MODE_2MHZ,
									GPIO_PIN9_ALT_FUNC_PUSH_PULL);
	//GPIO configuration for USART1 Rx
	GPIO_InputInit(GPIOA,
								 GPIO_PORT_REG_HIGH,
								 GPIO_PIN10,
								 GPIO_PIN10_INPUT_PULLUP_OR_PULLDOWN,
								 GPIO_PULLUP_ENABLE);	
	USART_Init(USART1,BAUD_57600,RX_DMA_ENABLE,(USART_TX_ENABLE | USART_RX_ENABLE));
	//DMA 1 channel 5 configuration for USART1 Rx
	DMA_USART_Rx_Init(DMA1_Channel5,
										USART1,
										rxPacketBuffer,
										RXBUFFER_SIZE, 
										DMA_CHANNEL5_MEMORY_INC_MODE |
										DMA_CHANNEL5_CIRCULAR_BUFFER |
										DMA_CHANNEL_ENABLE);	
}

/**
	@brief Ask the sensor to take an image of the finger pressed on surface
	@return <code>FINGERPRINT_OK</code> on success
	@return <code>FINGERPRINT_NOFINGER</code> if no finger detected
	@return <code>FINGERPRINT_PACKETRECIEVEERR</code> on communication error
	@return <code>FINGERPRINT_IMAGEFAIL</code> on imaging error
*/
uint8_t AS608_GetImage(void)
{
  SEND_CMD_PACKET(FINGERPRINT_GETIMAGE);	
}

/**
	@brief Ask the sensor to convert image to feature template
	@param slot Location to place feature template (put one in 1 and another in
	2 for verification to create model)
	@return <code>FINGERPRINT_OK</code> on success
	@return <code>FINGERPRINT_IMAGEMESS</code> if image is too messy
	@return <code>FINGERPRINT_PACKETRECIEVEERR</code> on communication error
	@return <code>FINGERPRINT_FEATUREFAIL</code> on failure to identify
	fingerprint features
	@return <code>FINGERPRINT_INVALIDIMAGE</code> on failure to identify
	fingerprint features
*/
uint8_t AS608_Image2Tz(uint8_t slot)
{
  SEND_CMD_PACKET(FINGERPRINT_IMAGE2TZ, slot);	
}

/**
	@brief Ask the sensor to take two print feature template and create a
	model
	@return <code>FINGERPRINT_OK</code> on success
	@return <code>FINGERPRINT_PACKETRECIEVEERR</code> on communication error
	@return <code>FINGERPRINT_ENROLLMISMATCH</code> on mismatch of fingerprints
*/
uint8_t AS608_CreateModel(void)
{
  SEND_CMD_PACKET(FINGERPRINT_REGMODEL);	
}

/**
	@brief Ask the sensor to delete ALL models in memory
	@return <code>FINGERPRINT_OK</code> on success
	@return <code>FINGERPRINT_BADLOCATION</code> if the location is invalid
	@return <code>FINGERPRINT_FLASHERR</code> if the model couldn't be written
	to flash memory
	@return <code>FINGERPRINT_PACKETRECIEVEERR</code> on communication error
*/
uint8_t AS608_EmptyDatabase(void)
{
  SEND_CMD_PACKET(FINGERPRINT_EMPTY);	
}

/**
	@brief Ask the sensor to store the calculated model for later matching
	@param location The model location #
	@return <code>FINGERPRINT_OK</code> on success
	@return <code>FINGERPRINT_BADLOCATION</code> if the location is invalid
	@return <code>FINGERPRINT_FLASHERR</code> if the model couldn't be written
	to flash memory
	@return <code>FINGERPRINT_PACKETRECIEVEERR</code> on communication error
*/
uint8_t AS608_StoreModel(uint16_t id)
{
  SEND_CMD_PACKET(FINGERPRINT_STORE, 0x01, (uint8_t)(id >> 8), (uint8_t)(id & 0xFF));	
}

/**
	@brief Ask the sensor to load a fingerprint model from flash into buffer 1
	@param location The model location #
	@return <code>FINGERPRINT_OK</code> on success
	@return <code>FINGERPRINT_BADLOCATION</code> if the location is invalid
	@return <code>FINGERPRINT_PACKETRECIEVEERR</code> on communication error
*/
uint8_t AS608_LoadModel(uint16_t id)
{
  SEND_CMD_PACKET(FINGERPRINT_LOAD, 0x01, (uint8_t)(id >> 8), (uint8_t)(id & 0xFF));	
}

/**
	@brief Ask the sensor to transfer 256-byte fingerprint template from the
	buffer to the UART
	@return <code>FINGERPRINT_OK</code> on success
	@return <code>FINGERPRINT_PACKETRECIEVEERR</code> on communication error
*/
uint8_t AS608_GetModel(void)
{
  SEND_CMD_PACKET(FINGERPRINT_UPLOAD, 0x01);	
}

/**
	@brief Ask the sensor to delete a model in memory
	@param location The model location #
	@return <code>FINGERPRINT_OK</code> on success
	@return <code>FINGERPRINT_BADLOCATION</code> if the location is invalid
	@return <code>FINGERPRINT_FLASHERR</code> if the model couldn't be written
	to flash memory
	@return <code>FINGERPRINT_PACKETRECIEVEERR</code> on communication error
*/
uint8_t AS608_DeleteModel(uint16_t id)
{
  SEND_CMD_PACKET(FINGERPRINT_DELETE, (uint8_t)(id >> 8), (uint8_t)(id & 0xFF), 0x00, 0x01);	
}

/**
	@brief Ask the sensor to search the current slot 1 fingerprint features to
	match saved templates. The matching location is stored in <b>fingerID</b> and
	the matching confidence in <b>confidence</b>
	@return <code>FINGERPRINT_OK</code> on fingerprint match success
	@return <code>FINGERPRINT_NOTFOUND</code> no match made
	@return <code>FINGERPRINT_PACKETRECIEVEERR</code> on communication error
*/
uint8_t AS608_FingerFastSearch(void)
{
  GET_CMD_PACKET(FINGERPRINT_HISPEEDSEARCH, 0x01, 0x00, 0x00, 0x00, 0xA3);
  return packet.data[0];	
}

