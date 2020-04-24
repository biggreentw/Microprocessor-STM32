# 【108下】1344微處理機 Microprocessor
- NCTU ME 0511019張峻豪
-  [Github](https://github.com/biggreentw/Microprocessor-STM32)
- robert0908tw@gmail.com
---
## 腳位
![](https://i.imgur.com/pe74mIM.jpg)

---
## Lab2-2
1. ### 相對與對路徑
    不知為何用不了相對路徑，只能用絕對路徑。
```cpp=
//相對路徑 (X)
#include "..\inc\stm32l476xx.h"

//絕對路徑 (O)
#include "C:\Users\TimYeh\workspace\C_LED_ex\inc\stm32l476xx.h"
#include "C:\Users\TimYeh\workspace\C_LED_ex\inc\helper_functions.h"
#include "C:\Users\TimYeh\workspace\C_LED_ex\inc\led_button.h"
#include "C:\Users\TimYeh\workspace\C_LED_ex\inc\7seg.h"
```
2. ### 相關設定
    stm32l476xx.h中的預設
    1. #### AHB2 peripheral clock enable register (RCC_AHB2ENR)
        決定GPIO通道的開啟
        ![](https://i.imgur.com/3TMGf1K.jpg)
        ```cpp=
        #include "stm32l476xx.h"
        
        RCC->AHB2ENR =0x07; //開啟 ABC
        
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;//只開A
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;//只開B
        ```
    2. #### GPIO port mode register (GPIOx_MODER) (x =A..H)
        腳位模式設定
        ![](https://i.imgur.com/u8QSUbR.jpg)
        ```cpp=
        #include "stm32l476xx.h"
        
        gpio->MODER &= ~(0b11 << (2*LED_pin));//歸零
        gpio->MODER |= (0b01 << (2*LED_pin));//把01(output mode)移到對應pin
        ```
    3. #### GPIO port output type register (GPIOx_OTYPER) (x = A..H)
        output type設定
        ![](https://i.imgur.com/3OCEy8d.jpg)

        ![](https://i.imgur.com/juX9nvP.jpg)
        ```cpp=
         #include "stm32l476xx.h"
         
        gpio->OTYPER =0x00;//全部設為push-pull
        ```
    4. #### GPIO port pull-up/pull-down register (GPIOx_PUPDR)(x = A..H)
        Pull-up:低電位時表示邏輯的 1，高電位表示邏輯的 0
        Pull-down:高電位表示邏輯 1，低電位表示邏輯 0
        ![](https://i.imgur.com/fjkfPWA.png)        ![](https://i.imgur.com/ss0xp5R.png)
        ![](https://i.imgur.com/x8hHOU9.jpg)
        ```cpp=
         #include "stm32l476xx.h"
         
        gpio->PUPDR &= ~(0b11 << (2*LED_pin));//歸零
        gpio->PUPDR |= (0b01 << (2*LED_pin));//把01(pull-up)移到對應pin
        ```
    5. #### GPIO初始化(通道選擇、模式...)
        
    ```cpp=
    int init_led(GPIO_TypeDef* gpio, int LED_pin){
	RCC->AHB2ENR =0x07;
	gpio->OTYPER =0x00;
    
    gpio->PUPDR &= ~(0b11 << (2*LED_pin));//歸零
    gpio->PUPDR |= (0b01 << (2*LED_pin));//把01(pull-up)移到對應pin
        
	gpio->MODER &= ~(0b11 << (2*LED_pin));//歸零
	gpio->MODER |= (0b01 << (2*LED_pin));//把01移到對應pin

	return 0;
    ```
    
    ```cpp=
    if(init_led(LED_gpioC, LED1_pin) != 0 ||
        init_led(LED_gpioB, LED2_pin) != 0 ||
        init_led(LED_gpioB, LED3_pin) != 0 ||
        init_led(LED_gpioB, LED4_pin) != 0||
        init_button(BUTTON_gpio, BUTTON_pin) != 0){
		// Fail to init LED ,BUTTON
        // 如果fonction(初始化)失敗，會回傳1
		return -1;//停止程式
	}
    ```
    6. #### 按鈕判斷
        
    ```cpp=
    int state=0;
	int button_press_cycle_per_second =10;
	int debounce_cycles = 100;
	int debounce_threshold = debounce_cycles*0.7;
	int last_button_state=0;
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
                //0 >> 1, 
                //1 >> 2, 
                //2 >> 0
		    if(state==2){
		        state=0;
		    }
		    else{
		        state+=1;
		    }
                //切換模式時，要將led_data、shift_direction初始化，不然會亂掉。
		    led_data[0] = 0b00001;
                led_data[1] = 0b000011;
                led_data[2] = 0b0000111;
		    shift_direction = 0;
	        }
		last_button_state =0;
	    }
    }//for
    }//while
    ```
    
    7. #### 4顆LED跑馬燈
    
    
    以state=1 (一次亮2顆LED) 為例子
    led_data= 0b000011
    |count|C0|C1|C2|C3|C4|C5|
    | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
    |0|0|0|0|0|1|1|

    ```cpp=
    int led_data[3] = {0b0001,0b000011,0b00000111};
    int led_data_B1[3] = {0b0001,0b000011,0b00000111};
    int led_data_B2[3] = {0b1000,0b110000,0b11100000};
    const gpio cs_[4] = {{GPIOC},{GPIOB},{GPIOB},{GPIOB}}//自建struct
    while(1){
	for(int a=0;a<4;a++){
        if((led_data[state]>>(a+state))&0x1){
        //a=0, 000011 -> 000001 -> 1
        //a=1. 000011 -> 000000 -> 0
        //a=2. 000011 -> 000000 -> 0
        //a=3. 000011 -> 000000 -> 0
        //after shift 1
        //a=0, 000110 -> 000011 -> 1
        //a=1. 000110 -> 000001 -> 1
        //a=2. 000110 -> 000000 -> 0
        //a=3. 000110 -> 000000 -> 0
        //...
        //...
        //after shift 4
        //a=0, 110000 -> 011000 -> 0
        //a=1. 110000 -> 001100 -> 0
        //a=2. 110000 -> 000110 -> 0
        //a=3. 110000 -> 000011 -> 1
        //after u_shift 1
        //a=0, 011000 -> 001100 -> 0
        //a=1. 011000 -> 000110 -> 0
        //a=2. 011000 -> 000011 -> 1
        //a=3. 011000 -> 000001 -> 1
        //after u_shift 2
        //a=0, 001100 -> 000110 -> 0
        //a=1. 001100 -> 000011 -> 1
        //a=2. 001100 -> 000001 -> 1
        //a=3. 001100 -> 000000 -> 0
	        reset_gpio(cs_[a].port, leds[a]);//1>>亮
	    }
	    else{
		set_gpio(cs_[a].port, leds[a]);//0>>暗
	    }
    
    if(shift_direction==0){
	    led_data[state] = (led_data[state]<<1);
	}
	else{
	    led_data[state] = (led_data[state]>>1);
	}

	// Check to see if change shift direction
	if(led_data[state]==led_data_B1[state]  ||
       led_data[state]==led_data_B2[state] ){
	    shift_direction = 1-shift_direction;
	}
    }//while
    ```
    8. #### GPIO  GPIO_TypeDef* array
        自建struct
```cpp=
typedef struct {
    GPIO_TypeDef* port;
}gpio;

const gpio cs_[] = {
 { GPIOA},
 { GPIOB},
 ....,
}

//...
//And call:
cs_[0].port->BSRR = //......;
GPIOA->BSRR = //......;same
