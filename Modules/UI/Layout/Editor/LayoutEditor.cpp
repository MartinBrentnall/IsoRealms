/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "LayoutEditor.h"

#include "Modules/UI/Layout/Layout.h"
#include "Modules/UI/UI.h"

namespace IsoRealms::UI {
  LayoutEditor::LayoutEditor(Layout& layout, IDialogManager& dialogManager) :
            cHatHandler(layout.getUI().getProject().getApplication().getHatHandler()),
            cAnalogueInputsByName({
              {"MoveViewIn",      &cDistanceInSpeed},
              {"MoveViewOut",     &cDistanceOutSpeed},
              {"MoveCursorX",     &cXSpeed},
              {"MoveCursorY",     &cYSpeed},
              {"MoveCursorZ",     &cZSpeed},
              {"RotateViewPitch", &cPitchSpeed},
              {"RotateViewYaw",   &cYawSpeed}
            }),
            cDigitalInputsByName({
              {"MoveCursorBackward", &cActiveDown},
              {"MoveCursorDown",     &cActiveLower},
              {"MoveCursorFaster",   &cActiveFast},
              {"MoveCursorForward",  &cActiveUp},
              {"MoveCursorLeft",     &cActiveLeft},
              {"MoveCursorRight",    &cActiveRight},
              {"MoveCursorSlower",   &cActiveSlow},
              {"MoveCursorUp",       &cActiveHigher},
              {"MoveView",           &cZoomingView},
              {"RotateView",         &cRotatingView},
              {"Cancel",             &cCancel},
              {"ConfigureTool",      &cConfigureTool},
              {"NextTheme",          &cNextTheme},
              {"NextTool",           &cNextTool},
              {"PreviousTheme",      &cPreviousTheme},
              {"PreviousTool",       &cPreviousTool},
              {"ToolMode",           &cToolMode},
              {"UseTool",            &cUseTool},
              {"Exit",               &cExit},
            }),
            cActiveLeft(*this, SignalInputID::MOVE_CURSOR_LEFT),
            cActiveRight(*this, SignalInputID::MOVE_CURSOR_RIGHT),
            cActiveUp(*this, SignalInputID::MOVE_CURSOR_FORWARD),
            cActiveDown(*this, SignalInputID::MOVE_CURSOR_BACKWARD),
            cActiveHigher(*this, SignalInputID::MOVE_CURSOR_UP),
            cActiveLower(*this, SignalInputID::MOVE_CURSOR_DOWN),
            cActiveSlow(*this, SignalInputID::MOVE_CURSOR_SLOWER),
            cActiveFast(*this, SignalInputID::MOVE_CURSOR_FASTER),
            cRotatingView(*this, SignalInputID::ROTATE_VIEW),
            cZoomingView(*this, SignalInputID::MOVE_VIEW),
            cCancel(*this, SignalInputID::CANCEL),
            cConfigureTool(*this, SignalInputID::CONFIGURE_TOOL),
            cNextTheme(*this, SignalInputID::NEXT_THEME),
            cNextTool(*this, SignalInputID::NEXT_TOOL),
            cPreviousTheme(*this, SignalInputID::PREVIOUS_THEME),
            cPreviousTool(*this, SignalInputID::PREVIOUS_TOOL),
            cToolMode(*this, SignalInputID::TOOL_MODE),
            cUseTool(*this, SignalInputID::USE_TOOL),
            cExit(*this, SignalInputID::EXIT),
            cSignalConsumed(false),
            cHasFocus(true),
            cDrawTool(*this),
            cPropertiesTool(*this),
            cLocationTool(*this),
            cOffsetTool(*this),
            cDeleteTool(*this),
            cOrderingTool(*this),
            cCopyTool(*this),
            cLayout(layout),
            cSelectedComponent(nullptr),
            cToolbar({&cDrawTool, &cPropertiesTool, &cLocationTool, &cOffsetTool, &cDeleteTool, &cOrderingTool, &cCopyTool}, [this](ILayoutTool* tool) {
              cSelectedTool = tool;
            }, [this](ILayoutTool* tool) {
              tool->renderIcon();
            }),
            cSelectedTool(&cDrawTool),
            cCursorSpeedX(0.0f),
            cCursorSpeedY(0.0f),
            cZoomSpeed(0.0f),
            cZoomFactorStep(0),
            cZoomFactor(1.0f),
            cPanX(0.0f),
            cPanY(0.0f),
            cDragging(false),
            cSelectionOverride(false),
            cCycleSelection(false),
            cDrawComponent(false),
            cAspectRatio(1.0f / layout.getUI().getProject().getApplication().getScreenAspectRatio()),
            cEditingProperties(false),
            cPropertiesUI(layout.getUI().getProject(), *this, [this]() {
              cEditingProperties = false;
            }, [](IEditable* editor) {
              std::cout << "WARNING: LayoutEditor::LayoutEditor: This UI does not support editables." << std::endl;
            }),
            cFont(nullptr),
            cFontSize(0.03f),
            cDrawingStarted(false),
            cGridX(0.025f),
            cGridY(0.025f),
            cHandleLocationFunctions{
              {Handle::WEST,      [this]() {return Point2D(cSelectedComponentLeft  - cEditHandleRadius,               (cSelectedComponentBottom + cSelectedComponentTop) / 2.0f);}},
              {Handle::EAST,      [this]() {return Point2D(cSelectedComponentRight + cEditHandleRadius,               (cSelectedComponentBottom + cSelectedComponentTop) / 2.0f);}},
              {Handle::SOUTH,     [this]() {return Point2D((cSelectedComponentLeft + cSelectedComponentRight) / 2.0f, cSelectedComponentBottom - cEditHandleRadius);}},
              {Handle::NORTH,     [this]() {return Point2D((cSelectedComponentLeft + cSelectedComponentRight) / 2.0f, cSelectedComponentTop    + cEditHandleRadius);}},
              {Handle::NORTHWEST, [this]() {return Point2D(cSelectedComponentLeft  - cEditHandleRadius,               cSelectedComponentTop    + cEditHandleRadius);}},
              {Handle::SOUTHWEST, [this]() {return Point2D(cSelectedComponentLeft  - cEditHandleRadius,               cSelectedComponentBottom - cEditHandleRadius);}},
              {Handle::SOUTHEAST, [this]() {return Point2D(cSelectedComponentRight + cEditHandleRadius,               cSelectedComponentBottom - cEditHandleRadius);}},
              {Handle::NORTHEAST, [this]() {return Point2D(cSelectedComponentRight + cEditHandleRadius,               cSelectedComponentTop    + cEditHandleRadius);}},
            } {
    cToolbar.selectToolRelative(0);
  }

