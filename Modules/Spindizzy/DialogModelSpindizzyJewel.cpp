/*
 * Copyright 2015 Martin Brentnall
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
#include "DialogModelSpindizzyJewel.h"

DialogModelSpindizzyJewel::DialogModelSpindizzyJewel(IEditingContext* editingContext, ResourceModelSpindizzyJewel* modelType, IResourceAccessor* resources, const std::string& resourceName) : DialogOKCancelUndo(editingContext, resources, "Spindizzy Jewel Model", resourceName) {
  cModelType = modelType;
  RectangularComponent* mContent = new RectangularComponent("Modules/Spindizzy/DialogModelSpindizzyJewel", resources);
  cResourceSelector = editingContext->getResourceSelector();
  cOriginalFrameColour  = cModelType->getFrameColour();
  cOriginalCycleColours = cModelType->getCycleColours();
  cColourSelectorFrame  = new ComponentResourceColourSelector(this, cOriginalFrameColour, cResourceSelector);
  undo();
  mContent->addStringListener(new StringListener(this), "numberCycleColours");
  mContent->setSelectable("frameColour", cColourSelectorFrame);
  // TODO: Set fill selectables
  mContent->addComponent("previewPane", new ComponentIconModel(cModelType));
  addComponent("content", mContent);
}

void DialogModelSpindizzyJewel::undo() {
  std::cout << "Setting frame..." << std::endl;
  cColourSelectorFrame->resourceSelected(cOriginalFrameColour);
  std::cout << "Setting " << cOriginalCycleColours.size() << " cycle colours..." << std::endl;
  setCycleColours(cOriginalCycleColours.size());
  for (unsigned int i = 0; i < cOriginalCycleColours.size(); i++) {
    std::cout << "  Setting cycle colour " << i << std::endl;
    cColourSelectorsCycle[i]->resourceSelected(cOriginalCycleColours[i]);
  }
}

void DialogModelSpindizzyJewel::setCycleColours(unsigned int count) {
  std::cout << "Setting " << count << " cycle colours (currently " << cColourSelectorsCycle.size() << ")..." << std::endl;
  while (cColourSelectorsCycle.size() < count) {
    ComponentResourceColourSelector* mColourSelector = new ComponentResourceColourSelector(this, new Colour(1.0f, 0.0f, 0.0f), cResourceSelector);
    cColourSelectorsCycle.push_back(mColourSelector);
//     addComponent("cycleColours", new SelectableComponent(mColourSelector));
    std::cout << "  Added cycle colour, now there are " << cColourSelectorsCycle.size() << " colours" << std::endl;
  }
  while (cColourSelectorsCycle.size() > count) {
    cColourSelectorsCycle.pop_back();
    std::cout << "  Removed cycle colour, now there are " << cColourSelectorsCycle.size() << " colours" << std::endl;
  }
}

ResourceModelSpindizzyJewel* DialogModelSpindizzyJewel::getResource() {
  return cModelType;
}

void DialogModelSpindizzyJewel::selected(ISelector* selector, IColour* colour) {
  std::cout << "Seleciton on " << this << std::endl;
  if (selector == cColourSelectorFrame) {cModelType->setFrameColour(colour);}
  std::cout << "There are " << cColourSelectorsCycle.size() << " cycle colours" << std::endl;
  for (unsigned int i = 0; i < cColourSelectorsCycle.size(); i++) {
    std::cout << "  Checking colour " << i << std::endl;
    if (selector == cColourSelectorsCycle[i]) {cModelType->setCycleColour(i, colour);}
    if (i == 30) {
      exit(1);
    }
  }
}

DialogModelSpindizzyJewel::StringListener::StringListener(DialogModelSpindizzyJewel* parent) {
  cParent = parent;
}

void DialogModelSpindizzyJewel::StringListener::valueChanged(const std::string& value) {
  unsigned int mCycleColours = min(1, max(32, atoi(value.c_str())));
  cParent->setCycleColours(mCycleColours);
}
