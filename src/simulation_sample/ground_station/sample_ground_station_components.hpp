/**
 * @file sample_ground_station_components.hpp
 * @brief An example of ground station related components list
 */

#ifndef S2E_SIMULATION_SAMPLE_GROUND_STATION_SAMPLE_GROUND_STATION_COMPONENTS_HPP_
#define S2E_SIMULATION_SAMPLE_GROUND_STATION_SAMPLE_GROUND_STATION_COMPONENTS_HPP_

#include <components/real/communication/antenna.hpp>
#include <components/real/communication/ground_station_calculator.hpp>

/**
 * @class SampleGsComponents
 * @brief An example of ground station related components list class
 */
class SampleGsComponents {
 public:
  /**
   * @fn SampleGsComponents
   * @brief Constructor
   */
  SampleGsComponents(const SimulationConfiguration* configuration);
  /**
   * @fn ~SampleGsComponents
   * @brief Destructor
   */
  ~SampleGsComponents();
  /**
   * @fn CompoLogSetUp
   * @brief Log setup for ground station components
   */
  void CompoLogSetUp(Logger& logger);

  // Getter
  /**
   * @fn GetAntenna
   * @brief Return antenna
   */
  inline Antenna* GetAntenna() const { return antenna_; }
  /**
   * @fn GetGsCalculator
   * @brief Return ground station calculator
   */
  inline GroundStationCalculator* GetGsCalculator() const { return gs_calculator_; }

 private:
  Antenna* antenna_;                              //!< Antenna on ground station
  GroundStationCalculator* gs_calculator_;        //!< Ground station calculation algorithm
  const SimulationConfiguration* configuration_;  //!< Simulation setting
};

#endif  // S2E_SIMULATION_SAMPLE_GROUND_STATION_SAMPLE_GROUND_STATION_COMPONENTS_HPP_
