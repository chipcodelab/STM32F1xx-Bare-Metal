/**
 ******************************************************************************
 * @file			: main.c
 * @type			: Source File
 * @author			: C.D HALWATURA
 * @date			: 21 Oct 2024
 * @brief			: LED Blink connected to PA5 and PC13 pin
 *
 * Copyright (c) 2023 CHIPCODELAB All rights reserved.
 *
 ******************************************************************************
 */

/******************************************************************************
 *                    	Compatible Microcontrollers
 * 						===========================
 *
 *
 * Series						- 	STM32F1xx / ARM - CORTEX M3
 * Package						- 	LQFP 48
 * Compatible Microcontrollers	- 	STM32F103C8T6
 * 									STM32F101xx
 * 									STM32F102xx
 * 									STM32F103xx
 * 									STM32F105xx
 * 									STM32F107xx
******************************************************************************/

/************************************
 * STANDARED C HEADERS
 ************************************/

#include <stdint.h>

/************************************
 * MAIN HEADER
 ************************************/

/************************************
 * GLOBAL DEFINES
 ************************************/

/************************************
 * EXTERNAL VARIABLES
 ************************************/

/******************************************************************************/
/*                                                                            */
/*                            Port/Pin Assignments                            */
/*                                                                            */
/******************************************************************************/

/*
 *
 * LED_1 PA5
 *
 * Port	- A
 * Pin	- 5
 *
 * LED_2 PC13
 *
 * Port - C
 * Pin	- 13
 *
 * */

/*************** Registry boundary address and bit definitions *****************/

#define	PERIPH_BASE							(0x40000000UL) /* Symlink to Peripheral base address => 0x40000000 */


#define APB2_OFFSET							(0x10000UL) /* APB2 Bus boundary address offset => 0x10000 */
#define APB2_BASE							(PERIPH_BASE + APB2_OFFSET) /* APB2 base address =>  0x40010000 */

#define	GPIOA_OFFSET						(0x800UL)	/* GPIOA offset => 0x800 */
#define	GPIOA_BASE							(APB2_BASE + GPIOA_OFFSET) /*GPIOA Base => 0x40010800 */

#define GPIOC_OFFSET						(0x1000UL) /*GPIOC offset => 0x1000 */
#define	GPIOC_BASE							(APB2_BASE + GPIOC_OFFSET) /*GPIOC base => 0x40011000 */

#define	RCC_OFFSET							(0x21000UL) /*RCC offset => 0x21000 */
#define	RCC_BASE							(PERIPH_BASE + RCC_OFFSET)	/*RCC base => 0x40021000 */



/*
 *
 * RCC_APB2ENR - APB2 peripheral clock enable register
 *
 */

#define	RCC_APB2ENR_OFFSET					(0x18UL) /*RCC_APB2ENR register offset => 0x18 */
#define	RCC_APB2ENR							(*(volatile unsigned int *)(RCC_BASE + RCC_APB2ENR_OFFSET))	/* RCC_AHB1ENR register address => 0x40021018 */

/*
 *
 * RCC_APB2ENR - APB2 peripheral clock enable register
 *
 * IOPA_EN => GPIOA for PA5, port A
 * IOPC_EN => GPIOC for PC13, port C
 *
 * Bit 2 IOPAEN: IO port A clock enable
 * 0: IO port A clock disabled
 * 1: IO port A clock enabled
 *
 */

#define IOPA_EN								(1U<<2) /* Shift 1 to registry bit position 2 */
#define IOPC_EN								(1U<<4) /* Shift 1 to registry bit position 4 */

/*
 *
 * GPIOx_CRL - Port configuration register low
 *
 */

#define	GPIOA_CRL_OFFSET					(0x00UL) /* GPIOx_CRL register address offset => 0x00 */
#define	GPIOA_MODE_R						(*(volatile unsigned int *)(GPIOA_BASE + GPIOA_CRL_OFFSET))	/* GPIOA Mode register => 0x40010800 */

/*
 *
 * GPIOx_CRH - Port configuration register high
 *
 */

#define	GPIOC_CRL_OFFSET					(0x04UL) /* GPIOx_CRH register address offset => 0x04 */
#define	GPIOC_MODE_R						(*(volatile unsigned int *)(GPIOC_BASE + GPIOC_CRL_OFFSET)) /* GPIOC mode register => 0x40011004 */

