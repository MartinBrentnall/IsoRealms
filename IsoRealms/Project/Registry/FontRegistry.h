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

#include <functional>

#include "IsoRealms/Resources/Providers/ResourceLiteralDummy.h"
#include "IsoRealms/Resources/Type/IFont.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Utils.h"

#include "ResourceClientManager.h"
#include "IResourceUser.h"

namespace IsoRealms {
  class FontRegistry : public ResourceClientManager<FontRegistry, IComponentData, IFont> {
    public:
    FontRegistry();

    private:
    class Dummy : public IFont {
      public:

      /********************\
      * Implements IFont *
      \********************/
      void print(float x, float y, float size, Alignment alignment, const std::string& text) override;
      float getWidth(float size, const std::string& text) override;
      float getHeight(float size, const std::string& text) override;
      unsigned int getChar(float position, float size, const std::string& text) override;
      bool renderResourceIcon() const override;
    };

    ResourceLiteralDummy<IComponentData, IFont, Dummy> cNone;
  };
}
