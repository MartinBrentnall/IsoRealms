/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "SimpleVertex.h"

namespace IsoRealms::Basics {
  SimpleVertex::SimpleVertex(Basics& basics, IResourceData& data) :
            cDefX(0.0),
            cDefY(0.0),
            cDefZ(0.0),
            cRuntimeX(0.0),
            cRuntimeY(0.0),
            cRuntimeZ(0.0),
            cLuaBinding(data.getProject().getLuaState(), this),
            cStateNotifier(nullptr) {
  }

  SimpleVertex::SimpleVertex(Basics& basics, IResourceData& data, JSONObject object) :
            SimpleVertex(basics, data) {
    cRuntimeX = cDefX = object.getFloat(JSON_X);
    cRuntimeY = cDefY = object.getFloat(JSON_Y);
    cRuntimeZ = cDefZ = object.getFloat(JSON_Z);

    data.getProject().init([this]() {
      cStateNotifier->stateChanged();
    });
  }

  void SimpleVertex::registerAssets(ResourceAssetRegistry& assets) {
    cStateNotifier = assets.add<IVertex>(this, "", "Simple Vertices");
    assets.add<IBinding>(&cLuaBinding, "", "Variables/Vertices");
  }

  void SimpleVertex::save(JSONObject object) const {
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

  void SimpleVertex::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeFloat("X", [this]() {return cDefX;}, [this](float value) {cDefX = value;});
    owner.createPropertyNativeFloat("Y", [this]() {return cDefY;}, [this](float value) {cDefY = value;});
    owner.createPropertyNativeFloat("Z", [this]() {return cDefZ;}, [this](float value) {cDefZ = value;});
  }

  void SimpleVertex::removed() {
    // Nothing to do.
  }

  void SimpleVertex::reset() {
    cRuntimeX = cDefX;
    cRuntimeY = cDefY;
    cRuntimeZ = cDefZ;
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

  void SimpleVertex::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool SimpleVertex::isDefaultConfiguration() const {
    return true;
  }

  void SimpleVertex::setX(double x) {
    cRuntimeX = x;
    cStateNotifier->stateChanged();
  }

  void SimpleVertex::setY(double y) {
    cRuntimeY = y;
    cStateNotifier->stateChanged();
  }

  void SimpleVertex::setZ(double z) {
    cRuntimeZ = z;
    cStateNotifier->stateChanged();
  }
}
