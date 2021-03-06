#pragma once
#ifndef DSY_OLED_DISPLAY_H
#define DSY_OLED_DISPLAY_H /**< Macro */
#include <cmath>
#include <stdlib.h>
#include <stdint.h>
#include "util/oled_fonts.h"
#include "daisy_core.h"

#ifndef SSD1309_HEIGHT
#define SSD1309_HEIGHT 64 /**< SSD1309 height in pixels */
#endif

#ifndef SSD1309_WIDTH
#define SSD1309_WIDTH 128 /**< SSD1309 width in pixels */
#endif

namespace daisy
{
/** @addtogroup feedback
    @{ 
*/

/** 
    Human Interface Driver for using an OLED Display (SSD1309)
    For all `bool on` arguments: true is on, false is off.
    Credit to Aleksander Alekseev (github.com/afiskon/stm32-ssd1306) on github for a great starting point.
    adapted for SSD1309 and H7 by shensley, 2020
*/

class OledDisplay
{
  public:
    /** GPIO Pins that need to be used independent of peripheral used. */
    enum Pins
    {
        DATA_COMMAND, /**< Data command pin. */
        RESET,        /**< Reset pin */
        NUM_PINS,     /**< Num pins */
    };
    OledDisplay() {}
    ~OledDisplay() {}

    // TODO:
    /* - add I2C Support.
    - add configuration for specific spi/i2c peripherals (currently only uses SPI1, w/ hardware controlled chip select.
    - re-add support for SSD1306 displays
    */

    /** 
    Takes an argument for the pin cfg
    \param pin_cfg should be a pointer to an array of OledDisplay::NUM_PINS dsy_gpio_pins
    */
    void Init(dsy_gpio_pin* pin_cfg, bool nss_hard_output = true);

    /** 
    Fills the entire display with either on/off.
    \param on Sets on or off.
    */
    void Fill(bool on);

    /**
    Sets the pixel at the specified coordinate to be on/off.
    \param x   x Coordinate
    \param y   y coordinate
    \param on  on or off
    */
    void DrawPixel(uint_fast8_t x, uint_fast8_t y, bool on);

    /**
    Draws a line from (x1, y1) to (y1, y2)
    \param x1  x Coordinate of the starting point
    \param y1  y Coordinate of the starting point
    \param x2  x Coordinate of the ending point
    \param y2  y Coordinate of the ending point
    \param on  on or off
    */
    void DrawLine(uint_fast8_t x1,
                  uint_fast8_t y1,
                  uint_fast8_t x2,
                  uint_fast8_t y2,
                  bool         on);

    /**
    Draws a rectangle based on two coordinates.
    \param x1 x Coordinate of the first point
    \param y1 y Coordinate of the first point
    \param x2 x Coordinate of the second point
    \param y2 y Coordinate of the second point
    \param on on or off
    */
    void DrawRect(uint_fast8_t x1,
                  uint_fast8_t y1,
                  uint_fast8_t x2,
                  uint_fast8_t y2,
                  bool         on,
                  bool         fill = false);

    /**
    Draws an arc around the specified coordinate
    \param x           x Coordinate of the center of the arc
    \param y           y Coordinate of the center of the arc
    \param radius      radius of the arc
    \param start_angle angle where to start the arc
    \param sweep       total angle of the arc
    \param on  on or off
    */
    void DrawArc(uint_fast8_t x,
                 uint_fast8_t y,
                 uint_fast8_t radius,
                 int_fast16_t start_angle,
                 int_fast16_t sweep,
                 bool         on);

    /**
    Draws a circle around the specified coordinate
    \param x           x Coordinate of the center of the circle
    \param y           y Coordinate of the center of the circle
    \param radius      radius of the circle
    \param on  on or off
    */
    void
    DrawCircle(uint_fast8_t x, uint_fast8_t y, uint_fast8_t radius, bool on)
    {
        DrawArc(x, y, radius, 0, 360, on);
    };

    /** 
    Writes the character with the specific FontDef
    to the display buffer at the current Cursor position.
    \param ch character to be written
    \param font font to be written in
    \param on    on or off
    \return &
    */
    char WriteChar(char ch, FontDef font, bool on);

    /** 
    Similar to WriteChar, except it will handle an entire String.
    Wrapping does not happen automatically, so the width
    of the string must be kept within the dimensions of the screen.
    \param str string to be written
    \param font font to use
    \param on  on or off
    \return &
    */
    char WriteString(const char* str, FontDef font, bool on);

    /** 
    Similar to WriteChar, except it will handle an entire buffer.
    Wrapping does not happen automatically, so the width
    of the string must be kept within the dimensions of the screen.
    \param str string to be written
    \param font font to use
    \param on  on or off
    \return &
    */
    char WriteBuffer(const char* buf, size_t bytes, FontDef font, bool on);

    /** 
    Moves the 'Cursor' position used for WriteChar, and WriteStr to the specified coordinate.
    \param x x pos
    \param y y pos
    */
    void SetCursor(uint8_t x, uint8_t y);

    /** 
    Writes the current display buffer to the OLED device using SPI or I2C depending on 
    how the object was initialized.
    */
    void Update(uint8_t sub_buffers=255);

    /** 
    Calculates checksums for sub-buffers, and updates those that have changed.
    */
    void DirtyUpdate();

    /** 
    Compares current with previous buffers, find first and last bytes that have
    changed in each page.
    */
    void SmartUpdate(uint8_t sub_buffers=255);
    void InitSmartUpdate();
    
    void Reset();

  private:
    void SendCommand(uint8_t byte);
    void SendData(uint8_t* buff, size_t size);
    uint8_t CalcBufferChecksums();
    
    bool using_smart_update;
};
/** @} */
} // namespace daisy

#endif
