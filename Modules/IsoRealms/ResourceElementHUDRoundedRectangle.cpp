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

ResourceElementHUDRoundedRectangle::ResourceElementHUDRoundedRectangle(IElementRelationManager* manager, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) : ResourceElementHUDAbstract(manager) {
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

IElement* ResourceElementHUDRoundedRectangle::createHUDElement(DOMNodeWrapper* node, DOMNodeWrapper* cache, BlockLocation* location, IResourceAccessor* resources, bool asTemplate, HUDComponentPosition* elementTransformer) {
  return new ElementHUDRoundedRectangle(this, elementTransformer);
}

void ResourceElementHUDRoundedRectangle::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  // TODO: Initialise.
}

void ResourceElementHUDRoundedRectangle::save(DOMNodeWriter*, DOMNodeWriter*, IResourceLocator*) {
  // TODO
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

ResourceElementHUDRoundedRectangle::~ResourceElementHUDRoundedRectangle() {
  // TODO: Unbind the texture, etc.
}

IElementType* ResourceElementHUDRoundedRectangle::getElementType() {
  return this;
}

float ResourceElementHUDRoundedRectangle::getCornerSize() {
  return cCornerSize;
}

void ResourceElementHUDRoundedRectangle::setCornerTexture() {
  cCornerTexture->set();
}
