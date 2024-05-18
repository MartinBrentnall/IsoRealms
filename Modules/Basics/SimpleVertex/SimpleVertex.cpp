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
#include "SimpleVertex.h"

namespace IsoRealms::Basics {
  const std::string SimpleVertex::JSON_X = "x";
  const std::string SimpleVertex::JSON_Y = "y";
  const std::string SimpleVertex::JSON_Z = "z";

  const std::string SimpleVertex::PROPERTY_X = "X";
  const std::string SimpleVertex::PROPERTY_Y = "Y";
  const std::string SimpleVertex::PROPERTY_Z = "Z";

  SimpleVertex::SimpleVertex(IProject* project, Basics* basics) :
            cDefX(0.0),
            cDefY(0.0),
            cDefZ(0.0),
            cRuntimeX(0.0),
            cRuntimeY(0.0),
            cRuntimeZ(0.0),
            cLuaBinding(project, this),
            cStateNotifier(nullptr) {
    project->reset([this]() {
      cRuntimeX = cDefX;
      cRuntimeY = cDefY;
      cRuntimeZ = cDefZ;
    });
  }

  SimpleVertex::SimpleVertex(IProject* project, Basics* basics, JSONObject object, IOptions* options, IResourceData* data) :
            SimpleVertex(project, basics) {
    cRuntimeX = cDefX = object.getFloat(JSON_X);
    cRuntimeY = cDefY = object.getFloat(JSON_Y);
    cRuntimeZ = cDefZ = object.getFloat(JSON_Z);

    project->init([this](IAssets* resources) {
      cStateNotifier->stateChanged(this);
    });
  }

  void SimpleVertex::registerAssets(IAssetRegistry* assets) {
    cStateNotifier = assets->add(this, "", "Simple Vertices");
    assets->add(&cLuaBinding, "", "Simple Vertices");
  }

  void SimpleVertex::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
    assets->remove(&cLuaBinding);
    cStateNotifier = nullptr;
  }

  void SimpleVertex::save(JSONObject object, IAssetIdentifier* identifier) const {
    object.addFloat(JSON_X, cDefX);
    object.addFloat(JSON_Y, cDefY);
    object.addFloat(JSON_Z, cDefZ);
  }

  void SimpleVertex::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool SimpleVertex::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> SimpleVertex::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  double SimpleVertex::getX() const {
    return cRuntimeX;
  }

  double SimpleVertex::getY() const {
    return cRuntimeY;
  }

  double SimpleVertex::getZ() const {
    return cRuntimeZ;
  }

  bool SimpleVertex::renderAssetIcon() const {
    return renderIcon();
  }

  void SimpleVertex::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void SimpleVertex::setX(double x) {
    cRuntimeX = x;
    cStateNotifier->stateChanged(this);
  }

  void SimpleVertex::setY(double y) {
    cRuntimeY = y;
    cStateNotifier->stateChanged(this);
  }

  void SimpleVertex::setZ(double z) {
    cRuntimeZ = z;
    cStateNotifier->stateChanged(this);
  }
}
