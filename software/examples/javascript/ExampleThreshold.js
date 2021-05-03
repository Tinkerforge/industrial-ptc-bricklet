var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Industrial PTC Bricklet

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var ptc = new Tinkerforge.BrickletIndustrialPTC(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Configure threshold for temperature "greater than 30 °C"
        // with a debounce period of 1s (1000ms)
        ptc.setTemperatureCallbackConfiguration(1000, false, '>', 30*100, 0);
    }
);

// Register temperature callback
ptc.on(Tinkerforge.BrickletIndustrialPTC.CALLBACK_TEMPERATURE,
    // Callback function for temperature callback
    function (temperature) {
        console.log('Temperature: ' + temperature/100.0 + ' °C');
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
