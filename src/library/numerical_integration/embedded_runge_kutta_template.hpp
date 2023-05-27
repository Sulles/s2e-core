/**
 * @file embedded_runge_kutta_template.hpp
 * @brief Class for Embedded Runge-Kutta method
 */
#ifndef S2E_LIBRARY_NUMERICAL_INTEGRATION_EMBEDDED_RUNGE_KUTTA_TEMPLATE_HPP_
#define S2E_LIBRARY_NUMERICAL_INTEGRATION_EMBEDDED_RUNGE_KUTTA_TEMPLATE_HPP_

#include "embedded_runge_kutta.hpp"

namespace libra {

template <size_t N>
void EmbeddedRungeKutta<N>::Integrate() {
  this->CalcSlope();

  Vector<N> lower_current_state = this->current_state_;   //!< eta in the equation
  Vector<N> higher_current_state = this->current_state_;  //!< eta_hat in the equation
  for (size_t i = 0; i < this->number_of_stages_; i++) {
    lower_current_state = lower_current_state + this->weights_[i] * this->step_width_s_ * this->slope_[i];
    higher_current_state = higher_current_state + higher_order_weights_[i] * this->step_width_s_ * this->slope_[i];
  }

  // Error evaluation
  Vector<N> truncation_error = lower_current_state - higher_current_state;
  local_truncation_error_ = truncation_error.CalcNorm();

  // State update
  this->current_state_ = higher_current_state;
  this->current_time_s_ += this->step_width_s_;
}

template <size_t N>
void EmbeddedRungeKutta<N>::ControlStepWidth(const double error_tolerance) {
  double updated_step_width_s = pow(error_tolerance / local_truncation_error_, 1.0 / ((double)(this->approximation_order_ + 1))) * this->step_width_s_;
  if (updated_step_width_s <= 0.0) return; // TODO: Error handling
  this->step_width_s_ = updated_step_width_s;
}

}  // namespace libra

#endif  // S2E_LIBRARY_NUMERICAL_INTEGRATION_EMBEDDED_RUNGE_KUTTA_TEMPLATE_HPP_
