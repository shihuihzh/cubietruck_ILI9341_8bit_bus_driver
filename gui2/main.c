#include "LCD/lcd.h"
#include "gui.h"
#include "test.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103RBT6,正点原子MiniSTM32开发板,主频72MHZ
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtech.net
//淘宝网站：http://qdtech.taobao.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:QDtech2008@gmail.com 
//Skype:QDtech2008
//技术交流QQ群:324828016
//创建日期:2013/5/13
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2009-2019
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//5V接DC 5V电源
//GND接地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为8位并行模式
//8位模式接线：8位模式9341模块接高8位总线，即:
//液晶屏LCD_D0对应单片机PB8
//液晶屏LCD_D1对应单片机PB9
//液晶屏LCD_D2对应单片机PB10
//液晶屏LCD_D3对应单片机PB11
//液晶屏LCD_D4对应单片机PB12
//液晶屏LCD_D5对应单片机PB13
//液晶屏LCD_D6对应单片机PB14
//液晶屏LCD_D7对应单片机PB15
//=======================================液晶屏控制线接线==========================================//
//LCD_RST	接PC5	//复位信号
//LCD_CS	接PC9	//片选信号
//LCD_RS	接PC8	//寄存器/数据选择信号
//LCD_WR	接PC7	//写信号
//LCD_RD	接PC6	//读信号
//=========================================触摸屏触接线=========================================//
//本模块触摸测试需连接外部XPT2046触摸采样芯片，所以本程序不进行触摸测试。
//不使用触摸或者模块本身不带触摸，则可不连接
**************************************************************************************************/	

int main(void)
{		
	LCD_Init();	   //液晶屏初始化
	

	while(1)
	{			
		LCD_Init();	   //液晶屏初始化
		main_test(); 		//测试主界面
		Test_Color();  		//简单刷屏填充测试
		Test_FillRec();		//GUI矩形绘图测试
		Test_Circle(); 		//GUI画圆测试
		English_Font_test();//英文字体示例测试
		Chinese_Font_test();//中文字体示例测试
		Pic_test();			//图片显示示例测试
		//Touch_Test();		//触摸屏手写测试  
	}				  
	
 }

