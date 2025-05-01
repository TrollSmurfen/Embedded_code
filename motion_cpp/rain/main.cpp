#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "lcd.h"

// Define pins
#define SDA_PIN 4
#define SCL_PIN 5
#define TRIG_PIN 6
#define ECHO_PIN 7
#define BUZZER_PIN 8
#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

// Function to measure distance using the ultrasonic sensor
float measure_distance() {
    // Send a 10us pulse to the TRIG pin
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    // Measure the pulse width on the ECHO pin
    while (gpio_get(ECHO_PIN) == 0) {}; // Wait for HIGH
    absolute_time_t start_time = get_absolute_time();
    while (gpio_get(ECHO_PIN) == 1) {}; // Wait for LOW
    absolute_time_t end_time = get_absolute_time();

    // Calculate the time difference in microseconds
    int64_t pulse_duration = absolute_time_diff_us(start_time, end_time);

    // Convert to distance (in cm)
    float distance = (pulse_duration / 2.0) * 0.0343;
    return distance;
}

// Function to set RGB color
void set_rgb_color(bool red, bool green, bool blue) {
    gpio_put(RED_PIN, red);
    gpio_put(GREEN_PIN, green);
    gpio_put(BLUE_PIN, blue);
}

// Function to toggle the buzzer rapidly for sound variation
void play_buzzer(int delay_us, int duration_ms) {
    int cycles = (duration_ms * 1000) / (delay_us * 2);
    for (int i = 0; i < cycles; i++) {
        gpio_put(BUZZER_PIN, 1);
        sleep_us(delay_us);
        gpio_put(BUZZER_PIN, 0);
        sleep_us(delay_us);
    }
}

// Stop the buzzer completely
void stop_buzzer() {
    gpio_put(BUZZER_PIN, 0);
}

int main() {
    stdio_init_all();

    // Initialize I2C for the LCD
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Initialize GPIO for the ultrasonic sensor
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_put(TRIG_PIN, 0);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    // Initialize GPIO for RGB LED
    gpio_init(RED_PIN);
    gpio_set_dir(RED_PIN, GPIO_OUT);

    gpio_init(GREEN_PIN);
    gpio_set_dir(GREEN_PIN, GPIO_OUT);

    gpio_init(BLUE_PIN);
    gpio_set_dir(BLUE_PIN, GPIO_OUT);

    // Initialize GPIO for buzzer
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);

    // Initialize the LCD
    lcd_init();

    while (true) {
        // Measure distance
        float distance = measure_distance();

        // Display the distance on the LCD
        lcd_clear();
        lcd_set_cursor(0, 0);
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "Dist: %.2f cm", distance);
        lcd_print(buffer);

        // Set RGB color and buzzer behavior based on distance
        if (distance < 5.0) {
            // Red for very close objects
            set_rgb_color(false, true, true);
            play_buzzer(500, 500); // Fast toggling for a higher-pitched sound
        } else if (distance < 10.0) {
            // Blue for moderately close objects
            set_rgb_color(true, true, false);
            play_buzzer(1000, 500); // Slower toggling for a lower-pitched sound
        } else {
            // Green for objects far away
            set_rgb_color(true, false, true);
            stop_buzzer(); // Turn off the buzzer
        }

        sleep_ms(500);  // Update every 500ms
    }
}





/*#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "lcd.h"

// Define pins
#define SDA_PIN 4
#define SCL_PIN 5
#define TRIG_PIN 6
#define ECHO_PIN 7
#define BUZZER_PIN 8
#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

// Function to measure distance using the ultrasonic sensor
float measure_distance() {
    // Send a 10us pulse to the TRIG pin
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    // Measure the pulse width on the ECHO pin
    while (gpio_get(ECHO_PIN) == 0) {}; // Wait for HIGH
    absolute_time_t start_time = get_absolute_time();
    while (gpio_get(ECHO_PIN) == 1) {}; // Wait for LOW
    absolute_time_t end_time = get_absolute_time();

    // Calculate the time difference in microseconds
    int64_t pulse_duration = absolute_time_diff_us(start_time, end_time);

    // Convert to distance (in cm)
    float distance = (pulse_duration / 2.0) * 0.0343;
    return distance;
}

// Function to set RGB color
void set_rgb_color(bool red, bool green, bool blue) {
    gpio_put(RED_PIN, red);
    gpio_put(GREEN_PIN, green);
    gpio_put(BLUE_PIN, blue);
}

int main() {
    stdio_init_all();

    // Initialize I2C for the LCD
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Initialize GPIO for the ultrasonic sensor
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_put(TRIG_PIN, 0);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    // Initialize GPIO for the buzzer
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);

    // Initialize GPIO for RGB LED
    gpio_init(RED_PIN);
    gpio_set_dir(RED_PIN, GPIO_OUT);

    gpio_init(GREEN_PIN);
    gpio_set_dir(GREEN_PIN, GPIO_OUT);

    gpio_init(BLUE_PIN);
    gpio_set_dir(BLUE_PIN, GPIO_OUT);

    // Initialize the LCD
    lcd_init();

    while (true) {
        // Measure distance
        float distance = measure_distance();

        // Display the distance on the LCD
        lcd_clear();
        lcd_set_cursor(0, 0);
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "Dist: %.2f cm", distance);
        lcd_print(buffer);

        // Set RGB color based on distance
        if (distance < 5.0) {
            // Red for very close objects
            set_rgb_color(false, true, true);
            gpio_put(BUZZER_PIN, 1); // Turn on buzzer
        } else if (distance < 10.0) {
            // Blue for moderately close objects
            set_rgb_color(true, true, false);
            gpio_put(BUZZER_PIN, 1); // Turn on buzzer
        } else {
            // Green for objects far away
            set_rgb_color(true, false, true);
            gpio_put(BUZZER_PIN, 0); // Turn off buzzer
        }

        sleep_ms(500);  // Update every 500ms
    }
}*/


