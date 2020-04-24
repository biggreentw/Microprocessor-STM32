#include "C:\Users\TimYeh\workspace\C_LED_ex\inc\led_button.h"

// Only allow GPIOA and GPIOB for now
// Can easily extended by adding "else if" cases
int init_led(GPIO_TypeDef* gpio, int LED_pin){
	// Enable AHB2 Clock
	/*
	if(gpio==GPIOA){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;//啟用B C RCC->AHB2ENR= 0x06    (0000 0110)  >> stm32.h的關係  >> 用程式啟用
	}
	else if(gpio==GPIOB){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	}
	else{
		// Error! Add other cases to suit other GPIO pins
		return -1;
	}*/
	RCC->AHB2ENR =0x07;

	gpio->OTYPER =0x000;


	// Set GPIO pins to output mode (01)
	// First Clear bits(&) then set bits(|)
	gpio->MODER &= ~(0b11 << (2*LED_pin));//歸零
	gpio->MODER |= (0b01 << (2*LED_pin));//把01移到對應pin

	return 0;
}

// Only allow GPIOC for now
// Can easily extended by adding "else if" cases
int init_button(GPIO_TypeDef* gpio, int button_pin){
	// Enable AHB2 Clock
	if(gpio==GPIOC){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	}
	else{
		// Error! Add other cases to suit other GPIO pins
		return -1;
	}

	// Set GPIO pins to input mode (00)
	// First Clear bits(&) then set bits(|)
	gpio->MODER &= ~(0b11 << (2*button_pin));
	gpio->MODER |= (0b00 << (2*button_pin));

	return 0;
}
