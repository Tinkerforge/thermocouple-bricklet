#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Thermocouple Bricklet

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
tinkerforge call thermocouple-bricklet $uid set-debounce-period 10000

# Handle incoming temperature reached callbacks
tinkerforge dispatch thermocouple-bricklet $uid temperature-reached &

# Configure threshold for temperature "greater than 30 °C"
tinkerforge call thermocouple-bricklet $uid set-temperature-callback-threshold threshold-option-greater 3000 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
