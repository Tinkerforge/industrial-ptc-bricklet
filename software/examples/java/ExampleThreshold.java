import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIndustrialPTC;

public class ExampleThreshold {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Industrial PTC Bricklet
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialPTC ptc = new BrickletIndustrialPTC(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Add temperature listener
		ptc.addTemperatureListener(new BrickletIndustrialPTC.TemperatureListener() {
			public void temperature(int temperature) {
				System.out.println("Temperature: " + temperature/100.0 + " °C");
			}
		});

		// Configure threshold for temperature "greater than 30 °C"
		// with a debounce period of 1s (1000ms)
		ptc.setTemperatureCallbackConfiguration(1000, false, '>', 30*100, 0);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
