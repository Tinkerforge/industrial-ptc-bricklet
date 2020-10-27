using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Industrial PTC Bricklet

	// Callback function for temperature callback
	static void TemperatureCB(BrickletIndustrialPTC sender, int temperature)
	{
		Console.WriteLine("Temperature: " + temperature/100.0 + " °C");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialPTC ip = new BrickletIndustrialPTC(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register temperature callback to function TemperatureCB
		ip.TemperatureCallback += TemperatureCB;

		// Configure threshold for temperature "greater than 30 °C"
		// with a debounce period of 1s (1000ms)
		ip.SetTemperatureCallbackConfiguration(1000, false, '>', 30*100, 0);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
