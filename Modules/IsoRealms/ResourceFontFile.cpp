/*
 * Copyright 2009 Martin Brentnall
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
#include "ResourceFontFile.h"

ResourceFontFile::ResourceFontFile(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
}

void ResourceFontFile::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  cFilename = node->getAttribute("file");
  cDetail = node->getIntegerAttribute("detail");
  std::string mFontLocation = System::getProgramResource(cFilename);

  // TODO: Correct for screen shape.
  cScale = cDetail / 2.0;
  cTextures = new GLuint[128];
  cWidths = new double[128];

  // Create and initilize a freetype font library.
  FT_Library library;
  if (FT_Init_FreeType(&library)) {
    throw std::runtime_error("FT_Init_FreeType failed");
  }

  // Create a freetype face.
  FT_Face face;

  // Try to load the font from the given file.
  if (FT_New_Face(library, mFontLocation.c_str(), 0, &face)) {
    throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");
  }

  // Set the character size (1 pixel is 64 units)
  FT_Set_Char_Size(face, cDetail << 6, cDetail << 6, 96, 96);

  // Allocate memory for OpenGL stuff we're creating.
  cGLListBase = glGenLists(128);
  glGenTextures(128, cTextures);

  // Create each of the fonts display lists.
  for (unsigned char c = 0; c < 128; c++) {
    makeDisplayList(face, c, cGLListBase, cTextures);
  }

  // Free up some stuff that we no longer need.
  FT_Done_Face(face);
  FT_Done_FreeType(library);
}

void ResourceFontFile::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("file", cFilename);
  node->addAttribute("detail", cDetail);
}

void ResourceFontFile::saveCache(DOMNodeWriter* cache) {
  // Nothing to do
}

int ResourceFontFile::nextPowerOfTwo(int a) {
  int rval = 1;
  while (rval < a) {
    rval <<= 1;
  }
  return rval;
}

void ResourceFontFile::makeDisplayList(FT_Face face, char ch, GLuint listBase, GLuint* tex_base) {

  // Load the glyph.
  if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT)) {
    throw std::runtime_error("FT_Load_Glyph failed");
  }
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  
  // Move the face's glyph into a Glyph object.
  FT_Glyph glyph;
  if (FT_Get_Glyph(face->glyph, &glyph)) {
    throw std::runtime_error("FT_Get_Glyph failed");
  }

  // Convert the glyph to a bitmap.
  FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
  FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;

  // This reference will make accessing the bitmap easier
  FT_Bitmap& bitmap = bitmap_glyph->bitmap;

  // Use our helper function to get the widths of
  // the bitmap data that we will need in order to create
  // our texture.
  unsigned int width = nextPowerOfTwo(bitmap.width);
  unsigned int height = nextPowerOfTwo(bitmap.rows);

  // Allocate memory for the texture data.
  GLubyte* expanded_data = new GLubyte[2 * width * height];

  // Fill in the data for the expanded bitmap.
  for (unsigned int j = 0; j < height; j++) {
    for (unsigned int i = 0; i < width; i++) {
      expanded_data[2 * (i + j * width)] = expanded_data[2 * (i + j * width) + 1] = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
    }
  }

  // Set up some texture paramaters.
  glBindTexture(GL_TEXTURE_2D, tex_base[static_cast<int>(ch)]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // Create the texture.
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

  // Get rid of the expanded data, which we no longer need.
  delete[] expanded_data;

  // Create the display list
  glNewList(listBase + ch, GL_COMPILE);
  glBindTexture(GL_TEXTURE_2D, tex_base[static_cast<int>(ch)]);

  // Move to the side slightly for spacing between characters.
  glTranslatef(bitmap_glyph->left / cScale, 0, 0);

  // Move down slightly for characters that go below the line (g, y, q, etc.).
  glPushMatrix();
  glTranslatef(0, (int) (bitmap_glyph->top - bitmap.rows) / cScale, 0);

  // Crop empty space from the characters (Store in x and y).
  float x = (float) bitmap.width / (float) width;
  float y = (float) bitmap.rows / (float) height;

  // Ensure proper allgnment of texture and draw it.
  glBegin(GL_QUADS);
  glTexCoord2d(x, y); glVertex2f(bitmap.width / cScale, 0);
  glTexCoord2d(x, 0); glVertex2f(bitmap.width / cScale, bitmap.rows / cScale);
  glTexCoord2d(0, 0); glVertex2f(0,                     bitmap.rows / cScale);
  glTexCoord2d(0, y); glVertex2f(0,                     0);
  glEnd();
  glPopMatrix();
  glTranslatef((face -> glyph -> advance.x >> 6) / cScale, 0, 0);

  //Finnish the display list
  glEndList();

  // Store the width of the character
  cWidths[static_cast<int>(ch)] = (((face -> glyph -> advance.x >> 6) / cScale) + bitmap_glyph->left / cScale);
}

// De-allocate OpenGL stuff.
void ResourceFontFile::clean() {
  glDeleteLists(cGLListBase, 128);
  glDeleteTextures(128, cTextures);
  delete [] cTextures;
}

// Convert OpenGL space co-ordinates into pixel co-ordinates.
void ResourceFontFile::pushScreenCoordinateMatrix() {
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();
}

// Pops the projection matrix without changing the current MatrixMode.
void ResourceFontFile::popProjectionMatrix() {
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
}

void ResourceFontFile::print(float x, float y, float size, IFont::Alignment alignment, const char *fmt, ...) {
  if (fmt == NULL) {
    return;
  }

  // We want a coordinate system where things coresponding to window pixels.
  pushScreenCoordinateMatrix();

  GLuint font = cGLListBase;

  char text[2048]; // Holds Our String
  va_list ap; // Pointer To List Of Arguments

  va_start(ap, fmt); // Parses The String For Variables
  vsprintf(text, fmt, ap); // And Converts Symbols To Actual Numbers
  va_end(ap); // Results Are Stored In Text

  // Split the text onto seperate lines.
  const char* start_line = text;
  const char* c = text;
  std::vector<std::string> lines;
  while (*c) {
    if (*c == '\n') {
      std::string line;
      for (const char *n = start_line; n < c; n++) {
        line.append(1, *n);
      }
      lines.push_back(line);
      start_line = c + 1;
    }
    c++;
  }
  if (start_line) {
    std::string line;
    for (const char *n = start_line; n < c; n++) {
      line.append(1, *n);
    }
    lines.push_back(line);
  }

  glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
  glMatrixMode(GL_MODELVIEW);
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glListBase(font);

  float modelview_matrix[16];	
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

  // Widths are required in case the text is to be right or center aligned
  std::vector<double> mTotalWidths;
  if (alignment != IFont::LEFT) {
    for (unsigned int i = 0; i < lines.size(); i ++) {
      double mTotalWidth = 0.0;
      for (unsigned int j = 0; j < lines[i].length(); j++) {
        mTotalWidth += cWidths[static_cast<int>(lines[i][j])];
      }
      mTotalWidths.push_back(mTotalWidth);
    }
  }

  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();

  // Display the actual text to the screen, line by line.
  for (unsigned int i = 0 ;i < lines.size(); i ++) {
    glPushMatrix();
    glLoadIdentity();
    // TODO: Align constants
    glTranslatef(x, y - size * i * 2.5f, 0.0f);
    if (size != 1.0f) {
      glScalef(size * mAspectRatio, size, 0.0f);
    }
    if (alignment != IFont::LEFT) {
      double mAlignmentOffset = alignment == IFont::CENTER ? mTotalWidths[i] / 2.0 : mTotalWidths[i];
      glTranslatef(-mAlignmentOffset, 0.0f, 0.0f);
    }
    glMultMatrixf(modelview_matrix);
    glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
    glPopMatrix();
  }

  glPopAttrib();
  popProjectionMatrix();
}

float ResourceFontFile::getWidth(float size, const char *fmt, ...) {
  char text[2048]; // Holds Our String
  va_list ap; // Pointer To List Of Arguments

  va_start(ap, fmt); // Parses The String For Variables
  vsprintf(text, fmt, ap); // And Converts Symbols To Actual Numbers
  va_end(ap); // Results Are Stored In Text

  // Split the text onto seperate lines.
  const char* start_line = text;
  const char* c = text;
  std::vector<std::string> lines;
  while (*c) {
    if (*c == '\n') {
      std::string line;
      for (const char *n = start_line; n < c; n++) {
        line.append(1, *n);
      }
      lines.push_back(line);
      start_line = c + 1;
    }
    c++;
  }
  if (start_line) {
    std::string line;
    for (const char *n = start_line; n < c; n++) {
      line.append(1, *n);
    }
    lines.push_back(line);
  }

  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();

  float mHighestWidth = 0.0f;
  for (unsigned int i = 0; i < lines.size(); i ++) {
    float mTotalWidth = 0.0f;
    for (unsigned int j = 0; j < lines[i].length(); j++) {
      mTotalWidth += cWidths[static_cast<int>(lines[i][j])] * mAspectRatio;
    }
    if (mTotalWidth > mHighestWidth) {
      mHighestWidth = mTotalWidth;
    }
  }
  return mHighestWidth * size;
}

float ResourceFontFile::getHeight(float size, const char *fmt, ...) {
  char text[2048]; // Holds Our String
  va_list ap; // Pointer To List Of Arguments

  va_start(ap, fmt); // Parses The String For Variables
  vsprintf(text, fmt, ap); // And Converts Symbols To Actual Numbers
  va_end(ap); // Results Are Stored In Text

  // Split the text onto seperate lines.
  const char* start_line = text;
  const char* c = text;
  std::vector<std::string> lines;
  while (*c) {
    if (*c == '\n') {
      std::string line;
      for (const char *n = start_line; n < c; n++) {
        line.append(1, *n);
      }
      lines.push_back(line);
      start_line = c + 1;
    }
    c++;
  }
  if (start_line) {
    std::string line;
    for (const char *n = start_line; n < c; n++) {
      line.append(1, *n);
    }
    lines.push_back(line);
  }
  return lines.size() * size * 2.5f;
}

unsigned int ResourceFontFile::getChar(float position, float size, const char* fmt, ...) {
  char text[2048]; // Holds Our String
  va_list ap; // Pointer To List Of Arguments

  va_start(ap, fmt); // Parses The String For Variables
  vsprintf(text, fmt, ap); // And Converts Symbols To Actual Numbers
  va_end(ap); // Results Are Stored In Text

  // Split the text onto seperate lines.
  std::vector<std::string> lines;

  float mDifference = FLT_MAX;
  unsigned int i;
  for (i = 0; i <= strlen(text); i++) {
    char mSubText[2048];
    strncpy(mSubText, text, i);
    mSubText[i] = '\0';
    float mNewDifference = position - getWidth(size, mSubText);
    if (mNewDifference < 0.0f) {
      mNewDifference = -mNewDifference;
    }
    if (mNewDifference > mDifference) {
      return mNewDifference > mDifference ? i - 1: i - 2;
    }
    mDifference = mNewDifference;
  }
  return i - 1;
}
