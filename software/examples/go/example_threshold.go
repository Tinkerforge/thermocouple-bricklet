package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
	"github.com/Tinkerforge/go-api-bindings/thermocouple_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Thermocouple Bricklet.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	t, _ := thermocouple_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Get threshold receivers with a debounce time of 10 seconds (10000ms).
	t.SetDebouncePeriod(10000)

	t.RegisterTemperatureReachedCallback(func(temperature int32) {
		fmt.Printf("Temperature: %f °C\n", float64(temperature)/100.0)
	})

	// Configure threshold for temperature "greater than 30 °C".
	t.SetTemperatureCallbackThreshold('>', 30*100, 0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}
