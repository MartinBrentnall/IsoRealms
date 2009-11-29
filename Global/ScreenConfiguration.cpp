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
#include "ScreenConfiguration.h"

ScreenConfiguration::ScreenConfiguration(DOMNodeWrapper *node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Size") {
      parseSize(mNode);
    } else if (mValueAsString == "Fullscreen") {
      cFullScreen = mNode->getBooleanValue();
    } else if (mValueAsString == "Depth") {
      cScreenDepth = mNode->getIntegerValue();
    }
  }
}

void ScreenConfiguration::parseSize(DOMNodeWrapper *node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Width") {
      cScreenWidth = mNode->getIntegerValue();
      if (cScreenWidth <= 0) {
        throw ParseException("Invalid screen width \"" + Utils::toString(cScreenWidth) + "\" (expected positive integer)");
      }
    } else if (mValueAsString == "Height") {
      cScreenHeight = mNode->getIntegerValue();
      if (cScreenHeight <= 0) {
        throw ParseException("Invalid screen height \"" + Utils::toString(cScreenHeight) + "\" (expected positive integer)");
      }
    }
  }
}

void ScreenConfiguration::openScreen(std::string title) {

  // Setup SDL-OpenGL integration
  const SDL_VideoInfo *mVideoInfo = SDL_GetVideoInfo();
  if (mVideoInfo == NULL) {
    throw InitException("Could not get video info: " + std::string(SDL_GetError()));
  }
  int mVideoFlags = SDL_OPENGL | SDL_HWPALETTE | SDL_RESIZABLE;
  mVideoFlags |= mVideoInfo->hw_available ? SDL_HWSURFACE : SDL_SWSURFACE;
  if (mVideoInfo -> blit_hw) {
    mVideoFlags |= SDL_HWACCEL;
  }
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, cScreenDepth);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 0);

  // Setup SDL window
  cWindow = SDL_SetVideoMode(cScreenWidth, cScreenHeight, cScreenDepth, mVideoFlags);
  if (cWindow == NULL) {
    throw InitException("Could not to create window: " + std::string(SDL_GetError()));
  }
  SDL_WM_SetCaption(title.c_str(), title.c_str());

  // Setup OpenGL options
  float mFogColour[4] = {0.0f, 0.0f, 0.2f, 0.0f};
  glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogfv(GL_FOG_COLOR, mFogColour);
  glFogf(GL_FOG_DENSITY, 0.001f);
  glHint(GL_FOG_HINT, GL_FASTEST);
  glFogf(GL_FOG_START, 20.0f);
  glFogf(GL_FOG_END, 40.0f);
/*  glEnable(GL_FOG);*/
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);

//  glEnable(GL_LIGHTING);
//  float global_ambient[] = {0.0f, 0.0f, 0.5f, 1.0f};
//  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
//  glShadeModel(GL_SMOOTH);

  glViewport(0, 0, cScreenWidth, cScreenHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (GLfloat) cScreenWidth / (GLfloat) cScreenHeight, 0.1, 400.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Enable normal keyboard typing
  if (cFullScreen) {
    SDL_WM_ToggleFullScreen(cWindow);
  }
}

float ScreenConfiguration::getAspectRatio() {
  return (float) cScreenHeight / (float) cScreenWidth;
}

float ScreenConfiguration::getXLocation(int x) {
  float mHalfScreen = cScreenWidth / 2.0f;
  return (x - mHalfScreen) / mHalfScreen;
}

float ScreenConfiguration::getYLocation(int y) {
  float mHalfScreen = cScreenHeight / 2.0f;
  return -(y - mHalfScreen) / mHalfScreen;
}

