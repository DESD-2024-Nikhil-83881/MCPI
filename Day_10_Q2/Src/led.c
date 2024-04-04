/*
 * led.c
 *
 *  Created on: Mar 27, 2024
 *      Author: nikhil
 */

#include "led.h"
#include "stm32f4xx.h"

void LedInit(uint32_t pin) {

	RCC->AHB1ENR |= BV(LED_GPIO_EN);
	LED_GPIO->MODER &= ~BV(pin * 2 + 1);
	LED_GPIO->MODER |= BV(pin * 2);
	LED_GPIO->OSPEEDR &= ~(BV(pin * 2 + 1) | BV(pin * 2));
	LED_GPIO->PUPDR &= ~(BV(pin * 2 + 1) | BV(pin * 2));
	LED_GPIO->OTYPER &= ~BV(pin);
}

void LedOn(uint32_t pin) {

	LED_GPIO->ODR |= BV(pin);
}

void LedOff(uint32_t pin) {

	LED_GPIO->ODR &= ~BV(pin);
}

void LedBlink(uint32_t pin, uint32_t delay) {
	LedOn(pin);
	DelayMs(delay);
	LedOff(pin);
}

