#ifndef PIEALARM_SIGNAL_H
#define PIEALARM_SIGNAL_H

#include <signal.h>

namespace pie_alarm::utils {

template <typename RETURN, typename... ARGS>
class SignalProxy;

template <typename RETURN, typename... ARGS>
class SignalProxy<RETURN(ARGS...)> {
 public:
  using SignalType = sigc::signal<RETURN(ARGS...)>;
  SignalProxy(const SignalType& signal) : signal_(signal) {}

  using slot_type = sigc::slot<RETURN(ARGS...)>;
  sigc::connection Connect(const slot_type& slot_) {
    return signal_.connect(slot_);
  }
  sigc::connection Connect(slot_type&& slot_) { return signal_.connect(slot_); }

 private:
  SignalType signal_;
};

}  // namespace pie_alarm::utils

#endif  // PIEALARM_SIGNAL_H