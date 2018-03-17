import SUNXI_GPIO as GPIO
import time

RED_LEDS = (GPIO.PG0, GPIO.PG1, GPIO.PG2, GPIO.PG3, GPIO.PG4, GPIO.PG5, GPIO.PG6, GPIO.PG7, GPIO.PC19, GPIO.PC20, GPIO.PC21, GPIO.PC22, GPIO.PB18, GPIO.PB19)


GPIO.init()

GPIO.setcfg(RED_LEDS[0], GPIO.OUT)
old = time.time()
state = GPIO.LOW
for i in range(0, 100000):
    GPIO.output(RED_LEDS[0], state)
    time.sleep(0.1)
    if state == GPIO.LOW:
        state = GPIO.HIGH
    else:
        state = GPIO.LOW

print("time: %f" % (time.time() - old))
