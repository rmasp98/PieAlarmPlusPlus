#ifndef PIEALARM_ACTIVITY_HPP
#define PIEALARM_ACTIVITY_HPP

#include "nlohmann/json.hpp"
#include "utils/Signal.hpp"

namespace pie_alarm::alarm {

class Activity {
 public:
  virtual ~Activity() = default;

  virtual void Start() = 0;
  virtual void Stop(bool ignoreCallbackComplete = false) {
    if (!ignoreCallbackComplete) completeSignal_.emit();
  }

  virtual nlohmann::json Serialise() const = 0;

  using SignalEvent = pie_alarm::utils::SignalProxy<void()>;
  SignalEvent SignalComplete() { return SignalEvent(completeSignal_); }

 protected:
  sigc::signal<void()> completeSignal_;
};

// class MusicActivity : public Activity {
//  public:
//   MusicActivity() = default;
//   virtual ~MusicActivity() = default;

//   nlohmann::json Serialise() const override { return {}; };
//   void Start() override {}
//   void Stop() override {}
// };

}  // namespace pie_alarm::alarm

#endif  // PIEALARM_ACTIVITY_HPP