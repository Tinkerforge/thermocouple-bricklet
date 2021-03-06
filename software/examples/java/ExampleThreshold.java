import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletThermocouple;

public class ExampleThreshold {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Thermocouple Bricklet
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletThermocouple t = new BrickletThermocouple(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
		t.setDebouncePeriod(10000);

		// Add temperature reached listener
		t.addTemperatureReachedListener(new BrickletThermocouple.TemperatureReachedListener() {
			public void temperatureReached(int temperature) {
				System.out.println("Temperature: " + temperature/100.0 + " °C");
			}
		});

		// Configure threshold for temperature "greater than 30 °C"
		t.setTemperatureCallbackThreshold('>', 30*100, 0);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
