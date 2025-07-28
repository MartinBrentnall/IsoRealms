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
#include "FileFont.h"

namespace IsoRealms::Basics {
  const std::string FileFont::JSON_DETAIL       = "detail";
  const std::string FileFont::JSON_FILENAME     = "filename";
  const std::string FileFont::JSON_LINE_SPACING = "lineSpacing";
  const std::string FileFont::JSON_SCALE        = "scale";
  const std::string FileFont::JSON_OFFSET_X     = "offsetX";
  const std::string FileFont::JSON_OFFSET_Y     = "offsetY";

  const int   FileFont::DEFAULT_DETAIL       = 64;
  const float FileFont::DEFAULT_LINE_SPACING = 2.5f;
  const float FileFont::DEFAULT_SCALE        = 1.0f;

  FileFont::FileFont(IProject& project, Basics& basics, IResourceData& data) :
            cDefFilename(project),
            cDefDetail(DEFAULT_DETAIL),
            cDefLineSpacing(DEFAULT_LINE_SPACING),
            cDefScale(DEFAULT_SCALE),
            cDefOffsetX(0.0f),
            cDefOffsetY(0.0f),
            cProcessedGLListBase(0) {
  }
  
  FileFont::FileFont(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            FileFont(project, basics, data) {
    cDefFilename.load(JSON_FILENAME, object);
    cDefDetail = object.getInteger(JSON_DETAIL, DEFAULT_DETAIL);
    cDefLineSpacing = object.getFloat(JSON_LINE_SPACING, DEFAULT_LINE_SPACING);
    cDefScale = object.getFloat(JSON_SCALE, DEFAULT_SCALE);
    cDefOffsetX = object.getFloat(JSON_OFFSET_X);
    cDefOffsetY = object.getFloat(JSON_OFFSET_Y);

    project.mainThreadInit([this]() {
      FT_Library mFTLibrary;
      if (FT_Init_FreeType(&mFTLibrary)) {
        throw std::runtime_error("FT_Init_FreeType failed");
      }

      FT_Face mFTFace;
      std::string mFontLocation = cDefFilename.getPath();
      if (FT_New_Face(mFTLibrary, mFontLocation.c_str(), 0, &mFTFace)) {
        throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");
      }

      // Set the character size (1 pixel is 64 units)
      FT_Set_Char_Size(mFTFace, cDefDetail << 6, cDefDetail << 6, 96, 96);

      // Allocate memory for OpenGL stuff we're creating.
      cProcessedGLListBase = glGenLists(128);
      glGenTextures(128, cProcessedTextureIDs);

      // Create each of the fonts display lists.
      for (unsigned char i = 0; i < 128; i++) {
        if (FT_Load_Glyph(mFTFace, FT_Get_Char_Index(mFTFace, i), FT_LOAD_DEFAULT)) {
          throw std::runtime_error("FT_Load_Glyph failed");
        }

        // Move the face's glyph into a Glyph object.
        FT_Glyph mFTGlyph;
        if (FT_Get_Glyph(mFTFace->glyph, &mFTGlyph)) {
          throw std::runtime_error("FT_Get_Glyph failed");
        }

        // Convert the glyph to a bitmap.
        FT_Glyph_To_Bitmap(&mFTGlyph, ft_render_mode_normal, 0, 1);
        FT_BitmapGlyph mFTBitmapGlyph = (FT_BitmapGlyph) mFTGlyph; // TODO: I don't like this C-style cast.

        unsigned int width = Utils::nextPowerOfTwo(mFTBitmapGlyph->bitmap.width);
        unsigned int height = Utils::nextPowerOfTwo(mFTBitmapGlyph->bitmap.rows);

        // Allocate memory for the texture data.
        std::unique_ptr<GLubyte[]> mExpandedData = std::make_unique<GLubyte[]>(2 * width * height);

        // Fill in the data for the expanded bitmap.
        for (unsigned int j = 0; j < height; j++) {
          for (unsigned int i = 0; i < width; i++) {
            mExpandedData[2 * (i + j * width)] = mExpandedData[2 * (i + j * width) + 1] = (i >= mFTBitmapGlyph->bitmap.width || j >= mFTBitmapGlyph->bitmap.rows) ? 0 : mFTBitmapGlyph->bitmap.buffer[i + mFTBitmapGlyph->bitmap.width * j];
          }
        }

        // Set up some texture paramaters.
        glBindTexture(GL_TEXTURE_2D, cProcessedTextureIDs[static_cast<int>(i)]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Create the texture.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, mExpandedData.get());

        // Create the display list
        glNewList(cProcessedGLListBase + i, GL_COMPILE);
        glBindTexture(GL_TEXTURE_2D, cProcessedTextureIDs[static_cast<int>(i)]);

        // Move to the side slightly for spacing between characters.
        float mScale = cDefDetail / 2.0f;
        glTranslatef(mFTBitmapGlyph->left / mScale, 0, 0);

        // Move down slightly for characters that go below the line (g, y, q, etc.).
        glPushMatrix();
        glTranslatef(0, (int) (mFTBitmapGlyph->top - mFTBitmapGlyph->bitmap.rows) / mScale, 0);

        // Crop empty space from the characters (Store in x and y).
        float mX = (float) mFTBitmapGlyph->bitmap.width / (float) width;
        float mY = (float) mFTBitmapGlyph->bitmap.rows / (float) height;

        // Ensure proper alignment of texture and draw it.
        glBegin(GL_QUADS);
        glTexCoord2d(mX, mY); glVertex2f(mFTBitmapGlyph->bitmap.width / mScale, 0);
        glTexCoord2d(mX, 0);  glVertex2f(mFTBitmapGlyph->bitmap.width / mScale, mFTBitmapGlyph->bitmap.rows / mScale);
        glTexCoord2d(0,  0);  glVertex2f(0,                                     mFTBitmapGlyph->bitmap.rows / mScale);
        glTexCoord2d(0,  mY); glVertex2f(0,                                     0);
        glEnd();
        glPopMatrix();
        glTranslatef((mFTFace->glyph->advance.x >> 6) / mScale, 0, 0);
        glEndList();

        // Store the width of the character
        cProcessedWidths[static_cast<int>(i)] = (((mFTFace->glyph->advance.x >> 6) / mScale) + mFTBitmapGlyph->left / mScale);
      }
      FT_Done_Face(mFTFace);
      FT_Done_FreeType(mFTLibrary);
    });
  }

  void FileFont::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IFont>(this, "", "Fonts");
  }
  
