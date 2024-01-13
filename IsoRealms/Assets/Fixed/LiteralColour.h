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

#include <GL/glew.h>
#include <iostream>

#include "IsoRealms/Assets/Type/IColour.h"
#include "IsoRealms/Persistence/DOMNode.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/IAssetIdentifier.h"
#include "IsoRealms/IAssetRegistry.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  class LiteralColour : public IColour {
    private:
    float cRed;   /// Red intensity
    float cGreen; /// Green intensity
    float cBlue;  /// Blue intensity
    float cAlpha; /// Alpha intensity

    public:
      
    /**
     * Construct a fixed colour of black.
     */
    LiteralColour();

    /**
    * Create a fixed copy of the specified colour.  An relative intensity may
    * optionally be specified.  For example, 0.5 creates a colour of half the
    * brightness of the specified colour, and 2.0 creates a colour of double
    * the brightness of the specified colour.
    *
    * @param colour Colour to copy.
    * @param intensity The relatively intensity of the new colour.
    */
    LiteralColour(const IColour& colour, const float intensity = 1.0f);
      
    /**
    * Construct an colour of the specified intensity values, from 0 to 1.
    *
    * @param rede Red intensity.
    * @param green Green intensity.
    * @param blue Blue intensity.
    * @param alpha Alpha intensity.
    */
    LiteralColour(const float red, const float green, const float blue, const float alpha = 1.0f);

    /**
    * Construct a colour resulting from weighted mixtures of the two specified colours.
    *
    * @param a First colour.
    * @param b Second colour.
    * @param weight 0.0 weighs fully towards a, 1.0 weighs fully towards b.
    */
    LiteralColour(const IColour& a, const IColour& b, const float weight = 0.5f);

    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const; // TODO: How is this used?

    bool operator==(const LiteralColour& colour) const;
    bool operator!=(const LiteralColour& colour) const;

    /**********************\
     * Implements IColour *
    \**********************/
    void set() const override;
    float getRed() const override;
    float getGreen() const override;
    float getBlue() const override;
    float getAlpha() const override;
  };
}
