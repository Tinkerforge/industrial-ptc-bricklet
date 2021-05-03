use std::{error::Error, io, thread};
use tinkerforge::{industrial_ptc_bricklet::*, ip_connection::IpConnection};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Industrial PTC Bricklet.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let ptc = IndustrialPtcBricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    let temperature_receiver = ptc.get_temperature_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `ptc` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for temperature in temperature_receiver {
            println!("Temperature: {} °C", temperature as f32 / 100.0);
        }
    });

    // Configure threshold for temperature "greater than 30 °C"
    // with a debounce period of 1s (1000ms).
    ptc.set_temperature_callback_configuration(1000, false, '>', 30 * 100, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
