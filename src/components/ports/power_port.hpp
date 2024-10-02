/**
 * @file power_port.hpp
 * @brief Class to emulate electrical power port
 */

#ifndef S2E_COMPONENTS_PORTS_POWER_PORT_HPP_
#define S2E_COMPONENTS_PORTS_POWER_PORT_HPP_

#include <string>

namespace s2e::components {

/**
 * @class PowerPort
 * @brief Class to emulate electrical power port
 * @details When the power switch is turned off, the component doesn't work same with the real world.
 */
class PowerPort {
 public:
  /**
   * @fn PowerPort
   * @brief Default Constructor for users who don't want to use this feature
   * @note The power switch is turned on with the default parameters
   */
  PowerPort();
  /**
   * @fn PowerPort
   * @brief Constructor
   * @param [in] port_id: ID of the power port
   * @param [in] current_limit_A: Threshold to detect over current [A]
   */
  PowerPort(const int port_id, const double current_limit_A);
  /**
   * @fn PowerPort
   * @brief Constructor
   * @param [in] port_id: ID of the power port
   * @param [in] current_limit_A: Threshold to detect over current [A]
   * @param [in] minimum_voltage_V: Minimum voltage_V to work the component [V]
   * @param [in] assumed_power_consumption_W: Assumed power consumption of the component [W]
   */
  PowerPort(const int port_id, const double current_limit_A, const double minimum_voltage_V, const double assumed_power_consumption_W);
  /**
   * @fn ~PowerPort
   * @brief Destructor
   */
  ~PowerPort();

  /**
   * @fn Update
   * @brief Update the power state considering the over current
   * @return Power switch state
   */
  bool Update(void);

  // Getters
  /**
   * @fn GetVoltage_V
   * @brief Return the voltage_V of this power line [V]
   */
  inline double GetVoltage_V(void) const { return voltage_V_; }
  /**
   * @fn GetCurrentConsumption_A
   * @brief Return the current consumption of this power line [A]
   */
  inline double GetCurrentConsumption_A() const { return current_consumption_A_; }
  /**
   * @fn GetAssumedPowerConsumption_W
   * @brief Return the assumed power consumption of this power line [W]
   */
  inline double GetAssumedPowerConsumption_W() const { return assumed_power_consumption_W_; }
  /**
   * @fn GetIsOn
   * @brief Return the power switch state
   */
  inline bool GetIsOn() const { return is_on_; }

  // Setters
  /**
   * @fn SetVoltage_V
   * @brief Set voltage_V to control the power switch state
   * @return Power switch state
   */
  bool SetVoltage_V(const double voltage_V);
  /**
   * @fn SetAssumedPowerConsumption_W
   * @brief Set assumed power consumption [W]
   * @note Users can use this function to change the power consumption of the component depending on the execution state.
   */
  inline void SetAssumedPowerConsumption_W(const double power_W) { assumed_power_consumption_W_ = power_W; }
  /**
   * @fn SetMinimumVoltage_V
   * @brief Set minimum voltage to work the component [V]
   */
  inline void SetMinimumVoltage_V(const double minimum_voltage_V) { minimum_voltage_V_ = minimum_voltage_V; }
  /**
   * @fn SetCurrentLimit_A
   * @brief Set threshold to detect over current [A]
   */
  inline void SetCurrentLimit_A(const double current_limit_A) { current_limit_A_ = current_limit_A; }

  // Others
  /**
   * @fn AddAssumedPowerConsumption_W
   * @brief Add assumed power consumption [W] to emulate power line which has multiple loads
   */
  inline void AddAssumedPowerConsumption_W(const double power_W) { assumed_power_consumption_W_ += power_W; }
  /**
   * @fn SubtractAssumedPowerConsumption_W
   * @brief Subtract assumed power consumption [W] to emulate power line which has multiple loads
   */
  void SubtractAssumedPowerConsumption_W(const double power_W);
  /**
   * @fn InitializeWithInitializeFile
   * @brief Initialize PowerPort class with initialize file
   */
  void InitializeWithInitializeFile(const std::string file_name);

 private:
  // PCU setting parameters
  const int kPortId;        //!< ID of the power port
  double current_limit_A_;  //!< Threshold to detect over current [A]

  // Components setting parameters
  double minimum_voltage_V_;            //!< Minimum voltage_V to work the component [V]
  double assumed_power_consumption_W_;  //!< Assumed power consumption of the component [W]

  double voltage_V_;              //!< Voltage of the power line[V]
  double current_consumption_A_;  //!< Current consumption calculated by I = P/V[A]
  bool is_on_;                    //!< Power switch state

  /**
   * @fn Initialize
   * @brief Initialize function
   */
  void Initialize(void);
};

}  // namespace s2e::components

#endif  // S2E_COMPONENTS_PORTS_POWER_PORT_HPP_
