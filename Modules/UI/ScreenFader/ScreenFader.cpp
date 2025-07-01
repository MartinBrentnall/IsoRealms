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
#include "ScreenFader.h"

namespace IsoRealms::UI {
  ScreenFader::ScreenFader(IProject& project, UI& ui, IResourceData& data) :
            cProject(project),
            cDefScreenA(data),
            cDefScreenB(data),
            cDefTransition(data),
            cRuntimeScreenA(project, true, true),
            cRuntimeScreenB(project, true, true) {
  }
  
  ScreenFader::ScreenFader(IProject& project, UI& ui, IResourceData& data, JSONObject object, IOptions& options) :
            ScreenFader(project, ui, data) {
    cDefScreenA.init(object, JSON_SCREEN_A);
    cDefScreenB.init(object, JSON_SCREEN_B);
    cDefTransition.init(object, JSON_TRANSITION);
  }

  void ScreenFader::registerAssets(IAssetRegistry& assets) {
    assets.add(this, "", "Screen Faders");
  }
  
  void ScreenFader::save(JSONObject object) const {
    cDefScreenA.save(object, JSON_SCREEN_A);
    cDefScreenB.save(object, JSON_SCREEN_B);
    cDefTransition.save(object, JSON_TRANSITION);
  }

  void ScreenFader::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool ScreenFader::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> ScreenFader::getProperties(IResourceData& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Screen>>("Screen A",   "TODO", cDefScreenA));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Screen>>("Screen B",   "TODO", cDefScreenB));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Float>>( "Transition", "TODO", cDefTransition));
    return mProperties;
  }

  void ScreenFader::renderScreen(float scale, float aspectRatio) const {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();

    cRuntimeScreenA.setRenderTarget();
    cDefScreenA->renderScreen(scale, aspectRatio);    
    cRuntimeScreenB.setRenderTarget();
    cDefScreenB->renderScreen(scale, aspectRatio);
    
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    IApplication& mApplication = cProject.getApplication();
    mApplication.setViewPort();

    // Draw Scene A
    cRuntimeScreenA.set();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1, 0); glVertex2f( 1.0f, -1.0f);
    glTexCoord2f(1, 1); glVertex2f( 1.0f,  1.0f);
    glTexCoord2f(0, 1); glVertex2f(-1.0f,  1.0f);
    glEnd();

    // Enable blending and draw Scene B over it
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 1.0f, 1.0f, cDefTransition->getValue());

    cRuntimeScreenB.set();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1, 0); glVertex2f( 1.0f, -1.0f);
    glTexCoord2f(1, 1); glVertex2f( 1.0f,  1.0f);
    glTexCoord2f(0, 1); glVertex2f(-1.0f,  1.0f);
    glEnd();

    glDisable(GL_BLEND);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  }

  bool ScreenFader::renderAssetIcon() const {
    return false;
  }

  void ScreenFader::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> ScreenFader::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ScreenFader::isDefaultConfiguration() const {
    return true;
  }

  const std::string ScreenFader::JSON_SCREEN_A   = "screenA";
  const std::string ScreenFader::JSON_SCREEN_B   = "screenB";
  const std::string ScreenFader::JSON_TRANSITION = "transition";
}
