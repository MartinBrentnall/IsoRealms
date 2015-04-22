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
#include "ResourceTexturesSpindizzyLiftsC64.h"

const float ResourceTexturesSpindizzyLiftsC64::CIRCLE_RESOLUTION            = 5.0f * (M_PI / 180.0);
const float ResourceTexturesSpindizzyLiftsC64::CIRCLE_OUTLINE_OUTER         = 1.0f;
const float ResourceTexturesSpindizzyLiftsC64::CIRCLE_OUTLINE_INNER         = 0.6f;
const float ResourceTexturesSpindizzyLiftsC64::CIRCLE_COLOUR_OUTER          = 0.9f;
const float ResourceTexturesSpindizzyLiftsC64::CIRCLE_COLOUR_INNER          = 0.7f;
const float ResourceTexturesSpindizzyLiftsC64::SQUARE_OUTLINE_OUTER         = 1.0f;
const float ResourceTexturesSpindizzyLiftsC64::SQUARE_OUTLINE_INNER         = 0.51f;
const float ResourceTexturesSpindizzyLiftsC64::SQUARE_COLOUR_OUTER          = 0.87f;
const float ResourceTexturesSpindizzyLiftsC64::SQUARE_COLOUR_INNER          = 0.64f;
const float ResourceTexturesSpindizzyLiftsC64::SQUARE_TRIANGLE_OUTER        = 0.2f;
const float ResourceTexturesSpindizzyLiftsC64::SQUARE_TRIANGLE_INNER        = 0.0f;
const float ResourceTexturesSpindizzyLiftsC64::DIAMOND_OUTLINE_OUTER        = 1.0f;
const float ResourceTexturesSpindizzyLiftsC64::DIAMOND_OUTLINE_INNER        = 0.6f;
const float ResourceTexturesSpindizzyLiftsC64::DIAMOND_COLOUR_OUTER         = 0.9f;
const float ResourceTexturesSpindizzyLiftsC64::DIAMOND_COLOUR_INNER         = 0.7f;
const float ResourceTexturesSpindizzyLiftsC64::DIAMOND_SQUARE_OUTLINE_OUTER = 0.70f;
const float ResourceTexturesSpindizzyLiftsC64::DIAMOND_SQUARE_OUTLINE_INNER = 0.40f;
const float ResourceTexturesSpindizzyLiftsC64::DIAMOND_SQUARE_COLOUR_OUTER  = 0.62f;
const float ResourceTexturesSpindizzyLiftsC64::DIAMOND_SQUARE_COLOUR_INNER  = 0.48f;

const std::string ResourceTexturesSpindizzyLiftsC64::LIFT_CIRCLE_BOTH  = "LiftCircleFilled";
const std::string ResourceTexturesSpindizzyLiftsC64::LIFT_CIRCLE_ONE   = "LiftCircleOne";
const std::string ResourceTexturesSpindizzyLiftsC64::LIFT_CIRCLE_NONE  = "LiftCircleEmpty";
const std::string ResourceTexturesSpindizzyLiftsC64::LIFT_SQUARE_BOTH  = "LiftSquareFilled";
const std::string ResourceTexturesSpindizzyLiftsC64::LIFT_SQUARE_ONE   = "LiftSquareOne";
const std::string ResourceTexturesSpindizzyLiftsC64::LIFT_SQUARE_NONE  = "LiftSquareEmpty";
const std::string ResourceTexturesSpindizzyLiftsC64::LIFT_DIAMOND_BOTH = "LiftDiamondFilled";
const std::string ResourceTexturesSpindizzyLiftsC64::LIFT_DIAMOND_ONE  = "LiftDiamondOne";
const std::string ResourceTexturesSpindizzyLiftsC64::LIFT_DIAMOND_NONE = "LiftDiamondEmpty";

ResourceTexturesSpindizzyLiftsC64::ResourceTexturesSpindizzyLiftsC64(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  initialise();
  generateTextures();
}

std::string ResourceTexturesSpindizzyLiftsC64::stripMemberName(const std::string& name) {
  return name.substr(0, name.rfind('_'));
}

