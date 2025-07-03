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
#include "MenuItemFileList.h"

#include "IsoRealms/Editing.h"
#include "IsoRealms/Input.h"

#include "Modules/UI/Menu/Menu.h"

namespace IsoRealms::UI {
  const std::string MenuItemFileList::JSON_FOLDER       = "folder";
  const std::string MenuItemFileList::JSON_ID           = "id";
  const std::string MenuItemFileList::JSON_ON_SELECTION = "onSelection";
  const std::string MenuItemFileList::JSON_TYPE         = "type";
  const std::string MenuItemFileList::JSON_USER         = "user";

  const std::string MenuItemFileList::BINDING_TYPE = "FileList";
  
  MenuItemFileList::MenuItemFileList(IProject& project, Menu& menu) :
            cProjectCallbackManager(project),
            cProject(project),
            cHatHandler(project.getApplication().getHatHandler()),
            cDefID(""),
            cDefFolder(""),
            cDefUser(false),
            cDefAction(menu.getResourceData().getDummyActionClient()),
            cLuaBinding(project, this) {
    cProjectCallbackManager.reset([this]() {
      refresh();
      cRuntimeSelectedFile = 0;
    });
  }

  MenuItemFileList::MenuItemFileList(IProject& project, Menu& menu, JSONObject object) :
            cProjectCallbackManager(project),
            cProject(project),
            cHatHandler(project.getApplication().getHatHandler()),
            cDefID(object.getString(JSON_ID)),
            cDefFolder(object.getString(JSON_FOLDER)),
            cDefUser(object.getBoolean(JSON_USER)),
            cDefAction(menu.getResourceData().getDummyActionClient()),
            cLuaBinding(project, this) {
    cDefAction.init(object, JSON_ON_SELECTION);
    cProjectCallbackManager.reset([this]() {
      refresh();
      cRuntimeSelectedFile = 0;
    });
  }

  void MenuItemFileList::refresh() {
    cRuntimeFiles.clear();
    std::string mFullPath = System::getPath(cDefFolder, cDefUser);
    std::vector<std::string> mFileList = System::getFileList(mFullPath);
    for (std::string mFile : mFileList) {
      std::size_t mExtensionPosition = mFile.find_last_of('.');
      std::string mProjectName = mFile.substr(0, mExtensionPosition);
      cRuntimeFiles.emplace_back(std::make_unique<File>(mProjectName, cDefFolder + "/" + mFile));
    }
    cRuntimeSelectedFile = std::clamp(cRuntimeSelectedFile, 0, std::max(0, static_cast<int>(cRuntimeFiles.size() - 1)));
  }
  
  void MenuItemFileList::reset() {
    refresh();
    cRuntimeSelectedFile = 0;
  }
  
  void MenuItemFileList::registerAssets(IAssetRegistry& assets) {
    assets.add(this, cDefID, "System");
    assets.add(&cLuaBinding, BINDING_TYPE + "/" + cDefID, "System");
  }
  
  bool MenuItemFileList::input(sf::Event& event) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Up:     return up();
          case sf::Keyboard::Down:   return down();
          case sf::Keyboard::Return: cDefAction.execute(); return true;
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
        if (event.joystickButton.button == ButtonMapping::CROSS) {cDefAction.execute(); return true;}
        break;
      }

      default: break;
    }
    return false;
  }

  void MenuItemFileList::selectTop() {
    cRuntimeSelectedFile = 0;
  }

  void MenuItemFileList::selectBottom() {
    cRuntimeSelectedFile = cRuntimeFiles.size() - 1;
  }

  void MenuItemFileList::render(float aspectRatio, float y, bool selected, const Menu& menu) const {
    float mFontSize = menu.getFontSize();
    for (int i = 0; i < static_cast<int>(cRuntimeFiles.size()); i++) {
      cRuntimeFiles[i]->render(y - i * mFontSize * 2.0f, selected && cRuntimeSelectedFile == i, menu);
    }
  }

  float MenuItemFileList::getHeight(const Menu& menu) const {
    return menu.getFontSize() * 2.0f * cRuntimeFiles.size();
  }

  float MenuItemFileList::getSelectedY(const Menu& menu) const {
    return menu.getFontSize() * 2.0f * cRuntimeSelectedFile;
  }

  bool MenuItemFileList::renderAssetIcon() const {
    return false;
  }

  void MenuItemFileList::saveAsset(JSONObject object) const {
    object.addString(JSON_ID,     cDefID);
    object.addString(JSON_FOLDER, cDefFolder);
    object.addBoolean(JSON_USER,  cDefUser);
    cDefAction.save(object, JSON_ON_SELECTION);
  }

  std::vector<std::unique_ptr<IProperty>> MenuItemFileList::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeString>( "ID",           "TODO", [this]() {return cDefID;},     [this](const std::string& value) {cDefID     = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeString>( "Folder",       "TODO", [this]() {return cDefFolder;}, [this](const std::string& value) {cDefFolder = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeBoolean>("User Data",    "TODO", [this]() {return cDefUser;},   [this](bool               value) {cDefUser   = value;},              cProject));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("On Selection", "TODO", cDefAction));
    return mProperties;
  }

  bool MenuItemFileList::isDefaultConfiguration() const {
    return false; // TODO: Implement this.
  }

  std::string MenuItemFileList::getValue() const {
    return cRuntimeFiles[cRuntimeSelectedFile]->getPath();
  }

  MenuItemFileList::File::File(const std::string& label, const std::string& path) :
            cDefLabel(label),
            cDefPath(path) {
  }

  void MenuItemFileList::File::render(float y, bool selected, const Menu& menu) const {
    float mShadowOffset = menu.getShadowOffset();
    float mFontSize = menu.getFontSize();
    LiteralColour mWhite(1.0f, 1.0f, 1.0f);
    const IColour& mColour = selected ? static_cast<const IColour&>(**menu.getSelectionColour())
                                      : static_cast<const IColour&>(mWhite);
    Utils::shadowPrint(0.0, y, **menu.getFont(), mFontSize, mColour, mShadowOffset, IFont::Alignment::CENTER,  cDefLabel);
  }

  std::string MenuItemFileList::File::getPath() const {
    return cDefPath;
  }

  bool MenuItemFileList::up() {
    if (cRuntimeSelectedFile > 0) {
      cRuntimeSelectedFile--;
      return true;
    }
    return false;
  }

  bool MenuItemFileList::down() {
    if (cRuntimeSelectedFile < static_cast<int>(cRuntimeFiles.size())) {
      cRuntimeSelectedFile++;
      return true;
    }
    return false;
  }
}
