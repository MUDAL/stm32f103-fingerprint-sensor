#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

extern void SysTick_Init(void);
extern uint32_t SysTick_GetTick(void);
extern void SysTick_DelayMs(uint32_t delayTime);

#endif /* INC_SYSTICK_H_ */
