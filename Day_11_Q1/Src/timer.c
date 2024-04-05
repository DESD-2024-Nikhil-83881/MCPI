/*
 * timer.c
 *
 *  Created on: Apr 5, 2024
 *      Author: nikhil
 */
#include "timer.h"

void TimerInit(void) {


    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    GPIOC->MODER |= (2 << 6*2);
    GPIOC->AFR[0] |= (3 << GPIO_AFRL_AFSEL6_Pos);
    RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;

    TIM8->PSC = (TIM_PSC-1);
    TIM8->ARR = (100-1);
    TIM8->CCR1 = 1-1;

    TIM8->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
    TIM8->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM8->CR1 |= TIM_CR1_ARPE;
    TIM8->CCER |= TIM_CCER_CC1E;

    TIM8->BDTR |= TIM_BDTR_MOE;
    TIM8->CR1 |= TIM_CR1_CEN;
}

