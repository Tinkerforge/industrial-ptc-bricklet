function octave_example_threshold()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Industrial PTC Bricklet

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    ip = javaObject("com.tinkerforge.BrickletIndustrialPTC", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register temperature callback to function cb_temperature
    ip.addTemperatureCallback(@cb_temperature);

    % Configure threshold for temperature "greater than 30 °C"
    % with a debounce period of 1s (1000ms)
    ip.setTemperatureCallbackConfiguration(1000, false, ">", 30*100, 0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for temperature callback
function cb_temperature(e)
    fprintf("Temperature: %g °C\n", e.temperature/100.0);
end
