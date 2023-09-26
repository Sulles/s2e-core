/**
 * @file initialize_gyro_sensor.cpp
 * @brief Initialize functions for gyro sensor
 */
#include "initialize_gyro_sensor.hpp"

#include <library/initialize/initialize_file_access.hpp>

#include "../../base/sensor.hpp"

GyroSensor InitGyroSensor(ClockGenerator* clock_generator, int sensor_id, const std::string file_name, double component_step_time_s,
                          const Dynamics* dynamics) {
  IniAccess gyro_conf(file_name);
  const char* sensor_name = "GYRO_SENSOR_";
  const std::string section_name = sensor_name + std::to_string(static_cast<long long>(sensor_id));
  const char* GSection = section_name.c_str();

  libra::Quaternion quaternion_b2c;
  gyro_conf.ReadQuaternion(GSection, "quaternion_b2c", quaternion_b2c);
  int prescaler = gyro_conf.ReadInt(GSection, "prescaler");
  if (prescaler <= 1) prescaler = 1;

  // Sensor
  Sensor<kGyroDimension> sensor_base =
      ReadSensorInformation<kGyroDimension>(file_name, component_step_time_s * (double)(prescaler), GSection, "rad_s");

  GyroSensor gyro(prescaler, clock_generator, sensor_base, sensor_id, quaternion_b2c, dynamics);

  return gyro;
}

GyroSensor InitGyroSensor(ClockGenerator* clock_generator, PowerPort* power_port, int sensor_id, const std::string file_name,
                          double component_step_time_s, const Dynamics* dynamics) {
  IniAccess gyro_conf(file_name);
  const char* sensor_name = "GYRO_SENSOR_";
  const std::string section_name = sensor_name + std::to_string(static_cast<long long>(sensor_id));
  const char* GSection = section_name.c_str();

  libra::Quaternion quaternion_b2c;
  gyro_conf.ReadQuaternion(GSection, "quaternion_b2c", quaternion_b2c);
  int prescaler = gyro_conf.ReadInt(GSection, "prescaler");
  if (prescaler <= 1) prescaler = 1;

  // Sensor
  Sensor<kGyroDimension> sensor_base =
      ReadSensorInformation<kGyroDimension>(file_name, component_step_time_s * (double)(prescaler), GSection, "rad_s");

  // PowerPort
  power_port->InitializeWithInitializeFile(file_name);

  GyroSensor gyro(prescaler, clock_generator, power_port, sensor_base, sensor_id, quaternion_b2c, dynamics);
  return gyro;
}
