
#include "bsp_api.h"
#include "common_data.h"
#include "printerdemo.h"
#include "slint-ra-fsp.h"
#include "slint.h"

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 854

void init_printer_demo(MainWindow *printer_demo) {
  auto printer_queue = std::make_shared<slint::VectorModel<PrinterQueueItem>>();
  auto default_queue = printer_demo->global<PrinterQueue>().get_printer_queue();
  for (int i = 0; i < default_queue->row_count(); ++i) {
    printer_queue->push_back(*default_queue->row_data(i));
  }
  printer_demo->global<PrinterQueue>().set_printer_queue(printer_queue);

  printer_demo->global<PrinterQueue>().on_start_job(
      [=](slint::SharedString name) {
        std::time_t now = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now());
        char time_buf[100] = {0};
        std::strftime(time_buf, sizeof(time_buf), "%H:%M:%S %d/%m/%Y",
                      std::localtime(&now));
        PrinterQueueItem item;
        item.status = "waiting";
        item.progress = 0;
        item.title = std::move(name);
        item.owner = "info@slint.dev";
        item.pages = 1;
        item.size = "100kB";
        item.submission_date = time_buf;
        printer_queue->push_back(item);
      });

  printer_demo->global<PrinterQueue>().on_cancel_job(
      [=](int index) { printer_queue->erase(int(index)); });

  static slint::Timer printer_queue_progress_timer(
      std::chrono::seconds(1), [=]() {
        if (printer_queue->row_count() > 0) {
          auto top_item = *printer_queue->row_data(0);
          top_item.progress += 1;
          if (top_item.progress > 100) {
            printer_queue->erase(0);
          } else {
            top_item.status = "printing";
            printer_queue->set_row_data(0, top_item);
          }
        } else {
          for (int i = 0; i < default_queue->row_count(); ++i) {
            printer_queue->push_back(*default_queue->row_data(i));
          }
        }
      });
}

extern "C" void slint_main() {
  // Setup configuration
  SlintPlatformConfiguration config;
  config.size = slint::PhysicalSize({DISPLAY_WIDTH, DISPLAY_HEIGHT});
  config.framebuffer = fb_background;
  config.framebuffer_size = sizeof(fb_background);
  config.display_cfg = &g_display0_cfg;
  config.rotation =
      slint::platform::SoftwareRenderer::RenderingRotation::Rotate270;
  slint_ra8d1_init(config);

  auto ui = MainWindow::create();
  init_printer_demo(&*ui);
  ui->show();
  slint::run_event_loop();
}
