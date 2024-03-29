/*
 * lcd.c
 *
 *  Created on: Mar 29, 2024
 *      Author: nikhil
 */


#include "lcd.h"

void LcdBusyWait(void) {
	uint32_t state;
	LCD_DATA_GPIO->MODER &= ~(BV(LCD_BF*2)|BV(LCD_BF*2+1));
	LCD_CTRL_GPIO->BSRR = BV(LCD_RS_CLR) | BV(LCD_RW) | BV(LCD_EN);

	do {
		state = LCD_DATA_GPIO->IDR;
	} while((state & BV(LCD_BF)) != 0);
	LCD_CTRL_GPIO->BSRR = BV(LCD_RW_CLR) | BV(LCD_EN_CLR);
	LCD_DATA_GPIO->MODER |= BV(LCD_BF*2);
}

void LcdWriteNibble(uint8_t rs, uint8_t val) {

	if(rs == LCD_CMD)
		LCD_CTRL_GPIO->BSRR = BV(LCD_RS_CLR) | BV(LCD_RW_CLR);
	else
		LCD_CTRL_GPIO->BSRR = BV(LCD_RS) | BV(LCD_RW_CLR);
	LCD_DATA_GPIO->ODR &= ~(BV(LCD_D7) | BV(LCD_D6) | BV(LCD_D5) | BV(LCD_D4));
	LCD_DATA_GPIO->ODR |= val << LCD_D4;
	LCD_CTRL_GPIO->BSRR = BV(LCD_EN);
	DelayMs(1);
	LCD_CTRL_GPIO->BSRR = BV(LCD_EN_CLR);
}

void LcdWrite(uint8_t rs, uint8_t val) {
	uint8_t high = val >> 4, low = val & 0x0F;
	LcdWriteNibble(rs, high);
	LcdWriteNibble(rs, low);
	LcdBusyWait();
	DelayMs(3);
}

void LcdInit(void) {
	DelayMs(50);

	RCC->AHB1ENR |= BV(LCD_DATA_GPIO_EN);
	RCC->AHB1ENR |= BV(LCD_CTRL_GPIO_EN);

	LCD_DATA_GPIO->MODER &= ~(BV(LCD_D7*2+1) | BV(LCD_D6*2+1) | BV(LCD_D5*2+1) | BV(LCD_D4*2+1));
	LCD_DATA_GPIO->MODER |= BV(LCD_D7*2) | BV(LCD_D6*2) | BV(LCD_D5*2) | BV(LCD_D4*2);
	LCD_DATA_GPIO->OSPEEDR &= ~(BV(LCD_D7*2+1) | BV(LCD_D7*2) | BV(LCD_D6*2+1) | BV(LCD_D6*2) | BV(LCD_D5*2+1) | BV(LCD_D5*2) | BV(LCD_D4*2+1) | BV(LCD_D4*2));
	LCD_DATA_GPIO->PUPDR &= ~(BV(LCD_D7*2+1) | BV(LCD_D7*2) | BV(LCD_D6*2+1) | BV(LCD_D6*2) | BV(LCD_D5*2+1) | BV(LCD_D5*2) | BV(LCD_D4*2+1) | BV(LCD_D4*2));
	LCD_DATA_GPIO->OTYPER &= ~(BV(LCD_D7) | BV(LCD_D6) | BV(LCD_D5) | BV(LCD_D4));
	LCD_DATA_GPIO->BSRR = (BV(LCD_D7_CLR) | BV(LCD_D6_CLR) | BV(LCD_D5_CLR) | BV(LCD_D4_CLR));

	LCD_CTRL_GPIO->MODER &= ~(BV(LCD_RS*2+1) | BV(LCD_RW*2+1) | BV(LCD_EN*2+1));
	LCD_CTRL_GPIO->MODER |= BV(LCD_RS*2) | BV(LCD_RW*2) | BV(LCD_EN*2);
	LCD_CTRL_GPIO->OSPEEDR &= ~(BV(LCD_RS*2+1) | BV(LCD_RS*2) | BV(LCD_RW*2+1) | BV(LCD_RW*2) | BV(LCD_EN*2+1) | BV(LCD_EN*2));
	LCD_CTRL_GPIO->PUPDR &= ~(BV(LCD_RS*2+1) | BV(LCD_RS*2) | BV(LCD_RW*2+1) | BV(LCD_RW*2) | BV(LCD_EN*2+1) | BV(LCD_EN*2));
	LCD_CTRL_GPIO->OTYPER &= ~(BV(LCD_RS) | BV(LCD_RW) | BV(LCD_EN));
	LCD_CTRL_GPIO->BSRR = (BV(LCD_RS_CLR) | BV(LCD_RW_CLR) | BV(LCD_EN_CLR));

	DelayMs(200);
	LcdWrite(LCD_CMD, LCD_FN_SET);
	LcdWrite(LCD_CMD, LCD_DISP_ON);
	LcdWrite(LCD_CMD, LCD_ENTRY_MODE);
	LcdWrite(LCD_CMD, LCD_CLEAR);
	DelayMs(200);
}

void LcdPuts(uint8_t line, char str[]) {
	int i;
	LcdWrite(LCD_CMD, line);
	for(i=0; str[i]!='\0'; i++)
		LcdWrite(LCD_DATA, str[i]);
}

