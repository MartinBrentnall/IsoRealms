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

#include <algorithm>
#include <stack>
#include <vector>

#include "IsoRealms/AnimatedFloat.h"
#include "IsoRealms/IModule.h"
#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/Options/LocalOptions.h"
#include "IsoRealms/ResourceDefinition.h"

#include "IUIScreen.h"
#include "IUIStyle.h"
#include "UISignalID.h"

namespace IsoRealms {
  class UIManager final {
    public:
    UIManager(IProject& project, IUIStyle& style, std::function<void()> finishCallback, std::function<void(IEditable*)> editorCallback);

    void render(float aspectRatio) const;
    void update(unsigned int milliseconds);
    void input(UISignalID id);
    bool input(sf::Event& event);  

    void openUI(std::unique_ptr<IUIScreen> screen);
    void closeUI();
    void edit(IEditable* editable);
    void hide();
    void show();
    bool isHidden() const;
    IProject& getProject() const;

    private:
    class UIScreen {
      public:
      UIScreen(std::unique_ptr<IUIScreen> screen);
      void updateSlideActive(unsigned int milliseconds);
      bool updateSlideClosing(unsigned int milliseconds);
      void updateSlideInactive(unsigned int milliseconds);

      std::unique_ptr<IUIScreen> cScreen;
      int cSlideAnimation;
      std::string cBreadcrumb;
    };

    IProject& cProject;
    IUIStyle& cStyle;
    std::function<void()> cFinishCallback;
    std::function<void(IEditable*)> cEditorCallback;

    std::vector<std::unique_ptr<UIScreen>> cRuntimeUIs;       /// Stack of project UIs.
    std::vector<std::unique_ptr<UIScreen>> cRuntimeClosedUIs; /// Closed project UIs that are still being animated.

    mutable AnimatedFloat cHighlightLeft;
    mutable AnimatedFloat cHighlightRight;
    mutable AnimatedFloat cHighlightTop;
    mutable AnimatedFloat cHighlightBottom;
    
    bool cHidden;
    int cHideAnimation;
  };
}
