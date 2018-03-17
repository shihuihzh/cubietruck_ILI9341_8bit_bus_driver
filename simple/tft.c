#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gpio_lib.h"

#define LCD_RD SUNXI_GPC(19)
#define LCD_WR SUNXI_GPC(20)
#define LCD_RS SUNXI_GPC(21)
#define LCD_CS SUNXI_GPC(22)
#define LCD_REST SUNXI_GPB(18)

#define DATA SUNXI_GPIO_G
int DATA_IO[] = {SUNXI_GPG(0), SUNXI_GPG(1), SUNXI_GPG(2), SUNXI_GPG(3), SUNXI_GPG(4), SUNXI_GPG(5), SUNXI_GPG(6), SUNXI_GPG(7)};

int sunxi_gpio_output2(unsigned int pin, unsigned int val) {

    unsigned int bank = GPIO_BANK(pin);
    unsigned int num = GPIO_NUM(pin);

    if(SUNXI_PIO_BASE == 0)
    {
        return -1;
    }
    struct sunxi_gpio *pio =&((struct sunxi_gpio_reg *)SUNXI_PIO_BASE)->gpio_bank[bank];

    if(val)
        *(&pio->dat) |= 1 << num;
    else
        *(&pio->dat) &= ~(1 << num);

    return 0;
}

int sunxi_gpio_output_bank2(unsigned int bank, unsigned int val) {

    if(SUNXI_PIO_BASE == 0)
    {
        return -1;
    }
    struct sunxi_gpio *pio =&((struct sunxi_gpio_reg *)SUNXI_PIO_BASE)->gpio_bank[bank];

    //*(&pio->dat) &= 0;
    //*(&pio->dat) |= val;
    //memcpy (&pio->dat, &val, sizeof(val));
    *(&pio->dat) = val;
    return 0;
}

// write pin data
int digitalWrite(unsigned int pin, unsigned int val)
{
    return sunxi_gpio_output2(pin, val);
}

// delay
void delay(int d)
{
    usleep(d * 1000);
}

void Lcd_Writ_Bus(unsigned char d)
{
    sunxi_gpio_output_bank2(DATA, 0x0 | d);
}

void Lcd_Write_Com(unsigned char VH)
{
    digitalWrite(LCD_RS, LOW);
    digitalWrite(LCD_CS, LOW);
    Lcd_Writ_Bus(VH);
    digitalWrite(LCD_WR, LOW);
    digitalWrite(LCD_WR, HIGH);
    digitalWrite(LCD_CS, HIGH);
}

void Lcd_Write_Data(unsigned char VH)
{
    digitalWrite(LCD_RS, HIGH);
    digitalWrite(LCD_CS, LOW);
    Lcd_Writ_Bus(VH);
    digitalWrite(LCD_WR, LOW);
    digitalWrite(LCD_WR, HIGH);
    digitalWrite(LCD_CS, HIGH);
}

void Lcd_Write_Com_Data(unsigned char com, unsigned char dat)
{
    Lcd_Write_Com(com);
    Lcd_Write_Data(dat);
}

void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    Lcd_Write_Com(0x2a);
    Lcd_Write_Data(x1 >> 8);
    Lcd_Write_Data(x1);
    Lcd_Write_Data(x2 >> 8);
    Lcd_Write_Data(x2);
    Lcd_Write_Com(0x2b);
    Lcd_Write_Data(y1 >> 8);
    Lcd_Write_Data(y1);
    Lcd_Write_Data(y2 >> 8);
    Lcd_Write_Data(y2);
    Lcd_Write_Com(0x2c);
}

void LCD_Clear(unsigned int j)
{
    unsigned int i, m;
    Address_set(0, 0, 240, 320);
    //Lcd_Write_Com(0x02c); //write_memory_start
    //digitalWrite(LCD_RS,HIGH);
    digitalWrite(LCD_CS, LOW);

    for (i = 0; i < 240; i++)
        for (m = 0; m < 320; m++)
        {
            Lcd_Write_Data(j >> 8);
            Lcd_Write_Data(j);
        }
    digitalWrite(LCD_CS, HIGH);
}

