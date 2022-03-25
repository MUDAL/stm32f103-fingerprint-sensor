#include "stm32f10x.h"                  // Device header
#include "clock.h"
#include "systick.h"
#include "gpio.h"
#include "as608.h"

uint8_t id = 1; //fingerprint ID (1 to 127)
bool removeFingerPlaceFinger = false;
bool fingerprintStored = false;

//Functions
static void System_Config(void)
{
	Clock_HSI_8MHz_Init();
  SysTick_Init();
  GPIO_Reset();	
}

void EnrollFinger(void);

int main(void)
{
	System_Config();
	AS608_Init();
	EnrollFinger();
	while(1)
	{
	}
}

void EnrollFinger(void)
{
	while(AS608_GetImage() != FINGERPRINT_OK){}
	while(AS608_Image2Tz(1) != FINGERPRINT_OK){}
	removeFingerPlaceFinger = true; //Remove finger then place same finger again
	while(AS608_GetImage() != FINGERPRINT_OK){}
	while(AS608_Image2Tz(2) != FINGERPRINT_OK){}
	removeFingerPlaceFinger = false;
	if(AS608_CreateModel() == FINGERPRINT_OK)
	{//Prints matched
		if(AS608_StoreModel(id) == FINGERPRINT_OK)
		{
			fingerprintStored = true;//Fingerprint stored
		}
	}
	else
	{
		fingerprintStored = false;//Prints unmatched
	}
}

