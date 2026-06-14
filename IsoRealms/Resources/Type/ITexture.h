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

#include <GL/glew.h>

#include "IResource.h"

namespace IsoRealms {
  class ITexture : public IResource {
    public:
    virtual void set() const = 0;
    virtual void hintTextureInUse(bool inUse) = 0;
    virtual ITexture* getTexture() = 0;
    virtual void coord(float x, float y) const = 0;

    virtual bool renderResourceIcon() const override {
      set();
      glBegin(GL_QUADS);
      glTexCoord2f(1.0f, 0.0f); glVertex2f(-1.0f,  1.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex2f( 1.0f, -1.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex2f( 1.0f,  1.0f);
      glEnd();
      return true;
    }

    virtual ~ITexture() {}
  };
}
