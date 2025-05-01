import paho.mqtt.client as mqtt

# Callback for when a message is received
def on_message(client, userdata, msg):
    message = msg.payload.decode()
    print(f"Received message: {message}")
    if "ALERT" in message:
        print("Triggering alert: Taking action!")
        # Add your alert logic here (e.g., send email, turn on a device)

# Callback for when the client connects to the broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT broker")
        client.subscribe("motion/topic")
    else:
        print(f"Connection failed with code {rc}")

# MQTT setup
broker_address = "192.168.2.1"  # Replace with your broker's IP
client = mqtt.Client("Broker_Subscriber")
client.on_connect = on_connect
client.on_message = on_message

# Connect to the broker and start listening
client.connect(broker_address, 1883)
client.loop_forever()
