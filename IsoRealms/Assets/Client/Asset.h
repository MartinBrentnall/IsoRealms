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

#include <functional>

#include "IsoRealms/Assets/Registry/IAssetUser.h"
#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/IApplication.h"
#include "IsoRealms/Persistence/JSONDocument.h"

namespace IsoRealms {
  class IAssets;
  
  template<class TYPE, class MANAGER> class Asset : public IAssetUser<TYPE> {
    public:
    Asset(MANAGER& manager, TYPE* asset) :
              cManager(manager),
              cAsset(asset) {
    };

    virtual ~Asset() {
      if (cAsset != nullptr) {
        cManager.release(this, cAsset);
      }
    }
    
    void init(JSONObject object, const std::string& member) {
      cManager.getProject().init([this, object, member](IAssets& assets) {
        set(object, member);
      });
    }
    
    void set(JSONObject object, const std::string& member) {
      JSONObject mAssetObject = object.getObject(member);
      cManager.release(this, cAsset);
      cAsset = getAsset(cManager, mAssetObject);
      loadClientConfiguration(mAssetObject);
    }
    
    virtual void setID(const std::string& id) {
      cManager.release(this, cAsset);
      cAsset = getAsset(cManager, id);
      stateChanged(cAsset);
    }
    
    void save(JSONObject object, const std::string& name) const {
      JSONObject mAssetObject = object.addObject(name);
      saveClientConfiguration(mAssetObject);
      cManager.save(mAssetObject, cAsset);
    }

    virtual std::string getID() const {
      return cManager.getID(cAsset);
    }

    std::vector<std::unique_ptr<IProperty>> getAssetProperties() {
      std::vector<std::unique_ptr<IProperty>> mProperties = getClientProperties();
      std::vector<std::unique_ptr<IProperty>> mAssetProperties = getTheAssetProperties(cAsset);
      mProperties.insert(std::end(mProperties), std::make_move_iterator(std::begin(mAssetProperties)), std::make_move_iterator(std::end(mAssetProperties)));
      return mProperties;
    }

    virtual bool renderAssetIcon() const {
      return cAsset->renderAssetIcon();
    }

    bool isDefaultConfigured() const {
      return cAsset->isDefaultConfiguration() && isDefaultConfiguration();
    }

    TYPE* operator->() const {
      return cAsset;
    }

    TYPE* operator*() const {
      return cAsset;
    }

    IApplication& getApplication() {
      return cManager.getProject().getApplication();
    }

    /*******************************\
     * Implements IAssetUser<TYPE> *
    \*******************************/
    void relinquish(TYPE* asset) override {
      if (cAsset == asset) {
        cAsset = createLiteralAsset(cManager);
      }
    }

    bool renderProviderIcon(const std::string& id) const {
      return id == getID() ? renderAssetIcon() : renderOtherProviderIcon(id);
    }

    protected:
    MANAGER& cManager;
    TYPE* cAsset;

    virtual TYPE* createLiteralAsset(MANAGER& manager) = 0;
    virtual TYPE* getAsset(MANAGER& manager, JSONObject object) = 0;
    virtual TYPE* getAsset(MANAGER& manager, const std::string& id) = 0;
    virtual std::vector<std::string> getAvailableProviders() const = 0;
    virtual bool renderOtherProviderIcon(const std::string& id) const = 0;
    virtual bool hasConfiguration() const = 0;
    virtual bool isDefaultConfiguration() const = 0;
    virtual void stateChanged(TYPE* asset) {
      // Nothing to do.
    }
    virtual void loadClientConfiguration(JSONObject object) {
      // Nothing to do.
    }
    virtual void saveClientConfiguration(JSONObject object) const {
      // Nothing to do.
    }
    virtual std::vector<std::unique_ptr<IProperty>> getClientProperties() {
      return std::vector<std::unique_ptr<IProperty>>();
    }
    virtual std::vector<std::unique_ptr<IProperty>> getTheAssetProperties(TYPE* asset) {
      return asset->getAssetProperties();
    }

    private:
    Asset(Asset<TYPE, MANAGER> const& asset) = delete;
    Asset& operator=(Asset<TYPE, MANAGER> const& asset) = delete;
  };
}
