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
#include "WorldView.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const std::string WorldView::JSON_CAMERA = "camera";
  const std::string WorldView::JSON_TYPE   = "type";
  const std::string WorldView::JSON_WORLD  = "world";
  const std::string WorldView::JSON_ZOOM   = "zoom";

  const std::string WorldView::TYPE_ZONE_VIEW = "ZoneView";

  WorldView::WorldView(IProject& project, Spindizzy& spindizzy, IResourceData& data) :
            cSpindizzy(spindizzy),
            cResourceData(data),
            cDefWorld(nullptr),
            cDefCamera(spindizzy, *this),
            cDefZoneViewType(spindizzy, *this),
            cDefZoom(1.0f),
            cRuntimeZone(nullptr),
            cLuaBinding(project, this) {
  }
    
  WorldView::WorldView(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options) :
            WorldView(project, spindizzy, data) {
    cDefZoom = object.getFloat(JSON_ZOOM, 1.0f);
    cDefCamera.set(object, JSON_CAMERA);
    cDefZoneViewType.set(object, JSON_TYPE);
    project.init([this, object](IAssets& resources) {
      cDefWorld = cSpindizzy.getWorld(object.getString(JSON_WORLD));
      cDefWorld->registerView(*this);
      std::vector<std::unique_ptr<Zone>>& mZones = cDefWorld->getZones();
      for (std::unique_ptr<Zone>& mZone : mZones) {
        addZoneView(mZone.get());
      }
    });
  }

  void WorldView::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IScreen>(this, "", "Spindizzy World Views");
    assets.add<IBinding>(&cLuaBinding, "", "Spindizzy World Views");
    cDefCamera->registerAssets(assets, "Camera");
  }

  void WorldView::save(JSONObject object) const {
    object.addString(JSON_WORLD, cSpindizzy.getID(cDefWorld));
    object.addFloat(JSON_ZOOM, cDefZoom, 1.0f);
    cDefCamera.save(object, JSON_CAMERA);
    cDefZoneViewType.save(object, JSON_TYPE);
  }

  void WorldView::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool WorldView::renderIcon() const {
    return false;
  }

  void WorldView::getProperties(PropertyMaker& owner) {
//    owner.createPropertyAsset<World>(       "World",        cDefWorld); // TODO:
    owner.createPropertyAsset<Camera>(      "Camera",       cDefCamera);
    owner.createPropertyAsset<ZoneViewType>("ZoneViewType", cDefZoneViewType);
    owner.createPropertyNativeFloat(        "Zoom",         [this]() {return cDefZoom;}, [this](float value) {cDefZoom = value;}, [](float value) {return value > 0.0f;}); // TODO: Should this be part of the camera???  e.g. CameraZoom
  }
  
  void WorldView::updateRuntime(unsigned int milliseconds) {
    cDefCamera->updateRuntime(milliseconds);
  }

  void WorldView::reset() {
    cRuntimeZone = nullptr;
    for (std::unique_ptr<ZoneView>& mZoneView : cRuntimeZoneViews) {
      mZoneView->cView->reset();
    }
    cDefCamera->reset();
  }
  
  void WorldView::registerAssets(ISpindizzyRegistry* registry) {
    LocalSpindizzyRegistry mLocalRegistry(registry, TYPE_ZONE_VIEW);
    cDefZoneViewType->registerAssets(&mLocalRegistry);
  }

  void WorldView::addZoneView(Zone* zone) {
    std::unique_ptr<IZoneView> mZoneView = cDefZoneViewType->createZoneView(zone);
    cRuntimeZoneViews.emplace_back(std::make_unique<ZoneView>(zone, std::move(mZoneView)));
  }
  
  void WorldView::removeZoneView(Zone* zone) {
    for (std::unique_ptr<ZoneView>& mZoneView : cRuntimeZoneViews) {
      if (zone == mZoneView->cZone) {
        Utils::removeElementUnique(cRuntimeZoneViews, mZoneView.get());
        break;
      }
    }
  }
  
  World* WorldView::getWorld() const {
    return cDefWorld;
  }

  Spindizzy& WorldView::getSpindizzy() {
    return cSpindizzy;
  }

  IResourceData& WorldView::getResourceData() {
    return cResourceData;
  }

  ICamera* WorldView::getCamera() {
    return *cDefCamera;
  }

  void WorldView::setZone(Zone* zone) {
    cRuntimeZone = zone;
    cDefCamera->setZone(zone);
  }
  
  void WorldView::renderScreen(float scale, float aspectRatio) const {
    if (cDefWorld != nullptr) {
      glEnable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);
      glBindTexture(GL_TEXTURE_2D, 0);
      glPushMatrix();
      float mZoom = std::max(cDefCamera->getXZoom() / aspectRatio, cDefCamera->getYZoom()) / cDefZoom;
      glOrtho(-mZoom, mZoom, -mZoom, mZoom, -60.0f, 60.0f); // TODO: Magic numbers -60.0f and 60.0f
      glRotatef(cDefCamera->getPitch()->getValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(cDefCamera->getYaw()->getValue(), 0.0f, 0.0f, 1.0f);
      glTranslatef(-cDefCamera->getXLocation(), -cDefCamera->getYLocation(), -cDefCamera->getZLocation());
      if (cRuntimeZone == nullptr) {
        for (const std::unique_ptr<ZoneView>& mZoneView : cRuntimeZoneViews) {
          mZoneView->cView->render(mZoneView->cZone, this);
        }
      } else {
// TODO: Use the actual cRuntimeZoneViews
        cRuntimeZone->renderRuntime(this);
        cDefWorld->renderRuntime();
      }
      glPopMatrix();
      glDisable(GL_DEPTH_TEST);
    }
  }

  const IFloat* WorldView::getYaw() const {
    return cDefCamera->getYaw();
  }

  const IFloat* WorldView::getPitch() const {
    return cDefCamera->getPitch();
  }

  bool WorldView::renderAssetIcon() const {
    return false;
  }

  void WorldView::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void WorldView::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool WorldView::isDefaultConfiguration() const {
    return true;
  }

  WorldView::ZoneView::ZoneView(Zone* zone, std::unique_ptr<IZoneView> view) :
            cZone(zone),
            cView(std::move(view)) {
  }
}
