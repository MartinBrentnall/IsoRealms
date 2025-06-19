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
#include "ScreenModel.h"

#include "IsoRealms/Utils.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::UI {
  ScreenModel::ScreenModel(IProject& project, IResourceData& owner) :
            cProjectCallbackManager(project),
            cDefModel(owner),
            cDefModelInstance(cDefModel.createInstance()) {
    cProjectCallbackManager.updateRuntime([this](unsigned int milliseconds) {
      cDefModelInstance->update(milliseconds);
    });

    cProjectCallbackManager.updateEditing([this](unsigned int milliseconds) {
      cDefModelInstance->update(milliseconds);
    });
  }
  
  ScreenModel::ScreenModel(IProject& project, IResourceData& owner, JSONObject object) :
            ScreenModel(project, owner) {
    cDefModel.set(object, JSON_MODEL);
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

  std::vector<std::unique_ptr<IProperty>> ScreenModel::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Model>>("Model", "TODO", cDefModel));
    return mProperties;
  }
  
  bool ScreenModel::isDefaultConfiguration() const {
    return cDefModel.isDefaultConfigured();
  }

  const std::string ScreenModel::JSON_MODEL = "model";
}
