/*
 * timer.c
 *
 *  Created on: Apr 4, 2024
 *      Author: nikhil
 */

#include "timer.h"

void TimerInit(void) {

	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
	TIM7->PSC = TIM_PR-1;
}

void TimerDelayMs(uint32_t ms) {

	uint16_t cnt = (TCLK / 1000) * ms / TIM_PR;
	TIM7->ARR = cnt - 1;
	TIM7->CNT = 0;
	TIM7->CR1 |= TIM_CR1_CEN;
	while(! (TIM7->SR & TIM_SR_UIF))
		;
	TIM7->SR &= ~TIM_SR_UIF;
	TIM7->CR1 &= ~TIM_CR1_CEN;
}




