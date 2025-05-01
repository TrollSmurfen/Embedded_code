#include "lcd.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

void i2c_write_byte(uint8_t value) {
    i2c_write_blocking(i2c0, addr, &value, 1, false);
}

void lcd_toggle_enable(uint8_t value) {
    #define DELAY_US 600
    sleep_us(DELAY_US);
    i2c_write_byte(value | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(value & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

void lcd_send_byte(uint8_t value, int mode) {
    uint8_t high = mode | (value & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((value << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void lcd_send_command(uint8_t value) {
    lcd_send_byte(value, LCD_COMMAND);
}

void lcd_char(char value) {
    lcd_send_byte(value, LCD_CHARACTER);
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_char(*str++);
    }
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? 0x00 + col : 0x40 + col;
    lcd_send_command(0x80 | address);
}

void lcd_clear(void) {
    lcd_send_command(LCD_CLEARDISPLAY);
    sleep_ms(2);
}

void lcd_init(void) {
    sleep_ms(50);
    lcd_send_command(0x03);
    sleep_ms(5);
    lcd_send_command(0x03);
    sleep_us(150);
    lcd_send_command(0x03);
    sleep_ms(1);
    lcd_send_command(0x02);

    lcd_send_command(LCD_FUNCTIONSET | LCD_2LINE);
    lcd_send_command(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
    lcd_send_command(LCD_ENTRYMODESET | LCD_ENTRYLEFT);
    lcd_clear();
}
