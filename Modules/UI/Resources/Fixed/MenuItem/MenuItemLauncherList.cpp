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
#include "MenuItemLauncherList.h"

#include "Modules/UI/Menu/Menu.h"

namespace IsoRealms::UI {
  MenuItemLauncherList::MenuItemLauncherList(const Metadata& metadata, Menu& menu) :
            cHatHandler(menu.getComponentData().getProject().getApplication().getHatHandler()),
            cMenu(menu),
            cActionContext(menu.getComponentData(), *this),
            cDefAction(cActionContext),
            cLuaBinding(menu.getComponentData().getProject().getLuaState(), this),
            cLauncherBinding(menu.getComponentData().getProject().getLuaState(), nullptr, nullptr, true) {
  }

  MenuItemLauncherList::MenuItemLauncherList(const Metadata& metadata, Menu& menu, JSONObject object) :
            MenuItemLauncherList(metadata, menu) {
  }

  void MenuItemLauncherList::publish(ResourcePublisher& publisher) {
    publisher.publish<IBinding>(&cLuaBinding, BINDING_TYPE + "/" + cDefID, "Menu Items/Launcher Lists");
  }

  void MenuItemLauncherList::setProject(Project& project) {
    cRuntimeLaunchers.clear();
    for (int i = 0; i < project.getLaunchConfigurationCount(); i++) {
      cRuntimeLaunchers.emplace_back(std::make_unique<Launcher>(project.getLaunchConfiguration(i)));
    }
  }
  void MenuItemLauncherList::reset() {
    cRuntimeSelectedLauncher = 0;
  }

  bool MenuItemLauncherList::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Up:     return up();
          case sf::Keyboard::Down:   return down();
          case sf::Keyboard::Return: {
            cLauncherBinding.setValue(cRuntimeLaunchers[cRuntimeSelectedLauncher]->getLaunchConfiguration());
            cDefAction.execute();
            return true;
          }
          default:                   break;
        }
        break;
      }

      case sf::Event::JoystickMoved: {
        if (cHatHandler.upPressed())   {return up();}
        if (cHatHandler.downPressed()) {return down();}
        break;
      }

      case sf::Event::JoystickButtonPressed: {
        if (event.joystickButton.button == GameControllerButton::CROSS) {cDefAction.execute(); return true;}
        break;
      }

      default: break;
    }
    return false;
  }

  void MenuItemLauncherList::selectTop() {
    cRuntimeSelectedLauncher = 0;
  }

  void MenuItemLauncherList::selectBottom() {
    cRuntimeSelectedLauncher = cRuntimeLaunchers.size() - 1;
  }

  void MenuItemLauncherList::render(float aspectRatio, float y, bool selected, const Menu& menu) const {
    float mFontSize = menu.getFontSize();
    for (int i = 0; i < static_cast<int>(cRuntimeLaunchers.size()); i++) {
      cRuntimeLaunchers[i]->render(y - i * mFontSize * 2.0f, selected && cRuntimeSelectedLauncher == i, menu);
    }
  }

  float MenuItemLauncherList::getHeight(const Menu& menu) const {
    return menu.getFontSize() * 2.0f * cRuntimeLaunchers.size();
  }

  float MenuItemLauncherList::getSelectedY(const Menu& menu) const {
    return menu.getFontSize() * 2.0f * cRuntimeSelectedLauncher;
  }

  std::string MenuItemLauncherList::getTreeItemLabel() const {
    return cDefID;
  }

  void MenuItemLauncherList::getResourceProperties(IComponentDefiner& definer) {
    definer.propertyString(JSON_ID, [this]() {return cDefID;}, [this](const std::string& value) {
      cDefID = value;
      cMenu.getComponentData().republish();
    });
    definer.propertyResource(JSON_ON_SELECTION, cDefAction);
  }

  bool MenuItemLauncherList::isDefaultConfiguration() const {
    return false; // TODO: Implement this.
  }

  std::string MenuItemLauncherList::getBindingID(const IBinding* binding) const {
    if (binding == &cLauncherBinding) {
      return "Launcher";
    }
    return "";
  }

  IBinding* MenuItemLauncherList::getBinding(const std::string& id) {
    return id == "Launcher" ? &cLauncherBinding : nullptr;
  }

  void MenuItemLauncherList::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    getTreeItemInfoFunction(TreeItemInfo{"Launcher", "Launcher"});
  }

  void MenuItemLauncherList::releaseBinding(const IBinding* resource) {
    // Nothing to do.
  }

  MenuItemLauncherList::Launcher::Launcher(const ProjectLaunchConfiguration* configuration) :
            cDefLaunchConfiguration(configuration) {
  }

  void MenuItemLauncherList::Launcher::render(float y, bool selected, const Menu& menu) const {
    float mShadowOffset = menu.getShadowOffset();
    float mFontSize = menu.getFontSize();
    LiteralColour mWhite(1.0f, 1.0f, 1.0f);
    const IColour& mColour = selected ? static_cast<const IColour&>(**menu.getSelectionColour())
                                      : static_cast<const IColour&>(mWhite);
    Utils::shadowPrint(0.0, y, **menu.getFont(), mFontSize, mColour, mShadowOffset, IFont::Alignment::CENTER,  cDefLaunchConfiguration->getName());
  }

  std::string MenuItemLauncherList::Launcher::getName() const {
    return cDefLaunchConfiguration->getName();
  }

  const ProjectLaunchConfiguration* MenuItemLauncherList::Launcher::getLaunchConfiguration() const {
    return cDefLaunchConfiguration;
  }

  bool MenuItemLauncherList::up() {
    if (cRuntimeSelectedLauncher > 0) {
      cRuntimeSelectedLauncher--;
      return true;
    }
    return false;
  }

  bool MenuItemLauncherList::down() {
    if (cRuntimeSelectedLauncher < static_cast<int>(cRuntimeLaunchers.size()) - 1) {
      cRuntimeSelectedLauncher++;
      return true;
    }
    return false;
  }
}
