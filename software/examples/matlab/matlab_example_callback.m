function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIndustrialPTC;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Industrial PTC Bricklet

    ipcon = IPConnection(); % Create IP connection
    ip = handle(BrickletIndustrialPTC(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register temperature callback to function cb_temperature
    set(ip, 'TemperatureCallback', @(h, e) cb_temperature(e));

    % Set period for temperature callback to 1s (1000ms) without a threshold
    ip.setTemperatureCallbackConfiguration(1000, false, 'x', 0, 0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for temperature callback
function cb_temperature(e)
    fprintf('Temperature: %g °C\n', e.temperature/100.0);
end
