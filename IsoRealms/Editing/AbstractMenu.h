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

#include <string>

#include "IsoRealms/Property/IPropertyAppearance.h"
#include "IsoRealms/Types.h"

#include "IConfiguratorScreen.h"

namespace IsoRealms {
  class Configurator;

  class AbstractMenu : public IConfiguratorScreen {
    public:
    AbstractMenu(Configurator* parent, IPropertyAppearance* appearance);

    void setTitle(const std::string& title);

//     unsigned int getSelectedItem();
//     void print(float x, float y, float scale, IFont::Alignment alignment, const std::string& text);
//     float getWidth(const std::string& text) const;
//     IFont* getFont();
//
    /**********************************\
     * Implements IConfiguratorScreen *
    \**********************************/
    void render() override;
    void update(unsigned int milliseconds, bool active) override;
    bool updateForClosure(unsigned int milliseconds) override;
    bool input(sf::Event& event) override;

    virtual float getLeftSelectionBoundary(unsigned int item) = 0;
    virtual float getRightSelectionBoundary(unsigned int item) = 0;
    virtual float getLeftSelectionHighlight(unsigned int item) = 0;
    virtual float getRightSelectionHighlight(unsigned int item) = 0;
    virtual unsigned int getItemCount() = 0;
    virtual void renderOverlay() = 0;
    virtual void renderItem(unsigned int item) = 0;
    virtual void updateItems(unsigned int milliseconds) = 0;
    virtual bool isMenuInputLocked() = 0;
    virtual bool input(unsigned int item, sf::Event& event) = 0;
    virtual void inputKeyDownLock(sf::Event& event) = 0;

    virtual ~AbstractMenu() {
    };

    protected:
    Configurator* cParent;
    IPropertyAppearance* cIntAppearance;
    unsigned int cSelectedItem;
    std::string cTitle;

    private:
    float cSelectionAnimation;
    float cAnimation;
    float cLeftHighlightAnimation;
    float cRightHighlightAnimation;
    float cLeftBoundaryAnimation;
    float cRightBoundaryAnimation;
    float cScroll;
    float cScrollAnimation;
    float cLockAnimation;

    void selectNextMenuItem();
    void selectPreviousMenuItem();
  };
}
