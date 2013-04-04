/*
 * Copyright 2009 Martin Brentnall
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
#include "SpindizzyLiftSet.h"

DefaultElementHandler<SpindizzyLift>* SpindizzyLiftSet::createHandler() {
  return new SpindizzyLiftHandler(this);
}

void SpindizzyLiftSet::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  DOMNodeWriter* mConfigurationNode = node->addBranch("Configuration");
  mConfigurationNode->addAttribute("surfaceRegistry", resourceLocator->getPath(cCollidableSurfaceRegistry));
  if (cLiftMovedScript != NULL) {
    DOMNodeWriter* mLiftMovedScriptNode = mConfigurationNode->addBranch("LiftMovedScript");
    cLiftMovedScript->save(mLiftMovedScriptNode, resourceLocator);
  }
  for (unsigned int i = 0; i < cElementTypes.size(); i++) {
    DOMNodeWriter* mCameraNode = node->addBranch("LiftType");
    std::string mResourceName = resourceLocator->getPath(cElementTypes[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mCameraNode->addAttribute("name", mResourceName);
    cElementTypes[i]->save(mCameraNode, resourceLocator);
  }
}

void SpindizzyLiftSet::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  cEditing = runtimeContext->isEditing();
  cLiftMovedScript = NULL;
  cLocked = 0;
  IArgumentSource* mModuleArgumentSource = new ArgumentSourceCustom<SpindizzyLiftSet>(this);
  runtimeContext->registerArgument("Module", "Module", mModuleArgumentSource);
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "LiftType") {
      SpindizzyLiftType* mLiftType = new SpindizzyLiftType(this, &cSpindizzyLiftProperties);
      cElementTypes.push_back(mLiftType);
      std::string mName = mNode->getAttribute("name");
      IArgumentSource* mArgumentSource = new ArgumentSourceCustom<ISpindizzyLiftType>(mLiftType);
      runtimeContext->add(mLiftType, mName, mNode);
      runtimeContext->registerArgument("SpindizzyLiftType", mName, mArgumentSource);
    } else if (mValueAsString == "Configuration") {
      runtimeContext->add(this, mNode);
    } else {
      // TODO: Throw something!
    }
  }
}

void SpindizzyLiftSet::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cCollidableSurfaceRegistry = resources->getSurfaceRegistry(node->getAttribute("surfaceRegistry"));
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "LiftMovedScript") {
      cLiftMovedScript = resources->getLuaScript(mNode);
    } else {
      // TODO: Throw something!
    }
  }
}

IZone* SpindizzyLiftSet::getCurrentZone() {
  return cZone;
}

void SpindizzyLiftSet::executeLiftMovedScript() {
  cLiftMovedScript->execute();
}

void SpindizzyLiftSet::registerInterceptingSurface(IRollableSurface* surface) {
  cCollidableSurfaceRegistry->registerRollableSurface(surface, true);
}

/* TODO: DO THIS WHEN ZONE CHANGES!  void SpindizzyLiftSet::zoneContextChanged(IZone* zone) {
  cZone = zone;
  if (cZone != NULL) {
    // TODO: Dynamic casts are bad
    IElementContainer* mContainer = dynamic_cast<IElementContainer*>(cZone);
    setHandlerActive(mContainer, true);
  }
}
*/
void SpindizzyLiftSet::addLock() {
  cLocked++;
}

void SpindizzyLiftSet::removeLock() {
  cLocked--;
}

bool SpindizzyLiftSet::isLocked() {
  return cLocked > 0;
}

bool SpindizzyLiftSet::isEditing() {
  return cEditing;
}

void SpindizzyLiftSet::registerElement(IElementContainer* container, SpindizzyLift* lift) {
  std::vector<SpindizzyLift*> mLifts = cLiftsByContainer[container];
  mLifts.push_back(lift);
  cLiftsByContainer[container] = mLifts;
}

void SpindizzyLiftSet::reset(IZone* zone) {
  std::vector<SpindizzyLift*> mLifts = cLiftsByContainer[zone];
  for (unsigned int i = 0; i < mLifts.size(); i++) {
    mLifts[i]->reset();
  }
}
  
extern "C" IPlugin* create() {
  return new SpindizzyLiftSet();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
