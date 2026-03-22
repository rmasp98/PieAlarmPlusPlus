#include "MainPage.hpp"

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>

#include <chrono>
#include <format>
#include <iostream>

namespace pie_alarm::ui {

MainPage::MainPage()
    : builder_(Gtk::Builder::create_from_file("../../src/ui/main/MainPage.ui")),
      timer_(std::chrono::milliseconds(1000),
             [this]() { this->UpdateTime(); }) {
  if (auto alarmButton = builder_->get_widget<Gtk::Button>("AlarmButton")) {
    alarmButton->signal_clicked().connect(
        sigc::mem_fun(*this, &MainPage::OnAlarmClicked));
  }

  if (auto container = builder_->get_widget<Gtk::Box>("Container")) {
    set_child(*container);
  }

  UpdateTime();
}

void MainPage::UpdateTime() {
  if (auto timeLabel = builder_->get_widget<Gtk::Label>("TimeLabel")) {
    auto const& utcNow = std::chrono::system_clock::now();
    auto const& localNow = std::chrono::current_zone()->to_local(utcNow);
    timeLabel->set_text(std::format("{:%R}", localNow));
  }
}

void MainPage::OnAlarmClicked() { RequestPageChange(PageEnum::AlarmList); }

}  // namespace pie_alarm::ui