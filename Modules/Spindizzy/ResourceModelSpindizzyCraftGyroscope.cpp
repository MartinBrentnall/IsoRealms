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
#include "ResourceModelSpindizzyCraftGyroscope.h"

const float ResourceModelSpindizzyCraftGyroscope::CIRCLE_RESOLUTION = 5.0f * (M_PI / 180.0);

ResourceModelSpindizzyCraftGyroscope::ResourceModelSpindizzyCraftGyroscope(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cColour1 = new Colour(1.0f, 0.0f, 0.0f);
  cColour2 = new Colour(1.0f, 1.0f, 0.0f);
  cColour3 = new Colour(0.0f, 1.0f, 0.0f);
  cColour4 = new Colour(0.0f, 0.25f, 1.0f);
  cColourOutline = new Colour(0.0f, 0.0f, 0.0f);
  updateTexture();
  Vertex* mVertex = new Vertex(0.0f, 0.0f, 0.0f);
  cSampleModel = new ModelSpindizzyCraftGyroscope(mVertex, &cTexture);
}

void ResourceModelSpindizzyCraftGyroscope::setColour1(IColour* colour) {
  cColour1 = colour;
  updateTexture();
}

void ResourceModelSpindizzyCraftGyroscope::setColour2(IColour* colour) {
  cColour2 = colour;
  updateTexture();
}

void ResourceModelSpindizzyCraftGyroscope::setColour3(IColour* colour) {
  cColour3 = colour;
  updateTexture();
}

void ResourceModelSpindizzyCraftGyroscope::setColour4(IColour* colour) {
  cColour4 = colour;
  updateTexture();
}

void ResourceModelSpindizzyCraftGyroscope::setOutlineColour(IColour* colour) {
  cColourOutline = colour;
  updateTexture();
}

IColour* ResourceModelSpindizzyCraftGyroscope::getColour1() {
  return cColour1;
}

IColour* ResourceModelSpindizzyCraftGyroscope::getColour2() {
  return cColour2;
}

IColour* ResourceModelSpindizzyCraftGyroscope::getColour3() {
  return cColour3;
}
  
IColour* ResourceModelSpindizzyCraftGyroscope::getColour4() {
  return cColour4;
}
  
IColour* ResourceModelSpindizzyCraftGyroscope::getOutlineColour() {
  return cColourOutline;
}
  
void ResourceModelSpindizzyCraftGyroscope::updateTexture() {
  cTexture = new Texture();

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();

  cTexture->setRenderTarget();
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  renderCircle(1.0f, 0.88f, cColourOutline);
  renderCircle(0.88f, 0.0f, 90.0f, cColour1);
  renderCircle(0.88f, 90.0f, 180.0f, cColour2);
  renderCircle(0.88f, 180.0f, 270.0f, cColour3);
  renderCircle(0.88f, 270.0f, 360.0f, cColour4);
    
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  Configuration* mConfiguration = Configuration::getInstance();
  mConfiguration->setViewPort();
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}
  
I3DModel* ResourceModelSpindizzyCraftGyroscope::createModel(Vertex* location, float scale) {
  return new ModelSpindizzyCraftGyroscope(location, &cTexture);
}

void ResourceModelSpindizzyCraftGyroscope::destroyModel(I3DModel* model) {
  delete model;
}

void ResourceModelSpindizzyCraftGyroscope::updateIcon(unsigned int milliseconds) {
  cSampleModel->update(milliseconds);
}

void ResourceModelSpindizzyCraftGyroscope::renderIcon() {
  glTranslatef(0.0f, -1.0f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glScalef(2.0f, 2.0f, 2.0f);
  cSampleModel->render();
}

void ResourceModelSpindizzyCraftGyroscope::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resourceLocator) {
  node->addAttribute("colour1", resourceLocator->getPath(cColour1));
  node->addAttribute("colour2", resourceLocator->getPath(cColour2));
  node->addAttribute("colour3", resourceLocator->getPath(cColour3));
  node->addAttribute("colour4", resourceLocator->getPath(cColour4));
  node->addAttribute("outline", resourceLocator->getPath(cColourOutline));
}

void ResourceModelSpindizzyCraftGyroscope::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resourceAccessor) {
  cColour1       = resourceAccessor->getColour(node->getAttribute("colour1"));
  cColour2       = resourceAccessor->getColour(node->getAttribute("colour2"));
  cColour3       = resourceAccessor->getColour(node->getAttribute("colour3"));
  cColour4       = resourceAccessor->getColour(node->getAttribute("colour4"));
  cColourOutline = resourceAccessor->getColour(node->getAttribute("outline"));
  updateTexture();
}

void ResourceModelSpindizzyCraftGyroscope::resourceChanged(IColour* colour) {
  if (colour == cColour1 || colour == cColour2 || colour == cColour3 || colour == cColour4 || colour == cColourOutline) {
    updateTexture();
  }
}

void ResourceModelSpindizzyCraftGyroscope::resourcePendingDestruction(IColour* destroyee, IColour* replacement) {
  bool mChanged = false;
  if (destroyee == cColour1)       {cColour1       = replacement; mChanged = true;}
  if (destroyee == cColour2)       {cColour2       = replacement; mChanged = true;}
  if (destroyee == cColour3)       {cColour3       = replacement; mChanged = true;}
  if (destroyee == cColour4)       {cColour4       = replacement; mChanged = true;}
  if (destroyee == cColourOutline) {cColourOutline = replacement; mChanged = true;}
  if (mChanged) {
    updateTexture();
  }
}

// TODO: Nasty stuff below here.  Clean up!
void ResourceModelSpindizzyCraftGyroscope::renderCircle(float radius, float startAngle, float endAngle, IColour* colour) {
  if (colour->getAlpha() == 0.0f) {
    return;
  }
  glBegin(GL_TRIANGLE_FAN);
  colour->set();
  glVertex2f(0.0f, 0.0f);
  float mStartAngle = startAngle * (M_PI / 180.0f);
  float mEndAngle = endAngle * (M_PI / 180.0f);
  for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
    glVertex2f(sin(angle) * radius, cos(angle) * radius);
  }
  glEnd();
}

void ResourceModelSpindizzyCraftGyroscope::renderCircle(float outerRadius, float innerRadius, IColour* colour) {
  glBegin(GL_TRIANGLE_STRIP);
  glEnable(GL_BLEND);
  colour->set();
  float mStartAngle = 0.0f * (M_PI / 180.0f);
  float mEndAngle = 360.0f * (M_PI / 180.0f);
  for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
    glVertex2f(sin(angle) * innerRadius, cos(angle) * innerRadius);
    glVertex2f(sin(angle) * outerRadius, cos(angle) * outerRadius);
  }
  glEnd();
}
