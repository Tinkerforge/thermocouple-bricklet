#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change to your UID

# Set period for temperature callback to 1s (1000ms)
# Note: The temperature callback is only called every second
#       if the temperature has changed since the last call!
tinkerforge call thermocouple-bricklet $uid set-temperature-callback-period 1000

# Handle incoming temperature callbacks (parameter has unit °C/100)
tinkerforge dispatch thermocouple-bricklet $uid temperature
