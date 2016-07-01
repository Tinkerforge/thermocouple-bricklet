<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletThermocouple.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletThermocouple;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Thermocouple Bricklet

$ipcon = new IPConnection(); // Create IP connection
$t = new BrickletThermocouple(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current temperature (unit is °C/100)
$temperature = $t->getTemperature();
echo "Temperature: " . $temperature/100.0 . " °C\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
