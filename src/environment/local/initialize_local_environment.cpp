/**
 * @file initialize_local_environment.cpp
 * @brief Initialize functions for local environment classes
 */

#include "initialize_local_environment.hpp"

#include <library/initialize/initialize_file_access.hpp>
#include <string>

#define CALC_LABEL "calculation"
#define LOG_LABEL "logging"

GeomagneticField InitGeomagneticField(std::string initialize_file_path) {
  auto conf = IniAccess(initialize_file_path);
  const char* section = "MAGNETIC_FIELD_ENVIRONMENT";

  std::string fname = conf.ReadString(section, "coefficient_file");
  double mag_rwdev = conf.ReadDouble(section, "magnetic_field_random_walk_standard_deviation_nT");
  double mag_rwlimit = conf.ReadDouble(section, "magnetic_field_random_walk_limit_nT");
  double mag_wnvar = conf.ReadDouble(section, "magnetic_field_white_noise_standard_deviation_nT");

  GeomagneticField geomagnetic_field(fname, mag_rwdev, mag_rwlimit, mag_wnvar);
  geomagnetic_field.IsCalcEnabled = conf.ReadEnable(section, CALC_LABEL);
  geomagnetic_field.is_log_enabled_ = conf.ReadEnable(section, LOG_LABEL);

  return geomagnetic_field;
}

SolarRadiationPressureEnvironment InitSolarRadiationPressureEnvironment(std::string initialize_file_path,
                                                                        LocalCelestialInformation* local_celestial_information) {
  auto conf = IniAccess(initialize_file_path);
  const char* section = "SOLAR_RADIATION_PRESSURE_ENVIRONMENT";

  SolarRadiationPressureEnvironment srp_env(local_celestial_information);
  srp_env.IsCalcEnabled = conf.ReadEnable(section, CALC_LABEL);
  srp_env.is_log_enabled_ = conf.ReadEnable(section, LOG_LABEL);

  return srp_env;
}

Atmosphere InitAtmosphere(const std::string initialize_file_path, const LocalCelestialInformation* local_celestial_information,
                          const SimulationTime* simulation_time) {
  auto conf = IniAccess(initialize_file_path);
  const char* section = "ATMOSPHERE";
  double f107_threshold = 50.0;
  double f107_default = 150.0;

  std::string model = conf.ReadString(section, "model");
  std::string table_path = conf.ReadString(section, "nrlmsise00_table_file");
  double rho_stddev = conf.ReadDouble(section, "air_density_standard_deviation");
  bool is_manual_param_used = conf.ReadEnable(section, "is_manual_parameter_used");
  double manual_daily_f107 = conf.ReadDouble(section, "manual_daily_f107");
  if (manual_daily_f107 < f107_threshold) {
    std::cerr << "Daily F10.7 may be too low. It is set as 150.0 in this simulation. "
                 "Check [ATMOSPHERE] section in LocalEnvironment.ini."
              << std::endl;
    manual_daily_f107 = f107_default;
  }
  double manual_average_f107 = conf.ReadDouble(section, "manual_average_f107");
  if (manual_average_f107 < f107_threshold) {
    std::cerr << "Average F10.7 may be too low. It is set as 150.0 in this "
                 "simulation. Check [ATMOSPHERE] section in LocalEnvironment.ini."
              << std::endl;
    manual_average_f107 = f107_default;
  }
  double manual_ap = conf.ReadDouble(section, "manual_ap");

  Atmosphere atmosphere(model, table_path, rho_stddev, is_manual_param_used, manual_daily_f107, manual_average_f107, manual_ap,
                        local_celestial_information, simulation_time);
  atmosphere.SetCalcFlag(conf.ReadEnable(section, CALC_LABEL));
  atmosphere.is_log_enabled_ = conf.ReadEnable(section, LOG_LABEL);

  return atmosphere;
}
