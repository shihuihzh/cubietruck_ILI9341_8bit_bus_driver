import SUNXI_GPIO as GPIO
import time

RED_LEDS = (GPIO.PG0, GPIO.PG1, GPIO.PG2, GPIO.PG3, GPIO.PG4, GPIO.PG5, GPIO.PG6, GPIO.PG7, GPIO.PC19, GPIO.PC20, GPIO.PC21, GPIO.PC22, GPIO.PB18, GPIO.PB19)


GPIO.init()

for led in RED_LEDS:
    GPIO.setcfg(led, GPIO.OUT)
    GPIO.output(led, GPIO.HIGH)
    time.sleep(0.3) 
    GPIO.output(led, GPIO.LOW)
