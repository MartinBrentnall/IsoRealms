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

#include <algorithm>
#include <GL/glew.h>
#include <string>

#include "IsoRealms/IAssetRegistry.h"
#include "IsoRealms/IAssetRemover.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/Literals.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/ProjectCallbackManager.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

#include "Modules/UI/Assets/Type/IMenuItem.h"

namespace IsoRealms::UI {
  
  /**
   * Menu item that represents a list of files that can be selected.
   */
  class MenuItemFileList final : public IMenuItem,
                                 public IString {
    public:
    MenuItemFileList(IProject& project, Menu& menu);
    MenuItemFileList(IProject& project, Menu& menu, JSONObject object);

    /***********************\
     * Scripting Interface *
    \***********************/
    void refresh();
    void reset();
    
    /************************\
     * Implements IMenuItem *
    \************************/
    void registerAssets(IAssetRegistry& assets) override;
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) override;
    bool input(sf::Event& event) override;
    void selectTop() override;
    void selectBottom() override;
    void render(float aspectRatio, float y, bool selected, const Menu& menu) const override;
    float getHeight(const Menu& menu) const override;
    float getSelectedY(const Menu& menu) const override;

    /**********************\
     * Implements IString *
    \**********************/
    std::string getValue() const override;

    /*******************************************\
     * Implements IAsset via IMenuItem/IString *
    \*******************************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:
    
    // JSON members.
    static const std::string JSON_FOLDER;
    static const std::string JSON_ID;
    static const std::string JSON_ON_SELECTION;
    static const std::string JSON_TYPE;
    static const std::string JSON_USER;

    // Constants.
    static const std::string BINDING_TYPE;
  
    // Private types.
    class File {
      public:
      
      // Interface to be used by the parent.
      File(const std::string& label, const std::string& path);
      void render(float y, bool selected, const Menu& menu) const;
      std::string getPath() const;
      
      private:
      
      // Definition data.
      std::string cDefLabel; /// Label to show for this file.
      std::string cDefPath;  /// The path of this file (relative to the data folder from which it originates).
    };

    // External Interfaces.
    ProjectCallbackManager cProjectCallbackManager;
    IProject& cProject;
    HatHandler& cHatHandler;

    // Definition data.
    std::string cDefID;     /// ID of this menu item for binding.
    std::string cDefFolder; /// Path of folder to list files from.
    bool cDefUser;   /// True: path is relative to user data folder.  False: path is relative to program data folder.
    Action cDefAction;      /// Action that confirming a file selection will trigger.
    
    // Runtime data.
    std::vector<std::unique_ptr<File>> cRuntimeFiles; /// List of files that can be selected.
    int cRuntimeSelectedFile;                         /// Index of the selected file.
    
    // Scripting support.
    LuaBinding<MenuItemFileList> cLuaBinding; /// Allows menu file lists to be bound to lua variables.
    
    // Private functions.
    bool up();
    bool down();
  };
}