void ResourceTexturesSpindizzyLiftsC64::initialise() {
  cColour1 = new Colour(1.0f, 1.0f, 1.0f);
  cColour2 = new Colour(0.7f, 0.7f, 0.7f);
  cOutlineColour = new Colour(0.0f, 0.0f, 0.0f);

  TRANSPARENT = new Colour(0.0, 0.0, 0.0, 0.0);

  cTextures[LIFT_CIRCLE_ONE]   = new Texture();
  cTextures[LIFT_CIRCLE_NONE]  = new Texture();
  cTextures[LIFT_CIRCLE_BOTH]  = new Texture();
  cTextures[LIFT_SQUARE_ONE]   = new Texture();
  cTextures[LIFT_SQUARE_NONE]  = new Texture();
  cTextures[LIFT_SQUARE_BOTH]  = new Texture();
  cTextures[LIFT_DIAMOND_ONE]  = new Texture();
  cTextures[LIFT_DIAMOND_NONE] = new Texture();
  cTextures[LIFT_DIAMOND_BOTH] = new Texture();
}

/* Generation functions */
void ResourceTexturesSpindizzyLiftsC64::clear(IColour* colour) {
  glClearColor(colour->getRed(), colour->getGreen(), colour->getBlue(), colour->getAlpha());
  glClear(GL_COLOR_BUFFER_BIT);
}

void ResourceTexturesSpindizzyLiftsC64::renderCircle(float radius, IColour* colour) {
  glBegin(GL_TRIANGLE_FAN);
  colour->set();
  glVertex2f(0.0f, 0.0f);
  float mStartAngle = 0.0f * (M_PI / 180.0f);
  float mEndAngle = 360.0f * (M_PI / 180.0f);
  for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
    glVertex2f(sin(angle) * radius, cos(angle) * radius);
  }
  glEnd();
}

void ResourceTexturesSpindizzyLiftsC64::renderCircle(float outerRadius, float innerRadius, IColour* colour) {
  glBegin(GL_TRIANGLE_STRIP);
  colour->set();
  float mStartAngle = 0.0f * (M_PI / 180.0f);
  float mEndAngle = 360.0f * (M_PI / 180.0f);
  for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
    glVertex2f(sin(angle) * innerRadius, cos(angle) * innerRadius);
    glVertex2f(sin(angle) * outerRadius, cos(angle) * outerRadius);
  }
  glEnd();
}

void ResourceTexturesSpindizzyLiftsC64::renderSquare(float outer, float inner, IColour* colour) {
  colour->set();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex2f(-outer, -outer);
  glVertex2f(-inner, -inner);
  glVertex2f(-outer,  outer);
  glVertex2f(-inner,  inner);
  glVertex2f( outer,  outer);
  glVertex2f( inner,  inner);
  glVertex2f( outer, -outer);
  glVertex2f( inner, -inner);
  glVertex2f(-outer, -outer);
  glVertex2f(-inner, -inner);
  glEnd();
}

void ResourceTexturesSpindizzyLiftsC64::renderSquareHalf(float outer, float inner, IColour* colour) {
  colour->set();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex2f(-outer, -outer);
  glVertex2f(-inner, -inner);
  glVertex2f(-outer,  outer);
  glVertex2f(-inner,  inner);
  glVertex2f( outer,  outer);
  glVertex2f( inner,  inner);
/*  glVertex2f( outer, -outer);
  glVertex2f( inner, -inner);
  glVertex2f(-outer, -outer);
  glVertex2f(-inner, -inner);*/
  glEnd();
}

void ResourceTexturesSpindizzyLiftsC64::renderDiamond(float outer, float inner, IColour* colour) {
  colour->set();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex2f(-inner,  0.0f);
  glVertex2f(-outer,  0.0f);
  glVertex2f(0.0f,   -inner);
  glVertex2f(0.0f,   -outer);
  glVertex2f( inner,  0.0f);
  glVertex2f( outer,  0.0f);
  glVertex2f(0.0f,    inner);
  glVertex2f(0.0f,    outer);
  glVertex2f(-inner,  0.0f);
  glVertex2f(-outer,  0.0f);
  glEnd();
}

