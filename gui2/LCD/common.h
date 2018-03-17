#ifndef __COMMON_H
#define __COMMON_H

#include "gpio_lib.h"

#define LCD_RD SUNXI_GPC(19)
#define LCD_WR SUNXI_GPC(20)
#define LCD_RS SUNXI_GPC(21)
#define LCD_CS SUNXI_GPC(22)
#define LCD_REST SUNXI_GPB(18)

#define DATA SUNXI_GPIO_G

typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned int u32;

static int DATA_IO[] = {SUNXI_GPG(0), SUNXI_GPG(1), SUNXI_GPG(2), SUNXI_GPG(3), SUNXI_GPG(4), SUNXI_GPG(5), SUNXI_GPG(6), SUNXI_GPG(7)};

//#define	LCD_LED PCout(10) //LCD背光    		 PC10 
//如果使用官方库函数定义下列底层，速度将会下降到14帧每秒，建议采用我司推荐方法
//以下IO定义直接操作寄存器，快速IO操作，刷屏速率可以达到28帧每秒！ 

#define	LCD_CS_SET  sunxi_gpio_output(LCD_CS, HIGH);    //片选端口  	PC9
#define	LCD_RS_SET	sunxi_gpio_output(LCD_RS, HIGH);    //数据/命令   	PC8	   
#define	LCD_WR_SET	sunxi_gpio_output(LCD_WR, HIGH);    //写数据		PC7
#define	LCD_RD_SET	sunxi_gpio_output(LCD_RD, HIGH);    //读数据		PC6
#define	LCD_RST_SET	sunxi_gpio_output(LCD_REST, HIGH);    //复位			PC5
								    
#define	LCD_CS_CLR  sunxi_gpio_output(LCD_CS, LOW);     //片选端口  	PC9
#define	LCD_RS_CLR	sunxi_gpio_output(LCD_RS, LOW);     //数据/命令  	PC8	   
#define	LCD_WR_CLR	sunxi_gpio_output(LCD_WR, LOW);     //写数据		PC7
#define	LCD_RD_CLR	sunxi_gpio_output(LCD_RD, LOW);     //读数据		PC6
#define	LCD_RST_CLR	sunxi_gpio_output(LCD_REST, LOW);    //复位			PC5				

#define DATAOUT(x) sunxi_gpio_output_bank(DATA, 0x0 | x);

extern int digitalWrite(unsigned int pin, unsigned int val);
extern int digitalWriteBank(unsigned int val);
extern void delay(int d);

#endif