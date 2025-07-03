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
#pragma once

#include <functional>
#include <memory>
#include <set>
#include <stack>

#include "IsoRealms/AnimatedFloat.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  template <class TOOL, class TOOL_INSTANCE> class EditorToolbar {
    public:
    EditorToolbar(std::vector<TOOL_INSTANCE> tools, std::function<void(TOOL_INSTANCE)> selectToolFunction, std::function<void(TOOL_INSTANCE)> renderToolIconFunction) :
              cTools(tools),
              cSelectToolFunction(selectToolFunction),
              cRenderToolIconFunction(renderToolIconFunction),
              cSelectedTool(nullptr) {
    }
    
    void render(float aspectRatio) const {
      glPushAttrib(GL_TRANSFORM_BIT);
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      glPopAttrib();
      glDisable(GL_DEPTH_TEST);
      glEnable(GL_BLEND);

      glBindTexture(GL_TEXTURE_2D, 0);
      glLoadIdentity();
      
      float mCursorXPosition = cPaletteSelectionX.animation() * (ICON_WIDTH + ICON_SPACING);
      float mPaletteWidth = cTools.size() * (ICON_WIDTH + ICON_SPACING) - ICON_SPACING;
      float mDifference = aspectRatio;
      float mScrollAmount = std::min((mPaletteWidth - mDifference) - (ICON_WIDTH / 2.0f - BORDER_SPACING),  std::max(mDifference - (ICON_WIDTH / 2.0f + BORDER_SPACING), mCursorXPosition));
      glColor4f(0.0f, 0.0f, 0.0f, 0.75f);
      glBegin(GL_QUADS);
      Utils::renderRectangle(-1.0f, -1.0f, 1.0f, -1.0f + ICON_HEIGHT + BORDER_SPACING * 2.0f);
      glEnd();
      glScalef(1.0f / aspectRatio, 1.0f, 1.0f);
      glTranslatef(-mScrollAmount, 0.0f, 0.0f);
      glColor3f(1.0f, 0.0f, 0.0f);
      Utils::renderRoundedRectangle((mCursorXPosition - ICON_WIDTH * 0.5f) - 0.01f, BOTTOM_BORDER - 0.01f,
                                    (mCursorXPosition + ICON_WIDTH * 0.5f) + 0.01f, BOTTOM_BORDER + ICON_HEIGHT + 0.01f, 0.01f);
      glPopMatrix();

      for (unsigned int i = 0; i < cTools.size(); i++) {
        glPushMatrix();
  //       if (mPaletteWidth < PALETTE_SPACE) {
  //         glTranslatef(((-mPaletteWidth * 0.5f + ICON_WIDTH * 0.5f) + i * ICON_WIDTH) + i * ICON_SPACING, BOTTOM_BORDER + ICON_HEIGHT * 0.5f, 0.0f);
  //       } else {
          glTranslatef(i * (ICON_WIDTH + ICON_SPACING) - mScrollAmount, BOTTOM_BORDER + ICON_HEIGHT * 0.5f, 0.0f);
  //       }
        glScalef(ICON_WIDTH * 0.5f, ICON_HEIGHT * 0.5f, 0.0f);
        cRenderToolIconFunction(cTools[i]);
        glPopMatrix();
      }
      glDisable(GL_BLEND);
      glEnable(GL_DEPTH_TEST);
    }

    void update(unsigned int milliseconds) {
      cPaletteSelectionX.update(milliseconds);
    }

    bool input(sf::Event& event) {
      switch (event.type) {
        // case sf::Event::MouseWheelScrolled: {
        //   switch (event.mouseWheelScroll.wheel) {
        //     case sf::Mouse::VerticalWheel: {
        //       if (event.mouseWheelScroll.delta > 0) {
        //         selectToolRelative(-1);
        //       } else {
        //         selectToolRelative(1);
        //       }
        //       return true;
        //     }
        //
        //     default: {
        //       break;
        //     }
        //   }
        //   break;
        // }
        
//         case sf::Event::JoystickButtonPressed: {
//           switch (event.joystickButton.button) {
//             case 4: {
//               selectToolRelative(-1);
//               return true;
//             }
//
//             case 5: {
//               selectToolRelative(1);
//               return true;
//             }
//
//             default: {
//               break;
//             }
//           }
//         }
        
        case sf::Event::KeyPressed: {
          switch (event.key.code) {
            case sf::Keyboard::F2: {
              selectToolRelative(-1);
              return true;
            }

            case sf::Keyboard::F3: {
              selectToolRelative(1);
              return true;
            }

            default: {
              break;
            }
          }
        }

        default: {
          break;
        }

//       case sf::Event::MouseButtonPressed: {
//         switch (event.mouseButton.button) {
//           case sf::Mouse::Left: {
//             IApplication& mApplication = cWorld.getSpindizzy()->getProject()->getApplication();
//             Point2D mLocation = mApplication.normalise(event.mouseButton.x, event.mouseButton.y);
//             float mPaletteWidth = cTools.size() * (ICON_WIDTH + ICON_SPACING) - ICON_SPACING;
//             if (mLocation.getY() >= BOTTOM_BORDER && mLocation.getY() <= BOTTOM_BORDER + ICON_HEIGHT) {
//               for (unsigned int i = 0; i < cTools.size(); i++) {
//                 float mIconLeft = mPaletteWidth < PALETTE_SPACE ? (-mPaletteWidth * 0.5f) + i * (ICON_WIDTH + ICON_SPACING)
//                                                                 : (LEFT_BORDER + i * (ICON_WIDTH + ICON_SPACING));
//                 float mIconRight = mIconLeft + ICON_WIDTH;
//                 if (mLocation.getX() >= mIconLeft && mLocation.getX() <= mIconRight) {
//                   if (cSelectedTool == nullptr) {
//                     cPaletteSelectionX.init(i);
//                   } else {
//                     cPaletteSelectionX = i;
//                   }
//                   if (cSelectedTool != nullptr) {
//                     cSelectedTool->processCursorMovement(&cLocation, nullptr);
//                   }
//                   cSelectedTool = cTools[i];
//                   cSelectedTool->processCursorMovement(nullptr, &cLocation);
//                   break;
//                 }
//               }
//             }
//             return true;
//           }
//
//           default: {
//             break;
//           }
//         }
//         break;
//       }
      }
      return false;
    }

    void selectInstance(TOOL tool) {
      for (TOOL_INSTANCE mTool : cTools) {
        if (mTool->isTool(tool)) {
          cSelectedTool = mTool;
          cSelectToolFunction(cSelectedTool);
          break;
        }
      }
    }

    void removeTool(TOOL tool) {
      cTools.erase(std::remove_if(cTools.begin(), cTools.end(), [tool](TOOL_INSTANCE instance) {
        return instance->isTool(tool);
      }), cTools.end());
    }
    
    void selectToolRelative(int amount) {
      int mSelectedToolIndex = -1;
      if (cSelectedTool == nullptr) {
        mSelectedToolIndex = amount > 0 ? 0 : cTools.size() - 1;
        cPaletteSelectionX.init(mSelectedToolIndex);
      } else {
        for (unsigned int i = 0; i < cTools.size(); i++) {
          if (cTools[i] == cSelectedTool) {
            mSelectedToolIndex = i + amount;
            while (mSelectedToolIndex < 0) {
              mSelectedToolIndex += cTools.size();
            }
            while (mSelectedToolIndex >= static_cast<int>(cTools.size())) {
              mSelectedToolIndex -= cTools.size();
            }
            break;
          }
        }
      }

      cPaletteSelectionX = mSelectedToolIndex;
      cSelectedTool = cTools[mSelectedToolIndex];
      cSelectToolFunction(cSelectedTool);

  //    float mPaletteWidth = cTools.size() * (ICON_WIDTH + ICON_SPACING) - ICON_SPACING;
  //    float mPaletteScrollRange = std::max(0.0f, mPaletteWidth - PALETTE_SPACE);
  //    cPaletteScroll = std::clamp(LEFT_BORDER + mSelectedToolIndex * (ICON_WIDTH + ICON_SPACING) + ICON_WIDTH * 0.5f, 0.0f, mPaletteScrollRange);
    }
    
    private:
    static const float BORDER_SPACING;
    static const float ICON_WIDTH;
    static const float ICON_HEIGHT;
    static const float BOTTOM_BORDER;
    static const float ICON_SPACING;

    std::vector<TOOL_INSTANCE> cTools;
    std::function<void(TOOL_INSTANCE)> cSelectToolFunction;
    std::function<void(TOOL_INSTANCE)> cRenderToolIconFunction;
    TOOL_INSTANCE cSelectedTool;
    AnimatedFloat cPaletteSelectionX;
  };
  
  
  template <class TOOL, class TOOL_INSTANCE> const float EditorToolbar<TOOL, TOOL_INSTANCE>::BORDER_SPACING = 0.02f;
  template <class TOOL, class TOOL_INSTANCE> const float EditorToolbar<TOOL, TOOL_INSTANCE>::ICON_WIDTH     = 0.12f;
  template <class TOOL, class TOOL_INSTANCE> const float EditorToolbar<TOOL, TOOL_INSTANCE>::ICON_HEIGHT    = 0.12f;
  template <class TOOL, class TOOL_INSTANCE> const float EditorToolbar<TOOL, TOOL_INSTANCE>::BOTTOM_BORDER  = -1.0f + BORDER_SPACING;
  template <class TOOL, class TOOL_INSTANCE> const float EditorToolbar<TOOL, TOOL_INSTANCE>::ICON_SPACING   = 0.02f;
}
