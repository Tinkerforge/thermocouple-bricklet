/* thermocouple-bricklet
 * Copyright (C) 2015 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config.h: Thermocouple Bricklet specific configuration
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib/drivers/board/sam3s/SAM3S.h"

#include "thermocouple.h"

#define BRICKLET_FIRMWARE_VERSION_MAJOR 2
#define BRICKLET_FIRMWARE_VERSION_MINOR 0
#define BRICKLET_FIRMWARE_VERSION_REVISION 3

#define BRICKLET_HARDWARE_VERSION_MAJOR 1
#define BRICKLET_HARDWARE_VERSION_MINOR 1
#define BRICKLET_HARDWARE_VERSION_REVISION 0

#define BRICKLET_DEVICE_IDENTIFIER 266

#define LOGGING_LEVEL LOGGING_DEBUG
#define DEBUG_BRICKLET 0

#define BOARD_MCK 64000000

#define PIN_SDI (BS->pin1_ad)
#define PIN_SDO (BS->pin2_da)
#define PIN_CLK (BS->pin3_pwm)
#define PIN_NCS (BS->pin4_io)

#define BRICKLET_NO_OFFSET
#define BRICKLET_HAS_SIMPLE_SENSOR
#define INVOCATION_IN_BRICKLET_CODE
#define NUM_SIMPLE_VALUES 1
#define SIMPLE_VALUE_TYPE int32_t

typedef struct {
	int32_t value[NUM_SIMPLE_VALUES];
	int32_t last_value[NUM_SIMPLE_VALUES];

	uint32_t signal_period[NUM_SIMPLE_VALUES];
	uint32_t signal_period_counter[NUM_SIMPLE_VALUES];

	uint32_t threshold_debounce;
	uint32_t threshold_period_current[NUM_SIMPLE_VALUES];
	int32_t  threshold_min[NUM_SIMPLE_VALUES];
	int32_t  threshold_max[NUM_SIMPLE_VALUES];
	char     threshold_option[NUM_SIMPLE_VALUES];

	int32_t  threshold_min_save[NUM_SIMPLE_VALUES];
	int32_t  threshold_max_save[NUM_SIMPLE_VALUES];
	char     threshold_option_save[NUM_SIMPLE_VALUES];

	uint32_t tick;

	uint8_t api_conf_averaging;
	uint8_t api_conf_type;
	uint8_t api_conf_filter_option;

	bool error_over_under;
	bool error_open_circuit;
	bool error_callback;

	uint8_t error_counter;
	int16_t value_counter;
	int16_t value_counter_upto;
} BrickContext;

#endif