void ResourceTexturesSpindizzyLiftsC64::renderDiamondHalf(float outer, float inner, IColour* colour) {
  colour->set();
  glBegin(GL_TRIANGLE_STRIP);
  glVertex2f(-inner,  0.0f);
  glVertex2f(-outer,  0.0f);
  glVertex2f(0.0f,   -inner);
  glVertex2f(0.0f,   -outer);
  glVertex2f( inner,  0.0f);
  glVertex2f( outer,  0.0f);
  glVertex2f(0.0f,    inner);
  glVertex2f(0.0f,    outer);
  glEnd();
}

void ResourceTexturesSpindizzyLiftsC64::renderDiamondEdges(float outer, float edge, IColour* colour) {
  colour->set();
  glBegin(GL_TRIANGLES);
  glVertex2f(-outer,         0.0f);
  glVertex2f(-edge,         -outer + edge);
  glVertex2f(-edge,          outer - edge);
  glVertex2f( 0.0f,         -outer);
  glVertex2f( outer - edge, -edge);
  glVertex2f(-outer + edge, -edge);
  glVertex2f( outer,         0.0f);
  glVertex2f( edge,          outer - edge);
  glVertex2f( edge,         -outer + edge);
  glVertex2f( 0.0f,          outer);
  glVertex2f(-outer + edge,  edge);
  glVertex2f( outer - edge,  edge);
  glEnd();
}

void ResourceTexturesSpindizzyLiftsC64::renderLiftSquare() {
  clear(TRANSPARENT);
  renderSquare(SQUARE_OUTLINE_OUTER, SQUARE_OUTLINE_INNER, cOutlineColour);
  renderSquare(SQUARE_COLOUR_OUTER, SQUARE_COLOUR_INNER, cColour2);
}

void ResourceTexturesSpindizzyLiftsC64::renderLiftSquareHalf() {
  renderLiftSquare();
  glBegin(GL_TRIANGLES);
  cOutlineColour->set();
  glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_COLOUR_INNER);
  glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_TRIANGLE_INNER);
  glVertex2f(-SQUARE_TRIANGLE_INNER,  SQUARE_COLOUR_INNER);
  cColour1->set();
  glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_COLOUR_INNER);
  glVertex2f(-SQUARE_COLOUR_INNER,    SQUARE_TRIANGLE_OUTER);
  glVertex2f(-SQUARE_TRIANGLE_OUTER,  SQUARE_COLOUR_INNER);
  glEnd();
}

void ResourceTexturesSpindizzyLiftsC64::renderLiftSquareBoth() {
  renderLiftSquareHalf();
  glBegin(GL_TRIANGLES);
  cOutlineColour->set();
  glVertex2f(SQUARE_COLOUR_INNER,   -SQUARE_COLOUR_INNER);
  glVertex2f(SQUARE_COLOUR_INNER,   -SQUARE_TRIANGLE_INNER);
  glVertex2f(SQUARE_TRIANGLE_INNER, -SQUARE_COLOUR_INNER);
  cColour1->set();
  glVertex2f(SQUARE_COLOUR_INNER,   -SQUARE_COLOUR_INNER);
  glVertex2f(SQUARE_COLOUR_INNER,   -SQUARE_TRIANGLE_OUTER);
  glVertex2f(SQUARE_TRIANGLE_OUTER, -SQUARE_COLOUR_INNER);
  glEnd();
}

void ResourceTexturesSpindizzyLiftsC64::renderLiftDiamondBoth() {
  clear(TRANSPARENT);
  renderSquare(DIAMOND_SQUARE_OUTLINE_OUTER, DIAMOND_SQUARE_OUTLINE_INNER, cOutlineColour);
  renderSquare(DIAMOND_SQUARE_COLOUR_OUTER,  DIAMOND_SQUARE_COLOUR_INNER,  cColour1);
  renderDiamond(DIAMOND_OUTLINE_OUTER,       DIAMOND_OUTLINE_INNER,        cOutlineColour);
  renderDiamond(DIAMOND_COLOUR_OUTER,        DIAMOND_COLOUR_INNER,         cColour2);
}

