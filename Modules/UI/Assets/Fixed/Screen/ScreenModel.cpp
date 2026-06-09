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
#include "ScreenModel.h"

namespace IsoRealms::UI {
  ScreenModel::ScreenModel(const Metadata& /*metadata*/, IResourceData& owner) :
            cDefModel(owner),
            cDefModelInstance(cDefModel.createInstance()) {
  }
  
  ScreenModel::ScreenModel(const Metadata& metadata, IResourceData& owner, JSONObject object) :
            ScreenModel(metadata, owner) {
    cDefModel.set(object, JSON_MODEL);
  }

  void ScreenModel::updateRuntime(unsigned int milliseconds) {
    cDefModelInstance->update(milliseconds);
  }

  void ScreenModel::updateEditing(unsigned int milliseconds) {
    cDefModelInstance->update(milliseconds);
  }

  void ScreenModel::renderScreen(float scale, float aspectRatio) const {
    cDefModelInstance->render();
  }

  bool ScreenModel::renderAssetIcon() const {
    return cDefModel.renderAssetIcon();
  }
  
  void ScreenModel::saveAsset(JSONObject object) const {
    cDefModel.save(object, JSON_MODEL);
  }

  void ScreenModel::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyTreeSelector(JSON_MODEL, cDefModel);
  }
  
  bool ScreenModel::isDefaultConfiguration() const {
    return cDefModel.isDefaultConfigured();
  }
}
