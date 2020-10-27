Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Industrial PTC Bricklet

    ' Callback subroutine for temperature callback
    Sub TemperatureCB(ByVal sender As BrickletIndustrialPTC, _
                      ByVal temperature As Integer)
        Console.WriteLine("Temperature: " + (temperature/100.0).ToString() + " °C")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim ip As New BrickletIndustrialPTC(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register temperature callback to subroutine TemperatureCB
        AddHandler ip.TemperatureCallback, AddressOf TemperatureCB

        ' Set period for temperature callback to 1s (1000ms) without a threshold
        ip.SetTemperatureCallbackConfiguration(1000, False, "x"C, 0, 0)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
