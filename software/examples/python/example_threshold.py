#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your Industrial PTC Bricklet

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_industrial_ptc import BrickletIndustrialPTC

# Callback function for temperature callback
def cb_temperature(temperature):
    print("Temperature: " + str(temperature/100.0) + " °C")

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    ptc = BrickletIndustrialPTC(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Register temperature callback to function cb_temperature
    ptc.register_callback(ptc.CALLBACK_TEMPERATURE, cb_temperature)

    # Configure threshold for temperature "greater than 30 °C"
    # with a debounce period of 1s (1000ms)
    ptc.set_temperature_callback_configuration(1000, False, ">", 30*100, 0)

    input("Press key to exit\n") # Use raw_input() in Python 2
    ipcon.disconnect()