void ResourceTexturesSpindizzyLiftsC64::renderLiftDiamondHalf() {
  renderLiftDiamond();
  renderDiamondHalf(DIAMOND_OUTLINE_OUTER,       DIAMOND_OUTLINE_INNER,        cOutlineColour);
  renderDiamondHalf(DIAMOND_COLOUR_OUTER,        DIAMOND_COLOUR_INNER,         cColour2);
  renderSquareHalf(DIAMOND_SQUARE_OUTLINE_OUTER, DIAMOND_SQUARE_OUTLINE_INNER, cOutlineColour);
  renderSquareHalf(DIAMOND_SQUARE_COLOUR_OUTER,  DIAMOND_SQUARE_COLOUR_INNER,  cColour1);
}

void ResourceTexturesSpindizzyLiftsC64::renderLiftDiamond() {
  clear(TRANSPARENT);
  renderDiamondEdges(DIAMOND_OUTLINE_OUTER,  DIAMOND_SQUARE_COLOUR_OUTER,  cOutlineColour);
  renderDiamondEdges(DIAMOND_COLOUR_OUTER,   DIAMOND_SQUARE_COLOUR_OUTER,  cColour2);
  renderSquare(DIAMOND_SQUARE_OUTLINE_OUTER, DIAMOND_SQUARE_OUTLINE_INNER, cOutlineColour);
  renderSquare(DIAMOND_SQUARE_COLOUR_OUTER,  DIAMOND_SQUARE_COLOUR_INNER,  cColour1);
}

void ResourceTexturesSpindizzyLiftsC64::renderLiftCircle() {
  clear(TRANSPARENT);
  renderCircle(CIRCLE_OUTLINE_OUTER, CIRCLE_OUTLINE_INNER, cOutlineColour);
  renderCircle(CIRCLE_COLOUR_OUTER, CIRCLE_COLOUR_INNER, cColour1);
}

void ResourceTexturesSpindizzyLiftsC64::renderLiftCircleHalf() {
  renderLiftCircle();
  glBegin(GL_TRIANGLE_FAN);
  cColour2->set();
  glVertex2f(0.0f, 0.0f);
  float mStartAngle = 45.0f * (M_PI / 180.0f);
  float mEndAngle = 225.0f * (M_PI / 180.0f);
  for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
    glVertex2f(sin(angle) * CIRCLE_COLOUR_INNER, cos(angle) * CIRCLE_COLOUR_INNER);
  }
  glEnd();
}

void ResourceTexturesSpindizzyLiftsC64::renderLiftCircleBoth() {
  clear(TRANSPARENT);
  renderCircle(CIRCLE_OUTLINE_OUTER, cOutlineColour);
  renderCircle(CIRCLE_COLOUR_OUTER, cColour1);
  renderCircle(CIRCLE_COLOUR_INNER, cColour2);
}

void ResourceTexturesSpindizzyLiftsC64::generateTextures() {
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();

  cTextures[LIFT_CIRCLE_ONE]->setRenderTarget();   renderLiftCircleHalf();
  cTextures[LIFT_CIRCLE_NONE]->setRenderTarget();  renderLiftCircle();
  cTextures[LIFT_CIRCLE_BOTH]->setRenderTarget();  renderLiftCircleBoth();
  cTextures[LIFT_SQUARE_ONE]->setRenderTarget();   renderLiftSquareHalf();
  cTextures[LIFT_SQUARE_NONE]->setRenderTarget();  renderLiftSquare();
  cTextures[LIFT_SQUARE_BOTH]->setRenderTarget();  renderLiftSquareBoth();
  cTextures[LIFT_DIAMOND_ONE]->setRenderTarget();  renderLiftDiamondHalf();
  cTextures[LIFT_DIAMOND_NONE]->setRenderTarget(); renderLiftDiamond();
  cTextures[LIFT_DIAMOND_BOTH]->setRenderTarget(); renderLiftDiamondBoth();

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

void ResourceTexturesSpindizzyLiftsC64::destroyTextures() {
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    delete i->second;
  }
  for (std::map<std::string, GLuint>::iterator i = cTextureIDs.begin(); i != cTextureIDs.end(); i++) {
    glDeleteTextures(1, &(i->second));
  }
  cTextureIDs.clear();
}

