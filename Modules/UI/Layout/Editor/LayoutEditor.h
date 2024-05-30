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

#include <GL/glew.h>

#include "IsoRealms/AnimatedFloat.h"
#include "IsoRealms/Application.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::UI {
  class Layout;
  class LayoutComponent;

  class LayoutEditor : public IEditableScreen {
    public:
    LayoutEditor(Layout* layout);

    /******************************\
     * Implements IEditableScreen *
    \******************************/
    bool input(sf::Event& event) override;
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void notifyVisible() override;
    void notifyHidden() override;
    void notifyLostFocus() override;
    std::vector<std::string> getDigitalInputs() const override;
    std::vector<std::string> getAnalogueInputs() const override;
    std::vector<std::string> getSignalInputs() const override;
    void setDigitalInput(const std::string& name, IBoolean* input) override;
    void setAnalogueInput(const std::string& name, IFloat* input) override;
    int getSignalID(const std::string& name) const override;
    void signal(int id) override;
    void setAppearance(IFont* font, float scale) override;
    void unregisterAssets(IAssetRemover* assets) override;
    IScreen* screen() override;
    
    private:
    Layout* cLayout;
    LayoutComponent* cSelectedComponent;
    AnimatedFloat cZoomFactor;
    AnimatedFloat cPanX;
    AnimatedFloat cPanY;
    bool cDragging;
  };
}
