/*
 * i2c.c
 *
 *  Created on: Apr 2, 2024
 *      Author: Nilesh
 */

#include "i2c.h"

void I2CStart() {
    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));
}

void I2CRepeatStart() {
	I2CStart();
}

void I2CStop() {
    I2C1->CR1 |= I2C_CR1_STOP;
    while(!(I2C1->SR2 & I2C_SR2_BUSY));
}

void I2CSendSlaveAddress(uint8_t slaveaddr) {
    // send slaveaddr
    // wait until address is sent
    I2C1->DR = slaveaddr;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    // dummy read to clear flags
    (void)I2C1->SR1;
    (void)I2C1->SR2; // clear addr condition
}

void I2CSendData(uint8_t val) {
    // send data in write mode
    // wait until address is sent
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    // write dr and clear flags
    I2C1->DR = val;
    // wait until data byte is transferred
    while (!(I2C1->SR1 & I2C_SR1_BTF));
}

uint8_t I2CRecvDataAck(void) {
	uint8_t val;
	I2C1->CR1 |= I2C_CR1_ACK | I2C_CR1_POS;
	// wait until receive buffer is not empty
	while (!(I2C1->SR1 & I2C_SR1_RXNE));
	// read content and clear flags
	val = (uint8_t)I2C1->DR;
	return val;
}

uint8_t I2CRecvDataNAck(void) {
	uint8_t val;
	I2C1->CR1 &= ~(I2C_CR1_ACK | I2C_CR1_POS);
	// wait until receive buffer is not empty
	while (!(I2C1->SR1 & I2C_SR1_RXNE));
	// read content and clear flags
	val = I2C1->DR;
	return val;
}

int I2CIsDeviceReady(uint8_t slaveaddr) {

	slaveaddr &= ~BV(0);

    I2C1->DR = slaveaddr;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));

    (void)I2C1->SR2;
	return 1;
}

void I2CInit() {

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	DelayMs(50);


    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER &= ~(3U << I2C1_SCL*2);
    GPIOB->MODER |=  (2 << I2C1_SCL*2);
    GPIOB->OTYPER |= (1 << I2C1_SCL);
    GPIOB->PUPDR &= ~(3U << I2C1_SCL*2);
    GPIOB->MODER &= ~(3U << I2C1_SDA*2);
    GPIOB->MODER |=  (2 << I2C1_SDA*2);
    GPIOB->OTYPER |= (1 << I2C1_SDA);
    GPIOB->PUPDR &= ~(3U << I2C1_SDA*2);


    GPIOB->AFR[0] |= (4 << GPIO_AFRL_AFSEL6_Pos);
    GPIOB->AFR[0] |= (4 << GPIO_AFRL_AFSEL7_Pos);


	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	DelayMs(50);

    I2C1->CR1 = I2C_CR1_SWRST;
    I2C1->CR1 = 0;

    I2C1->CR2 &= ~(I2C_CR2_ITERREN);
    I2C1->CCR &= ~(1 << I2C_CCR_FS_Pos);

    I2C1->CR1 |= (1<<I2C_CR1_ACK_Pos);


    I2C1->CR2 |= (16 << I2C_CR2_FREQ_Pos);
    I2C1->CCR |= (80 << I2C_CCR_CCR_Pos);

    I2C1->TRISE |= (17 << I2C_TRISE_TRISE_Pos);

    I2C1->CR1 |= I2C_CR1_PE;
}
