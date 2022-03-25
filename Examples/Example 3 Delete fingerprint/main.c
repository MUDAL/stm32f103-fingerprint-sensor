#include "stm32f10x.h"                  // Device header
#include "clock.h"
#include "systick.h"
#include "gpio.h"
#include "as608.h"

uint8_t id = 1; //fingerprint ID (1 to 127)
bool fingerprintDeleted = false;

//Functions
static void System_Config(void)
{
	Clock_HSI_8MHz_Init();
  SysTick_Init();
  GPIO_Reset();	
}

void DeleteFingerprint(uint8_t id);

int main(void)
{
	System_Config();
	AS608_Init();
	DeleteFingerprint(1);
	while(1)
	{
	}
}

void DeleteFingerprint(uint8_t id)
{
	if(AS608_DeleteModel(id) == FINGERPRINT_OK)
	{
		fingerprintDeleted = true;//Fingerprint deleted
	}
	else
	{
		fingerprintDeleted = false;//Can't delete fingerprint
	}
}
