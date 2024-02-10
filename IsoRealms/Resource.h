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

#include <string>

#include "Assets/Registry/AssetIDException.h"
#include "IAssetBrowser.h"
#include "IAssetRegistry.h"
#include "IProject.h"
#include "IResource.h"
#include "IResourceData.h"
#include "IResourceType.h"
#include "LocalAssetRegistry.h"
#include "Options/IOptions.h"
#include "Property/IPropertyListener.h"
#include "System.h"
#include "Utils.h"

namespace IsoRealms {
  template <class MODULE, class RESOURCE> class Resource : public IResource,
                                                           public IResourceData,
                                                           public IPropertyListener {
    private:
    IResourceType* cParent;
    std::string cName;
    unsigned int cPropertiesEditCount;
    RESOURCE cResourceHandle;
    LocalAssetRegistry cAssetRegistry;
    std::set<IPropertyListener*> cPropertyClients;

    public:
    Resource(IResourceType* parent, IProject* project, MODULE* module, IAssetRegistry* registry, const std::string& name) :
              cParent(parent),
              cName(name),
              cPropertiesEditCount(0),
              cResourceHandle(project, module),
              cAssetRegistry(registry, cName) {
      bool mSuccess = false;
      unsigned int mExistingNameCount = 1;
      do {
        try {
          cResourceHandle.registerAssets(&cAssetRegistry);
          mSuccess = true;
        } catch (AssetIDException& e) {
          std::cout << "WARNING: Resource::Resource: Caught AssetIDException on registering assets: " << e.what() << std::endl;
          cName = name + " " + Utils::toString(mExistingNameCount++);
        }
      } while (!mSuccess);
    }
    
    Resource(IResourceType* parent, IProject* project, MODULE* module, IAssetRegistry* registry, DOMNode& node, IOptions* options) :
              cParent(parent),
              cName(node.getAttribute("name")),
              cPropertiesEditCount(0),
              cResourceHandle(project, module, node, options, this),
              cAssetRegistry(registry, cName) {
    }
    
    RESOURCE* getResource() {
      return &cResourceHandle;
    }
    
    /************************\
     * Implements IResource *
    \************************/
    std::string getName() override {
      return cName;
    }
    
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IPropertyListener* listener) override {
      
      // FIXME:IsoRealmsEditor: Don't allow same client more than once?
      cPropertyClients.insert(listener);
      
      std::vector<IProperty*> mProperties;
      //mProperties.push_back(std::make_unique<PropertyNativeString>("Name", &cName));
      // FIXME:IsoRealmsEditor: This will handle renaming of resources.
//       [this](const std::string& value) {
//         cName = value;
//         try {
//           cResourceHandle.registerAssets(&cAssetRegistry);
//         } catch (AssetIDException& e) {
//           // FIXME:IsoRealmsEditor: We might be catching this too far up the stack.  Maybe catch in the resource itself, then hand back another exception (e.g. AssetRegistrationException)?
//           return false;
//         }
//         // FIXME:IsoRealmsEditor: Any existing data folder needs to be renamed to the new corresponding name
//         return true;
//       }));
      std::vector<IProperty*> mResourceProperties = cResourceHandle.getProperties(browser, &cAssetRegistry, this);
      mProperties.insert(std::end(mProperties), std::begin(mResourceProperties), std::end(mResourceProperties));
      return mProperties;
    }
    
    void finishEditing() override {
//       if (cPropertiesEditCount == 0) {
//         throw IllegalStateException("ERROR: Resource::finishEditing: There are no properties being edited at this time." << std::endl;
//       }
//       
//       if (--cPropertiesEditCount == 0) {
           // FIXME:IsoRealmsEditor: Should be deleted in the module (e.g. cResourceHandle.deleteProperty()), except for the first one.
//         cProperties.clear();
//       }
    }
    
    bool renderIcon() override {
      if (!cResourceHandle.renderIcon()) {
        Utils::renderIconLeaf();
      }
      return true;
    }
    
    void hintInUse(bool inUse) override {
      cResourceHandle.hintInUse(inUse);
    }

    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) override {
      node->addAttribute("name", cName);
      cResourceHandle.save(node, identifier);
    }
    
    bool isResource(const RESOURCE* resource) const {
      return resource == &cResourceHandle;
    }
    
    void registerAssets() override {
      cResourceHandle.registerAssets(&cAssetRegistry);
    }

    void unregisterAssets(IAssetRemover* assets, IAssets* releaser) override {
      cResourceHandle.unregisterAssets(assets, releaser);
    }
    
    /****************************\
     * Implements IResourceData *
    \****************************/
    std::unique_ptr<DOMNode> openForRead(const std::string& file, bool user) override {
      std::string mFullPath = cParent->getProjectPathPrefix(user) + getResourceDataPath() + "/" + file;
      return System::fileExists(mFullPath, true) ? std::make_unique<DOMNode>(mFullPath, DOMNode::Type::USER) : nullptr;
    }
    
    void write(DOMNodeWriter* node, const std::string& file) override {
      node->save(cParent->getProjectPathPrefix(true) + getResourceDataPath() + "/" + file);
    }
    
    std::string getPath(const std::string& file, bool user) const override {
      return cParent->getProjectPathPrefix(user) + getResourceDataPath() + "/" + file;
    }

    void makeUserDataDirectory() override {
      System::makeUserDataDirectory(cParent->getProjectPathPrefix(true) + getResourceDataPath());
    }

    void propertyAdded(IProperty* property, unsigned int index) override {
      for (IPropertyListener* mClient : cPropertyClients) {
        mClient->propertyAdded(property, index + 1);
      }
    }
    
    void propertyRemoved(IProperty* property) override {
      for (IPropertyListener* mClient : cPropertyClients) {
        mClient->propertyRemoved(property);
      }
    }
  };
}
