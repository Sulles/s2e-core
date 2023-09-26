/**
 * @file initialize_magnetometer.cpp
 * @brief Initialize functions for magnetometer
 */
#include "initialize_magnetometer.hpp"

#include "../../base/sensor.hpp"
#include "library/initialize/initialize_file_access.hpp"

Magnetometer InitMagnetometer(ClockGenerator* clock_generator, int sensor_id, const std::string file_name, double component_step_time_s,
                              const GeomagneticField* geomagnetic_field) {
  IniAccess magsensor_conf(file_name);
  const char* sensor_name = "MAGNETOMETER_";
  const std::string section_name = sensor_name + std::to_string(static_cast<long long>(sensor_id));
  const char* MSSection = section_name.c_str();

  int prescaler = magsensor_conf.ReadInt(MSSection, "prescaler");
  if (prescaler <= 1) prescaler = 1;

  libra::Quaternion quaternion_b2c;
  magsensor_conf.ReadQuaternion(MSSection, "quaternion_b2c", quaternion_b2c);

  // Sensor
  Sensor<kMagnetometerDimension> sensor_base =
      ReadSensorInformation<kMagnetometerDimension>(file_name, component_step_time_s * (double)(prescaler), MSSection, "nT");

  Magnetometer magsensor(prescaler, clock_generator, sensor_base, sensor_id, quaternion_b2c, geomagnetic_field);
  return magsensor;
}

Magnetometer InitMagnetometer(ClockGenerator* clock_generator, PowerPort* power_port, int sensor_id, const std::string file_name,
                              double component_step_time_s, const GeomagneticField* geomagnetic_field) {
  IniAccess magsensor_conf(file_name);
  const char* sensor_name = "MAGNETOMETER_";
  const std::string section_name = sensor_name + std::to_string(static_cast<long long>(sensor_id));
  const char* MSSection = section_name.c_str();

  int prescaler = magsensor_conf.ReadInt(MSSection, "prescaler");
  if (prescaler <= 1) prescaler = 1;

  libra::Quaternion quaternion_b2c;
  magsensor_conf.ReadQuaternion(MSSection, "quaternion_b2c", quaternion_b2c);

  // Sensor
  Sensor<kMagnetometerDimension> sensor_base =
      ReadSensorInformation<kMagnetometerDimension>(file_name, component_step_time_s * (double)(prescaler), MSSection, "nT");

  // PowerPort
  power_port->InitializeWithInitializeFile(file_name);

  Magnetometer magsensor(prescaler, clock_generator, power_port, sensor_base, sensor_id, quaternion_b2c, geomagnetic_field);
  return magsensor;
}