  void FileFont::save(JSONObject object) const {
    cDefFilename.save(JSON_FILENAME, object);
    object.addInteger(JSON_DETAIL, cDefDetail, DEFAULT_DETAIL);
    object.addFloat(JSON_LINE_SPACING, cDefLineSpacing, DEFAULT_LINE_SPACING);
    object.addFloat(JSON_SCALE, cDefScale, DEFAULT_SCALE);
    object.addFloat(JSON_OFFSET_X, cDefOffsetX);
    object.addFloat(JSON_OFFSET_Y, cDefOffsetY);
  }

  void FileFont::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool FileFont::renderIcon() const {
    return false;
  }

  void FileFont::getProperties(PropertyMaker& owner) {
    owner.createPropertyAsset<File>(  "File",        cDefFilename);
    owner.createPropertyNativeInteger("Detail",      [this]() {return cDefDetail;},      [this](int   value) {cDefDetail      = value;});
    owner.createPropertyNativeFloat(  "Scale",       [this]() {return cDefScale;},       [this](float value) {cDefScale       = value;});
    owner.createPropertyNativeFloat(  "XOffset",     [this]() {return cDefOffsetX;},     [this](float value) {cDefOffsetX     = value;});
    owner.createPropertyNativeFloat(  "YOffset",     [this]() {return cDefOffsetY;},     [this](float value) {cDefOffsetY     = value;});
    owner.createPropertyNativeFloat(  "LineSpacing", [this]() {return cDefLineSpacing;}, [this](float value) {cDefLineSpacing = value;});
  }
  
