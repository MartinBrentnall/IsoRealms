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

#include "Modules/Equilibria/Equilibria.h"

namespace IsoRealms::Equilibria {
  WorldView::WorldView(Equilibria& equilibria, IComponentData& data) :
            cEquilibria(equilibria),
            cComponentData(data),
            cDefWorld(*this),
            cDefCamera(*this),
            cDefZoneViewType(*this),
            cDefZoom(DEFAULT_ZOOM),
            cRuntimeZone(nullptr),
            cLuaBinding(data.getProject().getLuaState(), this) {
  }
    
  void WorldView::registerAssets(ComponentAssetRegistry& assets) {
    assets.add<IScreen>(this, "", "Equilibria World Views");
    assets.add<IBinding>(&cLuaBinding, "", "Equilibria/World Views");
    cDefCamera->registerAssets(assets, "Camera");
  }

  void WorldView::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool WorldView::renderIcon() const {
    return false;
  }

  void WorldView::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    Options mHint;
    mHint.addOption(Options::PROPERTY_IMMEDIATE, "true");
    owner.createPropertyTreeSelector("world",  cDefWorld,        mHint);
    owner.createPropertyTreeSelector("camera", cDefCamera,       mHint);
    owner.createPropertyTreeSelector("type",   cDefZoneViewType, mHint);
    owner.createPropertyNativeFloat( "zoom",   [this]() {return cDefZoom;}, [this](float value) {cDefZoom = value;}, DEFAULT_ZOOM, [](float value) {return value > 0.0f;}); // TODO: Should this be part of the camera???  e.g. CameraZoom

    if (owner.loadsPersistedValues()) {
      cComponentData.getProject().init([this]() {
        cDefWorld->registerView(*this);
        std::vector<std::unique_ptr<Zone>>& mZones = cDefWorld->getZones();
        for (std::unique_ptr<Zone>& mZone : mZones) {
          addZoneView(mZone.get());
        }
      });
    }
  }

  void WorldView::removed() {
    // Nothing to do.
  }

  Equilibria& WorldView::getAssetManager() {
    return cEquilibria;
  }

  Project& WorldView::getProject() const {
    return cComponentData.getProject();
  }

  bool WorldView::isReadOnly() const {
    return cComponentData.isReadOnly();
  }

  void WorldView::setOwner(ProjectFile* owner) {
    cComponentData.setOwner(owner);
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
  
  void WorldView::registerAssets(const std::string& parentID) {
    cDefZoneViewType->registerAssets(cEquilibria, parentID + "/" + TYPE_ZONE_VIEW);
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
    return cDefWorld.get();
  }

  Equilibria& WorldView::getEquilibria() {
    return cEquilibria;
  }

  IComponentData& WorldView::getComponentData() {
    return cComponentData;
  }

  ICamera* WorldView::getCamera() {
    return *cDefCamera;
  }

  void WorldView::setZone(Zone* zone) {
    cRuntimeZone = zone;
    cDefCamera->setZone(zone);
  }
  
  void WorldView::renderScreen(float scale, float aspectRatio) const {
    if (cDefWorld.get() != nullptr) {
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

  void WorldView::getAssetProperties(IPropertyMaker& owner) {
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
