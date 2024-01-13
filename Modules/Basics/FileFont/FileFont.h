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

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
// #include <ft2build.h>
// #include <freetype2/freetype/freetype.h>
// #include <freetype2/freetype/ftglyph.h>
// #include <freetype2/freetype/ftoutln.h>
// #include <freetype2/freetype/fttrigon.h>

#include <GL/glew.h>

#include <vector>
#include <string>
#include <iostream>

#include <stdexcept>

#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a font loaded from a true-type font file.
   */
  class FileFont final : public IFont {

    public:
    
    /**********************\
     * Resource Interface *
    \**********************/
    FileFont(IProject* project, Basics* basics);
    FileFont(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);  
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    ~FileFont();

    /********************\
     * Implements IFont *
    \********************/
    void print(float, float, float, Alignment, const std::string& text) override;
    float getWidth(float, const std::string& text) override;
    float getHeight(float, const std::string& text) override;
    unsigned int getChar(float, float, const std::string& text) override;
    bool renderAssetIcon() const override;
    
    private:
    
    // DOM strings.
    static const std::string ATTRIBUTE_DETAIL;
    static const std::string ATTRIBUTE_FILENAME;
    static const std::string ATTRIBUTE_LINE_SPACING;
    static const std::string ATTRIBUTE_SCALE;
    static const std::string ATTRIBUTE_OFFSET_X;
    static const std::string ATTRIBUTE_OFFSET_Y;

    static const int   DEFAULT_DETAIL;
    static const float DEFAULT_LINE_SPACING;
    static const float DEFAULT_SCALE;

    // Definition data.
    std::string cDefFilename; /// Filename of the font to use.
    int cDefDetail;           /// Determines resolution of the font.
    float cDefLineSpacing;    /// Spacing between lines.
    float cDefScale;          /// Scaling of this font.
    float cDefOffsetX;
    float cDefOffsetY;

    // Processed data.
    GLuint cProcessedGLListBase;    /// Holds the first display list id.
    GLuint cProcessedTextureIDs[128]; /// Texture ID for each character.
    double cProcessedWidths[128];   /// Widths of each character allows us to calculate string widths.
  };
}
