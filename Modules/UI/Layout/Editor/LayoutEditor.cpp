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
#include "LayoutEditor.h"

#include "Modules/UI/Layout/Layout.h"

namespace IsoRealms::UI {
  LayoutEditor::LayoutEditor(Layout* layout) :
            cLayout(layout),
            cSelectedComponent(nullptr),
            cZoomFactor(1.0f),
            cPanX(0.0f),
            cPanY(0.0f),
            cDragging(false) {
  }

  void LayoutEditor::renderScreen(float scale, float aspectRatio) const {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();
    glDisable(GL_DEPTH_TEST);

    glBindTexture(GL_TEXTURE_2D, 0);
    glLoadIdentity();

    glTranslatef(cPanX.animation(), cPanY.animation(), 0.0f);

    // Grid
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_POINTS);
    for (float x = -1.0f; x < 1.0f; x += 0.02f) {
      for (float y = -1.0f; y < 1.0f; y += 0.02f) {
        glVertex2f(x, y);
      }
    }
    glEnd();
    
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 255);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(-1.0f,  1.0f);
    glVertex2f( 1.0f,  1.0f);
    glVertex2f( 1.0f, -1.0f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    glPushMatrix();    
    cLayout->renderEditing(aspectRatio);
    glPopMatrix();
    if (cSelectedComponent != nullptr) {
      cSelectedComponent->renderSelectionHighlight(aspectRatio, cZoomFactor.animation());
    }  
    glLoadIdentity();  
    glEnable(GL_DEPTH_TEST);
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
  }
  
  bool LayoutEditor::renderAssetIcon() const {
    return false; // TODO: What's this used for?
  }

  void LayoutEditor::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

//   void LayoutEditor::updateScreen(unsigned int milliseconds) {
//     cZoomFactor.update(milliseconds);
//     cPanX.update(milliseconds);
//     cPanY.update(milliseconds);
//   }

  bool LayoutEditor::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::MouseButtonPressed: {
//         switch (event.button.button) {
//           case SDL_BUTTON_LEFT: {
//             ScreenRay mPickRay = getPickRay(event.button.x, event.button.y);
//             if (cSelectedComponent != nullptr && cSelectedComponent->inputEditor(event, this, mPickRay.cNear.x, mPickRay.cNear.y, cZoomFactor.animation())) {
//               return true;
//             }
//             cSelectedComponent = cLayout->pickComponent(mPickRay.cNear.x, mPickRay.cNear.y);
//             glPopMatrix();
//             if (cSelectedComponent == nullptr) {
//               cDragging = true;
//             }
//             return true;
//           }
//
//           case SDL_BUTTON_WHEELUP: {
            // IApplication* mApplication = project->getApplication();
            // ScreenLocation mLocation = mApplication->normalise(event.button.x, event.button.y);
            // cPanY = cPanY.value() - (mLocation.getY() / cZoomFactor.value()) * 0.1;
            // cPanX = cPanX.value() - ((mLocation.getX() / cZoomFactor.value()) * 0.1);
            // cZoomFactor = cZoomFactor.value() * 1.1;
//             return true;
//           }
//
//           case SDL_BUTTON_WHEELDOWN: {
            // IApplication* mApplication = project->getApplication();
            // ScreenLocation mLocation = mApplication->normalise(event.button.x, event.button.y);
            // cZoomFactor = cZoomFactor.value() / 1.1;
            // cPanY = cPanY.value() + (mLocation.getY() / cZoomFactor.value()) * 0.1;
            // cPanX = cPanX.value() + ((mLocation.getX() / cZoomFactor.value()) * 0.1);
//             return true;
//           }
//         }
//         break;
      }
//
      case sf::Event::MouseButtonReleased: {
        if (cDragging) {
          cDragging = false;
        }
        return true;
      }

      case sf::Event::MouseMoved: {
//         ScreenRay mPickRay = getPickRay(event.button.x, event.button.y);
//         if (cSelectedComponent != nullptr && cSelectedComponent->inputEditor(event, this, mPickRay.cNear.x, mPickRay.cNear.y, cZoomFactor.animation())) {
//           // Nothing to do.
//         } else if (cDragging) {
//           IApplication* mApplication = project->getApplication();
//           float mXPan = mApplication->normalise(event.motion.xrel);
//           float mYPan = mApplication->normalise(event.motion.yrel);
//           cPanX = cPanX.value() + mXPan / cZoomFactor.value();
//           cPanY = cPanY.value() - mYPan / cZoomFactor.value();
//         }
//         return true;
      }

      default: break;
    }
    return false;
  }

  void LayoutEditor::notifyVisible() {
    // Nothing to do.
  }
  
  void LayoutEditor::notifyHidden() {
    // Nothing to do.
  }
  
  void LayoutEditor::notifyLostFocus() {
    // Nothing to do.
  }

  std::vector<std::string> LayoutEditor::getDigitalInputs() const {
    return std::vector<std::string>();
  }

  std::vector<std::string> LayoutEditor::getAnalogueInputs() const {
    return std::vector<std::string>();
  }

  void LayoutEditor::setDigitalInput(const std::string& name, IBoolean* input) {
    // Nothing to do.
  }

  void LayoutEditor::setAnalogueInput(const std::string& name, IFloat* input) {
    // Nothing to do.
  }

  void LayoutEditor::setExitAction(IAction* action) {
    // Nothing to do.
  }

  void LayoutEditor::setAppearance(IFont* font, float scale) {
    // Nothing to do.
  }

  void LayoutEditor::unregisterAssets(IAssetRemover* assets) {
    // Nothing to do.
  }

  IScreen* LayoutEditor::screen() {
    return nullptr; // TODO
  }
}
