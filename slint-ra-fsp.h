// Copyright © SixtyFPS GmbH <info@slint.dev>
// SPDX-License-Identifier: GPL-3.0-only OR LicenseRef-Slint-Software-3.0

#pragma once

#include "r_glcdc.h"
#include <slint-platform.h>

struct SlintPlatformConfiguration {
  /// The size of the screen in pixels (not rotated)
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

void slint_ra8d1_init(const SlintPlatformConfiguration &config);
