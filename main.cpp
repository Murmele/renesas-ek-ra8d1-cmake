
#include "bsp_api.h"
#include "common_data.h"
#include "printerdemo.h"
#include "slint-ra-fsp.h"
#include "slint.h"

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 854

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
  ui->show();
  slint::run_event_loop();
}
