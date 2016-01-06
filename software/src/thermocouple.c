/* thermocouple-bricklet
 * Copyright (C) 2015 Olaf Lüke <olaf@tinkerforge.com>
 *
 * thermocouple.c: Implementation of Thermocouple Bricklet messages
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

#include "thermocouple.h"

#include "brickletlib/bricklet_entry.h"
#include "brickletlib/bricklet_simple.h"
#include "bricklib/bricklet/bricklet_communication.h"
#include "bricklib/utility/util_definitions.h"
#include "bricklib/utility/init.h"
#include "config.h"

#define MAX31856_REG_READ  (0)
#define MAX31856_REG_WRITE (1 << 7)

#define SIMPLE_UNIT_THERMOCOUPLE 0

const SimpleMessageProperty smp[] = {
	{SIMPLE_UNIT_THERMOCOUPLE, SIMPLE_TRANSFER_VALUE, SIMPLE_DIRECTION_GET}, // TYPE_GET_THERMOCOUPLE
	{SIMPLE_UNIT_THERMOCOUPLE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_SET}, // TYPE_SET_THERMOCOUPLE_CALLBACK_PERIOD
	{SIMPLE_UNIT_THERMOCOUPLE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_GET}, // TYPE_GET_THERMOCOUPLE_CALLBACK_PERIOD
	{SIMPLE_UNIT_THERMOCOUPLE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_SET}, // TYPE_SET_THERMOCOUPLE_CALLBACK_THRESHOLD
	{SIMPLE_UNIT_THERMOCOUPLE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_GET}, // TYPE_GET_THERMOCOUPLE_CALLBACK_THRESHOLD
	{0, SIMPLE_TRANSFER_DEBOUNCE, SIMPLE_DIRECTION_SET}, // TYPE_SET_DEBOUNCE_PERIOD
	{0, SIMPLE_TRANSFER_DEBOUNCE, SIMPLE_DIRECTION_GET}, // TYPE_GET_DEBOUNCE_PERIOD
};

const SimpleUnitProperty sup[] = {
	{get_temperature, SIMPLE_SIGNEDNESS_INT, FID_THERMOCOUPLE, FID_THERMOCOUPLE_REACHED, SIMPLE_UNIT_THERMOCOUPLE} // temperature
};

const uint8_t smp_length = sizeof(smp);

void invocation(const ComType com, const uint8_t *data) {
	switch(((MessageHeader*)data)->fid) {
		case FID_GET_THERMOCOUPLE:
		case FID_SET_THERMOCOUPLE_CALLBACK_PERIOD:
		case FID_GET_THERMOCOUPLE_CALLBACK_PERIOD:
		case FID_SET_THERMOCOUPLE_CALLBACK_THRESHOLD:
		case FID_GET_THERMOCOUPLE_CALLBACK_THRESHOLD:
		case FID_SET_DEBOUNCE_PERIOD:
		case FID_GET_DEBOUNCE_PERIOD: {
			simple_invocation(com, data);
			break;
		}

		case FID_SET_CONFIGURATION: {
			set_configuration(com, (SetConfiguration*)data);
			break;
		}

		case FID_GET_CONFIGURATION: {
			get_configuration(com, (GetConfiguration*)data);
			break;
		}

		case FID_GET_ERROR_STATE: {
			get_error_state(com, (GetErrorState*)data);
			break;
		}

		default: {
			BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
			break;
		}
	}
}

void constructor(void) {
	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE, "BrickContext too big");

	PIN_SDO.type = PIO_INPUT;
	PIN_SDO.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&PIN_SDO, 1);

	PIN_SDI.type = PIO_OUTPUT_0;
	PIN_SDI.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&PIN_SDI, 1);

	PIN_CLK.type = PIO_OUTPUT_1;
	PIN_CLK.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&PIN_CLK, 1);

	PIN_NCS.type = PIO_OUTPUT_1;
	PIN_NCS.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&PIN_NCS, 1);

	simple_constructor();

	SLEEP_MS(100);

	BC->error_counter = 0;
	BC->value_counter = 0;

	BC->api_conf_averaging     = API_AVERAGING_16;
	BC->api_conf_type          = API_TYPE_K;
	BC->api_conf_filter_option = API_FILTER_OPTION_50HZ;
	apply_configuration();

	BC->error_callback     = false;
	BC->error_over_under   = false;
	BC->error_open_circuit = false;

	// 0 activates fault output to LED
	uint8_t fault_mask = ~(FAULT_MASK_OPEN | FAULT_MASK_OV_UV);
	max31856_write_register(REG_FAULT, &fault_mask, 1);
}

void apply_configuration(void) {
	uint8_t conf[2] = {0, 0};
	conf[0] = CONF0_CMODE_AUTO | CONF0_OCFAULT_40_ENABLED | (BC->api_conf_filter_option == API_FILTER_OPTION_50HZ ? CONF0_50HZ : 0);

	switch(BC->api_conf_averaging) {
		case API_AVERAGING_1:  conf[1] = CONF1_AVGSEL_1;  break;
		case API_AVERAGING_2:  conf[1] = CONF1_AVGSEL_2;  break;
		case API_AVERAGING_4:  conf[1] = CONF1_AVGSEL_4;  break;
		case API_AVERAGING_8:  conf[1] = CONF1_AVGSEL_8;  break;
		case API_AVERAGING_16: conf[1] = CONF1_AVGSEL_16; break;
	}

	switch(BC->api_conf_type) {
		case API_TYPE_B:   conf[1] |= CONF1_TYPE_B;   break;
		case API_TYPE_E:   conf[1] |= CONF1_TYPE_E;   break;
		case API_TYPE_J:   conf[1] |= CONF1_TYPE_J;   break;
		case API_TYPE_K:   conf[1] |= CONF1_TYPE_K;   break;
		case API_TYPE_N:   conf[1] |= CONF1_TYPE_N;   break;
		case API_TYPE_R:   conf[1] |= CONF1_TYPE_R;   break;
		case API_TYPE_S:   conf[1] |= CONF1_TYPE_S;   break;
		case API_TYPE_T:   conf[1] |= CONF1_TYPE_T;   break;
		case API_TYPE_G8:  conf[1] |= CONF1_TYPE_G8;  break;
		case API_TYPE_G32: conf[1] |= CONF1_TYPE_G32; break;
	}

	max31856_write_register(REG_CONF0, conf, 2);

	int16_t new_value_counter_upto;
	if(BC->api_conf_filter_option == API_FILTER_OPTION_50HZ) {
		new_value_counter_upto = 98 + (BC->api_conf_averaging-1)*20;
	} else {
		new_value_counter_upto = (8200 + (BC->api_conf_averaging-1)*1667)/100;
	}

	if(new_value_counter_upto != BC->value_counter_upto) {
		// If the averaging of filter option changes, we wait two times the normal conversion period
		BC->value_counter = -2*new_value_counter_upto;
		BC->value_counter_upto = new_value_counter_upto;
	}
}

void destructor(void) {
	simple_destructor();
}

void tick(const uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		BC->error_counter++;
		if(BC->error_counter >= ERROR_TEST_PERIOD) {
			BC->error_counter = 0;
			uint8_t fault_status = 0;
			max31856_read_register(REG_FAULT_STATUS, &fault_status, 1);
			bool error_open_circuit = fault_status & FAULT_STATUS_OPEN_CIRCUIT;
			bool error_over_under   = fault_status & FAULT_STATUS_OVER_UNDER;

			if(error_open_circuit != BC->error_open_circuit) {
				BC->error_open_circuit = error_open_circuit;
				BC->error_callback = true;
			}

			if(error_over_under != BC->error_over_under) {
				BC->error_over_under = error_over_under;
				BC->error_callback = true;
			}
		}
	}

	if(tick_type & TICK_TASK_TYPE_MESSAGE) {
		if(BC->error_callback) {
			BC->error_callback = false;
			ErrorStateCallback esc;
			BA->com_make_default_header(&esc, BS->uid, sizeof(ErrorStateCallback), FID_ERROR_STATE);
			esc.open_circuit = BC->error_open_circuit;
			esc.over_under   = BC->error_over_under;

			BA->send_blocking_with_timeout(&esc,
										   sizeof(ErrorStateCallback),
										   *BA->com_current);
		}
	}

	simple_tick(tick_type);
}

int32_t get_temperature(const int32_t value) {
	BC->value_counter++;
	int32_t temperature = value;

    if(BC->value_counter >= BC->value_counter_upto) {
    	BC->value_counter = 0;

		for(uint8_t i = 0; i < 8; i++) {
			uint8_t data[3] = {0, 0, 0};
			max31856_read_register(REG_LIN_TC_TEMP2, (uint8_t *)&data, 3);
			temperature = (data[2] | (data[1] << 8) | (data[0] << 16)) >> 5;

			// transfer sign of 19 bit temperature value to 32 bit variable
			if(temperature & 0x4000) {
				temperature |= 0xFFFC0000;
			}

			// For G8 and G32 we return the value as read from the MAX31856
			if(BC->api_conf_type != API_TYPE_G8 && BC->api_conf_type != API_TYPE_G32) {
				// *100/128 for 0.01°C steps
				temperature = temperature*100/128;
			}
		}
    }
    return temperature;
}

void get_configuration(const ComType com, const GetConfiguration *data) {
	GetConfigurationReturn gcr;

	gcr.header            = data->header;
	gcr.header.length     = sizeof(GetConfigurationReturn);
	gcr.averaging         = BC->api_conf_averaging;
	gcr.thermocouple_type = BC->api_conf_type;
	gcr.filter_option     = BC->api_conf_filter_option;

	BA->send_blocking_with_timeout(&gcr, sizeof(GetConfigurationReturn), com);
}

void set_configuration(const ComType com, const SetConfiguration *data) {
	if((data->averaging != API_AVERAGING_1 ||
		data->averaging != API_AVERAGING_2 ||
		data->averaging != API_AVERAGING_4 ||
		data->averaging != API_AVERAGING_8 ||
		data->averaging != API_AVERAGING_16)    ||
	   (data->thermocouple_type > API_TYPE_G32) ||
	   (data->filter_option     > API_FILTER_OPTION_60HZ)) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
	}

	BC->api_conf_averaging     = data->averaging;
	BC->api_conf_type          = data->thermocouple_type;
	BC->api_conf_filter_option = data->filter_option;

	apply_configuration();
}

void get_error_state(const ComType com, const GetErrorState *data) {
	GetErrorStateReturn gesr;

	gesr.header        = data->header;
	gesr.header.length = sizeof(GetErrorStateReturn);
	gesr.over_under    = BC->error_over_under;
	gesr.open_circuit  = BC->error_open_circuit;

	BA->send_blocking_with_timeout(&gesr, sizeof(GetErrorStateReturn), com);
}

void max31856_write_register(const uint8_t reg, const uint8_t *data, const uint8_t length) {
	PIN_NCS.pio->PIO_CODR = PIN_NCS.mask; // latch low
	SLEEP_NS(100);
	spibb_transceive_byte(reg | MAX31856_REG_WRITE);

	for(uint8_t i = 0; i < length; i++) {
		spibb_transceive_byte(data[i]);
	}
	SLEEP_NS(50);
	PIN_NCS.pio->PIO_SODR = PIN_NCS.mask; // latch high
}

void max31856_read_register(const uint8_t reg, uint8_t *data, const uint8_t length) {
	PIN_NCS.pio->PIO_CODR = PIN_NCS.mask; // latch low
	SLEEP_NS(100);
	spibb_transceive_byte(reg | MAX31856_REG_READ);

	for(uint8_t i = 0; i < length; i++) {
		data[i] = spibb_transceive_byte(0);
	}
	SLEEP_NS(50);
	PIN_NCS.pio->PIO_SODR = PIN_NCS.mask; // latch high
}

uint8_t spibb_transceive_byte(const uint8_t value) {
	uint8_t recv = 0;

	for(int8_t i = 7; i >= 0; i--) {
		PIN_CLK.pio->PIO_CODR = PIN_CLK.mask;
		if((value >> i) & 1) {
			PIN_SDI.pio->PIO_SODR = PIN_SDI.mask;
		} else {
			PIN_SDI.pio->PIO_CODR = PIN_SDI.mask;
		}

		SLEEP_US(1);
		if(PIN_SDO.pio->PIO_PDSR & PIN_SDO.mask) {
			recv |= (1 << i);
		}

		PIN_CLK.pio->PIO_SODR = PIN_CLK.mask;
		SLEEP_US(1);
	}

	return recv;
}
