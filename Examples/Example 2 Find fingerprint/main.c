#include "stm32f10x.h"                  // Device header
#include "clock.h"
#include "systick.h"
#include "gpio.h"
#include "as608.h"

uint8_t fingerprintStatus = FINGERPRINT_NOFINGER;
bool fingerprintOk = false;

//Functions
static void System_Config(void)
{
	Clock_HSI_8MHz_Init();
  SysTick_Init();
  GPIO_Reset();	
}

uint8_t FindFingerprint(void);

int main(void)
{
	System_Config();
	AS608_Init();
	while(1)
	{
		fingerprintStatus = FindFingerprint();
		switch(fingerprintStatus)
		{
			case FINGERPRINT_OK:
				fingerprintOk = true;
				break;
			case FINGERPRINT_NOTFOUND:
				fingerprintOk = false;
				break;
		}
	}
}

uint8_t FindFingerprint(void)
{
  uint8_t f_status = AS608_GetImage();
  if(f_status != FINGERPRINT_OK)  
  {
    return f_status;
  }
  f_status = AS608_Image2Tz(1);
  if(f_status != FINGERPRINT_OK) 
  { 
    return f_status;
  }
  f_status = AS608_FingerFastSearch();
  if(f_status != FINGERPRINT_OK)  
  {
    return f_status;
  }
  return f_status;	
}
