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

SpindizzyLiftSet::SpindizzyLiftSet(IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  cLiftMovedScript = NULL;
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  assignDummyPlugin(&cCollidableSurfaceRegistry, "CollidableSurfaceRegistry");
  cLocked = 0;
  cRuntimeContext->add(new LockControlCommand(this, true), "AddLock");
  cRuntimeContext->add(new LockControlCommand(this, false), "RemoveLock");
}

std::vector<IElementFactory*> SpindizzyLiftSet::getElementFactories() {
  return cElementFactories;
}

void SpindizzyLiftSet::destroy(IElement* element) {
  delete element;
}

void SpindizzyLiftSet::setEditingContext(IEditingContext* editingContext) {
  std::vector<std::string> mPath;
  mPath.push_back("Configure Spindizzy Lifts...");
  mPath.push_back("Elements");
  cComponentContainer = editingContext->getComponentContainer();
  OpenDialogCommand* mConfigureSpindizzyLiftsCommand = new OpenDialogCommand(cComponentContainer, this);
  editingContext->registerCommand(new DefaultCommandInfo(mPath, mConfigureSpindizzyLiftsCommand));
}

void SpindizzyLiftSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "ZoneContext") {
    IZoneContext* mPreviousZoneContext = cZoneContext;
    if (assignPlugin(implementation, &cZoneContext, *socket)) {
      mPreviousZoneContext->removeZoneContextListener(this);
      cZoneContext->addZoneContextListener(this);
    }
  } else if (socket->getType() == "CollidableSurfaceRegistry") {
    assignPlugin(implementation, &cCollidableSurfaceRegistry, *socket);
  } else {
    // TODO: Throw exception or something
  }  
}

IPlugin* SpindizzyLiftSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "ZoneContext")               {return cZoneContext;}
  if (socket->getType() == "CollidableSurfaceRegistry") {return cCollidableSurfaceRegistry;}
  // TODO: Throw wobbly!
  return NULL;
}

DefaultElementHandler<SpindizzyLift>* SpindizzyLiftSet::createHandler(IElementContainer* elementContainer) {
  return new SpindizzyLiftHandler(this, elementContainer);
}

void SpindizzyLiftSet::save(DOMNodeWriter* node) {
// TODO  cLiftMovedScript->save(node, "LiftMovedScript");
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    static_cast<SpindizzyLiftFactory*>(cElementFactories[i])->save(node);
  }
}

void SpindizzyLiftSet::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "LiftMovedScript") {
      cLiftMovedScript = cRuntimeContext->getLuaScript(mNode->getStringValue());
    } else if (mValueAsString == "LiftType") {
      SpindizzyLiftFactory* mLiftFactory = new SpindizzyLiftFactory(this, mNode, &cSpindizzyLiftProperties, cRuntimeContext);
      cElementFactories.push_back(mLiftFactory);
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

void SpindizzyLiftSet::zoneContextChanged(IZone* zone) {
  cZone = zone;
  if (cZone != NULL) {
    // TODO: Dynamic casts are bad
    IElementContainer* mContainer = dynamic_cast<IElementContainer*>(cZone);
    setHandlerActive(mContainer, true);
  }
}

bool SpindizzyLiftSet::isLocked() {
  return cLocked > 0;
}

bool SpindizzyLiftSet::isEditing() {
  return cRuntimeContext->isEditing();
}

SpindizzyLiftSet::LockControlCommand::LockControlCommand(SpindizzyLiftSet* parent, bool lock) {
  cParent = parent;
  cLock = lock;
}

void SpindizzyLiftSet::LockControlCommand::execute() {
  // TODO: Catch underflow?
  cParent->cLocked += cLock ? 1 : -1;
}

IHUDComponent* SpindizzyLiftSet::createComponent() {
  return new SpindizzyLiftConfigurationDialog(cComponentContainer);
}
    
extern "C" IElementSet* create(IRuntimeContext* runtimeContext) {
  return new SpindizzyLiftSet(runtimeContext);
}

extern "C" void destroy(IElementSet* elementSet) {
  delete elementSet;
}
