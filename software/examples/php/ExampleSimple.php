<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIndustrialPTC.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIndustrialPTC;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Industrial PTC Bricklet

$ipcon = new IPConnection(); // Create IP connection
$ip = new BrickletIndustrialPTC(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current temperature
$temperature = $ip->getTemperature();
echo "Temperature: " . $temperature/100.0 . " °C\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
