from twilio.rest import Client
import os  # For accessing environment variables

# Twilio Credentials (stored as environment variables)
ACCOUNT_SID = os.getenv("TWILIO_ACCOUNT_SID")
AUTH_TOKEN = os.getenv("TWILIO_AUTH_TOKEN")
TWILIO_WHATSAPP_NUMBER = os.getenv("TWILIO_WHATSAPP_NUMBER")
YOUR_WHATSAPP_NUMBER = os.getenv("MY_WHATSAPP_NUMBER")

# MQTT Broker Configuration (stored as environment variables)
MQTT_BROKER = os.getenv("MQTT_BROKER")  # Replace with your RPI0 IP address
MQTT_TOPIC = os.getenv("MQTT_TOPIC")   # Topic for motion sensor messages

# Initialize Twilio Client
client = Client(ACCOUNT_SID, AUTH_TOKEN)

def send_whatsapp_message(message):
    """Send a WhatsApp message via Twilio."""
    client.messages.create(
        from_=TWILIO_WHATSAPP_NUMBER,
        body=message,
        to=YOUR_WHATSAPP_NUMBER
    )
    print(f"WhatsApp Message sent: {message}")

# Your MQTT-related code and logic here
# Example usage:
send_whatsapp_message("Motion detected!")
