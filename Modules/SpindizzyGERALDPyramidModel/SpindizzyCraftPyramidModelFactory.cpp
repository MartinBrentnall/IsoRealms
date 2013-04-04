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
#include "SpindizzyCraftPyramidModelFactory.h"

const float SpindizzyCraftPyramidModelFactory::OUTLINE = 0.82f;

void SpindizzyCraftPyramidModelFactory::updateTextures() {
  cTextureTop = new Texture();
  cTextureSide = new Texture();
    
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();

  cTextureTop->setRenderTarget();  generateTextureTop();
  cTextureSide->setRenderTarget(); generateTextureSide();
    
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  Configuration* mConfiguration = Configuration::getInstance();
  mConfiguration->setViewPort();
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void SpindizzyCraftPyramidModelFactory::generateTextureTop() {
  cColourOutline->setBackground();
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_QUADS);
  cColourTop->set();
  glVertex2f(-OUTLINE, -OUTLINE);
  glVertex2f( OUTLINE, -OUTLINE);
  glVertex2f( OUTLINE,  OUTLINE);
  glVertex2f(-OUTLINE,  OUTLINE);
  glEnd();
}

void SpindizzyCraftPyramidModelFactory::generateTextureSide() {
  cColourOutline->setBackground();
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_TRIANGLES);
  cColourSide->set();
  // TODO: 'magic' numbers 0.74f and 0.62f should be calculated properly
  glVertex2f(-0.74, -OUTLINE);
  glVertex2f( 0.74, -OUTLINE);
  glVertex2f( 0.0f,  0.62f);
  glEnd();
}

I3DModel* SpindizzyCraftPyramidModelFactory::createModel(Vertex* location, float scale) {
  return new SpindizzyCraftPyramidModel(location, &cTextureTop, &cTextureSide);
}

void SpindizzyCraftPyramidModelFactory::destroyModel(I3DModel* model) {
  delete model;
}

void SpindizzyCraftPyramidModelFactory::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("top", resourceLocator->getPath(cColourTop));
  node->addAttribute("side", resourceLocator->getPath(cColourSide));
  node->addAttribute("outline", resourceLocator->getPath(cColourOutline));
}

void SpindizzyCraftPyramidModelFactory::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  cColourTop = resourceAccessor->getColour(node->getAttribute("top"));
  cColourSide = resourceAccessor->getColour(node->getAttribute("side"));
  cColourOutline = resourceAccessor->getColour(node->getAttribute("outline"));
  updateTextures();
}

void SpindizzyCraftPyramidModelFactory::resourceChanged(IColour* colour) {
  if (colour == cColourTop || colour == cColourSide || colour == cColourOutline) {
    updateTextures();
  }
}

void SpindizzyCraftPyramidModelFactory::resourcePendingDestruction(IColour* destroyee, IColour* replacement) {
  bool mChanged = false;
  if (destroyee == cColourTop)     {cColourTop = replacement;     mChanged = true;}
  if (destroyee == cColourSide)    {cColourSide = replacement;    mChanged = true;}
  if (destroyee == cColourOutline) {cColourOutline = replacement; mChanged = true;}
  if (mChanged) {
    updateTextures();
  }
}
