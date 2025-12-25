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
#pragma once

#include <functional>

#include "IsoRealms/Assets/Providers/AssetLiteralDummy.h"
#include "IsoRealms/Assets/Type/IScreenListener.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/IStateListener.h"
#include "IsoRealms/Utils.h"

#include "AssetClientManager.h"
#include "IAssetListener.h"
#include "IAssetUser.h"

namespace IsoRealms {
  class Project;

  class ScreenRegistry : public AssetClientManager<ScreenRegistry, IResourceData, IScreen> {
    private:
    class Proxy;

    public:
    ScreenRegistry(Project& project) :
              AssetClientManager(&cLiteralProvider),
              cProject(project) {
    }

    void notifyAllScreensAdded() {
      for (std::pair<IScreen* const, std::unique_ptr<Proxy>>& mPair : cProxyMapping) {
        for (IScreenListener* mListener : cListeners) {
          mListener->screenAdded(mPair.second.get());
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

    IScreen* getProxy(IScreen* screen) {
      std::map<IScreen*, std::unique_ptr<Proxy>>::iterator mProxy = cProxyMapping.find(screen);
      if (mProxy == cProxyMapping.end()) {
        throw ArgumentException("ERROR: Project::getProxy: No proxy found for specified screen.  This means the screen has probably not been registered yet!");
      }
      return mProxy->second.get();
    }

    void addScreenListener(IScreenListener* listener) {
      cListeners.push_back(listener);
    }

    void removeScreenListener(IScreenListener* listener) {
      Utils::removeElement(cListeners, listener);
    }

    IStateNotifier* add(IScreen* asset, const std::string& id, const std::string& category, bool stateChanges);
    IStateNotifier* add(IAssetProvider<IResourceData, IScreen>* provider, const std::string& id, const std::string& category, bool stateChanges);

    // void remove(IScreen* asset) {
    //   std::map<IScreen*, std::unique_ptr<Proxy>>::iterator mProxy = cProxyMapping.find(asset);
    //   if (mProxy == cProxyMapping.end()) {
    //     throw ArgumentException("ERROR: Project::remove: Proxy for specified screen asset not found.");
    //   }
    //   cScreens.remove(mProxy->second.get());
    // }
    //
    // bool hasReadOnlyReferences(IScreen* asset) const {
    //   std::map<IScreen*, std::unique_ptr<Proxy>>::const_iterator mProxy = cProxyMapping.find(asset);
    //   if (mProxy == cProxyMapping.end()) {
    //     throw ArgumentException("ERROR: Project::remove: Proxy for specified screen asset not found.");
    //   }
    //   return cScreens.hasReadOnlyReferences(mProxy->second.get());
    // }
    //
    // void overrideReadOnlyReferences(IScreen* asset, ProjectFile* owner) {
    //   std::map<IScreen*, std::unique_ptr<Proxy>>::iterator mProxy = cProxyMapping.find(asset);
    //   if (mProxy == cProxyMapping.end()) {
    //     throw ArgumentException("ERROR: Project::remove: Proxy for specified screen asset not found.");
    //   }
    //   cScreens.overrideReadOnlyReferences(mProxy->second.get(), owner);
    // }

    private:
    class Proxy : public IScreen {
      public:
      Proxy(ScreenRegistry& parent, IScreen* screen) :
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

      void getAssetProperties(PropertyMaker& owner) override {
        // Nothing to do.
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
      ScreenRegistry& cParent;
      IScreen* cScreen;
    };

    class Dummy : public IScreen {
      public:

      /**********************\
      * Implements IScreen *
      \**********************/
      void renderScreen(float scale, float aspectRatio) const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(PropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;
    };

    Project& cProject;
    AssetLiteralDummy<IResourceData, IScreen, Dummy> cLiteralProvider;
    std::map<IScreen*, std::unique_ptr<Proxy>> cProxyMapping;
    std::vector<IScreenListener*> cListeners;
  };
}
