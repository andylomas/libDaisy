// myShiftedLCD.cpp
#include "myUtils/myShiftedLCD.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

void bitWrite(uint8_t &bitString, uint8_t n, bool v)
{
    if (v)
    {
        bitString |= (uint8_t)1 << n;
    }
    else
    {
        bitString &= ~((uint8_t)1 << n);
    }
    
}

LiquidCrystal::LiquidCrystal()
{
}

LiquidCrystal::LiquidCrystal(
    const dsy_gpio_pin dataPin,
    const dsy_gpio_pin clockPin,
    const dsy_gpio_pin ssPin)
{
    init(dataPin, clockPin, ssPin);
}

void LiquidCrystal::init(
    const dsy_gpio_pin dataPin,
    const dsy_gpio_pin clockPin,
    const dsy_gpio_pin ssPin)
{
    // initialize SPI:
    _dataPin.pin  = dataPin;
    _dataPin.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&_dataPin);

    _clockPin.pin  = clockPin;
    _clockPin.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&_clockPin);

    _latchPin.pin  = ssPin;
    _latchPin.mode = DSY_GPIO_MODE_OUTPUT_PP;
    dsy_gpio_init(&_latchPin);
}

void LiquidCrystal::begin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
    if(lines > 1)
    {
        _displayfunction |= LCD_2LINE;
    }
    _numlines = lines;
    _currline = 0;

    // for some 1 line displays you can select a 10 pixel high font
    if((dotsize != 0) && (lines == 1))
    {
        _displayfunction |= LCD_5x10DOTS;
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
    dsy_tim_delay_us(50000);
    
    //put the LCD into 4 bit or 8 bit mode
    if(!(_displayfunction & LCD_8BITMODE))
    {
        // this is according to the hitachi HD44780 datasheet
        // figure 24, pg 46

        // we start in 8bit mode, try to set 4 bit mode
        write4bits(0x03);
        dsy_tim_delay_us(4500); // wait min 4.1ms

        // second try
        write4bits(0x03);
        dsy_tim_delay_us(4500); // wait min 4.1ms

        // third go!
        write4bits(0x03);
        dsy_tim_delay_us(150);

        // finally, set to 4-bit interface
        write4bits(0x02);
    }
    else
    {
        // this is according to the hitachi HD44780 datasheet
        // page 45 figure 23

        // Send function set command sequence
        command(LCD_FUNCTIONSET | _displayfunction);
        dsy_tim_delay_us(4500); // wait more than 4.1ms

        // second try
        command(LCD_FUNCTIONSET | _displayfunction);
        dsy_tim_delay_us(150);

        // third go
        command(LCD_FUNCTIONSET | _displayfunction);
    }

    // finally, set # lines, font size, etc.
    command(LCD_FUNCTIONSET | _displayfunction);

    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    // clear it off
    clear();

    // Initialize to default text direction (for romance languages)
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    command(LCD_ENTRYMODESET | _displaymode);
}

/********** high level commands, for the user! */
void LiquidCrystal::clear()
{
    command(LCD_CLEARDISPLAY); // clear display, set cursor position to zero
    dsy_tim_delay_us(2000);   // this command takes a long time!
}

void LiquidCrystal::home()
{
    command(LCD_RETURNHOME); // set cursor position to zero
    dsy_tim_delay_us(2000); // this command takes a long time!
}

void LiquidCrystal::setCursor(uint8_t col, uint8_t row)
{
    int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if(row > _numlines)
    {
        row = _numlines - 1; // we count rows starting w/0
    }

    command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystal::noDisplay()
{
    _displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal::display()
{
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystal::noCursor()
{
    _displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal::cursor()
{
    _displaycontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystal::noBlink()
{
    _displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal::blink()
{
    _displaycontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal::scrollDisplayLeft(void)
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal::scrollDisplayRight(void)
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystal::leftToRight(void)
{
    _displaymode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystal::rightToLeft(void)
{
    _displaymode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystal::autoscroll(void)
{
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystal::noAutoscroll(void)
{
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal::createChar(uint8_t location, uint8_t charmap[])
{
    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    for(int i = 0; i < 8; i++)
    {
        write(charmap[i]);
    }
}

void LiquidCrystal::print(char *str)
{
    for (int i = 0; str[i] != 0; i++)
    {
        write(str[i]);
    }
}

/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystal::command(uint8_t value)
{
    send(value, LOW);
}

inline size_t LiquidCrystal::write(uint8_t value)
{
    send(value, HIGH);
    return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void LiquidCrystal::send(uint8_t value, uint8_t mode)
{
    bitWrite(_bitString, 1, mode); //set RS to mode
    spiSendOut();
    //we are not using RW with SPI so we are not even bothering
    //or 8BITMODE so we go straight to write4bits
    write4bits(value >> 4);
    write4bits(value);
}

void LiquidCrystal::pulseEnable(void)
{
    bitWrite(_bitString, 3, LOW);
    spiSendOut();
 
    dsy_tim_delay_us(10);
    bitWrite(_bitString, 3, HIGH);
    spiSendOut();
    //dsy_tim_delay_us(1);    // enable pulse must be >450ns
    dsy_tim_delay_us(10); // enable pulse must be >450ns
    bitWrite(_bitString, 3, LOW);
    spiSendOut();
    //dsy_tim_delay_us(40);   // commands need > 37us to settle
    dsy_tim_delay_us(100); // commands need > 37us to settle
    //dsy_tim_delay_us(32);   // appears to work with the Daisy with 32 microseconds
}

void LiquidCrystal::write4bits(uint8_t value)
{
    for(int i = 4; i < 8; i++)
    {
        //we put the four bits in the _bit_string
        bitWrite(_bitString, i, ((value >> (i - 4)) & 0x01));
    }
    //and send it out
    spiSendOut();
    pulseEnable();
}

void LiquidCrystal::spiSendOut() //SPI #############################
{
    dsy_gpio_write(&_latchPin, LOW);
    shiftOut(&_dataPin, &_clockPin, MSBFIRST, _bitString);
    dsy_gpio_write(&_latchPin, HIGH);
}
