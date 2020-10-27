#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_industrial_ptc'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Industrial PTC Bricklet

ipcon = IPConnection.new # Create IP connection
ip = BrickletIndustrialPTC.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register temperature callback
ip.register_callback(BrickletIndustrialPTC::CALLBACK_TEMPERATURE) do |temperature|
  puts "Temperature: #{temperature/100.0} °C"
end

# Configure threshold for temperature "greater than 30 °C"
# with a debounce period of 1s (1000ms)
ip.set_temperature_callback_configuration 1000, false, '>', 30*100, 0

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
