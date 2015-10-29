/****************************************************************************
 *
 *   Copyright (C) 2015 Mark Charlebois. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#pragma once

#include "I2CDriverObj"

using namespace DriverFramework;

/**
 * The sensor independent data structure containing pressure values.
 */
struct pressure_sensor_data
{
	int32_t  t_fine; 			/*! used internally to calculate a temperature compensated pressure value. */
	uint32_t pressure_in_pa; 		/*! current pressure in Pascals */
	float    temperature_in_c; 		/*! current temperature in C at which the pressure was read */
	uint32_t sensor_read_counter;		/*! the total number of pressure sensor readings since the system was started */
	uint64_t last_read_time_in_usecs; 	/*! time stamp indicating the time at which the pressure in this data structure was read */
	uint64_t error_count; 			/*! the total number of errors detected when reading the pressure, since the system was started */
};

class PressureSensor : public I2CDriverObj
{
public:
	PressureSensor(const char device_path); :

	int open();
	int close();

	int readReg(int fildes, uint8_t address, uint8_t *out_buffer, int length);
	int writeReg(int fildes, uint8_t address, uint8_t *in_buffer, int length);

	// Return pressure in pascals
	uint32_t getPressure(deviceHandle handle);

	// Get temperature in degrees C
	float getTemperature(deviceHandle handle);

	int setAltimeter(deviceHandle handle, float altimeter_setting_in_mbars);

	int getSensorData(struct pressure_sensor_data &out_data, bool is_new_data_required);

private:
	pthread_mutex_t	m_lock;
	m_device_path(device_path)
	deviceHandle m_handle;
};