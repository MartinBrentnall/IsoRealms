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

#include "IsoRealms/Assets/Type/IScreenListener.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/IStateListener.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetListener.h"
#include "IAssetUser.h"
#include "ILiteralAssetProvider.h"

namespace IsoRealms {
  class Project;

  class ScreenClientManager {
    private:
    class ScreenProxy;

    public:
    ScreenClientManager(ILiteralAssetProvider<IResourceData, IScreen>* literal) :
              cScreens(literal) {
    }

    void notifyAllScreensAdded(IProject& project) {
      for (std::pair<IScreen* const, std::unique_ptr<ScreenProxy>>& mPair : cScreenProxyMapping) {
        for (IScreenListener* mListener : cListeners) {
          mListener->screenAdded(project, mPair.second.get());
        }
      }
    }

    void renderScreen(IScreen* screen, float scale, float aspectRatio) {
      for (IScreenListener* mListener : cListeners) {
        mListener->screenPreRender(screen);
      }
      screen->renderScreen(scale, aspectRatio);
      for (IScreenListener* mListener : cListeners) {
        mListener->screenPostRender(screen);
      }
    }

    void addScreenListener(IScreenListener* listener) {
      cListeners.push_back(listener);
    }

    void removeScreenListener(IScreenListener* listener) {
      Utils::removeElement(cListeners, listener);
    }

    IScreen* add(IScreen* asset, const std::string& id, const std::string& category, bool stateChanges, Project& project);

    void add(IAssetProvider<IResourceData, IScreen>* provider, const std::string& id, const std::string& category, bool stateChanges = false) {
      cScreens.add(provider, id, category, stateChanges);
    }

    void remove(IScreen* asset) {
      std::map<IScreen*, std::unique_ptr<ScreenProxy>>::iterator mProxy = cScreenProxyMapping.find(asset);
      if (mProxy == cScreenProxyMapping.end()) {
        throw ArgumentException("ERROR: Project::remove: Proxy for specified screen asset not found.");
      }
      cScreens.remove(mProxy->second.get());
    }

    void remove(IAssetProvider<IResourceData, IScreen>* provider) {
      cScreens.remove(provider);
    }

    IScreen* literal(IAssetUser<IScreen>* client, IResourceData& owner, const std::string& value) {
      return cScreens.literal(client, owner, value);
    }

    IScreen* get(IAssetUser<IScreen>* client, IResourceData& owner, JSONObject object, IStateListener<IScreen*>* listener, bool required, std::function<IScreen*(JSONObject object, IStateListener<IScreen*>* listener)> override) {
      return cScreens.get(client, owner, object, listener, required, override);
    }

    IScreen* get(IAssetUser<IScreen>* client, IResourceData& owner, const std::string& id, IStateListener<IScreen*>* listener) {
      return cScreens.get(client, owner, id, listener);
    }

    void release(IAssetUser<IScreen>* client, IScreen* asset) {
      cScreens.release(client, asset);
    }

    std::vector<std::string> getAll() {
      return cScreens.getAll();
    }

    std::string getID(const IScreen* asset) const {
      return cScreens.getID(asset);
    }

    void save(JSONObject object, const IScreen* asset) const {
      cScreens.save(object, asset);
    }

    bool renderIcon(const std::string& id) const {
      return cScreens.renderIcon(id);
    }

    void addAssetListener(IAssetListener<IResourceData, IScreen>* listener) {
      cScreens.addAssetListener(listener);
    }

    bool hasConfiguration(const std::string& id) const {
      return cScreens.hasConfiguration(id);
    }

    bool hasReadOnlyReferences(IScreen* asset) {
      std::map<IScreen*, std::unique_ptr<ScreenProxy>>::iterator mProxy = cScreenProxyMapping.find(asset);
      if (mProxy == cScreenProxyMapping.end()) {
        throw ArgumentException("ERROR: Project::remove: Proxy for specified screen asset not found.");
      }
      return cScreens.hasReadOnlyReferences(mProxy->second.get());
    }

    bool hasReadOnlyReferences(IAssetProvider<IResourceData, IScreen>* provider) {
      return cScreens.hasReadOnlyReferences(provider);
    }

    void overrideReadOnlyReferences(IScreen* asset, File* owner) {
      std::map<IScreen*, std::unique_ptr<ScreenProxy>>::iterator mProxy = cScreenProxyMapping.find(asset);
      if (mProxy == cScreenProxyMapping.end()) {
        throw ArgumentException("ERROR: Project::remove: Proxy for specified screen asset not found.");
      }
      cScreens.overrideReadOnlyReferences(mProxy->second.get(), owner);
    }

    void overrideReadOnlyReferences(IAssetProvider<IResourceData, IScreen>* provider, File* owner) {
      cScreens.overrideReadOnlyReferences(provider, owner);
    }

    private:
    class ScreenProxy : public IScreen {
      public:
      ScreenProxy(ScreenClientManager& parent, IScreen* screen) :
                cParent(parent) {
        if (screen == nullptr) {
          throw std::invalid_argument("Screen cannot be nullptr");
        }
        cScreen = screen;
      }

      /**********************\
       * Implements IScreen *
      \**********************/
      void renderScreen(float scale, float aspectRatio) const override {
        cParent.renderScreen(cScreen, scale, aspectRatio);
      }

      bool renderAssetIcon() const override {
        return cScreen->renderAssetIcon();
      }

      void saveAsset(JSONObject object) const override {
        // Nothing to do.
      }

      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override {
        return std::vector<std::unique_ptr<IProperty>>();
      }

      bool isDefaultConfiguration() const override {
        return cScreen->isDefaultConfiguration();
      }

      const IFloat* getYaw() const override {
        return cScreen->getYaw();
      }

      const IFloat* getPitch() const override {
        return cScreen->getPitch();
      }

      private:
      ScreenClientManager& cParent;
      IScreen* cScreen;
    };

    AssetClientManager<IResourceData, IScreen> cScreens;
    std::map<IScreen*, std::unique_ptr<ScreenProxy>> cScreenProxyMapping;
    std::vector<IScreenListener*> cListeners;
  };
}
