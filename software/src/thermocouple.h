/* thermocouple-bricklet
 * Copyright (C) 2015 Olaf Lüke <olaf@tinkerforge.com>
 *
 * thermocouple.h: Implementation of Thermocouple Bricklet messages
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

#ifndef THERMOCOUPLE_H
#define THERMOCOUPLE_H

#include <stdint.h>

#include "bricklib/com/com_common.h"

#define ERROR_TEST_PERIOD 40

#define API_AVERAGING_1   1
#define API_AVERAGING_2   2
#define API_AVERAGING_4   4
#define API_AVERAGING_8   8
#define API_AVERAGING_16 16

#define API_TYPE_B   0
#define API_TYPE_E   1
#define API_TYPE_J   2
#define API_TYPE_K   3
#define API_TYPE_N   4
#define API_TYPE_R   5
#define API_TYPE_S   6
#define API_TYPE_T   7
#define API_TYPE_G8  8
#define API_TYPE_G32 9

#define API_FILTER_OPTION_50HZ 0
#define API_FILTER_OPTION_60HZ 1

#define REG_CONF0 0x00
#define REG_CONF1 0x01
#define REG_FAULT 0x02
#define REG_COLD_JUNCTION_HIGH_FAULT_THRESHOLD 0x03
#define REG_COLD_JUNCTION_LOW_FAULT_THRESHOLD 0x04
#define REG_LIN_TEMP_HIGH_FAULT_THRESHOLD_MSB 0x05
#define REG_LIN_TEMP_HIGH_FAULT_THRESHOLD_LSB 0x06
#define REG_LIN_TEMP_LOW_FAULT_THRESHOLD_MSB 0x07
#define REG_LIN_TEMP_LOW_FAULT_THRESHOLD_LSB 0x08
#define REG_COLD_JUNCTION_TEMP_OFFSET 0x09
#define REG_COLD_JUNCTION_TEMP_MSB 0x0A
#define REG_COLD_JUNCTION_TEMP_LSB 0x0B
#define REG_LIN_TC_TEMP2 0x0C
#define REG_LIN_TC_TEMP1 0x0D
#define REG_LIN_TC_TEMP0 0x0E
#define REG_FAULT_STATUS 0x0F

#define CONF0_50HZ (1 << 0)
#define CONF0_FAULTCLR (1 << 1)
#define CONF0_FAULT (1 << 2)
#define CONF0_CJ (1 << 3)
#define CONF0_OCFAULT_DISABLED (0 << 4)
#define CONF0_OCFAULT_40_ENABLED (1 << 4)
#define CONF0_OCFAULT_60_ENABLED (2 << 4)
#define CONF0_OCFAULT_140_ENABLED (3 << 4)
#define CONF0_1SHOT (1 << 6)
#define CONF0_CMODE_AUTO (1 << 7)

#define CONF1_TYPE_B (0 << 0)
#define CONF1_TYPE_E (1 << 0)
#define CONF1_TYPE_J (2 << 0)
#define CONF1_TYPE_K (3 << 0)
#define CONF1_TYPE_N (4 << 0)
#define CONF1_TYPE_R (5 << 0)
#define CONF1_TYPE_S (6 << 0)
#define CONF1_TYPE_T (7 << 0)
#define CONF1_TYPE_G8 (8 << 0)
#define CONF1_TYPE_G32 (12 << 0)
#define CONF1_AVGSEL_1 (0 << 4)
#define CONF1_AVGSEL_2 (1 << 4)
#define CONF1_AVGSEL_4 (2 << 4)
#define CONF1_AVGSEL_8 (3 << 4)
#define CONF1_AVGSEL_16 (4 << 4)

#define FAULT_MASK_OPEN    (1 << 0)
#define FAULT_MASK_OV_UV   (1 << 1)
#define FAULT_MASK_TC_LOW  (1 << 2)
#define FAULT_MASK_TC_HIGH (1 << 3)
#define FAULT_MASK_CJ_LOW  (1 << 4)
#define FAULT_MASK_CJ_HIGH (1 << 5)

#define FAULT_STATUS_OPEN_CIRCUIT (1 << 0)
#define FAULT_STATUS_OVER_UNDER   (1 << 1)

#define FID_GET_THERMOCOUPLE 1
#define FID_SET_THERMOCOUPLE_CALLBACK_PERIOD 2
#define FID_GET_THERMOCOUPLE_CALLBACK_PERIOD 3
#define FID_SET_THERMOCOUPLE_CALLBACK_THRESHOLD 4
#define FID_GET_THERMOCOUPLE_CALLBACK_THRESHOLD 5
#define FID_SET_DEBOUNCE_PERIOD 6
#define FID_GET_DEBOUNCE_PERIOD 7
#define FID_THERMOCOUPLE 8
#define FID_THERMOCOUPLE_REACHED 9
#define FID_SET_CONFIGURATION 10
#define FID_GET_CONFIGURATION 11
#define FID_GET_ERROR_STATE 12
#define FID_ERROR_STATE 13

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetConfiguration;

typedef struct {
	MessageHeader header;
	uint8_t averaging;
	uint8_t thermocouple_type;
	uint8_t filter_option;
} __attribute__((__packed__)) GetConfigurationReturn;

typedef struct {
	MessageHeader header;
	uint8_t averaging;
	uint8_t thermocouple_type;
	uint8_t filter_option;
} __attribute__((__packed__)) SetConfiguration;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetErrorState;

typedef struct {
	MessageHeader header;
	bool over_under;
	bool open_circuit;
} __attribute__((__packed__)) GetErrorStateReturn;

typedef struct {
	MessageHeader header;
	bool over_under;
	bool open_circuit;
} __attribute__((__packed__)) ErrorStateCallback;

void get_configuration(const ComType com, const GetConfiguration *data);
void set_configuration(const ComType com, const SetConfiguration *data);
void get_error_state(const ComType com, const GetErrorState *data);

void apply_configuration(void);
int32_t get_temperature(const int32_t value);

void invocation(const ComType com, const uint8_t *data);
void constructor(void);
void destructor(void);
void tick(const uint8_t tick_type);

void max31856_write_register(const uint8_t reg, const uint8_t *data, const uint8_t length);
void max31856_read_register(const uint8_t reg, uint8_t *data, const uint8_t length);
uint8_t spibb_transceive_byte(const uint8_t value);

#endif
