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
  PropertyOptional::PropertyOptional(IPropertyMaker& owner, IResourceAccessManager& resourceAccessManager, IResourceData& resourceData, const PropertyData& data, std::function<void(const std::string&)> choiceCallback, Project& project, Application& application, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<std::string()> valueGetter) :
            Property(data, resourceAccessManager, nullptr),
            cNoneLabel(noneLabel),
            cNoneIcon(noneIcon),
            cValueGetter(valueGetter),
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

    // TODO: Probably shouldn't assume the value is none if we have a none option.
    const std::string mLabel = hasNoneOption() ? cNoneLabel : (cValueGetter ? cValueGetter() : "");
    mFont->print(x + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, mLabel);
    glPushMatrix();
    glTranslatef(x + mFontSize, y + mFontSize, 0.0f);
    glScalef(mFontSize, mFontSize, 0.0f);
    if (hasNoneOption()) {
      if (!cNoneIcon()) {
        Utils::renderIconNone();
      }
    } else if (!cWrapperType.renderAssetIcon()) {
      Utils::renderIconLeaf();
    }
    glPopMatrix();
  }

  float PropertyOptional::getValueWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    const std::string mLabel = hasNoneOption() ? cNoneLabel : (cValueGetter ? cValueGetter() : "");
    return mFont->getWidth(mFontSize, mLabel) + mFontSize * 2.25f;
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

  bool PropertyOptional::hasNoneOption() const {
    return !cNoneLabel.empty();
  }

  PropertyOptional::OptionWrapper::OptionWrapper(PropertyOptional& parent) :
            cParent(parent) {
  }

  TreeItemInfo PropertyOptional::OptionWrapper::getTreeItemInfo() const {
    if (cParent.hasNoneOption()) {
      return TreeItemInfo{"", cParent.cNoneLabel};
    }
    const std::string mLabel = cParent.cValueGetter();
    return TreeItemInfo{mLabel, mLabel};
  }

  std::string PropertyOptional::OptionWrapper::getTreeItemLabel() const {
    if (cParent.hasNoneOption()) {
      return "";
    }
    return cParent.cValueGetter();
  }

  bool PropertyOptional::OptionWrapper::renderAssetIcon() const {
    if (cParent.hasNoneOption()) {
      Utils::renderIconNone();
      return true;
    }
    return false;
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
    if (cParent.hasNoneOption()) {
      getTreeItemInfoFunction(TreeItemInfo{"", cParent.cNoneLabel});
    }
    cParent.cOptionalSource.forEachAvailableTreeItem(getTreeItemInfoFunction);
  }

  bool PropertyOptional::OptionWrapper::renderTreeItemIcon(const std::string& id) const {
    if (id == "" && cParent.hasNoneOption()) {
      if (!cParent.cNoneIcon()) {
        Utils::renderIconNone();
      }
    } else {
      Utils::renderIconBranch();
    }
    return true;
  }

  void PropertyOptional::OptionWrapper::setID(const std::string& id) {
    if (id != "") {
      cParent.cChoiceCallback(id);

      // Kinda hacky because PropertyMenu crashes if we refresh, but ProjectMenu doesn't (and that's the only place we need to refresh properties)
      // TODO: Fix the cause of the crash.
      if (cParent.hasNoneOption()) {
        cParent.cPropertyManager->refreshProperties();
      }
    }
  }
}
