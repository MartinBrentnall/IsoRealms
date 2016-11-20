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
#include "ResourceModelSpindizzyCraftPyramid.h"

const float ResourceModelSpindizzyCraftPyramid::OUTLINE = 0.82f;

ResourceModelSpindizzyCraftPyramid::ResourceModelSpindizzyCraftPyramid(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cColourTop = new Colour(1.0f, 1.0f, 0.0f);
  cColourSide = new Colour(1.0f, 0.0f, 0.0f);
  cColourOutline = new Colour(0.0f, 0.0f, 0.0f);
  updateTextures();
  Vertex* mVertex = new Vertex(0.0f, 0.0f, 0.0f);
  cSampleModel = new ModelSpindizzyCraftPyramid(mVertex, &cTextureTop, &cTextureSide);
}

void ResourceModelSpindizzyCraftPyramid::setTopColour(IColour* colour) {
  cColourTop = colour;
  updateTextures();
}

void ResourceModelSpindizzyCraftPyramid::setSideColour(IColour* colour) {
  cColourSide = colour;
  updateTextures();
}

void ResourceModelSpindizzyCraftPyramid::setOutlineColour(IColour* colour) {
  cColourOutline = colour;
  updateTextures();
}

IColour* ResourceModelSpindizzyCraftPyramid::getTopColour() {
  return cColourTop;
}

IColour* ResourceModelSpindizzyCraftPyramid::getSideColour() {
  return cColourSide;
}

IColour* ResourceModelSpindizzyCraftPyramid::getOutlineColour() {
  return cColourOutline;
}
  
void ResourceModelSpindizzyCraftPyramid::updateTextures() {
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

void ResourceModelSpindizzyCraftPyramid::generateTextureTop() {
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

void ResourceModelSpindizzyCraftPyramid::generateTextureSide() {
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

I3DModel* ResourceModelSpindizzyCraftPyramid::createModel(Vertex* location, float scale) {
  return new ModelSpindizzyCraftPyramid(location, &cTextureTop, &cTextureSide);
}

void ResourceModelSpindizzyCraftPyramid::destroyModel(I3DModel* model) {
  delete model;
}

void ResourceModelSpindizzyCraftPyramid::updateIcon(unsigned int milliseconds) {
  cSampleModel->update(milliseconds);
}

void ResourceModelSpindizzyCraftPyramid::renderIcon() {
  glTranslatef(0.0f, -1.0f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glScalef(2.0f, 2.0f, 2.0f);
  cSampleModel->render();
}

void ResourceModelSpindizzyCraftPyramid::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resourceLocator) {
  node->addAttribute("top", resourceLocator->getPath(cColourTop));
  node->addAttribute("side", resourceLocator->getPath(cColourSide));
  node->addAttribute("outline", resourceLocator->getPath(cColourOutline));
}

void ResourceModelSpindizzyCraftPyramid::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resourceAccessor) {
  cColourTop = resourceAccessor->getColour(node->getAttribute("top"));
  cColourSide = resourceAccessor->getColour(node->getAttribute("side"));
  cColourOutline = resourceAccessor->getColour(node->getAttribute("outline"));
  updateTextures();
}

void ResourceModelSpindizzyCraftPyramid::resourceChanged(IColour* colour) {
  if (colour == cColourTop || colour == cColourSide || colour == cColourOutline) {
    updateTextures();
  }
}

void ResourceModelSpindizzyCraftPyramid::resourcePendingDestruction(IColour* destroyee, IColour* replacement) {
  bool mChanged = false;
  if (destroyee == cColourTop)     {cColourTop = replacement;     mChanged = true;}
  if (destroyee == cColourSide)    {cColourSide = replacement;    mChanged = true;}
  if (destroyee == cColourOutline) {cColourOutline = replacement; mChanged = true;}
  if (mChanged) {
    updateTextures();
  }
}
