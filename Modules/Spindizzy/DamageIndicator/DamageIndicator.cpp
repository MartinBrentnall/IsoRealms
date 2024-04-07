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
#include "DamageIndicator.h"

#include "IsoRealms/Persistence/DOMNodeWriter.h"

namespace IsoRealms::Spindizzy {
  const std::string DamageIndicator::TAG_COLOUR = "Colour";
  const std::string DamageIndicator::TAG_SIZE   = "Size";

  DamageIndicator::DamageIndicator(IProject* project, Spindizzy* spindizzy) :
            cDefColour(project, 0.0f, 0.0f, 1.0f),
            cDefSize(project, 0.5f) {
  }

  DamageIndicator::DamageIndicator(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data) :
            DamageIndicator(project, spindizzy) {
    cDefColour.init(node, TAG_COLOUR);
    cDefSize.init(node, TAG_SIZE);
  }

  void DamageIndicator::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Coloured Screens");
  }

  void DamageIndicator::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }

  void DamageIndicator::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefColour.save(node, TAG_COLOUR);
    cDefSize.save(node, TAG_SIZE);
  }

  void DamageIndicator::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool DamageIndicator::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> DamageIndicator::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  void DamageIndicator::renderScreen(float scale, float aspectRatio) const {
    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
    glColor4f(cDefColour.getRed(), cDefColour.getGreen(), cDefColour.getBlue(), 0.0f);
    glVertex3f(-aspectRatio,                         1.0f - cDefSize->getValue(), 0.0f);
    glVertex3f( aspectRatio,                         1.0f - cDefSize->getValue(), 0.0f);
    cDefColour.set();
    glVertex3f( aspectRatio,                         1.0f,                        0.0f);
    glVertex3f(-aspectRatio,                         1.0f,                        0.0f);

    glVertex3f(-aspectRatio,                        -1.0f,                        0.0f);
    glVertex3f( aspectRatio,                        -1.0f,                        0.0f);
    glColor4f(cDefColour.getRed(), cDefColour.getGreen(), cDefColour.getBlue(), 0.0f);
    glVertex3f( aspectRatio,                        -1.0f + cDefSize->getValue(), 0.0f);
    glVertex3f(-aspectRatio,                        -1.0f + cDefSize->getValue(), 0.0f);

    glVertex3f(-aspectRatio + cDefSize->getValue(), -1.0f,                        0.0f);
    glVertex3f(-aspectRatio + cDefSize->getValue(),  1.0f,                        0.0f);
    cDefColour.set();
    glVertex3f(-aspectRatio,                         1.0f,                        0.0f);
    glVertex3f(-aspectRatio,                        -1.0f,                        0.0f);

    glVertex3f( aspectRatio,                        -1.0f,                        0.0f);
    glVertex3f( aspectRatio,                         1.0f,                        0.0f);
    glColor4f(cDefColour.getRed(), cDefColour.getGreen(), cDefColour.getBlue(), 0.0f);
    glVertex3f( aspectRatio - cDefSize->getValue(),  1.0f,                        0.0f);
    glVertex3f( aspectRatio - cDefSize->getValue(), -1.0f,                        0.0f);
    glEnd();
  }

  bool DamageIndicator::renderAssetIcon() const {
    return renderIcon();
  }
}

