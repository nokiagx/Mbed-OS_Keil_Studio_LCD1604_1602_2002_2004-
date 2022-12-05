/*
www.jinkelectric.top
 2022/12/05
1: textLCD.h
   TextLCD(PinName rs, PinName e, PinName d4, PinName d5, PinName d6, PinName d7, LCDType type = LCD16x4); 
  //LCD16x4此处更改为你所用的LCD类型，LCD16x4 here changes to the LCD type you use
2:其他请参考main.cpp文件  For others, please refer to the main.cpp file
 
 */


#include "mbed.h"
#include "TextLCD.h"


int main()
{

    TextLCD lcd(PA_9, PB_6, PB_5, PB_4,PB_10, PA_8); //LCD Shield for Arduino  rs, e, d4-d7 ，rw连接GND，Rw connection GND
    wait_us(10000);
    while (true) {
        lcd.locate(0,0);
        lcd.printf("AB:123456");
        wait_us(10);
        lcd.locate(1,1);
        lcd.printf("CD:123456");  // turns value 
        wait_us(10);
        lcd.locate(0,2);
        lcd.printf("EF:123456");  // turns value 
        wait_us(10);
        lcd.locate(1,3);
        lcd.printf("HI:123456");  // turns value 
    }
}
