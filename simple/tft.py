import SUNXI_GPIO as GPIO
import time

DATA_IO = (GPIO.PG0, GPIO.PG1, GPIO.PG2, GPIO.PG3, GPIO.PG4, GPIO.PG5, GPIO.PG6, GPIO.PG7)
DATA = GPIO.PG
LCD_RD = GPIO.PC19
LCD_WR =  GPIO.PC20     
LCD_RS =  GPIO.PC21        
LCD_CS =  GPIO.PC22       
LCD_REST = GPIO.PB18

def initLcd():
    # setup
    for d in DATA_IO:
        GPIO.setcfg(d, GPIO.OUT)
        GPIO.output(d, GPIO.LOW)

    GPIO.setcfg(LCD_RD, GPIO.OUT)
    GPIO.setcfg(LCD_WR, GPIO.OUT)
    GPIO.setcfg(LCD_RS, GPIO.OUT)
    GPIO.setcfg(LCD_CS, GPIO.OUT)
    GPIO.setcfg(LCD_REST, GPIO.OUT)
    
    GPIO.output(LCD_RD, GPIO.HIGH)
    GPIO.output(LCD_WR, GPIO.HIGH)
    GPIO.output(LCD_RS, GPIO.HIGH)
    GPIO.output(LCD_CS, GPIO.HIGH)
    GPIO.output(LCD_REST, GPIO.HIGH)

    # init
    GPIO.output(LCD_REST, GPIO.HIGH);
    time.sleep(0.005); 
    GPIO.output(LCD_REST, GPIO.LOW)
    time.sleep(0.015)
    GPIO.output(LCD_REST, GPIO.HIGH)
    time.sleep(0.015)

    GPIO.output(LCD_CS, GPIO.HIGH);
    GPIO.output(LCD_WR, GPIO.HIGH);
    GPIO.output(LCD_CS, GPIO.LOW);  #CS
    
    Lcd_Write_Com(0xCB) 
    Lcd_Write_Data(0x39)
    Lcd_Write_Data(0x2C)
    Lcd_Write_Data(0x00)
    Lcd_Write_Data(0x34)
    Lcd_Write_Data(0x02)

    Lcd_Write_Com(0xCF) 
    Lcd_Write_Data(0x00)
    Lcd_Write_Data(0XC1)
    Lcd_Write_Data(0X30)

    Lcd_Write_Com(0xE8) 
    Lcd_Write_Data(0x85)
    Lcd_Write_Data(0x00)
    Lcd_Write_Data(0x78)

    Lcd_Write_Com(0xEA) 
    Lcd_Write_Data(0x00)
    Lcd_Write_Data(0x00)
 
    Lcd_Write_Com(0xED) 
    Lcd_Write_Data(0x64)
    Lcd_Write_Data(0x03)
    Lcd_Write_Data(0X12)
    Lcd_Write_Data(0X81)

    Lcd_Write_Com(0xF7) 
    Lcd_Write_Data(0x20)
  
    Lcd_Write_Com(0xC0)   #Power control 
    Lcd_Write_Data(0x23)  #VRH[5:0] 
 
    Lcd_Write_Com(0xC1)   #Power control 
    Lcd_Write_Data(0x10)  #SAP[2:0];BT[3:0] 

    Lcd_Write_Com(0xC5)   #VCM control 
    Lcd_Write_Data(0x3e)  #Contrast
    Lcd_Write_Data(0x28)
 
    Lcd_Write_Com(0xC7)   #VCM control2 
    Lcd_Write_Data(0x86)  #--
 
    Lcd_Write_Com(0x36)   # Memory Access Control 
    Lcd_Write_Data(0x48)  

    Lcd_Write_Com(0x3A)   
    Lcd_Write_Data(0x55)

    Lcd_Write_Com(0xB1)   
    Lcd_Write_Data(0x00) 
    Lcd_Write_Data(0x18)
 
    Lcd_Write_Com(0xB6)   # Display Function Control 
    Lcd_Write_Data(0x08)
    Lcd_Write_Data(0x82);
    Lcd_Write_Data(0x27) 

    Lcd_Write_Com(0x11)   # Exit Sleep 
    time.sleep(0.120)
				
    Lcd_Write_Com(0x29)   # Display on 
    Lcd_Write_Com(0x2c)


def setData(b):
    GPIO.outputBank(DATA, b)


def Lcd_Write_Com(d):
    GPIO.output(LCD_RS, GPIO.LOW)
    GPIO.output(LCD_CS, GPIO.LOW)
    setData(d)
    GPIO.output(LCD_WR, GPIO.LOW)
    GPIO.output(LCD_WR, GPIO.HIGH)
    GPIO.output(LCD_CS, GPIO.HIGH)

def Lcd_Write_Data(d):
    GPIO.output(LCD_RS, GPIO.HIGH)
    GPIO.output(LCD_CS, GPIO.LOW)
    setData(d)
    GPIO.output(LCD_WR, GPIO.LOW)
    GPIO.output(LCD_WR, GPIO.HIGH)
    GPIO.output(LCD_CS, GPIO.HIGH)

def Address_set(x1,y1, x2, y2):
    Lcd_Write_Com(0x2a)
    Lcd_Write_Data(x1>>8)
    Lcd_Write_Data(x1)
    Lcd_Write_Data(x2>>8)
    Lcd_Write_Data(x2)
    Lcd_Write_Com(0x2b)
    Lcd_Write_Data(y1>>8)
    Lcd_Write_Data(y1)
    Lcd_Write_Data(y2>>8)
    Lcd_Write_Data(y2)
    Lcd_Write_Com(0x2c)

def LCD_Clear(j):
    Address_set(0,0,240,320)
    GPIO.output(LCD_CS, GPIO.LOW)


    for i in range(0, 240):
        for m in range(0, 320):
            Lcd_Write_Data(j >> 8)
            Lcd_Write_Data(j)

    GPIO.output(LCD_CS, GPIO.HIGH)


GPIO.init()
initLcd()
LCD_Clear(0xf800);
LCD_Clear(0x07E0);
LCD_Clear(0x001F);
