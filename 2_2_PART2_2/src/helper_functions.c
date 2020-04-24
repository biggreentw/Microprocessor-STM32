#include "C:\Users\TimYeh\workspace\C_LED_ex\inc\helper_functions.h"

int read_gpio(GPIO_TypeDef* gpio, int pin){//gpio-> GPIOx_IDR ���U�h��X0 0000 0000 0000 0010 �nŪ�����ʳ̥k�� and 1
	return (gpio->IDR >> pin) & 1;
}
void set_gpio(GPIO_TypeDef* gpio, int pin){//�M�wpin�}��X
		gpio->BSRR |= (1 << pin);
}
void reset_gpio(GPIO_TypeDef* gpio, int pin){//�M�wpin�}��X
		gpio->BRR |= (1 << pin);
}
void toggle_output(GPIO_TypeDef* gpio, int pin){
	if(read_gpio(gpio, pin)==0){
		set_gpio(gpio, pin);
	}
	else{
		reset_gpio(gpio, pin);
	}
}

void delay_without_interrupt(int msec){
	int loop_cnt = 500*msec;
	while(loop_cnt){
		loop_cnt--;
	}
	return;
}
