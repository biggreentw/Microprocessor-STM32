#include "C:\Users\TimYeh\workspace\C_LED_ex\inc\stm32l476xx.h"
#include "C:\Users\TimYeh\workspace\C_LED_ex\inc\helper_functions.h"
#include "C:\Users\TimYeh\workspace\C_LED_ex\inc\led_button.h"
#include "C:\Users\TimYeh\workspace\C_LED_ex\inc\7seg.h"

// Define pins for 4 leds
#define LED_gpioB GPIOB
#define LED_gpioC GPIOC
#define LED1_pin 3
#define LED2_pin 4
#define LED3_pin 5
#define LED4_pin 6

// Define pins for button (default use on-board button PC13)
#define BUTTON_gpio GPIOC
#define BUTTON_pin 13

// Define pins for 7seg
//#define SEG_gpio GPIOB
//#define DIN_pin 3
//#define CS_pin 4
//#define CLK_pin 5

// Use to decide which part of the code will run
// Use define & ifdef to control
#define lab_led
//#define lab_led_button
//#define lab_7seg_non_decode
//#define lab_7seg_decode

typedef struct {
    GPIO_TypeDef* port;
}gpio;



int main(){

#ifdef lab_led

	if(init_led(LED_gpioC, LED1_pin) != 0 || init_led(LED_gpioB, LED2_pin) != 0 || init_led(LED_gpioB, LED3_pin) != 0 || init_led(LED_gpioB, LED4_pin) != 0|| init_button(BUTTON_gpio, BUTTON_pin) != 0){
		// Fail to init LED ,BUTTON
		return -1;
	}

	// Direction to shift 0=left, 1=right
	int shift_direction = 0;
	int leds[4] = {LED1_pin, LED2_pin, LED3_pin, LED4_pin};

	int state=0;
	int button_press_cycle_per_second =10;
	int debounce_cycles = 100;
	int debounce_threshold = debounce_cycles*0.7;
	int last_button_state=0;


	int led_data[3] = {0b0001,0b000011,0b00000111};
	int led_data_B1[3] = {0b0001,0b000011,0b00000111};
	int led_data_B2[3] = {0b1000,0b110000,0b11100000};
	const gpio cs_[4] = {{GPIOC},{GPIOB},{GPIOB},{GPIOB}};//自建struct


	while(1){
	 for(int a=0;a<button_press_cycle_per_second;a++){

	    int pos_cnt=0;

	    for(int a=0;a<debounce_cycles;a++){
	        if(read_gpio(BUTTON_gpio, BUTTON_pin)==0){
		    pos_cnt++;
		}
	        delay_without_interrupt(1000/(button_press_cycle_per_second*debounce_cycles));
	    }

	    if(pos_cnt>debounce_threshold){//按鈕按下時
	        if(last_button_state==0){

	        }
		else{

		}
		last_button_state =1;//紀錄已按下
	    }
	    else{//按鈕沒按下
	        if(last_button_state==0){

	        }
		else{//若剛剛已按下 >> 判斷為按下剛放開
	            //模式切換順序

		    if(state==7){
		        state=0;
		    }
		    else{
		        state+=1;
		    }
	            //切換模式時，要將led_data、shift_direction初始化，不然會亂掉。
		    //led_data[0] = 0b0001;
	        led_data[1] = 0b000011;

	        //led_data[2] = 0b00000111;
		    //switch(state){
		    //case 0:
		    	//led_data[1] = 0b000011;

		    //}


	        shift_direction = 0;
	        for(int i=0;i<state;i++){
				if(shift_direction==0){
							 led_data[1] = (led_data[1]<<1);
						 }
						 else{
							 led_data[1] = (led_data[1]>>1);
						 }

						 // Check to see if change shift direction
						 if(led_data[1]==led_data_B1[1]  || led_data[1]==led_data_B2[1] ){
							 shift_direction = 1-shift_direction;
						 }
	        }



	        }
		last_button_state =0;
	    }
	}//for


	 for(int a=0;a<4;a++){
	     if((led_data[1]>>(a+1))&0x1){
	         reset_gpio(cs_[a].port, leds[a]);//1>>亮
	     }
	     else{
	 	set_gpio(cs_[a].port, leds[a]);//0>>暗
	     }
	 }





	 delay_without_interrupt(1000);
	}//while







#endif


	while(1){}

	return 0;
}
