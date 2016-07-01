#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Thermocouple Bricklet

# Get current temperature (unit is °C/100)
tinkerforge call thermocouple-bricklet $uid get-temperature
