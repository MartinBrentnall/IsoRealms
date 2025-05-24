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
#include <string>

#include "IsoRealms/Assets/Registry/IAssetProvider.h"
#include "IsoRealms/Assets/Type/IStateNotifier.h"

namespace IsoRealms {
  class IModelInstance;
  class IModel;
  class IActionType;
  class IBinding;
  class IBindingType;
  class IBoolean;
  class IColour;
  class IDialogGenerator;
  class IEditable;
  class IFloat;
  class IFont;
  class IInputHandler;
  class IInteger;
  class IScreen;
  class IProjectOptions;
  class IAssets;
  template <class ASSET_TYPE> class IAssetUser;
  class IString;
  class ITexture;
  class IVertex;
  class Project;

  class IAssetRegistry {
    public:
    // TODO: Experimental. Add other types if successful.
    virtual void add(IAssetProvider<Project, IScreen>* provider, const std::string& id, const std::string& category) = 0;
      
    /**
     * Add the specified model type to this registry.
     * 
     * @param asset The model type to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual void add(IModel* asset, const std::string& id, const std::string& category) = 0;
      
    /**
     * Add the specified action type to this registry.
     * 
     * @param asset The action type to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual void add(IActionType* asset, const std::string& id, const std::string& category) = 0;
      
    /**
     * Add the specified binding to this registry.
     * 
     * @param asset The binding to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual void add(IBinding* asset, const std::string& id, const std::string& category) = 0;
      
    /**
     * Add the specified binding type to this registry.
     *
     * @param asset The binding type to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual void add(IBindingType* asset, const std::string& id, const std::string& category) = 0;

    /**
     * Add the specified boolean to this registry.
     * 
     * @param asset The boolean to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual IStateNotifier<IBoolean>* add(IBoolean* asset, const std::string& id, const std::string& category) = 0;
      
    /**
     * Add the specified colour to this registry.
     * 
     * @param asset The colour to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual IStateNotifier<IColour>* add(IColour* asset, const std::string& id, const std::string& category) = 0;
      
    /**
     * Add the specified editor context to this registry.
     *
     * @param asset The editor context to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual void add(IEditable* asset, const std::string& id, const std::string& category) = 0;

    /**
     * Add the specified float to this registry.
     * 
     * @param asset The float to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual IStateNotifier<IFloat>* add(IFloat* asset, const std::string& id, const std::string& category) = 0;
      
    /**
     * Add the specified font to this registry.
     * 
     * @param asset The font to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual void add(IFont* asset, const std::string& id, const std::string& category) = 0;
      
    /**
     * Add the specified input handler to this registry.
     * 
     * @param asset The input handler to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual void add(IInputHandler* asset, const std::string& id, const std::string& category) = 0;
      
    /**
     * Add the specified integer to this registry.
     * 
     * @param asset The integer to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual IStateNotifier<IInteger>* add(IInteger* asset, const std::string& id, const std::string& category) = 0;
      
    /**
     * Add the specified screen to this registry.
     * 
     * @param asset The screen to add.
     * @param id The ID of the asset.  May be an empty string (default).
     * @return The screen to be used by clients.
     */
    virtual IScreen* add(IScreen* asset, const std::string& id, const std::string& category) = 0;
      
    /**
     * Add the specified project options to this registry.
     * 
     * @param asset The project options to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual void add(IProjectOptions* asset, const std::string& id, const std::string& category) = 0;

    /**
     * Add the specified asset collection to this registry.  
     * 
     * @param asset The asset collection to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual void add(IAssets* asset, const std::string& id, const std::string& category) = 0;
      
    /**
     * Add the specified string to this registry.
     * 
     * @param asset The string to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual IStateNotifier<IString>* add(IString* asset, const std::string& id, const std::string& category) = 0;
      
    /**
     * Add the specified texture to this registry.
     * 
     * @param asset The texture to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual IStateNotifier<ITexture>* add(ITexture* asset, const std::string& id, const std::string& category) = 0;
      
    /**
     * Add the specified vertex to this registry.
     * 
     * @param asset The vertex to add.
     * @param id The ID of the asset.  May be an empty string (default).
     */
    virtual IStateNotifier<IVertex>* add(IVertex* asset, const std::string& id, const std::string& category) = 0;
  };
}
