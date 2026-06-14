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

#include "IsoRealms/Resources/Providers/ResourceLiteralDummy.h"
#include "IsoRealms/Resources/Type/IScreenListener.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/IStateListener.h"
#include "IsoRealms/Utils.h"

#include "ResourceClientManager.h"
#include "IResourceListener.h"
#include "IResourceUser.h"

namespace IsoRealms {
  class Project;

  class ScreenRegistry : public ResourceClientManager<ScreenRegistry, IComponentData, IScreen> {
    private:
    class Proxy;

    public:
    ScreenRegistry(Project& project) :
              ResourceClientManager(&cNone),
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

    IStateNotifier* add(IScreen* resource, const std::string& id, const std::string& category, bool stateChanges);
    IStateNotifier* add(IResourceProvider<IComponentData, IScreen>* provider, const std::string& id, const std::string& category, bool stateChanges);

    // void remove(IScreen* resource) {
    //   std::map<IScreen*, std::unique_ptr<Proxy>>::iterator mProxy = cProxyMapping.find(resource);
    //   if (mProxy == cProxyMapping.end()) {
    //     throw ArgumentException("ERROR: Project::remove: Proxy for specified screen resource not found.");
    //   }
    //   cScreens.remove(mProxy->second.get());
    // }
    //
    // bool hasReadOnlyReferences(IScreen* resource) const {
    //   std::map<IScreen*, std::unique_ptr<Proxy>>::const_iterator mProxy = cProxyMapping.find(resource);
    //   if (mProxy == cProxyMapping.end()) {
    //     throw ArgumentException("ERROR: Project::remove: Proxy for specified screen resource not found.");
    //   }
    //   return cScreens.hasReadOnlyReferences(mProxy->second.get());
    // }
    //
    // void overrideReadOnlyReferences(IScreen* resource, ProjectFile* owner) {
    //   std::map<IScreen*, std::unique_ptr<Proxy>>::iterator mProxy = cProxyMapping.find(resource);
    //   if (mProxy == cProxyMapping.end()) {
    //     throw ArgumentException("ERROR: Project::remove: Proxy for specified screen resource not found.");
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

      bool renderResourceIcon() const override {
        return cScreen->renderResourceIcon();
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
      bool renderResourceIcon() const override;
    };

    Project& cProject;
    ResourceLiteralDummy<IComponentData, IScreen, Dummy> cNone;
    std::map<IScreen*, std::unique_ptr<Proxy>> cProxyMapping;
    std::vector<IScreenListener*> cListeners;
  };
}
