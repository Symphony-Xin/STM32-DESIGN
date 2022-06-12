#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "lsens.h"

#define LumLimit 50	//���ڴ�ʱ���Զ����⣨ռλֵ�����֤��
#define LumLimitPlus 25	//���ڴ�ʱ����ǿ���⣨ռλֵ�����֤��
/************************************************
 ALIENTEK ս��STM32F103������ʵ��19
 ���������� ʵ��   
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


 
 int main(void)
 {	 
 	u8 adcx; 
	u8 key;							//������Ϣ
	 int AutoOrManual=0;					//0:�Զ�ģʽ 1���ֶ�ģʽ��Ĭ�����Զ�ģʽ
	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	 
	 //BEEP_Init();			//��������������ʼ��
	KEY_Init();					//��ʼ���밴�����ӵ�Ӳ���ӿ�
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	 
  	LCD_Init();				//��ʼ��LCD
	Lsens_Init(); 			//��ʼ������������
	POINT_COLOR=RED;//��������Ϊ��ɫ  	
	//��ʾ��ʾ��Ϣ											      
	LCD_ShowString(30,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(30,70,200,16,16,"LSENS TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/1/14");	  
	POINT_COLOR=BLUE;//��������Ϊ��ɫ
	LCD_ShowString(30,130,200,16,16,"LSENS_VAL:");	
	
	while(1)		
	{
		key=KEY_Scan(0);		//��ȡ��������֧������ģʽ
		adcx=Lsens_Get_Val();
		LCD_ShowxNum(30+10*8,130,adcx,3,16,0);//��ʾADC��ֵ 
		
		
		if(key == 3){}		//�رձ���������䣬�����䣨������û������ʪ�ȣ�
			
		//�Զ����տ������
		//�ް������£��Ҳ������ֶ�ģʽ
		if(key == 0 && AutoOrManual == 0){
			
			delay_ms(1000);		//һ��ʱ����ʱ����ֹƵ������
				
			if(adcx < LumLimit && adcx > LumLimitPlus)		//һ������
			{
				LCD_ShowString(30,150,480,16,16,"LIGHT:WEAK LIGHT,LIGHT SUPPLY ON.");
				LED0=1;
				LED1=0;			//��������һ��ֵ����һ��
			}else if(adcx < 25){		//��������
				LCD_ShowString(30,150,480,16,16,"LIGHT:EXTREMELY WEAK LIGHT,EXTRA LIGHT SUPPLY ON.");
				LED0=1;
				LED1=1;			//��������һ��ֵ��������
			}
			else if(LED0 > LumLimit){		//�����մ����ض�ֵ����ص�
				LCD_ShowString(30,150,480,16,16,"LIGHT:ENOUGH LIGHT,plants are in good light condition.");
				LED0=0;LED1=0;
			}
		}
		
		//�ֶ����տ������
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
 