/*#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "lcd.h"

// Define pins
#define SDA_PIN 4
#define SCL_PIN 5
#define TRIG_PIN 6
#define ECHO_PIN 7
#define BUZZER_PIN 8
#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

// Function to measure distance using the ultrasonic sensor
float measure_distance() {
    // Send a 10us pulse to the TRIG pin
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    // Measure the pulse width on the ECHO pin
    while (gpio_get(ECHO_PIN) == 0) {}; // Wait for HIGH
    absolute_time_t start_time = get_absolute_time();
    while (gpio_get(ECHO_PIN) == 1) {}; // Wait for LOW
    absolute_time_t end_time = get_absolute_time();

    // Calculate the time difference in microseconds
    int64_t pulse_duration = absolute_time_diff_us(start_time, end_time);

    // Convert to distance (in cm)
    float distance = (pulse_duration / 2.0) * 0.0343;
    return distance;
}

// Function to set RGB color
void set_rgb_color(bool red, bool green, bool blue) {
    gpio_put(RED_PIN, red);
    gpio_put(GREEN_PIN, green);
    gpio_put(BLUE_PIN, blue);
}

int main() {
    stdio_init_all();

    // Initialize I2C for the LCD
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Initialize GPIO for the ultrasonic sensor
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_put(TRIG_PIN, 0);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    // Initialize GPIO for the buzzer
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);

    // Initialize GPIO for RGB LED
    gpio_init(RED_PIN);
    gpio_set_dir(RED_PIN, GPIO_OUT);

    gpio_init(GREEN_PIN);
    gpio_set_dir(GREEN_PIN, GPIO_OUT);

    gpio_init(BLUE_PIN);
    gpio_set_dir(BLUE_PIN, GPIO_OUT);

    // Initialize the LCD
    lcd_init();

    while (true) {
        // Measure distance
        float distance = measure_distance();

        // Display the distance on the LCD
        lcd_clear();
        lcd_set_cursor(0, 0);
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "Dist: %.2f cm", distance);
        lcd_print(buffer);

        // Set RGB color based on distance
        if (distance < 5.0) {
            // Red for very close objects
            set_rgb_color(true, false, false);
            gpio_put(BUZZER_PIN, 1); // Turn on buzzer
        } else if (distance < 10.0) {
            // Yellow (Red + Green) for moderately close objects
            set_rgb_color(true, true, false);
            gpio_put(BUZZER_PIN, 1); // Turn on buzzer
        } else {
            // Green for objects far away
            set_rgb_color(false, true, false);
            gpio_put(BUZZER_PIN, 0); // Turn off buzzer
        }

        sleep_ms(500);  // Update every 500ms
    }
}*/





/*#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "lcd.h"

// Define pins
#define SDA_PIN 4
#define SCL_PIN 5
#define TRIG_PIN 6
#define ECHO_PIN 7
#define BUZZER_PIN 8

// Function to measure distance using the ultrasonic sensor
float measure_distance() {
    // Send a 10us pulse to the TRIG pin
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    // Measure the pulse width on the ECHO pin
    while (gpio_get(ECHO_PIN) == 0) {}; // Wait for HIGH
    absolute_time_t start_time = get_absolute_time();
    while (gpio_get(ECHO_PIN) == 1) {}; // Wait for LOW
    absolute_time_t end_time = get_absolute_time();

    // Calculate the time difference in microseconds
    int64_t pulse_duration = absolute_time_diff_us(start_time, end_time);

    // Convert to distance (in cm)
    float distance = (pulse_duration / 2.0) * 0.0343;
    return distance;
}

int main() {
    stdio_init_all();

    // Initialize I2C for the LCD
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Initialize GPIO for the ultrasonic sensor
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_put(TRIG_PIN, 0);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    // Initialize GPIO for the buzzer
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);

    // Initialize the LCD
    lcd_init();

    while (true) {
        // Measure distance
        float distance = measure_distance();

        // Display the distance on the LCD
        lcd_clear();
        lcd_set_cursor(0, 0);
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "Dist: %.2f cm", distance);
        lcd_print(buffer);

        // Check if the distance is below the threshold (e.g., 10 cm)
        if (distance < 10.0) {
            gpio_put(BUZZER_PIN, 1); // Turn on the buzzer
        } else {
            gpio_put(BUZZER_PIN, 0); // Turn off the buzzer
        }

        sleep_ms(500);  // Update every 500ms
    }
}*/





/*#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "lcd.h"

#define SDA_PIN 4
#define SCL_PIN 5
#define TRIG_PIN 6
#define ECHO_PIN 7

// Function to measure distance using the ultrasonic sensor
float measure_distance() {
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    while (gpio_get(ECHO_PIN) == 0) {};
    absolute_time_t start_time = get_absolute_time();
    while (gpio_get(ECHO_PIN) == 1) {};
    absolute_time_t end_time = get_absolute_time();

    int64_t pulse_duration = absolute_time_diff_us(start_time, end_time);

    // Convert to distance in cm
    float distance = (pulse_duration / 2.0) * 0.0343;
    return distance;
}

int main() {
    stdio_init_all();

    // Initialize I2C for the LCD
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Initialize GPIO for the ultrasonic sensor
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_put(TRIG_PIN, 0);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    // Initialize the LCD
    lcd_init();

    while (true) {
        // Measure distance
        float distance = measure_distance();

        // Display the distance on the LCD
        lcd_clear();
        lcd_set_cursor(0, 0);
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "Dist: %.2f cm", distance);
        lcd_print(buffer);

        sleep_ms(500);  // Refresh every 500ms
    }
}*/
