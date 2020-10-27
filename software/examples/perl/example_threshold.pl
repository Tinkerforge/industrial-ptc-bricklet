#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIndustrialPTC;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Industrial PTC Bricklet

# Callback subroutine for temperature callback
sub cb_temperature
{
    my ($temperature) = @_;

    print "Temperature: " . $temperature/100.0 . " °C\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $ip = Tinkerforge::BrickletIndustrialPTC->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register temperature callback to subroutine cb_temperature
$ip->register_callback($ip->CALLBACK_TEMPERATURE, 'cb_temperature');

# Configure threshold for temperature "greater than 30 °C"
# with a debounce period of 1s (1000ms)
$ip->set_temperature_callback_configuration(1000, 0, '>', 30*100, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
