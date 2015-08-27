<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletThermocouple.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletThermocouple;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change to your UID

// Callback function for temperature greater than 30 °C (parameter has unit °C/100)
function cb_temperature_reached($temperature)
{
    echo "Temperature: " . $temperature/100.0 . " °C\n";
}

$ipcon = new IPConnection(); // Create IP connection
$t = new BrickletThermocouple(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
$t->setDebouncePeriod(10000);

// Register temperature reached callback to function cb_temperature_reached
$t->registerCallback(BrickletThermocouple::CALLBACK_TEMPERATURE_REACHED, 'cb_temperature_reached');

// Configure threshold for temperature "greater than 30 °C" (unit is °C/100)
$t->settemperatureCallbackThreshold('>', 30*100, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
