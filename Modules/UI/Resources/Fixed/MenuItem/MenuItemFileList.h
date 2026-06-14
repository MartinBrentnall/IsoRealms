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

#include <algorithm>
#include <GL/glew.h>
#include <string>

#include "IsoRealms.h"

#include "Modules/UI/Resources/Type/IMenuItem.h"

namespace IsoRealms::UI {
  
  /**
   * Menu item that represents a list of files that can be selected.
   */
  class MenuItemFileList final : public IMenuItem {
    public:
    MenuItemFileList(const Metadata& metadata, Menu& menu);
    MenuItemFileList(const Metadata& metadata, Menu& menu, JSONObject object);

    /***********************\
     * Scripting Interface *
    \***********************/
    void refresh();
    
    /************************\
     * Implements IMenuItem *
    \************************/
    void getResourceProperties(IComponentDefiner& definer) override;
    bool isDefaultConfiguration() const override;
    void publish(ResourcePublisher& publisher) override;
    void reset() override;
    bool input(sf::Event& event) override;
    void selectTop() override;
    void selectBottom() override;
    void render(float aspectRatio, float y, bool selected, const Menu& menu) const override;
    float getHeight(const Menu& menu) const override;
    float getSelectedY(const Menu& menu) const override;
    std::string getTreeItemLabel() const override;

    private:
    
    // JSON members.
    inline static const std::string JSON_FOLDER       = "folder";
    inline static const std::string JSON_ID           = "id";
    inline static const std::string JSON_ON_SELECTION = "onSelection";
    inline static const std::string JSON_USER         = "user";

    // Constants.
    inline static const std::string BINDING_TYPE = "FileList";
  
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

    class SelectedFile final : public IString {
      public:
      SelectedFile(MenuItemFileList& parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool isDefaultConfiguration() const override;
      std::string getConversionPath() const override;
      bool isConfigurable() const override;

      private:

      // External interfaces.
      MenuItemFileList& cParent;
    };

    // External Interfaces.
    HatHandler& cHatHandler;
    Menu& cMenu;

    // Definition data.
    std::string cDefID;     /// ID of this menu item for binding.
    std::string cDefFolder; /// Path of folder to list files from.
    bool cDefUser = false;  /// True: path is relative to user data folder.  False: path is relative to program data folder.
    Action cDefAction;      /// Action that confirming a file selection will trigger.
    
    // Runtime data.
    std::vector<std::unique_ptr<File>> cRuntimeFiles; /// List of files that can be selected.
    int cRuntimeSelectedFile;                         /// Index of the selected file.
    
    // Scripting support.
    LuaBinding<MenuItemFileList> cLuaBinding; /// Allows menu file lists to be bound to lua variables.
    
    // Client resources.
    SelectedFile cSelectedFile;

    // Private functions.
    bool up();
    bool down();
    std::string getValue() const;
  };
}
