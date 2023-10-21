/**
 ******************************************************************************
 * @file			: main.c
 * @author			: C.D HALWATURA
 * @brief			: LED Blink connected to PA5 and PC13 pin
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 CHIPCODELAB.
 * All rights reserved.
 *
 ******************************************************************************
 */

/*
 * Compatible Microcontrollers
 *
 * Series						- STM32F1xx / ARM - CORTEX M3
 * Package						- LQFP 48
 * Compatible Microcontrollers	- STM32F103C8T6, STM32F101xx, STM32F102xx, STM32F103xx, STM32F105xx, STM32F107xx
 */

#include <stdint.h>

/*
 *
 * LED Blink connected to PA5
 *
 * Port	- A
 * Pin	- 5
 *
 * LED Blink connected to PC13
 *
 * Port - C
 * Pin	- 13
 *
 * */

#define	PERIPH_BASE							(0x40000000UL)	//SYMLINK TO PERIPHEAL BASE ADDRESS => 0x40000000

//0x4002 0000 = 0x4000 0000 + 0x0002 0000 => (PERIPH_BASE + AHB1PERIPH_OFFSET)

#define APB2_PERIPH_OFFSET					(0x00010000UL) //0x00010000
#define APB2_PERIPH_BASE					(PERIPH_BASE + APB2_PERIPH_OFFSET) //0x40010000

//#define	AHB1_PERIPH_OFFSET				(0x00020000UL)	//SYMLINK TO AHB1_PERIPH_OFFSET => 0x0002 0000
//#define	AHB1_PERIPH_BASE				(PERIPH_BASE + AHB1_PERIPH_OFFSET) //SYMLINK TO AHB1_PERIPH_BASE => 0x4002 0000

//#define	GPIOA_OFFSET					(0x00UL)	//SYMLINK TO GPIO1_OFFSET => Identical to 0x0000 0000
//#define	GPIOA_BASE						(AHB1_PERIPH_BASE + GPIOA_OFFSET) //SYMLINK TO GPIOA_BASE => 0x4002 0000

#define	GPIOA_OFFSET						(0x00000800UL)	//SYMLINK TO GPIOA_OFFSET => 0x00000800
#define	GPIOA_BASE							(APB2_PERIPH_BASE + GPIOA_OFFSET) //SYMLINK TO GPIOA_BASE => 0x40010800

#define GPIOC_OFFSET						(0x00001000UL) //0x00001000
#define	GPIOC_BASE							(APB2_PERIPH_BASE + GPIOC_OFFSET) //0x40011000

//#define	RCC_OFFSET						(0x3800UL)	//SYMLINK TO THE RCC_OFFSET => 0x0000 3800
//#define	RCC_BASE						(AHB1_PERIPH_BASE + RCC_OFFSET)	//SYMLINK TO THE RCC_BASE	=>	0x4002 3800

#define	RCC_OFFSET							(0x00021000UL)	//SYMLINK TO THE RCC_OFFSET => 0x00021000
#define	RCC_BASE							(PERIPH_BASE + RCC_OFFSET)	//SYMLINK TO THE RCC_BASE	=>	0x40021000

//#define	AHB1_ENR_OFFSET					(0x30UL)	//SYMLINK TO AHB1_ENR_OFFSET =>	0x30
//#define	RCC_AHB1_EN_R					(*(volatile unsigned int *)(RCC_BASE + AHB1_ENR_OFFSET))	//SYMLINK TO RCC_AHB1_EN_R	=>	0x4002 3800 + 0x30

//RCC_APB2ENR
#define	APB2_ENR_OFFSET						(0x18UL)	//SYMLINK TO AHB1_ENR_OFFSET =>	0x18
#define	RCC_APB2_EN_R						(*(volatile unsigned int *)(RCC_BASE + APB2_ENR_OFFSET))	//SYMLINK TO RCC_AHB1_EN_R	=>	0x40021000 + 0x18

//#define	RCC_APB2_EN_R						(*(uint32_t*)(0x40021018UL))

//#define	GPIOA_EN						(1U<<0)		//SHIFT 1 TO POSSITION 0

//RCC_APB2ENR
//Bit 2 IOPAEN: IO port A clock enable
//0: IO port A clock disabled
//1: IO port A clock enabled
#define IOPA_EN								(1U<<2)		//SHIFT 1 TO POSSITION 2
#define IOPC_EN								(1U<<4)

//GPIOx_MODER
//#define	MODE_R_OFFSET					(0x00UL)	//SYMLINK TO MODE REGISTER OFFSET
//#define	GPIOA_MODE_R					(*(volatile unsigned int *)(GPIOA_BASE + MODE_R_OFFSET))	//0x4002 0000 + 0x00

