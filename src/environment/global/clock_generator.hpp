/**
 * @file clock_generator.hpp
 * @brief Class to generate clock for classes which have ITickable
 */

#ifndef S2E_ENVIRONMENT_GLOBAL_CLOCK_GENERATOR_HPP_
#define S2E_ENVIRONMENT_GLOBAL_CLOCK_GENERATOR_HPP_

#include <components/base/interface_tickable.hpp>
#include <vector>

#include "simulation_time.hpp"

namespace s2e::environment {

/**
 * @class ClockGenerator
 * @brief Class to generate clock for classes which have ITickable
 */
class ClockGenerator {
 public:
  /**
   * @fn ~ClockGenerator
   * @brief Destructor
   */
  ~ClockGenerator();

  /**
   * @fn RegisterComponent
   * @brief Register component which has ITickable
   * @param [in] tickable: Component class
   */
  void RegisterComponent(components::ITickable* tickable);
  /**
   * @fn RemoveComponent
   * @brief Removed registered component
   * @param [in] tickable: Registered component class
   */
  void RemoveComponent(components::ITickable* tickable);
  /**
   * @fn TickToComponents
   * @brief Execute tick function of all registered components
   */
  void TickToComponents();
  /**
   * @fn UpdateComponents
   * @brief Execute TickToComponents when component update timing
   * @param [in] simulation_time: Simulation time
   */
  void UpdateComponents(const SimulationTime* simulation_time);
  /**
   * @fn ClearTimerCount
   * @brief Clear time count
   */
  inline void ClearTimerCount(void) { timer_count_ = 0; }

 private:
  std::vector<components::ITickable*> components_;  //!< Component list fot tick
  unsigned int timer_count_;                        //!< Timer count TODO: change to long?
};

}  // namespace s2e::environment

#endif  // S2E_ENVIRONMENT_GLOBAL_CLOCK_GENERATOR_HPP_
