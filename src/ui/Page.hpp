#ifndef PIEALARM_PAGE_HPP
#define PIEALARM_PAGE_HPP

#include <gtkmm/frame.h>

#include "PageEnum.hpp"
#include "utils/Signal.hpp"

namespace pie_alarm::ui {

class Page : public Gtk::Frame {
 public:
  Page() = default;
  virtual ~Page() = default;

  using SignalEvent = pie_alarm::utils::SignalProxy<void()>;
  SignalEvent SignalBack() { return SignalEvent(backSignal_); }

  using SignalChangePageEvent = pie_alarm::utils::SignalProxy<void(PageEnum)>;
  SignalChangePageEvent SignalChangePage() {
    return SignalChangePageEvent(changePageSignal_);
  }

 protected:
  void TriggerBack() { backSignal_(); }
  void RequestPageChange(PageEnum NewPage) { changePageSignal_(NewPage); }

 private:
  sigc::signal<void()> backSignal_;
  sigc::signal<void(PageEnum)> changePageSignal_;
};

}  // namespace pie_alarm::ui

#endif  // PIEALARM_MAINPAGE_HPP