//GPIOx_CRL
#define	GPIOA_CRL_OFFSET					(0x00UL)	//SYMLINK TO MODE REGISTER OFFSET
#define	GPIOA_MODE_R						(*(volatile unsigned int *)(GPIOA_BASE + GPIOA_CRL_OFFSET))	//0x4002 0000 + 0x00
//#define	GPIOA_MODE_R						(*(uint32_t*)(0x40010800UL))


//GPIOx_CRH
#define	GPIOC_CRL_OFFSET					(0x04UL)
#define	GPIOC_MODE_R						(*(volatile unsigned int *)(GPIOC_BASE + GPIOC_CRL_OFFSET))
//#define	GPIOC_MODE_R						(*(uint32_t*)(0x40011004UL))


//#define	OD_R_OFFSET						(0x14UL)	//SYMLINK TO GPIOx_ODR OFFSET
//#define	GPIOA_OD_R						(*(volatile unsigned int *)(GPIOA_BASE + OD_R_OFFSET))	//SYMLINK TO GPIOA_ODR

//GPIOx_ODR
#define	OD_R_OFFSET							(0x0CUL)	//SYMLINK TO GPIOx_ODR OFFSET
#define	GPIOA_OD_R							(*(volatile unsigned int *)(GPIOA_BASE + OD_R_OFFSET))	//SYMLINK TO GPIOA_ODR
#define	GPIOC_OD_R							(*(volatile unsigned int *)(GPIOC_BASE + OD_R_OFFSET))

//#define	GPIOA_OD_R							(*(uint32_t*)(0x4001080CUL))
//#define	GPIOC_OD_R							(*(uint32_t*)(0x4001100CUL))

#define	PIN5								(1U<<5)		//SET BIT 1 TO ODR5 REGISTER TO TURN ON PIN
#define	PIN13								(1U<<13)

#define	LED_1								 PIN5
#define	LED_2								 PIN13


int main(void) {
	//1.	Enable clock access to the GPIOA

	/*
	 * Notes
	 *
	 * RCC_AHB1_EN_R = GPIOA_EN;	=> We can not do like this because
	 * RCC_AHB1_EN_R = ob 0000 0000 0000 1010 1000 0000 0000 0000	=>These bits may already enabled by the other part of the system.
	 * So we only need to change exact bit value. So we need to do as follows.
	 * RCC_AHB1_EN_R = ob 0000 0000 0000 1010 1000 0000 0000 0001
	 * So we need to use with or operator
	 *
	 * */

	//RCC_AHB1_EN_R |= GPIOA_EN;//Use |= operator instead of = here. This is call friendly programming.


	RCC_APB2_EN_R |= IOPA_EN;
	RCC_APB2_EN_R |= IOPC_EN;


	//2.	Set PA5 as a output pin => Onboard LED, LD2

	//GPIOA_MODE_R |= (1U << 10);	//Set bit number 10 to value 1
	//GPIOA_MODE_R &= ~(1U << 11);	//Set bit number 11 to value 0

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
	GPIOA_MODE_R &= ~(1U << 22);	//Set bit position 22 to value 0
	GPIOA_MODE_R &= ~(1U << 23); 	//Set bit position 23 to value 0

	//MODEy[1:0]: Port x mode bits
	/*
	 * 00: Input mode (reset state)
	 * 01: Output mode, max speed 10 MHz.
	 * 10: Output mode, max speed 2 MHz.
	 * 11: Output mode, max speed 50 MHz.
	 *
	 * */
	GPIOA_MODE_R |= (1U << 20);		//Set bit position 20 to value 1
	GPIOA_MODE_R &= ~(1U << 21);		//Set bit position 21 to value 1


	/*
	 * GPIOx_CRL
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
	GPIOC_MODE_R &= ~(1U << 22);	//Set bit position 22 to value 0
	GPIOC_MODE_R &= ~(1U << 23); 	//Set bit position 23 to value 0

	//MODEy[1:0]: Port x mode bits
	/*
	 * 00: Input mode (reset state)
	 * 01: Output mode, max speed 10 MHz.
	 * 10: Output mode, max speed 2 MHz.
	 * 11: Output mode, max speed 50 MHz.
	 *
	 * */
	GPIOC_MODE_R |= (1U << 20);		//Set bit position 20 to value 1
	GPIOC_MODE_R &= ~(1U << 21);	//Set bit position 21 to value 1


	while (1) {
		GPIOA_OD_R ^= LED_1;	// ^ == Toggle operator
		GPIOC_OD_R ^= LED_2;	// ^ == Toggle operator

		for (int i = 0; i < 250000; i++) {
		} //Set delay}

		GPIOA_OD_R ^= LED_1;	// ^ == Toggle operator
		GPIOC_OD_R ^= LED_2;	// ^ == Toggle operator

		for (int i = 0; i < 500000; i++) {
		} //Set delay}
	}
}
