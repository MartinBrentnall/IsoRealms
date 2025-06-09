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
#include "WorldView.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const std::string WorldView::JSON_CAMERA = "camera";
  const std::string WorldView::JSON_TYPE   = "type";
  const std::string WorldView::JSON_WORLD  = "world";
  const std::string WorldView::JSON_ZOOM   = "zoom";

  const std::string WorldView::TYPE_ZONE_VIEW = "ZoneView";

  WorldView::WorldView(IProject& project, Spindizzy& spindizzy, IResourceData& data) :
            cProjectCallbackManager(project),
            cSpindizzy(spindizzy),
            cDefWorld(nullptr),
            cDefCamera(spindizzy, *this),
            cDefZoneViewType(spindizzy, *this),
            cDefZoom(1.0f),
            cRuntimeZone(nullptr),
            cLuaBinding(project, this) {
    cProjectCallbackManager.reset([this]() {
      cRuntimeZone = nullptr;
      for (std::unique_ptr<ZoneView>& mZoneView : cRuntimeZoneViews) {
        mZoneView->cView->reset();
      }
    });
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

  void WorldView::registerAssets(IAssetRegistry& assets) {
    assets.add(static_cast<IScreen*>(this), "", "Spindizzy World Views");
    assets.add(&cLuaBinding, "", "Spindizzy World Views");
    LocalAssetRegistry mLocalRegistry(assets, "Camera");
    cDefCamera->registerAssets(mLocalRegistry);
  }

  void WorldView::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    assets.remove(static_cast<IScreen*>(this), relinquish);
    assets.remove(&cLuaBinding,                relinquish);
    cDefCamera->unregisterAssets(assets,       relinquish);
  }
  
  void WorldView::save(JSONObject object, IAssetIdentifier& identifier) const {
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

  std::vector<std::unique_ptr<IProperty>> WorldView::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
//    mProperties.emplace_back(std::make_unique<PropertyAsset<World>>(       "World",          "TODO", cDefWorld)); // TODO:
    mProperties.emplace_back(std::make_unique<PropertyAsset<Camera>>(      "Camera",         "TODO", cDefCamera));
    mProperties.emplace_back(std::make_unique<PropertyAsset<ZoneViewType>>("Zone View Type", "TODO", cDefZoneViewType));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>(        "Zoom",           "TODO", [this]() {return cDefZoom;}, [this](float value) {
      if (value > 0.0f) {
        cDefZoom = value;
        return true;
      }
      return false;
    }));
    return mProperties;
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

  std::vector<std::unique_ptr<IProperty>> WorldView::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool WorldView::isDefaultConfiguration() const {
    return true;
  }

  WorldView::ZoneView::ZoneView(Zone* zone, std::unique_ptr<IZoneView> view) :
            cZone(zone),
            cView(std::move(view)) {
  }
}
