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

#include "Modules/UI/Assets/Type/IMenuItem.h"

namespace IsoRealms::UI {

  /**
   * Menu item that represents a list of project launchers that can be selected.
   */
  class MenuItemLauncherList final : public IMenuItem,
                                     public IBindingRegistry {
    public:
    MenuItemLauncherList(const Metadata& metadata, Menu& menu);
    MenuItemLauncherList(const Metadata& metadata, Menu& menu, JSONObject object);

    /***********************\
     * Scripting Interface *
    \***********************/
    void setProject(Project& project);

    /************************\
     * Implements IMenuItem *
    \************************/
    void registerAssets(ResourceAssetRegistry& assets) override;
    void reset() override;
    bool input(sf::Event& event) override;
    void selectTop() override;
    void selectBottom() override;
    void render(float aspectRatio, float y, bool selected, const Menu& menu) const override;
    float getHeight(const Menu& menu) const override;
    float getSelectedY(const Menu& menu) const override;
    std::string getTreeItemLabel() const override;

    /*******************************************\
     * Implements IAsset via IMenuItem/IString *
    \*******************************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    /*******************************\
     * Implements IBindingRegistry *
    \*******************************/
    IBinding* getBinding(const std::string& id) override;
    void saveBinding(JSONObject object, const IBinding* binding) const override;
    void releaseBinding(const IBinding* asset) override;

    private:

    // JSON members.
    static const std::string JSON_ID;
    static const std::string JSON_ON_SELECTION;
    inline static const std::string JSON_LOCAL = "local";

    // Constants.
    static const std::string BINDING_TYPE;

    // Private types.
    class Launcher {
      public:

      // Interface to be used by the parent.
      Launcher(const ProjectLaunchConfiguration* configuration);
      void render(float y, bool selected, const Menu& menu) const;
      std::string getName() const;
      const ProjectLaunchConfiguration* getLaunchConfiguration() const;

      private:

      // Definition data.
      const ProjectLaunchConfiguration* cDefLaunchConfiguration;
    };

    // External Interfaces.
    const Metadata& cMetadata;
    HatHandler& cHatHandler;

    // Action client.
    ActionClient cActionClient;

    // Definition data.
    std::string cDefID; /// ID of this menu item for binding.
    Action cDefAction;  /// Action that confirming a launcher selection will trigger.

    // Runtime data.
    std::vector<std::unique_ptr<Launcher>> cRuntimeLaunchers; /// List of launchers that can be selected.
    int cRuntimeSelectedLauncher;                             /// Index of the selected launchers.

    // Scripting support.
    LuaBinding<MenuItemLauncherList> cLuaBinding;                 /// Allows menu launcher lists to be bound to lua variables.
    LocalLuaBinding<const ProjectLaunchConfiguration> cLauncherBinding; /// The binding to the selected launcher.

    // Private functions.
    bool up();
    bool down();
  };
}
