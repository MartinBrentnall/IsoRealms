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

#include "IsoRealms/AnimatedFloat.h"
#include "IsoRealms/Property/IProperty.h"
#include "IsoRealms/Property/IPropertyAppearance.h"
#include "IsoRealms/ResourceDefinition.h"

#include "AbstractMenu.h"

namespace IsoRealms {
  class PropertiesMenu final : public AbstractMenu {
    public:
    PropertiesMenu(Configurator* parent, IPropertyAppearance* appearance);

    void addItem(IProperty* property);
    void clear();

    // Input functions.
    void edit(unsigned int item);

    /***************************\
     * Implements AbstractMenu *
    \***************************/
    float getLeftSelectionBoundary(float aspectRatio, unsigned int item) override;
    float getRightSelectionBoundary(float aspectRatio, unsigned int item) override;
    float getLeftSelectionHighlight(float aspectRatio, unsigned int item) override;
    float getRightSelectionHighlight(float aspectRatio, unsigned int item) override;
    unsigned int getItemCount() override;
    void renderOverlay() override;
    void renderItem(float aspectRatio, unsigned int item, float x) override;
    void updateItems(unsigned int milliseconds) override;
    bool isMenuInputLocked() override;
    bool input(unsigned int item, sf::Event& event) override;
    void inputKeyDownLock(sf::Event& event) override;
    void refresh() override;

    private:
    std::vector<IProperty*> cProperties;
    std::vector<IProperty*> cClosingProperties;
    IProperty* cEditing;

    float getMaxLabelWidth() const;
  };
}
