// Copyright © Your Company <your@email>
// SPDX-License-Identifier: Your-License

#pragma once

#include <slint-platform.h>
#include <type_traits>

#include "bsp_api.h"
#include "r_glcdc.h"
#include "read_GT911_touch.h"
#include "touch_GT911.h"
// FIXME: this shouldn't be declared here.
extern const rm_comms_instance_t g_comms_i2c_device0;
extern rm_comms_i2c_instance_ctrl_t g_comms_i2c_device0_ctrl;
extern const rm_comms_cfg_t g_comms_i2c_device0_cfg;

struct SlintPlatformConfiguration {
  /// The size of the screen in pixels.
  slint::PhysicalSize size;
  /// Frame buffer pointer. Must be aligned properly for the GLCDC
  void *framebuffer;
  /// Frame buffer size in bytes
  size_t framebuffer_size;
  /// GLCDC configuration pointer
  const display_cfg_t *display_cfg;
  slint::platform::SoftwareRenderer::RenderingRotation rotation =
      slint::platform::SoftwareRenderer::RenderingRotation::NoRotation;
};

namespace slint::private_api {

struct Ra8d1WindowAdapter : public slint::platform::WindowAdapter {
  slint::platform::SoftwareRenderer m_renderer{
      slint::platform::SoftwareRenderer::RepaintBufferType::SwappedBuffers};
  bool needs_redraw = true;
  const slint::PhysicalSize m_size;

  explicit Ra8d1WindowAdapter(slint::PhysicalSize size) : m_size(size) {}

  slint::platform::AbstractRenderer &renderer() override { return m_renderer; }
  slint::PhysicalSize size() override { return m_size; }
  void request_redraw() override { needs_redraw = true; }
};

struct Ra8d1SlintPlatform : public slint::platform::Platform {
  using Pixel = slint::platform::Rgb565Pixel;

  Ra8d1WindowAdapter *m_window = nullptr;
  const slint::PhysicalSize size;
  slint::platform::SoftwareRenderer::RenderingRotation rotation;
  std::span<Pixel> buffer1;
  std::span<Pixel> buffer2;
  glcdc_instance_ctrl_t m_display_ctrl{};

  Ra8d1SlintPlatform(
      slint::PhysicalSize size,
      slint::platform::SoftwareRenderer::RenderingRotation rotation,
      std::span<Pixel> buffer1, std::span<Pixel> buffer2,
      const display_cfg_t *display_cfg)
      : size(size), rotation(rotation), buffer1(buffer1), buffer2(buffer2) {

    // Need to initialise the Touch Controller before the LCD, as only a Single
    // Reset line shared between them

    auto err =
        RM_COMMS_I2C_Open(&g_comms_i2c_device0_ctrl, &g_comms_i2c_device0_cfg);
    assert(FSP_SUCCESS == err);

    err = init_ts(&g_comms_i2c_device0_ctrl);
    assert(FSP_SUCCESS == err);

    err = enable_ts(&g_comms_i2c_device0_ctrl);
    assert(FSP_SUCCESS == err);

    // Initialize GLCDC
    err = R_GLCDC_Open(&m_display_ctrl, display_cfg);
    assert(FSP_SUCCESS == err);
    err = R_GLCDC_Start(&m_display_ctrl);
    assert(FSP_SUCCESS == err);
  }

  ~Ra8d1SlintPlatform() {
    while (FSP_SUCCESS != R_GLCDC_Stop(&m_display_ctrl)) {
      // Wait for GLCDC register update to complete before closing driver.
    }
    R_GLCDC_Close(&m_display_ctrl);
  }

  std::unique_ptr<slint::platform::WindowAdapter>
  create_window_adapter() override {
    auto w = std::make_unique<Ra8d1WindowAdapter>(size);
    w->m_renderer.set_rendering_rotation(rotation);
    m_window = w.get();
    return w;
  }

  std::chrono::milliseconds duration_since_start() override {
    return std::chrono::milliseconds(xTaskGetTickCount() * portTICK_PERIOD_MS);
  }

  void run_event_loop() override {
    float last_touch_x = 0;
    float last_touch_y = 0;
    bool touch_down = false;

    while (true) {
      slint::platform::update_timers_and_animations();

      if (m_window) {

        touch_data_t touch_data{};
        touchpad_read(&touch_data);
        if (touch_data.state == TOUCH_STATE_PRESSED) {
          // I manually measured on the device that the range are:
          // for x: 370->62
          // for y: 1221->855 (middle) 0->375 | the screen is split
          last_touch_x = m_window->m_size.width -
                         m_window->m_size.width *
                             (int(touch_data.point.x) - 62) / (370 - 62);
          int h2 = m_window->m_size.height / 2;
          last_touch_y =
              touch_data.point.y > 375
                  ? h2 - h2 * (int(touch_data.point.y) - 855) / (1221 - 855)
                  : h2 + h2 * touch_data.point.y / 375;
          m_window->window().dispatch_pointer_move_event(
              slint::LogicalPosition({last_touch_x, last_touch_y}));
          if (!touch_down) {
            m_window->window().dispatch_pointer_press_event(
                slint::LogicalPosition({last_touch_x, last_touch_y}),
                slint::PointerEventButton::Left);
          }
          touch_down = true;
        } else if (touch_down) {
          m_window->window().dispatch_pointer_release_event(
              slint::LogicalPosition({last_touch_x, last_touch_y}),
              slint::PointerEventButton::Left);
          m_window->window().dispatch_pointer_exit_event();
          touch_down = false;
        }

        if (std::exchange(m_window->needs_redraw, false)) {
          auto rotated = rotation == slint::platform::SoftwareRenderer::
                                         RenderingRotation::Rotate90 ||
                         rotation == slint::platform::SoftwareRenderer::
                                         RenderingRotation::Rotate270;

          m_window->m_renderer.render(buffer1, rotated
                                                   ? m_window->m_size.height
                                                   : m_window->m_size.width);

          // Update frame buffer
          while (R_GLCDC_BufferChange(
                     &m_display_ctrl,
                     reinterpret_cast<uint8_t *>(buffer1.data()),
                     DISPLAY_FRAME_LAYER_1) == FSP_ERR_INVALID_UPDATE_TIMING) {
          }
          std::swap(buffer1, buffer2);
        }
      }
      // TODO! Yield
    }
  }
};

} // namespace slint::private_api

inline void slint_ra8d1_init(const SlintPlatformConfiguration &config) {
  auto area = config.size.width * config.size.height;
  size_t buffer_size =
      area * sizeof(slint::private_api::Ra8d1SlintPlatform::Pixel);

  auto base_ptr =
      reinterpret_cast<slint::private_api::Ra8d1SlintPlatform::Pixel *>(
          config.framebuffer);
  std::span<slint::private_api::Ra8d1SlintPlatform::Pixel> buffer1(base_ptr,
                                                                   area);
  std::span<slint::private_api::Ra8d1SlintPlatform::Pixel> buffer2(
      base_ptr + area, area);

  assert(config.framebuffer_size >= buffer_size * 2);

  slint::platform::set_platform(
      std::make_unique<slint::private_api::Ra8d1SlintPlatform>(
          config.size, config.rotation, buffer1, buffer2, config.display_cfg));
}
