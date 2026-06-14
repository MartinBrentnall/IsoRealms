/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#ifdef _WIN32
#define _USE_MATH_DEFINES
#include <math.h>
#else
#include <cmath>
#endif

#include <string>

#include <GL/glew.h>

#include "IResource.h"

namespace IsoRealms {
  
  /**
   * Interface for colour value resources.
   */
  class IColour : public IResource {
    public:
    
    /**
     * Set this as the colour in the OpenGL context.
     */
    virtual void set() const = 0;
    
    /**
     * Returns the cRed intensity of this colour.
     *
     * @returns  The cRed intensity of this colour.
     */
    virtual float getRed() const = 0;

    /**
     * Returns the green intensity of this colour.
     *
     * @returns  The green intensity of this colour.
     */
    virtual float getGreen() const = 0;

    /**
     * Returns the blue intensity of this colour.
     *
     * @returns  The blue intensity of this colour.
     */
    virtual float getBlue() const = 0;

    /**
     * Returns the alpha intensity of this colour.
     *
     * @returns  The alpha intensity of this colour.
     */
    virtual float getAlpha() const = 0;

    virtual bool renderResourceIcon() const override {
      float mCircleResolution =   5.0f * (static_cast<float>(M_PI) / 180.0f);
      float mStartAngle       =   0.0f * (static_cast<float>(M_PI) / 180.0f);
      float mEndAngle         = 360.0f * (static_cast<float>(M_PI) / 180.0f);
      glBindTexture(GL_TEXTURE_2D, 0);
      glBegin(GL_TRIANGLE_FAN);
      set();
      glVertex2f(0.0f, 0.0f);
      for (float angle = mEndAngle; angle >= mStartAngle; angle -= mCircleResolution) {
        glVertex2f(std::sin(angle), cos(angle));
      }
      glEnd();
      glLineWidth(1.0f);
      glBegin(GL_LINE_LOOP);
      glColor3f(1.0f, 1.0f, 1.0f);
      for (float angle = mEndAngle; angle >= mStartAngle; angle -= mCircleResolution) {
        glVertex2f(std::sin(angle), cos(angle));
      }
      glEnd();
      return true;
    }
    
    virtual ~IColour() {}
  };
}