  bool LayoutEditor::signal(SignalInputID id) {
    if (cHasFocus) {
      if (cEditingProperties) {
        switch (id) {
          case SignalInputID::MOVE_CURSOR_FORWARD:  cPropertiesUI.input(UISignalID::MOVE_UP);    break;
          case SignalInputID::MOVE_CURSOR_BACKWARD: cPropertiesUI.input(UISignalID::MOVE_DOWN);  break;
          case SignalInputID::MOVE_CURSOR_LEFT:     cPropertiesUI.input(UISignalID::MOVE_LEFT);  break;
          case SignalInputID::MOVE_CURSOR_RIGHT:    cPropertiesUI.input(UISignalID::MOVE_RIGHT); break;
          case SignalInputID::USE_TOOL:             cPropertiesUI.input(UISignalID::CONFIRM);    break;
          case SignalInputID::CANCEL:               cPropertiesUI.input(UISignalID::CANCEL);     break;
          default:                                                                               break;
        }
        return true;
      }
      
      if (!cSignalConsumed) {
        switch (id) {
          case SignalInputID::EXIT: if (cExitAction != nullptr) {cExitAction->execute();} return true;
          default:                  break;
        }
      }
    }
    return false;
  }

  void LayoutEditor::renderScreen(float scale, float aspectRatio) const {
    glPushMatrix();    
    glScalef(cZoomFactor.animation(), cZoomFactor.animation(), 0.0f);
    glTranslatef(cPanX.animation(), cPanY.animation(), 0.0f);

    // Grid
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_POINTS);
    renderGrid(-1.0f * cAspectRatio, -0.5f * cAspectRatio);
    renderGrid( 0.0f,                -0.5f * cAspectRatio);
    renderGrid( 0.0f,                 0.5f * cAspectRatio);
    renderGrid( 1.0f * cAspectRatio,  0.5f * cAspectRatio);
    glEnd();
    
