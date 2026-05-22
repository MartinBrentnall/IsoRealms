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
#include "PropertyOptional.h"

#include "IsoRealms/Assets/Type/IFont.h"
#include "IsoRealms/PropertyMaker.h"

namespace IsoRealms {
  PropertyOptional::PropertyOptional(IPropertyMaker& owner, IResourceAccessManager& resourceAccessManager, IResourceData& resourceData, const PropertyData& data, std::function<void(const std::string&)> choiceCallback, Project& project, Application& application, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon) :
            Property(data, resourceAccessManager, nullptr),
            cNoneLabel(noneLabel),
            cNoneIcon(noneIcon),
            cOptionalSource(optionalSource),
            cWrapperType(*this),
            cSubProperty(owner, resourceAccessManager, resourceData, data, cWrapperType),
            cChoiceCallback(choiceCallback),
            cPropertyManager(nullptr),
            cProject(project),
            cApplication(application) {
  }

  void PropertyOptional::renderValue(IUIStyle& style, float y, float x, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    mFont->print(x + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cNoneLabel);
    glPushMatrix();
    glTranslatef(x + mFontSize, y + mFontSize, 0.0f);
    glScalef(mFontSize, mFontSize, 0.0f);
    if (!cNoneIcon()) {
      Utils::renderIconNone();
    }
    glPopMatrix();
  }

  float PropertyOptional::getValueWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, cNoneLabel) + mFontSize * 2.25f;
  }

  void PropertyOptional::confirm(IPropertyManager& manager, float y) {
    cPropertyManager = &manager;
    cSubProperty.confirm(manager, y);
  }

  bool PropertyOptional::hasConfiguration() const {
    return false;
  }

  void PropertyOptional::configure(IPropertyManager& manager) {
    // Nothing to do.
  }

  PropertyOptional::OptionWrapper::OptionWrapper(PropertyOptional& parent) :
            cParent(parent) {
  }

  TreeItemInfo PropertyOptional::OptionWrapper::getTreeItemInfo() const {
    return TreeItemInfo{"None", cParent.cNoneLabel};
  }

  std::string PropertyOptional::OptionWrapper::getTreeItemLabel() const {
    return "None";
  }

  bool PropertyOptional::OptionWrapper::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  bool PropertyOptional::OptionWrapper::hasConfiguration() const {
    return false;
  }

  bool PropertyOptional::OptionWrapper::isDefaultConfigured() const {
    return true;
  }

  void PropertyOptional::OptionWrapper::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  Application& PropertyOptional::OptionWrapper::getApplication() {
    return cParent.cApplication;
  }

  void PropertyOptional::OptionWrapper::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    cParent.cOptionalSource.forEachAvailableTreeItem(getTreeItemInfoFunction);
    getTreeItemInfoFunction(TreeItemInfo{"None", cParent.cNoneLabel});
  }

  bool PropertyOptional::OptionWrapper::renderTreeItemIcon(const std::string& id) const {
    return false; // TODO cParent.cSubProperty.renderTreeItemIcon(id);
  }

  void PropertyOptional::OptionWrapper::setID(const std::string& id) {
    if (id != "None") {
      cParent.cChoiceCallback(id);
//          cParent.cPropertyManager->refreshProperties(); // TODO: Causes a crash when loading a module.
    }
  }
}
