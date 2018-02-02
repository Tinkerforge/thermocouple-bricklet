#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_thermocouple'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Thermocouple Bricklet

ipcon = IPConnection.new # Create IP connection
t = BrickletThermocouple.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Get current temperature
temperature = t.get_temperature
puts "Temperature: #{temperature/100.0} °C"

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
