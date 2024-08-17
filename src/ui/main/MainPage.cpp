#include "MainPage.hpp"

#include <gtkmm/builder.h>

#include <chrono>
#include <format>
#include <iostream>

namespace pie_alarm::ui {

MainPage::MainPage()
    : timer_(std::chrono::milliseconds(1000),
             [this]() { this->UpdateTime(); }) {
  // TODO: figure out how to do paths properly
  auto builder =
      Gtk::Builder::create_from_file("../../src/ui/main/MainPage.ui");

  timeLabel_ = builder->get_widget<Gtk::Label>("TimeLabel");
  set_child(*timeLabel_);

  UpdateTime();
}

void MainPage::UpdateTime() {
  if (timeLabel_) {
    auto const& utcNow = std::chrono::system_clock::now();
    auto const& localNow = std::chrono::current_zone()->to_local(utcNow);
    timeLabel_->set_text(std::format("{:%R}", localNow));
  }
}

}  // namespace pie_alarm::ui