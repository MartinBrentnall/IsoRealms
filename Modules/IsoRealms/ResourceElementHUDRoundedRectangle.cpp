/*
 * Copyright 2015 Martin Brentnall
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
#include "ResourceElementHUDRoundedRectangle.h"

Texture* ResourceElementHUDRoundedRectangle::cCornerTexture = NULL;
unsigned int ResourceElementHUDRoundedRectangle::cInstanceCount = 0;
const float ResourceElementHUDRoundedRectangle::CIRCLE_RESOLUTION = 5.0f * (M_PI / 180.0);

void ResourceElementHUDRoundedRectangle::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  // TODO: Initialise.
}

void ResourceElementHUDRoundedRectangle::save(DOMNodeWriter*, DOMNodeWriter*, IResourceLocator*) {
  // TODO
}

void ResourceElementHUDRoundedRectangle::loadElement(DOMNodeWrapper* node, DOMNodeWrapper* cache, BlockLocation* location, IElementContainer* container, IResourceAccessor* resources, bool asTemplate) {
  container->addElement(this);
}

void ResourceElementHUDRoundedRectangle::configureElement() {
  // TODO
}

void ResourceElementHUDRoundedRectangle::destroy(IElement* element) {
  // TODO
}

void ResourceElementHUDRoundedRectangle::removeElement(IElement* element) {
  // TODO
}

Vertex* ResourceElementHUDRoundedRectangle::editorCursorStopped(Vertex* location) {
  return nullptr;
}

bool ResourceElementHUDRoundedRectangle::inputEdit(SDL_Event& event, ILayerEditingContext* editingContext) {
  return false;// TODO
}

void ResourceElementHUDRoundedRectangle::renderEditingPreview(Vertex& location) {
  // TODO
}

void ResourceElementHUDRoundedRectangle::updateEditingPreview(unsigned int milliseconds) {
  // TODO
}

void ResourceElementHUDRoundedRectangle::renderIcon() {
  // TODO
}

void ResourceElementHUDRoundedRectangle::updateIcon(unsigned int) {
  // TODO
}

ResourceElementHUDRoundedRectangle::ResourceElementHUDRoundedRectangle(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  if (cInstanceCount++ == 0) {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();

    cCornerTexture = new Texture();
    cCornerTexture->setRenderTarget();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(1.0f, 1.0f);
    float mStartAngle = 0.0f * (M_PI / 180.0f);
    float mEndAngle = 360.0f * (M_PI / 180.0f);
    float mRadius = 2.0f;
    for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
      glVertex2f(1.0f + sin(angle) * mRadius, 1.0f + cos(angle) * mRadius);
    }
    glEnd();

    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    Configuration* mConfiguration = Configuration::getInstance();
    mConfiguration->setViewPort();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
  }
  cCornerSize = 0.02f;
}

void ResourceElementHUDRoundedRectangle::renderCorner(float xStart, float yStart, float xEnd, float yEnd, float xTextureEnd, float yTextureEnd) {
  glTexCoord2f(0.0f,        0.0f);        glVertex2f(xStart, yStart);
  glTexCoord2f(xTextureEnd, 0.0f);        glVertex2f(xEnd,   yStart);
  glTexCoord2f(xTextureEnd, yTextureEnd); glVertex2f(xEnd,   yEnd);
  glTexCoord2f(0.0f,        yTextureEnd); glVertex2f(xStart, yEnd);
}

void ResourceElementHUDRoundedRectangle::renderRectangle(float xStart, float yStart, float xEnd, float yEnd) {
  glVertex2f(xStart, yStart);
  glVertex2f(xEnd,   yStart);
  glVertex2f(xEnd,   yEnd);
  glVertex2f(xStart, yEnd);
}

std::string ResourceElementHUDRoundedRectangle::getTypeName() {
  return ""; // TODO
}
  
std::vector<IObjectProperty*> ResourceElementHUDRoundedRectangle::getProperties(IComponentContainer* windowWorkspace) {
  return std::vector<IObjectProperty*>();
}

IElementType* ResourceElementHUDRoundedRectangle::getElementType() {
  return this;
}

void ResourceElementHUDRoundedRectangle::renderStatic() {
  // Nothing to do.
}

void ResourceElementHUDRoundedRectangle::setDirty() {
  // Nothing to do.
}

IElementBounds* ResourceElementHUDRoundedRectangle::getBounds() {
  return this;
}

void ResourceElementHUDRoundedRectangle::renderRuntime() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  GLfloat matrix[16]; 
  float mScreenAspectRatio = mScreen->getAspectRatio();
  
  // Get the current scaling
  glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
  float mYCornerSize = (cCornerSize / matrix[5]) * 2.0f;
  float mXCornerSize = (cCornerSize / (matrix[0] / mScreenAspectRatio)) * 2.0f;
  float mXStartCorner = -1.0f + mXCornerSize / mScreenAspectRatio;
  float mYStartCorner = -1.0f + mYCornerSize;
  float mXEndCorner = 1.0f - mXCornerSize / mScreenAspectRatio;
  float mYEndCorner = 1.0f - mYCornerSize;
  cCornerTexture->set();
  glEnable(GL_BLEND);
  glColor4f(0.2f, 0.2f, 0.2f, 0.5f);
  glBegin(GL_QUADS);
  renderCorner(-1.0f,       -1.0f,        mXStartCorner, mYStartCorner,  1.0f,  1.0f);
  renderCorner(-1.0f,        mYEndCorner, mXStartCorner, 1.0f,           1.0f, -1.0f);
  renderCorner(mXEndCorner, -1.0f,        1.0f,          mYStartCorner, -1.0f,  1.0f);
  renderCorner(mXEndCorner,  mYEndCorner, 1.0f,          1.0f,          -1.0f, -1.0f);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_QUADS);
  renderRectangle(-1.0f,          mYStartCorner, mXStartCorner, mYEndCorner);
  renderRectangle(mXStartCorner, -1.0f,          mXEndCorner,   1.0f);
  renderRectangle(mXEndCorner,    mYStartCorner, 1.0f,          mYEndCorner);
  glEnd();
  glDisable(GL_BLEND);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

bool ResourceElementHUDRoundedRectangle::renderSelectionHighlight() {
  return false;
}

void ResourceElementHUDRoundedRectangle::updateRuntime(unsigned int) {
  // Nothing to do
}

void ResourceElementHUDRoundedRectangle::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resourceLocator, BlockLocation& blockLocation) {
  // TODO: Save colour and roundiness
}

ResourceElementHUDRoundedRectangle::~ResourceElementHUDRoundedRectangle() {
  // TODO: Unbind the texture, etc.
}

float ResourceElementHUDRoundedRectangle::getWest() {
  return -0.5f;
}

float ResourceElementHUDRoundedRectangle::getEast() {
  return 0.5f;
}

float ResourceElementHUDRoundedRectangle::getSouth() {
  return -0.5f;
}

float ResourceElementHUDRoundedRectangle::getNorth() {
  return 0.5f;
}

float ResourceElementHUDRoundedRectangle::getTop() {
  return 0.0f;
}

float ResourceElementHUDRoundedRectangle::getBottom() {
  return 0.0f;
}
