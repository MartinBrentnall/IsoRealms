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
#pragma once

#include <cmath>
#include <GL/glew.h>

#include "IsoRealms/Types.h"

#include "PropertyAsset.h"

namespace IsoRealms {
  class PropertyAction : public PropertyAsset {
    private:
    IAssetUser<IAction>* cClient;
    IAssetBrowser* cBrowser;
    IBindingRegistry* cClientObjects;
    std::function<IAction*(IAction* action)> cConfirmationCallback;

    class DummyAction : public IAssetMenuItem {
      private:
      PropertyAction* cParent;

      public:
      DummyAction(PropertyAction* parent);

      /*****************************\
       * Implements IAssetMenuItem *
      \*****************************/
      void render(float yOffset) const override;
      bool executeAction() override;
      float getWidth() const override;
      std::vector<std::unique_ptr<ISelection>> getSelection(const std::string& id, const AssetMenu* menu, unsigned int item) const override;
    };

    std::string confirmSelection(const std::string& id);

    DummyAction cDummyAction;

    public:
    PropertyAction(const std::string& name, IAction* selected, IAssetUser<IAction>* client, IAssetBrowser* browser, IBindingRegistry* clientObjects, std::function<IAction*(IAction* action)> confirmationCallback);
    
    /************************************\
     * Implements PropertyAsset *
    \************************************/
    bool renderIcon(const std::string& id) const override;
  };
}
