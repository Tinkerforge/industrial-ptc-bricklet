#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Industrial PTC Bricklet

# Get current temperature
tinkerforge call industrial-ptc-bricklet $uid get-temperature
