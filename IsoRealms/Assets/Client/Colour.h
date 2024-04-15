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

#include "IsoRealms/Assets/Type/IColour.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"

namespace IsoRealms {
  class Colour : public IAssetUser<IColour>,
                 public IStateListener<IColour*>,
                 public IColour {
    public:
    Colour(IProject* project, float defaultRed, float defaultGreen, float defaultBlue, float defaultAlpha = 0.0f, std::function<void()> listener = nullptr);

    void init(DOMNode& node, const std::string& tag);
    void save(DOMNodeWriter* node, const std::string& tag) const;

    /**********************\
     * Implements IColour *
    \**********************/
    float getRed() const override {
      return cColour->getRed();
    }

    float getGreen() const override {
      return cColour->getGreen();
    }

    float getBlue() const override {
      return cColour->getBlue();
    }

    float getAlpha() const override {
      return cColour->getAlpha();
    }

    void set() const override {
      cColour->set();
    }

    /**********************************\
     * Implements IAssetUser<IColour> *
    \**********************************/
    void relinquish(IColour* asset) override;

    /***************************************\
     * Implements IStateListener<IColour*> *
    \***************************************/
    void stateChanged(IColour* asset) override;

    virtual ~Colour();

    private:
    IProject* cProject;
    float cDefaultRed;
    float cDefaultGreen;
    float cDefaultBlue;
    float cDefaultAlpha;
    IColour* cColour;
    std::function<void()> cListener;

    Colour(Colour const& colour) = delete;
    Colour& operator=(Colour const& colour) = delete;
  };
}

