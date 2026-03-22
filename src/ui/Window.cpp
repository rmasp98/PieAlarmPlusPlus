#include "Window.hpp"

#include <gtkmm/stylecontext.h>

#include <iostream>

#include "alarm/AlarmListPage.hpp"
#include "main/MainPage.hpp"

namespace pie_alarm::ui {

Window::Window() : cssProvider_(Gtk::CssProvider::create()) {
  set_title("Basic application");
  set_default_size(800, 480);

  Gtk::StyleContext::add_provider_for_display(
      get_display(), cssProvider_, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  // TODO: figure out how to do paths properly
  cssProvider_->load_from_path("../../src/ui/styles/default.css");

  OnPageChangeRequested(PageEnum::Main);
}

void Window::OnButtonClicked() {
  const PageEnum currentPage = pageHistory_.top();
  pageHistory_.pop();
  const PageEnum previousPage = pageHistory_.top();

  if (!SwitchPage(previousPage)) {
    // If the page switch fails, then add current page back to the history
    pageHistory_.push(currentPage);
  }
}

void Window::OnPageChangeRequested(PageEnum newPage) {
  if (SwitchPage(newPage)) {
    pageHistory_.push(newPage);
  }
}

bool Window::SwitchPage(PageEnum newPage) {
  currentPage_ = CreatePage(newPage);
  if (currentPage_) {
    currentPage_->SignalBack().Connect(
        sigc::mem_fun(*this, &Window::OnButtonClicked));
    currentPage_->SignalChangePage().Connect(
        sigc::mem_fun(*this, &Window::OnPageChangeRequested));

    set_child(*currentPage_);
    return true;
  }
  return false;
}

std::unique_ptr<Page> Window::CreatePage(PageEnum page) {
  switch (page) {
    case PageEnum::Main:
      return std::make_unique<MainPage>();
    case PageEnum::AlarmList:
      return std::make_unique<AlarmListPage>();
  }
  return nullptr;
}

}  // namespace pie_alarm::ui