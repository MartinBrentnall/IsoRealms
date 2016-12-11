/*
 * Copyright 2016 Martin Brentnall
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
#include "ResourceElementHUDAbstract.h"

const std::string ResourceElementHUDAbstract::X_AXIS_DESCRIPTOR = "width";
const std::string ResourceElementHUDAbstract::Y_AXIS_DESCRIPTOR = "length";
const std::string ResourceElementHUDAbstract::Z_AXIS_DESCRIPTOR = "height";

ResourceElementHUDAbstract::ResourceElementHUDAbstract(IElementRelationManager* manager) {
  cManager = manager;
}

float ResourceElementHUDAbstract::getSize(DOMNodeWrapper* node, const std::string& axisDescriptor) {
  float mSize = node->getFloatAttribute(axisDescriptor);
  if (mSize == 0.0f) {
    mSize = node->getFloatAttribute("size");
  }
  if (mSize == 0.0f) {
    std::cout << "Missing relation or dimension (" << axisDescriptor << ") for element" << std::endl;
    exit(1);
  }
  return mSize;
}

void ResourceElementHUDAbstract::loadElement(DOMNodeWrapper* node, DOMNodeWrapper* cache, BlockLocation* location, IElementContainer* container, IResourceAccessor* resources, bool asTemplate) {
  IUniverse* mUniverse = container->getUniverse();
  IHUDComponentRelation* mLeftRelation   = cManager->getRelation(mUniverse, node->getAttribute("left"),   "left");
  IHUDComponentRelation* mRightRelation  = cManager->getRelation(mUniverse, node->getAttribute("right"),  "right");
  IHUDComponentRelation* mBottomRelation = cManager->getRelation(mUniverse, node->getAttribute("bottom"), "bottom");
  IHUDComponentRelation* mTopRelation    = cManager->getRelation(mUniverse, node->getAttribute("top"),    "top");
  
  if (mLeftRelation == nullptr && mRightRelation == nullptr) {
    std::cout << "Cannot determine element X dimension" << std::endl;
    exit(1);
  }
  if (mBottomRelation == nullptr && mTopRelation == nullptr) {
    std::cout << "Cannot determine element Y dimension" << std::endl;
    exit(1);
  }
  if (mLeftRelation   == nullptr) {mLeftRelation   = new SizeRelation(mRightRelation,  -getSize(node, X_AXIS_DESCRIPTOR));}
  if (mRightRelation  == nullptr) {mRightRelation  = new SizeRelation(mLeftRelation,    getSize(node, X_AXIS_DESCRIPTOR));}
  if (mBottomRelation == nullptr) {mBottomRelation = new SizeRelation(mTopRelation,    -getSize(node, Y_AXIS_DESCRIPTOR));}
  if (mTopRelation    == nullptr) {mTopRelation    = new SizeRelation(mBottomRelation,  getSize(node, Y_AXIS_DESCRIPTOR));}
  
  HUDComponentPosition* cElementDimensions = new HUDComponentPosition(mLeftRelation, mRightRelation, mBottomRelation, mTopRelation);
  IElement* mElement = createHUDElement(node, cache, location, resources, asTemplate, cElementDimensions);
  cElementDimensions->setElement(mElement);
  container->addElement(cElementDimensions);
  
  std::string mName = node->getAttribute("name");
  cManager->addRelatableElement(mUniverse, mName, cElementDimensions);
}

ResourceElementHUDAbstract::SizeRelation::SizeRelation(IHUDComponentRelation* relation, float offset) {
  cRelation = relation;
  cOffset   = offset;
}

float ResourceElementHUDAbstract::SizeRelation::getLocation() {
  return cRelation->getLocation() + cOffset;
}

void ResourceElementHUDAbstract::SizeRelation::save(DOMNodeWriter*, const std::string&, IComponentSources*) {
  // TODO: Implement this
}

void ResourceElementHUDAbstract::SizeRelation::renderRelation() {
  cRelation->renderRelation();
}

Icon<IElementType>* ResourceElementHUDAbstract::getResourceIcon(IResourceBrowser<IElementType>* browser) {
  return nullptr;
}