int setup()
{
    // init gpio
    if (SETUP_OK != sunxi_gpio_init())
    {
        printf("Failed to initialize GPIO\n");
        return -1;
    }

    sunxi_gpio_set_cfgpin(LCD_RD, OUTPUT);
    sunxi_gpio_set_cfgpin(LCD_WR, OUTPUT);
    sunxi_gpio_set_cfgpin(LCD_RS, OUTPUT);
    sunxi_gpio_set_cfgpin(LCD_CS, OUTPUT);
    sunxi_gpio_set_cfgpin(LCD_REST, OUTPUT);

    digitalWrite(LCD_RD, HIGH);
    digitalWrite(LCD_WR, HIGH);
    digitalWrite(LCD_RS, HIGH);
    digitalWrite(LCD_CS, HIGH);
    digitalWrite(LCD_REST, HIGH);

    for (int i = 0; i < 8; i++)
    {
        sunxi_gpio_set_cfgpin(DATA_IO[i], OUTPUT);
        digitalWrite(DATA_IO[i], LOW);
    }

    // init lcd param
    digitalWrite(LCD_REST, HIGH);
    delay(5);
    digitalWrite(LCD_REST, LOW);
    delay(15);
    digitalWrite(LCD_REST, HIGH);
    delay(15);

    digitalWrite(LCD_CS, HIGH);
    digitalWrite(LCD_WR, HIGH);
    digitalWrite(LCD_CS, LOW); //CS

    Lcd_Write_Com(0xCB);
    Lcd_Write_Data(0x39);
    Lcd_Write_Data(0x2C);
    Lcd_Write_Data(0x00);
    Lcd_Write_Data(0x34);
    Lcd_Write_Data(0x02);

    Lcd_Write_Com(0xCF);
    Lcd_Write_Data(0x00);
    Lcd_Write_Data(0XC1);
    Lcd_Write_Data(0X30);

    Lcd_Write_Com(0xE8);
    Lcd_Write_Data(0x85);
    Lcd_Write_Data(0x00);
    Lcd_Write_Data(0x78);

    Lcd_Write_Com(0xEA);
    Lcd_Write_Data(0x00);
    Lcd_Write_Data(0x00);

    Lcd_Write_Com(0xED);
    Lcd_Write_Data(0x64);
    Lcd_Write_Data(0x03);
    Lcd_Write_Data(0X12);
    Lcd_Write_Data(0X81);

    Lcd_Write_Com(0xF7);
    Lcd_Write_Data(0x20);

    Lcd_Write_Com(0xC0);  //Power control
    Lcd_Write_Data(0x23); //VRH[5:0]

    Lcd_Write_Com(0xC1);  //Power control
    Lcd_Write_Data(0x10); //SAP[2:0];BT[3:0]

    Lcd_Write_Com(0xC5);  //VCM control
    Lcd_Write_Data(0x3e); //Contrast
    Lcd_Write_Data(0x28);

    Lcd_Write_Com(0xC7);  //VCM control2
    Lcd_Write_Data(0x86); //--

    Lcd_Write_Com(0x36); // Memory Access Control
    Lcd_Write_Data(0x48);

    Lcd_Write_Com(0x3A);
    Lcd_Write_Data(0x55);

    Lcd_Write_Com(0xB1);
    Lcd_Write_Data(0x00);
    Lcd_Write_Data(0x18);

    Lcd_Write_Com(0xB6); // Display Function Control
    Lcd_Write_Data(0x08);
    Lcd_Write_Data(0x82);
    Lcd_Write_Data(0x27);

    Lcd_Write_Com(0x11); //Exit Sleep
    delay(120);

    Lcd_Write_Com(0x29); //Display on
    Lcd_Write_Com(0x2c);

    return 0;
}

void loop()
{
    LCD_Clear(0xf800);
    printf("r\n");
    //delay(1000);
    LCD_Clear(0x07E0);
    printf("g\n");
    //delay(1000);
    LCD_Clear(0x001F);
    printf("b\n");
    //delay(1000);
}

int main()
{

    int ret = setup();
    if (!ret)
    {
        while (1)
        {
            loop();
        }
    }

    return ret;
}
