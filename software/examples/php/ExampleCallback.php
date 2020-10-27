<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIndustrialPTC.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIndustrialPTC;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Industrial PTC Bricklet

// Callback function for temperature callback
function cb_temperature($temperature)
{
    echo "Temperature: " . $temperature/100.0 . " °C\n";
}

$ipcon = new IPConnection(); // Create IP connection
$ip = new BrickletIndustrialPTC(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register temperature callback to function cb_temperature
$ip->registerCallback(BrickletIndustrialPTC::CALLBACK_TEMPERATURE, 'cb_temperature');

// Set period for temperature callback to 1s (1000ms) without a threshold
$ip->setTemperatureCallbackConfiguration(1000, FALSE, 'x', 0, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
