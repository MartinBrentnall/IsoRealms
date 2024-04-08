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

#include "Modules/UI/Menu/Menu.h"

namespace IsoRealms::UI {
  const std::string MenuItemFileList::TAG_ACTION = "Action";
  const std::string MenuItemFileList::TAG_TYPE   = "FileList";
  
  const std::string MenuItemFileList::ATTRIBUTE_FOLDER = "folder";
  const std::string MenuItemFileList::ATTRIBUTE_ID     = "id";
  const std::string MenuItemFileList::ATTRIBUTE_USER   = "user";

  const std::string MenuItemFileList::BINDING_TYPE = "FileList";
  
  MenuItemFileList::MenuItemFileList(DOMNode& node, IProject* project) :
            cHatHandler(project->getApplication()->getHatHandler()),
            cDefID(node.getAttribute(ATTRIBUTE_ID)),
            cDefFolder(node.getAttribute(ATTRIBUTE_FOLDER)),
            cDefUser(node.getBooleanAttribute(ATTRIBUTE_USER)),
            cDefAction(project),
            cLuaBinding(project, this) {
    cDefAction.init(node, TAG_ACTION);
    project->reset([this]() {
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
  
  void MenuItemFileList::registerAssets(IAssetRegistry* assets) {
    assets->add(this, cDefID, "System");
    assets->add(&cLuaBinding, BINDING_TYPE + "/" + cDefID, "System");
  }
  
  void MenuItemFileList::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
    assets->remove(&cLuaBinding);
  }
  
  void MenuItemFileList::save(DOMNodeWriter* node) const {
    DOMNodeWriter mNode = node->addBranch(TAG_TYPE);
    mNode.addAttribute(ATTRIBUTE_ID,     cDefID);
    mNode.addAttribute(ATTRIBUTE_FOLDER, cDefFolder);
    mNode.addAttribute(ATTRIBUTE_USER,   cDefUser);
    cDefAction.save(&mNode, TAG_ACTION);
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

  std::string MenuItemFileList::getValue() const {
    return cRuntimeFiles[cRuntimeSelectedFile]->getPath();
  }

  bool MenuItemFileList::renderAssetIcon() const {
    return false;
  }

  MenuItemFileList::File::File(const std::string& label, const std::string& path) :
            cDefLabel(label),
            cDefPath(path) {
  }

  void MenuItemFileList::File::render(float y, bool selected, const Menu& menu) const {
    float mShadowOffset = menu.getShadowOffset();
    float mFontSize = menu.getFontSize();
    LiteralColour mWhite(1.0f, 1.0f, 1.0f);
    const IColour& mColour = selected ? static_cast<const IColour&>(menu.getSelectionColour())
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
