/*
www.jinkelectric.top
 2022/12/05
1: textLCD.h
   TextLCD(PinName rs, PinName e, PinName d4, PinName d5, PinName d6, PinName d7, LCDType type = LCD16x4); 
  //LCD16x4此处更改为你所用的LCD类型，LCD16x4 here changes to the LCD type you use
2:其他请参考main.cpp文件  For others, please refer to the main.cpp file
 
 */
#include "TextLCD.h"
#include "mbed.h"

/**LCD引脚配置 如此这般 TextLCD lcd(PA_9, PB_6, PB_5, PB_4,PB_10, PA_8); , e, d4-d7 ，rw连接GND，Rw connection GND**/

TextLCD::TextLCD(PinName rs, PinName e, PinName d4, PinName d5,
                 PinName d6, PinName d7, LCDType type) : _rs(rs),
        _e(e), _d(d4, d5, d6, d7),
        _type(type) {

    _e  = 1;
    _rs = 0;            // command mode

    wait_us(1500);        // Wait 15ms to ensure powered up

    // send "Display Settings" 3 times (Only top nibble of 0x30 as we've got 4-bit bus)
    for (int i=0; i<3; i++) {
        writeByte(0x3);
        wait_us(1640);  // this command takes 1.64ms, so wait for it
    }
    writeByte(0x2);     // 4-bit mode
    wait_us(40);    // most instructions take 40us

    writeCommand(0x28); // Function set 001 BW N F - -
    writeCommand(0x0C);
    writeCommand(0x6);  // Cursor Direction and Display Shift : 0000 01 CD S (CD 0-left, 1-right S(hift) 0-no, 1-yes
    cls();
}

void TextLCD::character(int column, int row, int c) {
    int a = address(column, row);
    writeCommand(a);
    writeData(c);
}

void TextLCD::cls() {
    writeCommand(0x01); // cls, and set cursor to 0
   wait_us(1640);   // This command takes 1.64 ms
    locate(0, 0);
}

void TextLCD::locate(int column, int row) {
    _column = column;
    _row = row;
}

int TextLCD::_putc(int value) {
    if (value == '\n') {
        _column = 0;
        _row++;
        if (_row >= rows()) {
            _row = 0;
        }
    } else {
        character(_column, _row, value);
        _column++;
        if (_column >= columns()) {
            _column = 0;
            _row++;
            if (_row >= rows()) {
                _row = 0;
            }
        }
    }
    return value;
}

int TextLCD::_getc() {
    return -1;
}

void TextLCD::writeByte(int value) {
    _d = value >> 4;
    wait_us(40); // most instructions take 40us
    _e = 0;
    wait_us(40);
    _e = 1;
    _d = value >> 0;
    wait_us(40);
    _e = 0;
    wait_us(40);  // most instructions take 40us
    _e = 1;
}

void TextLCD::writeCommand(int command) {
    _rs = 0;
    writeByte(command);
}

void TextLCD::writeData(int data) {
    _rs = 1;
    writeByte(data);
}

int TextLCD::address(int column, int row) {
    switch (_type) {
        case LCD20x4:
            switch (row) {
                case 0:
                    return 0x80 + column; //0x80,0x00
                case 1:
                    return 0xc0 + column; //0xc0,0x40
                case 2:
                    return 0x90 + column; //0x94,0x10
                case 3:
                    return 0xd0 + column; //0xd4,0x50
            }
        case LCD16x4: //增加1604
            switch (row) {
                case 0:
 
                   return 0x80 + 0x00 + column;
                case 1:
 
                    return 0x80 + 0x40 + column;
                case 2:

                    return 0x80 + 0x10 + column;
                case 3:

                    return 0x80 + 0x50 + column;
                    }
        case LCD16x2B:
            return 0x80 + (row * 40) + column;
        case LCD16x2:
        case LCD20x2:
        default:
            return 0x80 + (row * 0x40) + column;
    }
}

int TextLCD::columns() {
    switch (_type) {
        case LCD20x4:
        case LCD20x2:
            return 20;
        case LCD16x2:
        case LCD16x4:  //增加1604
        case LCD16x2B:
        default:
            return 16;
    }
}

int TextLCD::rows() {
    switch (_type) {
        case LCD20x4:
        case LCD16x4:
            return 4;
        case LCD16x2:
        case LCD16x2B:
        case LCD20x2:
        default:
            return 2;
    }
}
