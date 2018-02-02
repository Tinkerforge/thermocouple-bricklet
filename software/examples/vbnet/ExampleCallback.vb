Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Thermocouple Bricklet

    ' Callback subroutine for temperature callback
    Sub TemperatureCB(ByVal sender As BrickletThermocouple, _
                      ByVal temperature As Integer)
        Console.WriteLine("Temperature: " + (temperature/100.0).ToString() + " °C")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim t As New BrickletThermocouple(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register temperature callback to subroutine TemperatureCB
        AddHandler t.TemperatureCallback, AddressOf TemperatureCB

        ' Set period for temperature callback to 1s (1000ms)
        ' Note: The temperature callback is only called every second
        '       if the temperature has changed since the last call!
        t.SetTemperatureCallbackPeriod(1000)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
