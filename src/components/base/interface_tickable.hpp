/**
 * @file interface_tickable.hpp
 * @brief Interface class for time update of components
 */

#ifndef S2E_COMPONENTS_BASE_CLASSES_INTERFACE_TICKABLE_HPP_
#define S2E_COMPONENTS_BASE_CLASSES_INTERFACE_TICKABLE_HPP_

namespace s2e::components {

/**
 * @class ITickable
 * @brief Interface class for time update of components
 */
class ITickable {
 public:
  /**
   * @fn Tick
   * @brief Pure virtual function to update clock of components
   */
  virtual void Tick(const unsigned int count) = 0;
  /**
   * @fn FastTick
   * @brief Pure virtual function to update clock faster than the base component update period of components
   * @note Usec ase: Calculate high-frequency disturbances
   */
  virtual void FastTick(const unsigned int fast_count) = 0;

  // Whether or not high-frequency disturbances need to be calculated
  /**
   * @fn GetNeedsFastUpdate
   * @brief Return fast update flag
   */
  inline bool GetNeedsFastUpdate() { return needs_fast_update_; }
  /**
   * @fn SetNeedsFastUpdate
   * @brief Set fast update flag
   */
  inline void SetNeedsFastUpdate(const bool need_fast_update) { needs_fast_update_ = need_fast_update; }

 protected:
  bool needs_fast_update_ = false;  //!< Whether or not high-frequency disturbances need to be calculated
};

}  // namespace s2e::components

#endif  // S2E_COMPONENTS_BASE_CLASSES_INTERFACE_TICKABLE_HPP_
