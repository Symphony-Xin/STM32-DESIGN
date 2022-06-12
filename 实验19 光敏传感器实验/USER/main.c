#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "lsens.h"

#define LumLimit 50	//低于此时，自动补光（占位值，需查证）
#define LumLimitPlus 25	//低于此时，加强补光（占位值，需查证）
/************************************************
 ALIENTEK 战舰STM32F103开发板实验19
 光敏传感器 实验   
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


 
 int main(void)
 {	 
 	u8 adcx; 
	u8 key;							//按键信息
	 int AutoOrManual=0;					//0:自动模式 1：手动模式，默认在自动模式
	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	 
	 //BEEP_Init();			//保留，蜂鸣器初始化
	KEY_Init();					//初始化与按键连接的硬件接口
	LED_Init();		  		//初始化与LED连接的硬件接口
	 
  	LCD_Init();				//初始化LCD
	Lsens_Init(); 			//初始化光敏传感器
	POINT_COLOR=RED;//设置字体为红色  	
	//显示提示信息											      
	LCD_ShowString(30,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(30,70,200,16,16,"LSENS TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/1/14");	  
	POINT_COLOR=BLUE;//设置字体为蓝色
	LCD_ShowString(30,130,200,16,16,"LSENS_VAL:");	
	
	while(1)		
	{
		key=KEY_Scan(0);		//获取按键，不支持连按模式
		adcx=Lsens_Get_Val();
		LCD_ShowxNum(30+10*8,130,adcx,3,16,0);//显示ADC的值 
		
		
		if(key == 3){}		//关闭报警处理语句，待补充（反正还没法测温湿度）
			
		//自动光照控制语句
		//无按键按下，且不处于手动模式
		if(key == 0 && AutoOrManual == 0){
			
			delay_ms(1000);		//一定时间延时，防止频繁触发
				
			if(adcx < LumLimit && adcx > LumLimitPlus)		//一档补光
			{
				LCD_ShowString(30,150,480,16,16,"LIGHT:WEAK LIGHT,LIGHT SUPPLY ON.");
				LED0=1;
				LED1=0;			//光照少于一定值，开一灯
			}else if(adcx < 25){		//二档补光
				LCD_ShowString(30,150,480,16,16,"LIGHT:EXTREMELY WEAK LIGHT,EXTRA LIGHT SUPPLY ON.");
				LED0=1;
				LED1=1;			//光照少于一定值，开二灯
			}
			else if(LED0 > LumLimit){		//若光照大于特定值，则关灯
				LCD_ShowString(30,150,480,16,16,"LIGHT:ENOUGH LIGHT,plants are in good light condition.");
				LED0=0;LED1=0;
			}
		}
		
		//手动光照控制语句
		if(key == KEY0_PRES){
			AutoOrManual=1;
			LCD_ShowString(30,150,480,16,16,"LIGHT:MANUAL MODE.LIGHT SUPPLY ON.");
			LED0=1;LED1=0;
		}
		if(key == KEY1_PRES){
			AutoOrManual=1;
			LCD_ShowString(30,150,480,16,16,"LIGHT:MANUAL MODE.EXTRA LIGHT SUPPLY ON.");
			LED0=1;LED1=1;
		}
		if(key == KEY2_PRES){
			AutoOrManual=1;
			LCD_ShowString(30,150,480,16,16,"LIGHT:LIGHT:MANUAL MODE.LIGHT SUPPLY OFF");
			LED0=0;LED1=0;
		}
		
		delay_ms(200);
	}
}
 
