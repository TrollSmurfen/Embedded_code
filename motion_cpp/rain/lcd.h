#pragma once
#include "hardware/i2c.h"

// LCD command and flag definitions
const int LCD_CLEARDISPLAY = 0x01;
const int LCD_RETURNHOME = 0x02;
const int LCD_ENTRYMODESET = 0x04;
const int LCD_DISPLAYCONTROL = 0x08;
const int LCD_FUNCTIONSET = 0x20;
const int LCD_SETDDRAMADDR = 0x80;

const int LCD_ENTRYLEFT = 0x02;
const int LCD_DISPLAYON = 0x04;
const int LCD_2LINE = 0x08;

const int LCD_BACKLIGHT = 0x08;
const int LCD_ENABLE_BIT = 0x04;

#define LCD_CHARACTER  1
#define LCD_COMMAND 0

static int addr = 0x27;

void i2c_write_byte(uint8_t value);
void lcd_toggle_enable(uint8_t value);
void lcd_send_byte(uint8_t value, int mode);
void lcd_send_command(uint8_t value);
void lcd_char(char value);
void lcd_print(const char *str);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_clear(void);
void lcd_init(void);
