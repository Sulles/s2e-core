/**
 * @file example_i2c_target_for_hils.cpp
 * @brief Example of component emulation for I2C target side communication in HILS environment
 */

#include "example_i2c_target_for_hils.hpp"

namespace s2e::components {

ExampleI2cTargetForHils::ExampleI2cTargetForHils(const int prescaler, environment::ClockGenerator* clock_generator, const int sils_port_id,
                                                 unsigned char i2c_address, OnBoardComputer* obc, const unsigned int hils_port_id,
                                                 simulation::HilsPortManager* hils_port_manager)
    : Component(prescaler, clock_generator), I2cTargetCommunicationWithObc(sils_port_id, hils_port_id, i2c_address, obc, hils_port_manager) {}

ExampleI2cTargetForHils::~ExampleI2cTargetForHils() {}

void ExampleI2cTargetForHils::MainRoutine(const int time_count) {
  UNUSED(time_count);

  // update telemetry data
  const unsigned char kTlmSize = 5;
  unsigned char tlm[kTlmSize] = {0};
  for (unsigned char i = 0; i < kTlmSize; i++) {
    tlm[i] = 'A' + tlm_counter_ + i;
  }
  WriteRegister(0, tlm, kTlmSize);
  tlm_counter_++;
  if (tlm_counter_ > kNumAlphabet - kTlmSize) {
    tlm_counter_ = 0;  // ABCDE, ..., VWXYZ, ABCDE, ...
  }

  ReceiveCommand();
  int additional_frame_num = kStoredFrameSize - GetStoredFrameCounter();
  if (additional_frame_num <= 0) return;

  // store telemetry in converter up to kStoredFrameSize
  for (int i = 0; i < additional_frame_num; i++) {
    SendTelemetry(kTlmSize);
    std::cout << "I2C Target sends to converter: " << tlm[0] << tlm[1] << tlm[2] << tlm[3] << tlm[4] << std::endl;
  }

  return;
}

}  // namespace s2e::components
