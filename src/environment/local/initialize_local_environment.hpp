/**
 * @file initialize_local_environment.hpp
 * @brief Initialize functions for local environment classes
 */

#ifndef S2E_ENVIRONMENT_LOCAL_INITIALIZE_LOCAL_ENVIRONMENT_HPP_
#define S2E_ENVIRONMENT_LOCAL_INITIALIZE_LOCAL_ENVIRONMENT_HPP_

#include "atmosphere.hpp"
#include "geomagnetic_field.hpp"
#include "solar_radiation_pressure_environment.hpp"

/**
 * @fn InitGeomagneticField
 * @brief Initialize magnetic field of the earth
 * @param [in] initialize_file_path: Path to initialize file
 */
GeomagneticField InitGeomagneticField(std::string initialize_file_path);
/**
 * @fn InitSolarRadiationPressureEnvironment
 * @brief Initialize solar radiation pressure
 * @param [in] initialize_file_path: Path to initialize file
 * @param [in] local_celestial_information: Local celestial information
 */
SolarRadiationPressureEnvironment InitSolarRadiationPressureEnvironment(std::string initialize_file_path,
                                                                        LocalCelestialInformation* local_celestial_information);
/**
 * @fn InitAtmosphere
 * @brief Initialize atmospheric density of the earth
 * @param [in] initialize_file_path: Path to initialize file
 * @param [in] local_celestial_information: Local celestial information
 */
Atmosphere InitAtmosphere(const std::string initialize_file_path, const LocalCelestialInformation* local_celestial_information,
                          const SimulationTime* simulation_time);

#endif  // S2E_ENVIRONMENT_LOCAL_INITIALIZE_LOCAL_ENVIRONMENT_HPP_