    // Guidelines
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 255);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-1.0f * cAspectRatio, -1.0f);
    glVertex2f(-1.0f * cAspectRatio,  1.0f);
    glVertex2f( 1.0f * cAspectRatio,  1.0f);
    glVertex2f( 1.0f * cAspectRatio, -1.0f);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(-1.0f * cAspectRatio,  0.0f);
    glVertex2f( 1.0f * cAspectRatio,  0.0f);
    glVertex2f( 0.0f,                -1.0f);
    glVertex2f( 0.0f,                 1.0f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    // Render the layout itself.
    glPushMatrix();    
    cLayout.renderEditing(cAspectRatio);
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    // Render controller cursor.
    glColor3f(1.0f, 1.0f, 1.0f);
    Utils::renderCircle(-cPanX.animation(), -cPanY.animation(), 0.02f / cZoomFactor.animation());
    glColor3f(1.0f, 0.0f, 0.2f);
    Utils::renderCircle(-cPanX.animation(), -cPanY.animation(), 0.015f / cZoomFactor.animation());

    // Render selection highlight (if there is one).
    if (cSelectedComponent != nullptr) {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 255);
      glLineWidth(6.0f);
      glBegin(GL_LINE_LOOP);
      glColor3f(0.0f, 1.0f, 0.0f);
      glVertex2f(cSelectedComponentLeft,  cSelectedComponentBottom);
      glVertex2f(cSelectedComponentRight, cSelectedComponentBottom);
      glVertex2f(cSelectedComponentRight, cSelectedComponentTop);
      glVertex2f(cSelectedComponentLeft,  cSelectedComponentTop);
      glEnd();
      glDisable(GL_LINE_STIPPLE);
      glLineWidth(1.0f);

      cSelectedComponent->renderSelectionHighlight(cAspectRatio);
    }
    cSelectedTool->renderSelection();
    glPopMatrix();
    
    // Render preview of new component boundaries.
    if (cDrawingStarted) {
      glColor3f(0.0f, 1.0f, 1.0f);
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 255);
      glBegin(GL_LINE_LOOP);
      glVertex2f(cDrawingStartX, cDrawingStartY);
      glVertex2f(cDrawingStartX, cDrawingEndY);
      glVertex2f(cDrawingEndX,   cDrawingEndY);
      glVertex2f(cDrawingEndX,   cDrawingStartY);
      glEnd();
      glDisable(GL_LINE_STIPPLE);
    }
    
    // Render overlays.
    cToolbar.render(aspectRatio);
    cPropertiesUI.render(aspectRatio);
  }
  
  bool LayoutEditor::renderAssetIcon() const {
    return false; // TODO: What's this used for?
  }

  void LayoutEditor::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void LayoutEditor::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool LayoutEditor::isDefaultConfiguration() const {
    return true;
  }

  void LayoutEditor::updateScreen(unsigned int milliseconds) {
    if (cCursorSpeedX != 0.0f || cCursorSpeedY != 0) {
      cPanX += -cCursorSpeedX / cZoomFactor.animation();
      cPanY += -cCursorSpeedY / cZoomFactor.animation();
    }
    if (cZoomSpeed != 0.0f) {
      cZoomFactorStep = std::clamp(cZoomFactorStep + cZoomSpeed, ZOOM_LIMIT_MINIMUM, ZOOM_LIMIT_MAXIMUM);
      cZoomFactor = std::pow(2.0f, cZoomFactorStep / 5.0f);
    }
    for (std::pair<std::string, EditorDigitalInput<LayoutEditor, SignalInputID>*> mPair : cDigitalInputsByName) {
      if (mPair.second->triggerOnChange()) {
        cSignalConsumed = true;
      }
    }
    cSignalConsumed = false;
    cZoomFactor.update(milliseconds);
    cEditHandleRadius = EDIT_HANDLE_RADIUS / cZoomFactor.animation();
    float mShortestDistance = 0.05f / cZoomFactor.animation();
    cSelectedComponentClosestHandle = Handle::NONE;
    for (std::pair<Handle, std::function<Point2D()>>& mPair : cHandleLocationFunctions) {
      Point2D mHandleLocation = mPair.second();
      float mDistance = Utils::distance(-cPanX.animation(), -cPanY.animation(), mHandleLocation.getX(), mHandleLocation.getY());

      if (mDistance < mShortestDistance) {
        mShortestDistance = mDistance;
        cSelectedComponentClosestHandle = mPair.first;
      }
    }

    cPanX.update(milliseconds);
    cPanY.update(milliseconds);
    cPropertiesUI.update(milliseconds);
    cToolbar.update(milliseconds);
    cSelectedTool->update(milliseconds);
  }

  bool LayoutEditor::input(sf::Event& event) {
    if (cHasFocus) {
      if (cEditingProperties) {
        return cPropertiesUI.input(event);
      } else if (cToolbar.input(event)) {
        return true;
      } else if (cSelectedTool->input(event)) {
        return true;
      } else switch (event.type) {
        case sf::Event::MouseButtonPressed: {
          switch (event.mouseButton.button) {
            case sf::Mouse::Button::Left: {
              if (cDrawComponent) {
                Application& mApplication = cLayout.getUI().getProject().getApplication();
                Point2D mLocation = mApplication.normalise(event.mouseButton.x, event.mouseButton.y);
                float mX = mLocation.getX() / cZoomFactor.animation() - cPanX.animation();
                float mY = mLocation.getY() / cZoomFactor.animation() - cPanY.animation();
                cDrawingStarted = true;
                cDrawingStartX = snapX(mX);
                cDrawingStartY = snapY(mY);
                std::cout << "DRAW: " << cDrawingStartX << " , " << cDrawingStartY << std::endl;
              } else if (!cSelectionOverride) {
                Application& mApplication = cLayout.getUI().getProject().getApplication();
                Point2D mLocation = mApplication.normalise(event.mouseButton.x, event.mouseButton.y);
                float mX = mLocation.getX() / cZoomFactor.animation() - cPanX.animation();
                float mY = mLocation.getY() / cZoomFactor.animation() - cPanY.animation();
                if (cCycleSelection) {
                  selectComponent(cLayout.pickNextComponent(mX, mY, cAspectRatio, cSelectedComponent));
                } else {
                  if (cSelectedComponent != nullptr) {// TODO && cSelectedComponent->pickHandle(mX, mY, cZoomFactor.animation(), cAspectRatio)) {
                    cDragging = true;
                    cPreviousMouseX = event.mouseButton.x;
                    cPreviousMouseY = event.mouseButton.y;
                    return true;
                  }
                  selectComponent(cLayout.pickComponent(mX, mY, cAspectRatio));
                }
              }
              if (cSelectedComponent == nullptr || cSelectionOverride || cDrawComponent) {
                cDragging = true;
                cPreviousMouseX = event.mouseButton.x;
                cPreviousMouseY = event.mouseButton.y;
              }
              return true;
            }

            default: {
              break;
            }
          }
          break;
        }

        case sf::Event::MouseButtonReleased: {
          if (cDrawingStarted) {
            if (cDrawingStartX != cDrawingEndX && cDrawingStartY != cDrawingEndY) {
              cLayout.createComponent(cDrawingStartX, cDrawingStartY, cDrawingEndX, cDrawingEndY, cAspectRatio);
            }
            cDrawingStarted = false;
          }
          cDragging = false;
          return true;
        }

        case sf::Event::MouseWheelScrolled: {
          switch (event.mouseWheelScroll.wheel) {
            case sf::Mouse::VerticalWheel: {
              Application& mApplication = cLayout.getUI().getProject().getApplication();
              Point2D mLocation = mApplication.normalise(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
              Point2D mBeforeLocation = transform(mLocation);
              cZoomFactorStep = std::clamp(std::round(cZoomFactorStep) + static_cast<int>(std::ceil(event.mouseWheelScroll.delta)), ZOOM_LIMIT_MINIMUM, ZOOM_LIMIT_MAXIMUM);
              cZoomFactor = std::pow(2.0f, cZoomFactorStep / 5.0f);
              Point2D mAfterLocation  = transform(mLocation);
              cPanX -= mBeforeLocation.getX() - mAfterLocation.getX();
              cPanY -= mBeforeLocation.getY() - mAfterLocation.getY();
              return true;
            }

            default : {
              break;
            }
          }
          break;
        }

        case sf::Event::MouseMoved: {
          if (cDragging) {
            Application& mApplication = cLayout.getUI().getProject().getApplication();
            Point2D mLocation = mApplication.normalise(event.mouseMove.x, event.mouseMove.y);
            float mX = mLocation.getX() / cZoomFactor.animation() - cPanX.animation();
            float mY = mLocation.getY() / cZoomFactor.animation() - cPanY.animation();
            float mXSnap = snapX(mX);
            float mYSnap = snapY(mY);
            if (cDrawingStarted) {
              cDrawingEndX = mXSnap;
              cDrawingEndY = mYSnap;
            } else if (!cSelectionOverride && cSelectedComponent != nullptr) { // TODO && cSelectedComponent->move(mXSnap, mYSnap, cAspectRatio)) {
              // Nothing to do.
            } else {
              Application& mApplication = cLayout.getUI().getProject().getApplication();
              float mXPan = mApplication.normalise(event.mouseMove.x - cPreviousMouseX);
              float mYPan = mApplication.normalise(event.mouseMove.y - cPreviousMouseY);
              cPanX = cPanX.value() + mXPan / cZoomFactor.value();
              cPanY = cPanY.value() - mYPan / cZoomFactor.value();
              cPreviousMouseX = event.mouseMove.x;
              cPreviousMouseY = event.mouseMove.y;
            }
          }
          return true;
        }

        case sf::Event::KeyPressed: {
          switch (event.key.code) {
            case sf::Keyboard::LControl:
            case sf::Keyboard::RControl: {
              cSelectionOverride = true;
              break;
            }
            
            case sf::Keyboard::LAlt:
            case sf::Keyboard::RAlt: {
              cCycleSelection = true;
              break;
            }
            
            case sf::Keyboard::LShift:
            case sf::Keyboard::RShift: {
              cDrawComponent = true;
              break;
            }

            case sf::Keyboard::F1: {
              openProperties();
              break;
            }

            default: {
              break;
            }
          }
          return true;
        }

        case sf::Event::KeyReleased: {
          switch (event.key.code) {
            case sf::Keyboard::LControl:
            case sf::Keyboard::RControl: {
              cSelectionOverride = false;
              break;
            }
            
            case sf::Keyboard::LAlt:
            case sf::Keyboard::RAlt: {
              cCycleSelection = false;
              break;
            }
            
            case sf::Keyboard::LShift:
            case sf::Keyboard::RShift: {
              cDrawComponent = false;
              break;
            }

            default: {
              break;
            }
          }
          return true;
        }
        
        case sf::Event::JoystickMoved: {
          if (cHatHandler.leftPressed())  {cToolbar.selectToolRelative(-1); return true;}
          if (cHatHandler.rightPressed()) {cToolbar.selectToolRelative( 1); return true;}

          float mAmount = std::abs(event.joystickMove.position) < ANALOGUE_INPUT_DEAD_ZONE ? 0 : (event.joystickMove.position - (event.joystickMove.position < 0 ? -ANALOGUE_INPUT_DEAD_ZONE : ANALOGUE_INPUT_DEAD_ZONE)) * (100.0f / (100.0f - ANALOGUE_INPUT_DEAD_ZONE));
          switch (event.joystickMove.axis) {
            case 0: {
              cCursorSpeedX = mAmount / 5000.0;
              break;
            }
            
            case 1: {
              cCursorSpeedY = mAmount / -5000.0;
              break;
            }
            
            case 5: {
              cZoomSpeed = mAmount / -500.0f;
              break;
            }

            default: {
              break;
            }
          }
          return true;
        }

        case sf::Event::JoystickButtonPressed: {
          switch (event.joystickButton.button) {
            case 4: {
              selectComponent(cLayout.pickPreviousComponent(-cPanX.animation(), -cPanY.animation(), cAspectRatio, cSelectedComponent));
              break;
            }

            case 5: {
              selectComponent(cLayout.pickNextComponent(-cPanX.animation(), -cPanY.animation(), cAspectRatio, cSelectedComponent));
              break;
            }
          }
          return true;
        }

        default: {
          break;
        }
      }
    }
    return false;
  }

  void LayoutEditor::resetInput() {
    // Nothing to do.
  }

  void LayoutEditor::notifyVisible() {
    // Nothing to do.
  }
  
  void LayoutEditor::notifyHidden() {
    // Nothing to do.
  }
  
  void LayoutEditor::notifyLostFocus() {
    cHasFocus = false;
  }

  void LayoutEditor::notifyGainedFocus() {
    cHasFocus = true;
  }

  std::vector<std::string> LayoutEditor::getDigitalInputs() const {
    std::vector<std::string> mDigitalInputNames;
    for (std::pair<std::string, EditorDigitalInput<LayoutEditor, SignalInputID>*> mPair : cDigitalInputsByName) {
      mDigitalInputNames.emplace_back(mPair.first);
    }
    return mDigitalInputNames;
  }

  std::vector<std::string> LayoutEditor::getAnalogueInputs() const {
    std::vector<std::string> mAnalogueInputNames;
    for (std::pair<std::string, EditorAnalogueInput*> mPair : cAnalogueInputsByName) {
      mAnalogueInputNames.emplace_back(mPair.first);
    }
    return mAnalogueInputNames;
  }

  void LayoutEditor::setDigitalInput(const std::string& name, IBoolean* input) {
    cDigitalInputsByName.find(name)->second->set(input);
  }

  void LayoutEditor::setAnalogueInput(const std::string& name, IFloat* input) {
    cAnalogueInputsByName.find(name)->second->set(input);
  }

  void LayoutEditor::setExitAction(IAction* action) {
    cExitAction = action;
  }

  void LayoutEditor::setAppearance(IFont* font, float scale) {
    cFont = font;
    cFontSize = scale;
  }

  IScreen* LayoutEditor::screen() {
    return this;
  }
  
  IFont* LayoutEditor::getFont() const {
    return cFont;
  }
  
  float LayoutEditor::getFontSize() const {
    return cFontSize;;
  }
  
  IFont* LayoutEditor::getCodeFont() const {
    return nullptr;
  }
  
  float LayoutEditor::getCodeFontSize() const {
    return 0.0f;
  }
  
  Project& LayoutEditor::getProject() const {
    return cLayout.getUI().getProject();
  }
  
  Point2D LayoutEditor::transform(const Point2D& normalisedLocation) const {
    return Point2D(normalisedLocation.getX() / cZoomFactor.value() - cPanX.value(), normalisedLocation.getY() / cZoomFactor.value() - cPanY.value());
  }
  
  float LayoutEditor::snapX(float x) const {
    return x < -0.5f * cAspectRatio ? Utils::round(std::min(  x, -0.5f * cAspectRatio - cGridX / 2.0f) + 1.0f * cAspectRatio,                cGridX, 0.0) - 1.0f * cAspectRatio
         : x >  0.5f * cAspectRatio ? Utils::round(std::max(  x,  0.5f * cAspectRatio + cGridX / 2.0f) - 1.0f * cAspectRatio,                cGridX, 0.0) + 1.0f * cAspectRatio
         :                            Utils::round(std::clamp(x, -0.5f * cAspectRatio + cGridX / 2.0f, 0.5f * cAspectRatio - cGridX / 2.0f), cGridX, 0.0);
  }
  
  float LayoutEditor::snapY(float y) const {
    return y < -0.5f ? Utils::round(std::min(  y,  -0.5f - cGridY / 2.0f) + 1.0f,                cGridY, 0.0) - 1.0f
         : y >  0.5f ? Utils::round(std::max(  y,   0.5f + cGridY / 2.0f) - 1.0f,                cGridY, 0.0) + 1.0f
         :             Utils::round(std::clamp(y,  -0.5f + cGridY / 2.0f, 0.5f - cGridY / 2.0f), cGridY, 0.0);
  }
  
  void LayoutEditor::renderGrid(float fromY, float toY, float x) const {
    bool mForward = toY > fromY;
    for (float y = fromY; mForward ? y < toY : y > toY; y += (mForward ? cGridY : -cGridY)) {
      glVertex2f(x, y);
    }
  }
  
  void LayoutEditor::renderGrid(float fromX, float toX) const {
    bool mForward = toX > fromX;
    for (float x = fromX; mForward ? x < toX : x > toX; x += (mForward ? cGridX : -cGridX)) {
      renderGrid(-1.0f, -0.5f, x);
      renderGrid( 0.0f, -0.5f, x);
      renderGrid( 0.0f,  0.5f, x);
      renderGrid( 1.0f,  0.5f, x);
    }
  }

  void LayoutEditor::openProperties() {
    const Metadata& mMetadata = cLayout.getUI().getMetadata("LayoutEditor");
    if (cSelectedComponent != nullptr) {
      cPropertiesUI.openUI(std::make_unique<PropertiesMenu>(cPropertiesUI, *this, cLayout.getResourceData(), [this, &mMetadata](PropertyMaker& owner) {
        cSelectedComponent->getProperties(owner, mMetadata);
      }), cSelectedComponent->getName() + " Configuration:");
      cEditingProperties = true;
    }
  }

  void LayoutEditor::deleteSelectedComponent() {
    if (cSelectedComponent != nullptr) {
      cLayout.deleteComponent(cSelectedComponent);
      selectComponent(nullptr);
    }
  }
  
  void LayoutEditor::moveSelectedComponentToBack() {
    if (cSelectedComponent != nullptr) {
      cLayout.moveComponentToBack(cSelectedComponent);
    }
  }

  void LayoutEditor::moveSelectedComponentToFront() {
    if (cSelectedComponent != nullptr) {
      cLayout.moveComponentToFront(cSelectedComponent);
    }
  }
  
  void LayoutEditor::pickHandle() {
    if (cSelectedComponent != nullptr) {
//      cSelectedComponent->pickHandle(cPanX.animation(), cPanY.animation(), cZoomFactor.animation(), cAspectRatio);
    }
  }  

  void LayoutEditor::renderEditingHandles() {
    if (cSelectedComponent != nullptr) {
      for (std::pair<Handle, std::function<Point2D()>>& mPair : cHandleLocationFunctions) {
        Point2D mHandleLocation = mPair.second();
        if (mPair.first == cSelectedComponentClosestHandle) {
          glColor3f(0.0f, 1.0f, 1.0f);
        } else {
          glColor3f(1.0f, 1.0f, 1.0f);
        }
        glBegin(GL_QUADS);
        glVertex2f(mHandleLocation.getX() - cEditHandleRadius, mHandleLocation.getY() - cEditHandleRadius);
        glVertex2f(mHandleLocation.getX() + cEditHandleRadius, mHandleLocation.getY() - cEditHandleRadius);
        glVertex2f(mHandleLocation.getX() + cEditHandleRadius, mHandleLocation.getY() + cEditHandleRadius);
        glVertex2f(mHandleLocation.getX() - cEditHandleRadius, mHandleLocation.getY() + cEditHandleRadius);
        glEnd();

        if (mPair.first == cSelectedComponentClosestHandle) {
          glColor3f(0.0f, 0.0f, 1.0f);
        } else {
          glColor3f(1.0f, 0.0f, 0.0f);
        }
        glLineWidth(3.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(mHandleLocation.getX() - cEditHandleRadius, mHandleLocation.getY() - cEditHandleRadius);
        glVertex2f(mHandleLocation.getX() + cEditHandleRadius, mHandleLocation.getY() - cEditHandleRadius);
        glVertex2f(mHandleLocation.getX() + cEditHandleRadius, mHandleLocation.getY() + cEditHandleRadius);
        glVertex2f(mHandleLocation.getX() - cEditHandleRadius, mHandleLocation.getY() + cEditHandleRadius);
        glEnd();
        glLineWidth(1.0f);
      }
    }
  }  
  
  void LayoutEditor::selectComponent(LayoutComponent* component) {
    cSelectedComponent = component;
    if (cSelectedComponent != nullptr) {
      updateSelectedComponentEdges();
    }
  }

  void LayoutEditor::resizeComponentHorizontal(float fixed, float x) {
    cSelectedComponent->setLeftEdgeLocation( std::min(fixed, x), cAspectRatio);
    cSelectedComponent->setRightEdgeLocation(std::max(fixed, x), cAspectRatio);
    updateSelectedComponentEdges();
  }

  void LayoutEditor::resizeComponentVertical(float fixed, float y) {
    cSelectedComponent->setBottomEdgeLocation(std::min(fixed, y));
    cSelectedComponent->setTopEdgeLocation(   std::max(fixed, y));
    updateSelectedComponentEdges();
  }

  void LayoutEditor::adjustComponentOffset(Handle handle, float x, float y) {
    switch (handle) {
      case Handle::WEST:      cSelectedComponent->setLeftEdgeOffset(  x, cAspectRatio); break;
      case Handle::EAST:      cSelectedComponent->setRightEdgeOffset( x, cAspectRatio); break;
      case Handle::SOUTH:     cSelectedComponent->setBottomEdgeOffset(y);               break;
      case Handle::NORTH:     cSelectedComponent->setTopEdgeOffset(   y);               break;
      case Handle::SOUTHWEST: cSelectedComponent->setLeftEdgeOffset(  x, cAspectRatio);
                              cSelectedComponent->setBottomEdgeOffset(y);               break;
      case Handle::SOUTHEAST: cSelectedComponent->setRightEdgeOffset( x, cAspectRatio);
                              cSelectedComponent->setBottomEdgeOffset(y);               break;
      case Handle::NORTHWEST: cSelectedComponent->setLeftEdgeOffset(  x, cAspectRatio);
                              cSelectedComponent->setTopEdgeOffset(   y);               break;
      case Handle::NORTHEAST: cSelectedComponent->setRightEdgeOffset( x, cAspectRatio);
                              cSelectedComponent->setTopEdgeOffset(   y);               break;
      case Handle::NONE:                                                                break;
    }
    updateSelectedComponentEdges();
  }

  void LayoutEditor::updateSelectedComponentEdges() {
    if (cSelectedComponent != nullptr) {
      cSelectedComponentLeft   = cSelectedComponent->getLeft(cAspectRatio);
      cSelectedComponentRight  = cSelectedComponent->getRight(cAspectRatio);
      cSelectedComponentBottom = cSelectedComponent->getBottom();
      cSelectedComponentTop    = cSelectedComponent->getTop();
    }
  }

  float LayoutEditor::getHandleXOffset(Handle handle) const {
    return handle == Handle::SOUTHWEST || handle == Handle::WEST || handle == Handle::NORTHWEST ?  EDIT_HANDLE_RADIUS / cZoomFactor.animation()
         : handle == Handle::SOUTHEAST || handle == Handle::EAST || handle == Handle::NORTHEAST ? -EDIT_HANDLE_RADIUS / cZoomFactor.animation()
         : 0.0;
  }

  float LayoutEditor::getHandleYOffset(Handle handle) const {
    return handle == Handle::SOUTHWEST || handle == Handle::SOUTH || handle == Handle::SOUTHEAST ?  EDIT_HANDLE_RADIUS / cZoomFactor.animation()
         : handle == Handle::NORTHWEST || handle == Handle::NORTH || handle == Handle::NORTHEAST ? -EDIT_HANDLE_RADIUS / cZoomFactor.animation()
         : 0.0;
  }

  LayoutEditor::DrawTool::DrawTool(LayoutEditor& parent) :
            cParent(parent),
            cPinnedX(0.0f), 
            cPinnedY(0.0f),
            cPinSet(false) {
  }
  
  bool LayoutEditor::DrawTool::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 0: {
            cPinnedX = cParent.snapX(-cParent.cPanX.animation());
            cPinnedY = cParent.snapY(-cParent.cPanY.animation());
            cPinSet = true;;
            return true;
          }
          
          default: {
            break;
          }
        }
        break;
      }
      
      case sf::Event::JoystickButtonReleased: {
        switch (event.joystickButton.button) {
          case 0: {
            float mEndX = cParent.snapX(-cParent.cPanX.animation());
            float mEndY = cParent.snapY(-cParent.cPanY.animation());
            if (cPinnedX != mEndX && cPinnedY != mEndY) {
              cParent.cLayout.createComponent(cPinnedX, cPinnedY, mEndX, mEndY, cParent.cAspectRatio);
            }
            cPinSet = false;
          }

          default: {
            break;
          }
        }
      }

      default: {
        break;
      }
    }
    return false;
  }
  
  void LayoutEditor::DrawTool::update(unsigned int milliseconds) {
    // Nothing to do.
  }

  void LayoutEditor::DrawTool::renderSelection() const {
    if (cPinSet) {
      float mEndX = cParent.snapX(-cParent.cPanX.animation());
      float mEndY = cParent.snapY(-cParent.cPanY.animation());
      glColor3f(0.0f, 1.0f, 1.0f);
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 255);
      glLineWidth(3.0f);
      glBegin(GL_LINE_LOOP);
      glVertex2f(cPinnedX, cPinnedY);
      glVertex2f(cPinnedX, mEndY);
      glVertex2f(mEndX,    mEndY);
      glVertex2f(mEndX,    cPinnedY);
      glEnd();
      glDisable(GL_LINE_STIPPLE);
      glLineWidth(1.0f);
    }
  }
  
  void LayoutEditor::DrawTool::renderIcon() const {
    Utils::renderIconCustom();
  }
  
  bool LayoutEditor::DrawTool::isTool(ILayoutTool* tool) const {
    return this == tool;
  }
  
  LayoutEditor::PropertiesTool::PropertiesTool(LayoutEditor& parent) :
            cParent(parent) {
  }
  
  bool LayoutEditor::PropertiesTool::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 0:  cParent.openProperties(); return true;
          default:                           break;
        }
        break;
      }
      
      default: {
        break;
      }
    }
    return false;
  }
  
  void LayoutEditor::PropertiesTool::update(unsigned int milliseconds) {
    // Nothing to do.
  }

  void LayoutEditor::PropertiesTool::renderSelection() const {
    // Nothing to do.
  }
  
  void LayoutEditor::PropertiesTool::renderIcon() const {
    Utils::renderIconLeaf();
  }
  
  bool LayoutEditor::PropertiesTool::isTool(ILayoutTool* tool) const {
    return this == tool;
  }
  
  LayoutEditor::LocationTool::LocationTool(LayoutEditor& parent) :
            cParent(parent),
            cResizingHorizontal(false),
            cResizingVertical(false) {
  }
  
  bool LayoutEditor::LocationTool::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 0: {
            switch (cParent.cSelectedComponentClosestHandle) {
              case Handle::WEST:      cResizingHorizontal = true;                            cResizingFixedHorizontal = cParent.cSelectedComponentRight;                                                            break;
              case Handle::EAST:      cResizingHorizontal = true;                            cResizingFixedHorizontal = cParent.cSelectedComponentLeft;                                                             break;
              case Handle::SOUTH:                                  cResizingVertical = true;                                                             cResizingFixedVertical = cParent.cSelectedComponentTop;    break;
              case Handle::NORTH:                                  cResizingVertical = true;                                                             cResizingFixedVertical = cParent.cSelectedComponentBottom; break;
              case Handle::SOUTHWEST: cResizingHorizontal = true;  cResizingVertical = true; cResizingFixedHorizontal = cParent.cSelectedComponentRight; cResizingFixedVertical = cParent.cSelectedComponentTop;    break;
              case Handle::SOUTHEAST: cResizingHorizontal = true;  cResizingVertical = true; cResizingFixedHorizontal = cParent.cSelectedComponentLeft;  cResizingFixedVertical = cParent.cSelectedComponentTop;    break;
              case Handle::NORTHWEST: cResizingHorizontal = true;  cResizingVertical = true; cResizingFixedHorizontal = cParent.cSelectedComponentRight; cResizingFixedVertical = cParent.cSelectedComponentBottom; break;
              case Handle::NORTHEAST: cResizingHorizontal = true;  cResizingVertical = true; cResizingFixedHorizontal = cParent.cSelectedComponentLeft;  cResizingFixedVertical = cParent.cSelectedComponentBottom; break;
              case Handle::NONE:                                                                                                                                                                                    break;
            }
            break;
          }

          default: {
            break;
          }
        }
        break;
      }

      case sf::Event::JoystickButtonReleased: {
        switch (event.joystickButton.button) {
          case 0: {
            cResizingHorizontal = false;
            cResizingVertical   = false;
            break;
          }

          default: {
            break;
          }
        }
        break;
      }

      default: {
        break;
      }
    }
    return false;
  }
  
  void LayoutEditor::LocationTool::update(unsigned int milliseconds) {
    if (cResizingHorizontal) {
      float mX = cParent.snapX(-cParent.cPanX.animation() + cParent.getHandleXOffset(-cParent.cPanX.animation() < cResizingFixedHorizontal ? Handle::WEST : Handle::EAST));
      cParent.resizeComponentHorizontal(cResizingFixedHorizontal, mX);
    } 
    if (cResizingVertical) {
      float mY = cParent.snapY(-cParent.cPanY.animation() + cParent.getHandleYOffset(-cParent.cPanY.animation() < cResizingFixedVertical ? Handle::SOUTH : Handle::NORTH));
      cParent.resizeComponentVertical(cResizingFixedVertical, mY);
    }
  }

  void LayoutEditor::LocationTool::renderSelection() const {
    cParent.renderEditingHandles();
  }
  
  void LayoutEditor::LocationTool::renderIcon() const {
    Utils::renderIconBranch();
  }
  
  bool LayoutEditor::LocationTool::isTool(ILayoutTool* tool) const {
    return this == tool;
  }
  
  LayoutEditor::OffsetTool::OffsetTool(LayoutEditor& parent) :
            cParent(parent) {
  }
  
  bool LayoutEditor::OffsetTool::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 0:  cDraggingHandle = cParent.cSelectedComponentClosestHandle; return true;
          default:                                                            break;
        }
        break;
      }

      case sf::Event::JoystickButtonReleased: {
        switch (event.joystickButton.button) {
          case 0:  cDraggingHandle = Handle::NONE; return true;
          default:                                 break;
        }
        break;
      }

      default: {
        break;
      }
    }
    return false;
  }
  
  void LayoutEditor::OffsetTool::update(unsigned int milliseconds) {
    float mX = cParent.snapX(-cParent.cPanX.animation() + cParent.getHandleXOffset(cDraggingHandle));
    float mY = cParent.snapY(-cParent.cPanY.animation() + cParent.getHandleYOffset(cDraggingHandle));
    cParent.adjustComponentOffset(cDraggingHandle, mX, mY);
  }

  void LayoutEditor::OffsetTool::renderSelection() const {
    cParent.renderEditingHandles();
  }
  
  void LayoutEditor::OffsetTool::renderIcon() const {
    Utils::renderIconBranch();
  }
  
  bool LayoutEditor::OffsetTool::isTool(ILayoutTool* tool) const {
    return this == tool;
  }
  
  LayoutEditor::DeleteTool::DeleteTool(LayoutEditor& parent) :
            cParent(parent) {
  }
  
  bool LayoutEditor::DeleteTool::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 0: cParent.deleteSelectedComponent(); return true;
          default:                                   break;
        }
        break;
      }
      
      default: {
        break;
      }
    }
    return false;
  }
  
  void LayoutEditor::DeleteTool::update(unsigned int milliseconds) {
    // Nothing to do.
  }

  void LayoutEditor::DeleteTool::renderSelection() const {
    if (cParent.cSelectedComponent != nullptr) {
      glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
      glEnable(GL_BLEND);
      glBegin(GL_QUADS);
      glVertex2f(cParent.cSelectedComponentLeft,  cParent.cSelectedComponentBottom);
      glVertex2f(cParent.cSelectedComponentRight, cParent.cSelectedComponentBottom);
      glVertex2f(cParent.cSelectedComponentRight, cParent.cSelectedComponentTop);
      glVertex2f(cParent.cSelectedComponentLeft,  cParent.cSelectedComponentTop);
      glEnd();
      glDisable(GL_BLEND);

      glLineWidth(6.0f);
      glColor3f(1.0f, 0.0f, 0.0f);
      glBegin(GL_LINES);
      glVertex2f(cParent.cSelectedComponentLeft,  cParent.cSelectedComponentBottom);
      glVertex2f(cParent.cSelectedComponentRight, cParent.cSelectedComponentTop);
      glVertex2f(cParent.cSelectedComponentRight, cParent.cSelectedComponentBottom);
      glVertex2f(cParent.cSelectedComponentLeft,  cParent.cSelectedComponentTop);
      glEnd();
      glLineWidth(1.0f);
    }
  }
  
  void LayoutEditor::DeleteTool::renderIcon() const {
    Utils::renderIconNone();
  }
  
  bool LayoutEditor::DeleteTool::isTool(ILayoutTool* tool) const {
    return this == tool;
  }
  
  LayoutEditor::OrderingTool::OrderingTool(LayoutEditor& parent) :
            cParent(parent) {
  }
  
  bool LayoutEditor::OrderingTool::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 0: cParent.moveSelectedComponentToFront(); return true;
          case 1: cParent.moveSelectedComponentToBack();  return true;
          default:                                        break;
        }
        break;
      }
      
      default: {
        break;
      }
    }
    return false;
  }

  void LayoutEditor::OrderingTool::update(unsigned int milliseconds) {
    // Nothing to do.
  }

  void LayoutEditor::OrderingTool::renderSelection() const {
    // Nothing to do.
  }
  
  void LayoutEditor::OrderingTool::renderIcon() const {
    Utils::renderIconTick();
  }
  
  bool LayoutEditor::OrderingTool::isTool(ILayoutTool* tool) const {
    return this == tool;
  }

  LayoutEditor::CopyTool::CopyTool(LayoutEditor& parent) :
            cParent(parent) {
  }

  bool LayoutEditor::CopyTool::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 0: {
            JSONDocument mDocument;
            JSONObject mComponentObject = mDocument.addObject("Component");
            cParent.cSelectedComponent->save(mComponentObject);
            LayoutComponent* mComponentCopy = cParent.cLayout.createComponent(mComponentObject);

            // Move the copy component down and right a bit so we can differentiate it from the original.
            float mPreviousLeft = mComponentCopy->getLeft(cParent.cAspectRatio);
            float mPreviousRight = mComponentCopy->getRight(cParent.cAspectRatio);
            float mPreviousTop = mComponentCopy->getTop();
            float mPreviousBottom = mComponentCopy->getBottom();
            float mNewLeft = cParent.snapX(mPreviousLeft + cParent.cGridX * 2.0f);
            float mNewTop = cParent.snapY(mPreviousTop - cParent.cGridY * 2.0f);
            float mNewRight = mPreviousRight + (mNewLeft - mPreviousLeft);
            float mNewBottom = mPreviousBottom - (mPreviousTop - mNewTop);
            mComponentCopy->setLeftEdgeLocation(  mNewLeft, cParent.cAspectRatio);
            mComponentCopy->setRightEdgeLocation( mNewRight, cParent.cAspectRatio);
            mComponentCopy->setBottomEdgeLocation(mNewBottom);
            mComponentCopy->setTopEdgeLocation(   mNewTop);

            cParent.cSelectedComponent = mComponentCopy;
            cParent.updateSelectedComponentEdges();
            return true;
          }

          default: {
            break;
          }
        }
        break;
      }

      default: {
        break;
      }
    }
    return false;
  }

  void LayoutEditor::CopyTool::update(unsigned int milliseconds) {
    // Nothing to do.
  }

  void LayoutEditor::CopyTool::renderSelection() const {
    // Nothing to do.
  }

  void LayoutEditor::CopyTool::renderIcon() const {
    Utils::renderIconTick();
  }

  bool LayoutEditor::CopyTool::isTool(ILayoutTool* tool) const {
    return this == tool;
  }

  const float LayoutEditor::ZOOM_LIMIT_MAXIMUM =  20.0f;
  const float LayoutEditor::ZOOM_LIMIT_MINIMUM = -20.0f;

  const float LayoutEditor::ANALOGUE_INPUT_DEAD_ZONE = 16.0f;

  const float LayoutEditor::EDIT_HANDLE_RADIUS = 0.013f;
}