void ResourceTexturesSpindizzyLiftsC64::resourceChanged(IColour* colour) {
  if (colour == cColour1 || colour == cColour2 || colour == cOutlineColour) {
    generateTextures();
  }
}

void ResourceTexturesSpindizzyLiftsC64::resourcePendingDestruction(IColour* destroyee, IColour* replacement) {
  bool mChanged = false;
  if (destroyee == cColour1)       {cColour1 = replacement;       mChanged = true;}
  if (destroyee == cColour2)       {cColour2 = replacement;       mChanged = true;}
  if (destroyee == cOutlineColour) {cOutlineColour = replacement; mChanged = true;}
  if (mChanged) {
    generateTextures();
  }
}

ITexture* ResourceTexturesSpindizzyLiftsC64::getTexture(const std::string& name) {
  std::map<std::string, Texture*>::iterator i = cTextures.find(name);
  return i != cTextures.end() ? i->second : cTextures.begin()->second;
}

void ResourceTexturesSpindizzyLiftsC64::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  cColour1 = resourceAccessor->getColour(node->getAttribute("colour1"));
  cColour2 = resourceAccessor->getColour(node->getAttribute("colour2"));
  cOutlineColour = resourceAccessor->getColour(node->getAttribute("outline"));
  generateTextures();
}

void ResourceTexturesSpindizzyLiftsC64::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  std::string mResourceName = resourceLocator->getPath(cTextures[LIFT_DIAMOND_NONE]);
  mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
  mResourceName = mResourceName.substr(0, mResourceName.find_last_of('_'));
  node->addAttribute("name", mResourceName);
  node->addAttribute("colour1", resourceLocator->getPath(cColour1));
  node->addAttribute("colour2", resourceLocator->getPath(cColour2));
  node->addAttribute("outline", resourceLocator->getPath(cOutlineColour));
}

IColour* ResourceTexturesSpindizzyLiftsC64::getColour1() {
  return cColour1;
}

IColour* ResourceTexturesSpindizzyLiftsC64::getColour2() {
  return cColour2;
}

IColour* ResourceTexturesSpindizzyLiftsC64::getColourOutline() {
  return cOutlineColour;
}

void ResourceTexturesSpindizzyLiftsC64::setColour1(IColour* colour) {
  cColour1 = colour;
  generateTextures();
}

void ResourceTexturesSpindizzyLiftsC64::setColour2(IColour* colour) {
  cColour2 = colour;
  generateTextures();
}

void ResourceTexturesSpindizzyLiftsC64::setColourOutline(IColour* colour) {
  cOutlineColour = colour;
  generateTextures();
}

std::vector<ITexture*> ResourceTexturesSpindizzyLiftsC64::getResources() {
  std::vector<ITexture*> mTextures;
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    mTextures.push_back(i->second);
  }
  return mTextures;
}

std::vector<std::string> ResourceTexturesSpindizzyLiftsC64::getResourceNames() {
  std::vector<std::string> mNames;
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    mNames.push_back(i->first);
  }
  return mNames;
}

ITexture* ResourceTexturesSpindizzyLiftsC64::getResource(const std::string& name) {
  return cTextures[name];
}

bool ResourceTexturesSpindizzyLiftsC64::contains(ITexture* texture) {
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    if (texture == i->second) {
      return true;
    }
  }
  return false;
}

void ResourceTexturesSpindizzyLiftsC64::addTextures(IResourceRegistry* runtimeContext, const std::string& name) {
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    runtimeContext->add(i->second, name + "_" + i->first);
  }
}

void ResourceTexturesSpindizzyLiftsC64::deleteResources(IResourceInstanceListener<ITexture>* listener) {
  for (std::map<std::string, Texture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    listener->resourcePendingDestruction(i->second);
  }
}

ResourceTexturesSpindizzyLiftsC64::~ResourceTexturesSpindizzyLiftsC64() {
  delete TRANSPARENT;
  destroyTextures();
}
