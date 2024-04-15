/*
 * Copyright 2023 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <functional>

#include "IsoRealms/Assets/Type/IFont.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"


namespace IsoRealms {
  class Font : public IAssetUser<IFont> {
    private:
    IProject* cProject;
    IFont* cFont;

    Font(Font const& font) = delete;
    Font& operator=(Font const& font) = delete;

    public:
    Font(IProject* project);

    void init(DOMNode& node, const std::string& tag);
    void save(DOMNodeWriter* node, const std::string& tag) const;

    IFont* operator->() const {
      return cFont;
    }

    IFont* operator*() const {
      return cFont;
    }

    /********************************\
     * Implements IAssetUser<IFont> *
    \********************************/
    void relinquish(IFont* asset) override;

    virtual ~Font();
  };
}
