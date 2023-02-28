/*
 * @file power_control_unit.cpp
 * @brief Component emulation of Power Control Unit
 */
#include "power_control_unit.hpp"

PCU::PCU(ClockGenerator* clock_generator) : Component(1, clock_generator) {}

PCU::PCU(int prescaler, ClockGenerator* clock_generator) : Component(prescaler, clock_generator) {}

PCU::~PCU() {}

void PCU::MainRoutine(const int time_count) {
  UNUSED(time_count);

  // double current_ = power_ports_[1]->GetCurrentConsumption_A();
}

int PCU::ConnectPort(const int port_id, const double current_Limit) {
  // The port is already used
  if (power_ports_[port_id] != nullptr) return -1;

  power_ports_[port_id] = new PowerPort(port_id, current_Limit);
  return 0;
}

int PCU::ConnectPort(const int port_id, const double current_Limit, const double minimum_voltage, const double assumed_power_consumption) {
  // The port is already used
  if (power_ports_[port_id] != nullptr) return -1;

  power_ports_[port_id] = new PowerPort(port_id, current_Limit, minimum_voltage, assumed_power_consumption);
  return 0;
}

int PCU::ClosePort(const int port_id) {
  // The port not used
  if (power_ports_[port_id] == nullptr) return -1;

  PowerPort* port = power_ports_.at(port_id);
  delete port;
  power_ports_.erase(port_id);
  return 0;
}

std::string PCU::GetLogHeader() const {
  std::string str_tmp = "";
  return str_tmp;
}

std::string PCU::GetLogValue() const {
  std::string str_tmp = "";
  return str_tmp;
}
