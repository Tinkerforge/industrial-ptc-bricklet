#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Industrial PTC Bricklet

# Handle incoming temperature callbacks
tinkerforge dispatch industrial-ptc-bricklet $uid temperature &

# Configure threshold for temperature "greater than 30 °C"
# with a debounce period of 1s (1000ms)
tinkerforge call industrial-ptc-bricklet $uid set-temperature-callback-configuration 1000 false threshold-option-greater 3000 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