/*
 *
 * GPIOx_ODR - Port output data register
 *
 */

#define	GPIOx_ODR_OFFSET					(0x0CUL) /* GPIOx_ODR offset => 0x0C */
#define	GPIOA_ODR							(*(volatile unsigned int *)(GPIOA_BASE + GPIOx_ODR_OFFSET))	/* GPIOA_ODR register => 0x4001080C */
#define	GPIOC_ODR							(*(volatile unsigned int *)(GPIOC_BASE + GPIOx_ODR_OFFSET)) /* GPIOC_ODR register => 0x4001100C */
//#define	GPIOA_ODR						(*(uint32_t*)(0x4001080CUL))
//#define	GPIOC_ODR						(*(uint32_t*)(0x4001100CUL))

#define	PIN5								(1U<<5) /* Shift 1 to registry bit position 5 */
#define	PIN13								(1U<<13) /* Shift 1 to registry bit position 13 */

#define	LED_1								 PIN5 /* LED1 - PA5 */
#define	LED_2								 PIN13 /* LED2 - PC13 */


int main(void) {
	/* 1.	Enable clock access to the GPIOA(IOPA), GPIOC(IOPC) using RCC_APB2ENR register */


	RCC_APB2ENR |= IOPA_EN;
	RCC_APB2ENR |= IOPC_EN;


	/* 2.	Set PA5 as a output mode using GPIOx_CRL register */


	/*
	 * GPIOx_CRL
	 *
	 * SET PA5 as output pin
	 *
	 * Set push-pull mode
	 *
	 * CNF5[1:0] => 0 => 0
	 * CNF5[1:0] => 1 => 0
	 *
	 * Max 50MHz out put
	 *
	 * MODE5[1:0] => 0 => 1
	 * MODE5[1:0] => 1 => 1
	 *
	 */

	//CNFy[1:0]: Port x configuration bits
	GPIOA_MODE_R &= ~(1U << 22); /* Set bit position 22 to value 0 */
	GPIOA_MODE_R &= ~(1U << 23); /* Set bit position 23 to value 0 */

	//MODEy[1:0]: Port x mode bits
	/*
	 * 00: Input mode (reset state)
	 * 01: Output mode, max speed 10 MHz.
	 * 10: Output mode, max speed 2 MHz.
	 * 11: Output mode, max speed 50 MHz.
	 *
	 */

	GPIOA_MODE_R |= (1U << 20); /* Set bit position 20 to value 1 */
	GPIOA_MODE_R &= ~(1U << 21); /* Set bit position 21 to value 1 */

	/* 3.	Set PC13 as a output mode using GPIOx_CRH register */

	/*
	 * GPIOx_CRH
	 *
	 * SET PC13 as output pin
	 *
	 * Set push-pull mode
	 *
	 * CNF5[1:0] => 0 => 0
	 * CNF5[1:0] => 1 => 0
	 *
	 * Max 50MHz out put
	 *
	 * MODE5[1:0] => 0 => 1
	 * MODE5[1:0] => 1 => 1
	 *
	 */

	//CNFy[1:0]: Port x configuration bits
	GPIOC_MODE_R &= ~(1U << 22); /* Set bit position 22 to value 0 */
	GPIOC_MODE_R &= ~(1U << 23); /* Set bit position 23 to value 0 */

	//MODEy[1:0]: Port x mode bits
	/*
	 * 00: Input mode (reset state)
	 * 01: Output mode, max speed 10 MHz.
	 * 10: Output mode, max speed 2 MHz.
	 * 11: Output mode, max speed 50 MHz.
	 *
	 */

	GPIOC_MODE_R |= (1U << 20);	/* Set bit position 20 to value 1 */
	GPIOC_MODE_R &= ~(1U << 21); /* Set bit position 21 to value 1 */


	while (1) {
		GPIOA_ODR ^= LED_1;	/* ^ == Toggle bit */
		GPIOC_ODR ^= LED_2;	/* ^ == Toggle bit */

		for (int i = 0; i < 250000; i++) {} /* Add delay */

		GPIOA_ODR ^= LED_1;	/* ^ == Toggle bit */
		GPIOC_ODR ^= LED_2;	/* ^ == Toggle bit */

		for (int i = 0; i < 500000; i++) {} /* Add delay */
	}
}
