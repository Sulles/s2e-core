/**
 * @file disturbances.cpp
 * @brief Class to manage all disturbances
 */

#include "disturbances.hpp"

#include <library/initialize/initialize_file_access.hpp>

#include "air_drag.hpp"
#include "geopotential.hpp"
#include "gravity_gradient.hpp"
#include "initialize_disturbances.hpp"
#include "magnetic_disturbance.hpp"
#include "solar_radiation_pressure_disturbance.hpp"
#include "third_body_gravity.hpp"

Disturbances::Disturbances(const SimulationConfiguration* simulation_configuration, const int spacecraft_id, const Structure* structure,
                           const GlobalEnvironment* global_environment) {
  InitializeInstances(simulation_configuration, spacecraft_id, structure, global_environment);
  InitializeForceAndTorque();
  InitializeAcceleration();
}

Disturbances::~Disturbances() {
  for (auto dist : disturbances_list_) {
    delete dist;
  }

  for (auto acc_dist : acceleration_disturbances_list_) {
    delete acc_dist;
  }
}

void Disturbances::Update(const LocalEnvironment& local_environment, const Dynamics& dynamics, const SimulationTime* simulation_time) {
  // Update disturbances that depend on the attitude (and the position)
  if (simulation_time->GetAttitudePropagateFlag()) {
    InitializeForceAndTorque();
    for (auto dist : disturbances_list_) {
      dist->UpdateIfEnabled(local_environment, dynamics);
      total_torque_b_Nm_ += dist->GetTorque_b_Nm();
      total_force_b_N_ += dist->GetForce_b_N();
    }
  }
  // Update disturbances that depend only on the position
  if (simulation_time->GetOrbitPropagateFlag()) {
    InitializeAcceleration();
    for (auto acc_dist : acceleration_disturbances_list_) {
      acc_dist->UpdateIfEnabled(local_environment, dynamics);
      total_acceleration_i_m_s2_ += acc_dist->GetAcceleration_i_m_s2();
    }
  }
}

void Disturbances::LogSetup(Logger& logger) {
  for (auto dist : disturbances_list_) {
    logger.AddLogList(dist);
  }
  for (auto acc_dist : acceleration_disturbances_list_) {
    logger.AddLogList(acc_dist);
  }
  logger.CopyFileToLogDirectory(initialize_file_name_);
}

void Disturbances::InitializeInstances(const SimulationConfiguration* simulation_configuration, const int spacecraft_id, const Structure* structure,
                                       const GlobalEnvironment* global_environment) {
  IniAccess ini_access = IniAccess(simulation_configuration->spacecraft_file_list_[spacecraft_id]);
  initialize_file_name_ = ini_access.ReadString("SETTING_FILES", "disturbance_file");

  GravityGradient* gg_dist = new GravityGradient(
      InitGravityGradient(initialize_file_name_, global_environment->GetCelestialInformation().GetCenterBodyGravityConstant_m3_s2()));
  disturbances_list_.push_back(gg_dist);

  SolarRadiationPressureDisturbance* srp_dist = new SolarRadiationPressureDisturbance(InitSolarRadiationPressureDisturbance(
      initialize_file_name_, structure->GetSurfaces(), structure->GetKinematicsParameters().GetCenterOfGravity_b_m()));
  disturbances_list_.push_back(srp_dist);

  ThirdBodyGravity* third_body_gravity =
      new ThirdBodyGravity(InitThirdBodyGravity(initialize_file_name_, simulation_configuration->initialize_base_file_name_));
  acceleration_disturbances_list_.push_back(third_body_gravity);

  if (global_environment->GetCelestialInformation().GetCenterBodyName() != "EARTH") return;
  // Earth only disturbances (TODO: implement disturbances for other center bodies)
  AirDrag* air_dist =
      new AirDrag(InitAirDrag(initialize_file_name_, structure->GetSurfaces(), structure->GetKinematicsParameters().GetCenterOfGravity_b_m()));
  disturbances_list_.push_back(air_dist);

  MagneticDisturbance* mag_dist = new MagneticDisturbance(InitMagneticDisturbance(initialize_file_name_, structure->GetResidualMagneticMoment()));
  disturbances_list_.push_back(mag_dist);

  Geopotential* geopotential = new Geopotential(InitGeoPotential(initialize_file_name_));
  acceleration_disturbances_list_.push_back(geopotential);
}

void Disturbances::InitializeForceAndTorque() {
  total_torque_b_Nm_ = Vector<3>(0.0);
  total_force_b_N_ = Vector<3>(0.0);
}

void Disturbances::InitializeAcceleration() { total_acceleration_i_m_s2_ = Vector<3>(0.0); }
