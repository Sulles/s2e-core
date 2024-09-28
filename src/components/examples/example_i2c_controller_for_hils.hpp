/**
 * @file example_i2c_controller_for_hils.hpp
 * @brief Example of component emulation for I2C controller side communication in HILS environment
 */

#ifndef S2E_COMPONENTS_EXAMPLES_EXAMPLE_I2C_CONTROLLER_FOR_HILS_HPP_
#define S2E_COMPONENTS_EXAMPLES_EXAMPLE_I2C_CONTROLLER_FOR_HILS_HPP_

#include <vector>

#include "../base/component.hpp"
#include "../base/i2c_controller.hpp"

namespace s2e::components {

/**
 * @class ExampleI2cControllerForHils
 * @brief Example of component emulation for I2C controller side communication in HILS environment
 * @details Supposed to be used in connection with the following I2C-USB Controller converter
 *          SC18IM700 Data Sheet: https://www.nxp.com/docs/en/data-sheet/SC18IM700.pdf
 *          telemetry size = 5 bytes(ASCII)
 */
class ExampleI2cControllerForHils : public Component, public I2cController {
 public:
  /**
   * @fn ExampleI2cControllerForHils
   * @brief Constructor
   * @param [in] prescaler: Frequency scale factor for update
   * @param [in] clock_generator: Clock generator
   * @param [in] hils_port_id: ID of HILS communication port
   * @param [in] baud_rate: Baud rate of HILS communication port
   * @param [in] tx_buffer_size: TX (Controller to Target) buffer size
   * @param [in] rx_buffer_size: RX (Target to Controller) buffer size
   * @param [in] hils_port_manager: HILS port manager
   */
  ExampleI2cControllerForHils(const int prescaler, environment::ClockGenerator* clock_generator, const unsigned int hils_port_id,
                              const unsigned int baud_rate, const unsigned int tx_buffer_size, const unsigned int rx_buffer_size,
                              simulation::HilsPortManager* hils_port_manager);
  /**
   * @fn ~ExampleI2cControllerForHils
   * @brief Destructor
   */
  ~ExampleI2cControllerForHils();

 protected:
  // Override functions for Component
  /**
   * @fn MainRoutine
   * @brief Main routine to receive command and send telemetry
   */
  void MainRoutine(const int time_count);

 private:
  /**
   * @fn RequestTlm
   * @brief Send command to the I2C-USB converter to send telemetry request command to the I2C Target
   */
  void RequestTlm();
  /**
   * @fn Receive
   * @brief Send command to the I2C-USB converter to read received data
   */
  void Receive();

  static const uint8_t kCmdHeader_ = 0x53;  //!< 'S' Header for SC18IM700
  static const uint8_t kReadCmd_ = 0x45;    //!< Read command for SC18IM700
  static const uint8_t kWriteCmd_ = 0x44;   //!< Write command for SC18IM700
  static const uint8_t kCmdFooter_ = 0x50;  //!< 'P' Footer for SC18IM700
};

}  // namespace s2e::components

#endif  // S2E_COMPONENTS_EXAMPLES_EXAMPLE_I2C_CONTROLLER_FOR_HILS_HPP_