  FileFont::~FileFont() {
    if (cProcessedGLListBase != 0) {
      glDeleteLists(cProcessedGLListBase, 128);
      glDeleteTextures(128, cProcessedTextureIDs);
    }
  }

  void FileFont::print(float x, float y, float size, Alignment alignment, const std::string& text) {
    if (!text.empty()) {
      GLuint font = cProcessedGLListBase;
      std::vector<std::string> lines = Utils::split(text, '\n');
      glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
      glMatrixMode(GL_MODELVIEW);
      glDisable(GL_LIGHTING);
      glEnable(GL_TEXTURE_2D);
      glDisable(GL_DEPTH_TEST);
      glEnable(GL_BLEND);
      glListBase(font);

      float modelview_matrix[16];	
      glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

      // Widths are required in case the text is to be right or center aligned
      std::vector<double> mTotalWidths;
      if (alignment != Alignment::LEFT) {
        for (unsigned int i = 0; i < lines.size(); i ++) {
          double mTotalWidth = 0.0;
          for (unsigned int j = 0; j < lines[i].length(); j++) {
            mTotalWidth += cProcessedWidths[static_cast<int>(lines[i][j])];
          }
          mTotalWidths.push_back(mTotalWidth);
        }
      }

      // Display the actual text to the screen, line by line.
      float mComposedScale = size * cDefScale;
      for (unsigned int i = 0; i < lines.size(); i ++) {
        glPushMatrix();
        glTranslatef(x + cDefOffsetX * 2.0f * mComposedScale, (y + cDefOffsetY * 2.0f * mComposedScale) - mComposedScale * i * cDefLineSpacing, 0.0f);
        if (mComposedScale != 1.0f) {
          glScalef(mComposedScale, mComposedScale, 1.0f);
        }
        if (alignment != Alignment::LEFT) {
          double mAlignmentOffset = alignment == Alignment::CENTER ? mTotalWidths[i] / 2.0 : mTotalWidths[i];
          glTranslatef(static_cast<float>(-mAlignmentOffset), 0.0f, 0.0f);
        }
        glCallLists(static_cast<GLsizei>(lines[i].length()), GL_UNSIGNED_BYTE, lines[i].c_str());
        glPopMatrix();
      }
      glPopAttrib();
      glBindTexture(GL_TEXTURE_2D, 0);
    }
  }

  float FileFont::getWidth(float size, const std::string& text) {
    std::vector<std::string> lines = Utils::split(text, '\n');
    float mHighestWidth = 0.0f;
    for (unsigned int i = 0; i < lines.size(); i ++) {
      float mTotalWidth = 0.0f;
      for (unsigned int j = 0; j < lines[i].length(); j++) {
        if (lines[i][j] >= 0 && lines[i][j] < 128) {
          mTotalWidth += static_cast<float>(cProcessedWidths[static_cast<int>(lines[i][j])]);
        }
      }
      if (mTotalWidth > mHighestWidth) {
        mHighestWidth = mTotalWidth;
      }
    }
    return mHighestWidth * size * cDefScale;
  }

  float FileFont::getHeight(float size, const std::string& text) {
    float mComposedScale = size * cDefScale;
    std::vector<std::string> lines = Utils::split(text, '\n');
    return lines.size() * cDefLineSpacing * mComposedScale;
  }

  unsigned int FileFont::getChar(float position, float size, const std::string& text) {
    float mClosestDistance = std::numeric_limits<float>::max();
    for (unsigned int i = 0; i <= text.length(); i++) {
      float mDistance = position - getWidth(size, text.substr(0, i));
      if (mDistance < 0.0f) {
        mDistance = -mDistance;
      }
      if (mDistance > mClosestDistance) {
        return mDistance > mClosestDistance ? i - 1: i - 2;
      }
      mClosestDistance = mDistance;
    }
    return static_cast<unsigned int>(text.length());
  }

  bool FileFont::renderAssetIcon() const {
    return renderIcon();
  }

  void FileFont::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void FileFont::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool FileFont::isDefaultConfiguration() const {
    return true;
  }
}
