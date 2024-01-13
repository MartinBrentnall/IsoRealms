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
#include "ModelScreen.h"

#include "IsoRealms/Persistence/DOMNodeWriter.h"

namespace IsoRealms::Basics {
  const std::string ModelScreen::TAG_MODEL = "Model";

  ModelScreen::ModelScreen(IProject* project, Basics* basics) :
            cDefModel(project),
            cDefModelInstance(cDefModel.createInstance()) {
    project->updateRuntime([this](unsigned int milliseconds) {
      cDefModelInstance->update(milliseconds);
    });

    project->updateEditing([this](unsigned int milliseconds) {
      cDefModelInstance->update(milliseconds);
    });
  }

  ModelScreen::ModelScreen(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            ModelScreen(project, basics) {
    cDefModel.init(node.getNode(TAG_MODEL));
  }

  void ModelScreen::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Model Screens");
  }
  
  void ModelScreen::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }
  
  void ModelScreen::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefModel.save(node, TAG_MODEL);
  }

  void ModelScreen::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool ModelScreen::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> ModelScreen::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  void ModelScreen::renderScreen(float scale, float aspectRatio) const {
    cDefModelInstance->render();
  }

  bool ModelScreen::renderAssetIcon() const {
    return renderIcon();
  }
}
