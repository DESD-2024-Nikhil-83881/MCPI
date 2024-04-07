/*
 * adc.c
 *
 *  Created on: Apr 6, 2024
 *      Author: nikhil
 */

#include "adc.h"

void ADC_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIOA->MODER |= 3 << GPIO_MODER_MODE1_Pos;

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	ADC1->SQR1 |= (1-1) << ADC_SQR1_L_Pos;

	ADC1->SQR3 |= 1 << ADC_SQR3_SQ1_Pos;

	ADC1->CR2 |= ADC_CR2_ADON;
}

uint16_t ADC_GetValue(void) {

	ADC1->CR2 |= ADC_CR2_SWSTART;

	while(!(ADC1->SR & ADC_SR_EOC))
		;

	return ADC1->DR;
}

