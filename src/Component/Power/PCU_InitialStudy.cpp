﻿#include "PCU_InitialStudy.h"

#include <Component/Power/CsvScenarioInterface.h>
#include <Environment/Global/ClockGenerator.h>

#include <cmath>

PCU_InitialStudy::PCU_InitialStudy(const int prescaler, ClockGenerator* clock_gen, const std::vector<SAP*> saps, BAT* bat, double compo_step_time)
    : ComponentBase(prescaler, clock_gen),
      saps_(saps),
      bat_(bat),
      cc_charge_current_(bat->GetCCChargeCurrent()),
      cv_charge_voltage_(bat->GetCVChargeVoltage()),
      compo_step_time_(compo_step_time) {
  bus_voltage_ = 0.0;
  power_consumption_ = 0.0;
}

PCU_InitialStudy::PCU_InitialStudy(ClockGenerator* clock_gen, const std::vector<SAP*> saps, BAT* bat)
    : ComponentBase(10, clock_gen),
      saps_(saps),
      bat_(bat),
      cc_charge_current_(bat->GetCCChargeCurrent()),
      cv_charge_voltage_(bat->GetCVChargeVoltage()),
      compo_step_time_(0.1) {
  bus_voltage_ = 0.0;
  power_consumption_ = 0.0;
}

PCU_InitialStudy::~PCU_InitialStudy() {}

std::string PCU_InitialStudy::GetLogHeader() const {
  std::string str_tmp = "";
  str_tmp += WriteScalar("power_consumption", "W");
  str_tmp += WriteScalar("bus_voltage", "V");
  return str_tmp;
}

std::string PCU_InitialStudy::GetLogValue() const {
  std::string str_tmp = "";
  str_tmp += WriteScalar(power_consumption_);
  str_tmp += WriteScalar(bus_voltage_);
  return str_tmp;
}

void PCU_InitialStudy::MainRoutine(int time_count) {
  double time_query = compo_step_time_ * time_count;
  power_consumption_ =
      CalcPowerConsumption(time_query);  // 時間はSimTimeから持ってきたほうが良い？そもそもtime_countがintなのでオーバーフローする可能性あり
  UpdateChargeCurrentAndBusVoltage();
  for (auto sap : saps_) {
    sap->SetVoltage(16.0);  // MPPTを想定
  }
}

double PCU_InitialStudy::CalcPowerConsumption(double time_query) const {
  if (CsvScenarioInterface::UseCsvPowerConsumption()) {
    return CsvScenarioInterface::GetPowerConsumption(time_query);
  }
  else {
    //仮の実装．
    // if (time_in_sec % 3600 < 600) {
    //   return 10.0;
    // } else if (time_in_sec % 3600 < 2000) {
    //   return 5.0;
    // } else if (time_in_sec % 3600 < 2500) {
    //   return 20.0;
    // } else {
    //   return 5.0;
    // }
    return 5.0;
  }
}

void PCU_InitialStudy::UpdateChargeCurrentAndBusVoltage() {
  double bat_voltage = bat_->GetBatVoltage();
  const double bat_resistance = bat_->GetBatResistance();
  double power_generation = 0.0;
  for (auto sap : saps_) {
    power_generation += sap->GetPowerGeneration();
  }
  double current_temp =
      (-bat_voltage + std::sqrt(bat_voltage * bat_voltage + 4.0 * bat_resistance * (power_generation - power_consumption_))) / (2.0 * bat_resistance);
  if (current_temp >= cc_charge_current_) {
    if (bat_voltage + cc_charge_current_ * bat_resistance < cv_charge_voltage_) {
      // CC Charge
      bat_->SetChargeCurrent(cc_charge_current_);
      bus_voltage_ = bat_voltage + bat_resistance * cc_charge_current_;
    } else {
      // CV Charge
      bat_->SetChargeCurrent((cv_charge_voltage_ - bat_voltage) / bat_resistance);
      bus_voltage_ = bat_voltage + bat_resistance * (cv_charge_voltage_ - bat_voltage) / bat_resistance;
    }
  } else {
    if (bat_voltage + current_temp * bat_resistance < cv_charge_voltage_) {
      // 自然に充電or放電
      bat_->SetChargeCurrent(current_temp);
      bus_voltage_ = bat_voltage + bat_resistance * current_temp;
    } else {
      // CV Charge
      bat_->SetChargeCurrent((cv_charge_voltage_ - bat_voltage) / bat_resistance);
      bus_voltage_ = bat_voltage + bat_resistance * (cv_charge_voltage_ - bat_voltage) / bat_resistance;
    }
  }
}
