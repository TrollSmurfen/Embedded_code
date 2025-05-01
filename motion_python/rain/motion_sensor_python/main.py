from machine import Pin, I2C, PWM
import utime
from pico_i2c_lcd import I2cLcd
from simple import MQTTClient  # Ensure this module is available or use umqtt.simple
import network

# LCD Settings
I2C_ADDR = 0x27
I2C_NUM_ROWS = 2
I2C_NUM_COLS = 16

# MQTT Settings
BROKER_ADDRESS = "192.168.0.23"  # Replace with your MQTT broker IP or domain
MQTT_TOPIC = "motion/topic"

# Wi-Fi Settings
SSID = "ssid"  # Replace with your Wi-Fi name
PASSWORD = "password"  # Replace with your Wi-Fi password

# Initialize Components
def initialize_components():
    global lcd, TRIG, ECHO, buzzer, red, green, blue, mqtt_client

    # Initialize I2C for LCD
    i2c = I2C(0, sda=Pin(0), scl=Pin(1), freq=400000)
    try:
        lcd = I2cLcd(i2c, I2C_ADDR, I2C_NUM_ROWS, I2C_NUM_COLS)
        lcd.putstr("LCD Ready")
    except Exception as e:
        print(f"LCD Initialization Error: {e}")
        return

    # Ultrasonic Sensor Pins
    TRIG = Pin(3, Pin.OUT)
    ECHO = Pin(2, Pin.IN)

    # Buzzer Pin
    buzzer = PWM(Pin(15))

    # RGB LED Pins
    red = PWM(Pin(16))
    green = PWM(Pin(17))
    blue = PWM(Pin(18))

    # Connect to Wi-Fi
    connect_to_wifi()

    # Initialize MQTT Client
    try:
        mqtt_client = MQTTClient("PicoW", BROKER_ADDRESS, port=1883)  # Default port 1883
        mqtt_client.connect()
        print(f"Connected to MQTT Broker: {BROKER_ADDRESS}")
        lcd.clear()
        lcd.putstr("MQTT Connected!")
    except Exception as e:
        print(f"Failed to connect to MQTT Broker: {e}")
        lcd.clear()
        lcd.putstr("MQTT Error!")
        utime.sleep(2)

# Connect to Wi-Fi
def connect_to_wifi():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(SSID, PASSWORD)
    lcd.putstr("Connecting Wi-Fi...")
    print("Connecting to Wi-Fi...")

    while not wlan.isconnected():
        utime.sleep(1)

    print(f"Wi-Fi Connected: {wlan.ifconfig()[0]}")
    lcd.clear()
    lcd.putstr("Wi-Fi Connected!")
    utime.sleep(2)

# Measure Distance Using Ultrasonic Sensor
def measure_distance():
    TRIG.low()
    utime.sleep_us(2)
    TRIG.high()
    utime.sleep_us(10)
    TRIG.low()

    try:
        start_time = utime.ticks_us()
        while ECHO.value() == 0:
            if utime.ticks_diff(utime.ticks_us(), start_time) > 1000000:  # Timeout
                return float('inf')

        signal_off = utime.ticks_us()
        start_time = utime.ticks_us()
        while ECHO.value() == 1:
            if utime.ticks_diff(utime.ticks_us(), start_time) > 1000000:  # Timeout
                return float('inf')

        signal_on = utime.ticks_us()
        time_passed = utime.ticks_diff(signal_on, signal_off)
        distance = (time_passed * 0.0343) / 2
        return distance
    except Exception as e:
        print(f"Distance measurement error: {e}")
        return float('inf')

# Set RGB LED Color Based on Distance
def set_rgb_color(distance):
    if distance < 10:
        red.duty_u16(0)  # Red on
        green.duty_u16(65534)
        blue.duty_u16(65534)
    elif 10 <= distance < 20:
        red.duty_u16(0)
        green.duty_u16(65534)  # Green on
        blue.duty_u16(0)
    else:
        red.duty_u16(65534)
        green.duty_u16(0)
        blue.duty_u16(65534)  # Blue on

# Control Buzzer Based on Distance
def control_buzzer(distance):
    if distance < 10:
        buzzer.freq(5000)  # High frequency
        buzzer.duty_u16(32768)
    elif 10 <= distance < 20:
        buzzer.freq(2500)  # Medium frequency
        buzzer.duty_u16(32768)
    elif 20 <= distance < 30:
        buzzer.freq(1000)  # Low frequency
        buzzer.duty_u16(32768)
    else:
        buzzer.duty_u16(0)  # Buzzer off

# Publish MQTT Message Based on Distance
def publish_mqtt_message(distance):
    if distance < 10:
        message = "ALERT: Motion detected! Distance < 10 cm"
    elif 10 <= distance < 20:
        message = "WARNING: Motion detected! Distance < 20 cm"
    elif 20 <= distance < 30:
        message = "NOTICE: Motion detected! Distance < 30 cm"
    else:
        message = "All clear. Distance > 30 cm"

    try:
        mqtt_client.publish(MQTT_TOPIC, message)
        print(f"MQTT Message Sent: {message}")
    except Exception as e:
        print(f"MQTT Publish Error: {e}")

# Main Function
def main():
    initialize_components()

    while True:
        distance = measure_distance()
        lcd.clear()
        lcd.putstr("Distance: {:.2f} cm".format(distance))
        set_rgb_color(distance)
        control_buzzer(distance)
        publish_mqtt_message(distance)
        utime.sleep(0.5)  # Adjust delay for smoother operation

# Run the Program
if __name__ == "__main__":
    main()

