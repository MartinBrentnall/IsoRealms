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
#include "ComponentEditor.h"

#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Persistence/JSONThing.h"
#include "IsoRealms/Project/ComponentType.h"
#include "IsoRealms/Project/Module.h"
#include "IsoRealms/Project/Options.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/Project/Registry/IResourceProvider.h"

#include "Editing.h"
#include "IDialogManager.h"
#include "Property/IPropertyManager.h"
#include "Metadata.h"
#include "PropertyData.h"

namespace IsoRealms {
  ComponentEditor::ComponentEditor(Application& application, IDialogManager& dialogManager) :
            cApplication(application),
            cDialogManager(dialogManager) {
  }

  void ComponentEditor::refreshMetadata(Project& project) {

    // Clear any existing metadata.
    cModuleMetadata.clear();
    cApplicationMetadata.clear();
    cMetadata.clear();

    // Load application metadata.
    std::locale mLocale("");
    std::string mMetadataPath = "Metadata/IsoRealms." + mLocale.name();
    std::string::size_type mLastExtensionIndex = mMetadataPath.find_last_of('.');
    std::string::size_type mLastDashIndex = mMetadataPath.find_last_of('_');
    std::string::size_type mLastSeparatorIndex = (mLastExtensionIndex != std::string::npos && (mLastDashIndex == std::string::npos || mLastExtensionIndex > mLastDashIndex))
                                               ? mLastExtensionIndex
                                               : mLastDashIndex;
    while (!System::fileExists(mMetadataPath + ".json", false) && mLastSeparatorIndex != std::string::npos) {
      mMetadataPath = mMetadataPath.substr(0, mLastSeparatorIndex);
      mLastExtensionIndex = mMetadataPath.find_last_of('.');
      mLastDashIndex = mMetadataPath.find_last_of('_');
      mLastSeparatorIndex = (mLastExtensionIndex != std::string::npos && (mLastDashIndex == std::string::npos || mLastExtensionIndex > mLastDashIndex))
                          ? mLastExtensionIndex
                          : mLastDashIndex;
    }

    if (!System::fileExists(mMetadataPath + ".json", false)) {
      mMetadataPath = "Metadata/IsoRealms.en";
    }
    JSONDocument mMetadataDocument(mMetadataPath + ".json", false);
    JSONObject mRootObject(mMetadataDocument, mMetadataDocument.getDocument());
    for (JSONThing mSectionThing : mRootObject) {
      std::string mSectionName = mSectionThing.getName();
      JSONObject mSectionObject = mSectionThing.getValue();
      cApplicationMetadata.emplace(mSectionName, std::make_unique<Metadata>(mSectionObject));
    }

    const std::vector<std::unique_ptr<Module>>& mModules = project.getModules();
    for (const std::unique_ptr<Module>& mModule : mModules) {
      cModuleMetadata[mModule->getName()] = std::make_unique<ModuleMetadata>(*mModule, *cApplicationMetadata["Component"].get());
    }

    // Start with the application metadata.
    pushApplicationMetadata("Application");
  }

  void ComponentEditor::openMenu(IComponentData& parent, IPropertyManager& properties) {
    cMetadataDepthAtMenuOpen.push_back(cMetadata.size());
    cParents.push_back(&parent);
    cPropertyManagers.push_back(&properties);
  }

  void ComponentEditor::resetMetadataForMenu() {
    if (!cMetadataDepthAtMenuOpen.empty()) {
      while (cMetadata.size() > cMetadataDepthAtMenuOpen.back()) {
        cMetadata.pop_back();
      }
    }
  }

  void ComponentEditor::closeMenu() {
    resetMetadataForMenu();
    if (!cMetadataDepthAtMenuOpen.empty()) {
      cMetadataDepthAtMenuOpen.pop_back();
    }
    if (!cParents.empty()) {
      cParents.pop_back();
    }
    if (!cPropertyManagers.empty()) {
      cPropertyManagers.pop_back();
    }
  }

  IComponentData& ComponentEditor::getParent() const {
    return *cParents.back();
  }

  IPropertyManager& ComponentEditor::getProperties() const {
    return *cPropertyManagers.back();
  }

  void ComponentEditor::propertyAdd(const std::string& key, const std::string& value, std::function<void()> addPropertyFunction, const Options& hint) {
    getProperties().addProperty(std::make_unique<PropertyAdd>(mergePropertyMetadata(cMetadata.back()->getPropertyData(key), hint), *this, value, addPropertyFunction));
  }

  void ComponentEditor::propertyBoolean(const std::string& key, std::function<bool()> getter, std::function<void(bool)> setter, bool defaultValue, std::function<void()> removeFunction) {
    IComponentData& mParent = getParent();
    getProperties().addProperty(std::make_unique<PropertyNativeBoolean>(cMetadata.back()->getPropertyData(key), *this, mParent, getter, setter, mParent.getProject(), removeFunction));
  }

  void ComponentEditor::propertyCode(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<void()> removeFunction) {
    getProperties().addProperty(std::make_unique<PropertyCode>(getParent().getProject(), cMetadata.back()->getPropertyData(key), *this, getter, setter, removeFunction));
  }

  void ComponentEditor::propertyColourChannel(const std::string& key, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) {
    IComponentData& mParent = getParent();
    getProperties().addProperty(std::make_unique<PropertyColourChannel>(mParent, key, *cMetadata.back(), cMetadata.back()->getPropertyData(key), *this, valueFunction, minRed, minGreen, minBlue, minAlpha, maxRed, maxGreen, maxBlue, maxAlpha, confirmationCallback));
  }

  void ComponentEditor::propertyColourHue(const std::string& key, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    IComponentData& mParent = getParent();
    getProperties().addProperty(std::make_unique<PropertyColourHue>(mParent, key, *cMetadata.back(), cMetadata.back()->getPropertyData(key), *this, valueFunction, saturation, lightness, alpha, confirmationCallback));
  }

  void ComponentEditor::propertyColourLightness(const std::string& key, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) {
    IComponentData& mParent = getParent();
    getProperties().addProperty(std::make_unique<PropertyColourLightness>(mParent, key, *cMetadata.back(), cMetadata.back()->getPropertyData(key), *this, valueFunction, hue, saturation, alpha, confirmationCallback));
  }

  void ComponentEditor::propertyColourSaturation(const std::string& key, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    IComponentData& mParent = getParent();
    getProperties().addProperty(std::make_unique<PropertyColourSaturation>(mParent, key, *cMetadata.back(), cMetadata.back()->getPropertyData(key), *this, valueFunction, hue, lightness, alpha, confirmationCallback));
  }

  void ComponentEditor::propertyCondition(const std::string& key, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter, const Options& hint) {
    getProperties().addProperty(std::make_unique<PropertyCondition>(cMetadata.back()->getPropertyData(key), *this, availableElements, getter, setter));
  }

  void ComponentEditor::propertyEditor(const std::string& key, IEditable* editable) {
    getProperties().addProperty(std::make_unique<PropertyEditor>(cMetadata.back()->getPropertyData(key), *this, editable));
  }

  void ComponentEditor::propertyFloat(const std::string& key, std::function<float()> getter, std::function<void(float)> setter, float defaultValue, std::function<bool(float)> validityChecker, std::function<void()> removeFunction) {
    getProperties().addProperty(std::make_unique<PropertyNativeFloat>(cMetadata.back()->getPropertyData(key), *this, getter, validityChecker, setter, removeFunction));
  }

  void ComponentEditor::propertyInteger(const std::string& key, std::function<int()> getter, std::function<void(int)> setter, int defaultValue, std::function<bool(int)> validityChecker, std::function<void()> removeFunction, const Options& hint) {
    if (hint.getOption(Options::PROPERTY_NO_EDIT) == "true") {
      return;
    }
    getProperties().addProperty(std::make_unique<PropertyNativeInteger>(cMetadata.back()->getPropertyData(key), *this, getter, setter, validityChecker, removeFunction));
  }

  void ComponentEditor::propertyKey(const std::string& key, std::function<std::string()> getter, std::function<void(sf::Keyboard::Key)> setter, std::function<void()> removeFunction) {
    getProperties().addProperty(std::make_unique<PropertyKey>(cMetadata.back()->getPropertyData(key), *this, getter, setter, removeFunction));
  }

  void ComponentEditor::propertyList(const std::string& key, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, const std::string& defaultValue, std::function<void()> removeFunction) {
    IComponentData& mParent = getParent();
    getProperties().addProperty(std::make_unique<PropertyList>(*this, mParent, mParent.getProject(), cMetadata.back()->getPropertyData(key), options, getter, setter, removeFunction));
  }

  void ComponentEditor::propertyResource(const std::string& key, ITreeSelectorObject& item, const Options& hint, std::function<void()> removeFunction) {
    if (hint.getOption(Options::PROPERTY_NO_EDIT) == "true") {
      return;
    }
    getProperties().addProperty(std::make_unique<PropertyTreeSelector>(*this, getParent(), mergePropertyMetadata(cMetadata.back()->getPropertyData(key), hint), item, removeFunction));
  }

  void ComponentEditor::propertyString(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, const std::string& defaultValue, std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction, std::function<void(std::function<void()>, std::function<void()>)> confirmCustom) {
    getProperties().addProperty(std::make_unique<PropertyNativeString>(cMetadata.back()->getPropertyData(key), *this, getter, setter, validityChecker, removeFunction, confirmCustom));
  }

  void ComponentEditor::propertyOptional(const std::string& key, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback, std::function<std::string()> valueGetter, const Options& hint) {
    IComponentData& mParent = getParent();
    getProperties().addProperty(std::make_unique<PropertyOptional>(*this, mParent, mergePropertyMetadata(cMetadata.back()->getPropertyData(key), hint), choiceCallback, mParent.getProject(), cApplication, optionalSource, noneLabel, noneIcon, valueGetter));
  }

  void ComponentEditor::propertyUnsignedInteger(const std::string& key, std::function<unsigned int()> getter, std::function<void(unsigned int)> setter, unsigned int defaultValue, std::function<bool(unsigned int)> validityChecker, std::function<void()> removeFunction) {
    getProperties().addProperty(std::make_unique<PropertyNativeUnsignedInteger>(cMetadata.back()->getPropertyData(key), *this, getter, setter, validityChecker, removeFunction));
  }

  void ComponentEditor::scopeModule(Module& module, std::function<void()> removeFunction) {
    std::string mModuleName = module.getName();
    cModuleMetadata[mModuleName]->scopeCategories(*this, getProperties(), module, removeFunction);
  }

  void ComponentEditor::scope(const std::string& key, const std::string& value, std::function<void()> subProperties, std::function<void()> removeFunction, const Options& hint, std::function<bool()> icon) {
    if (hint.getOption(Options::PROPERTY_NO_EDIT) == "true") {
      return;
    }
    getProperties().addProperty(std::make_unique<PropertyStruct>(getParent(), mergePropertyMetadata(cMetadata.back()->getPropertyData(key), hint), *this, value, subProperties, removeFunction, icon));
  }

  void ComponentEditor::spacer(float height) {
    getProperties().addSpacer(height);
  }

  bool ComponentEditor::isComponentReadOnly() const {
    return getParent().isReadOnly();
  }

  void ComponentEditor::confirm(const std::string& message, std::function<void()> confirm, std::function<void()> cancel) {
    cDialogManager.confirm(message, confirm, cancel);
  }

  void ComponentEditor::promoteComponentToProject() {
    IComponentData& mParent = getParent();
    mParent.setOwner(mParent.getProject().getProjectFile());
  }
  
  void ComponentEditor::pushComponentTypeMetadata(const Metadata& metadata) {
    cMetadata.push_back(&metadata);
  }

  void ComponentEditor::popComponentTypeMetadata() {
    cMetadata.pop_back();
  }

  PropertyData ComponentEditor::mergePropertyMetadata(const PropertyData& metadata, const Options& hint) {
    std::string mName        = hint.hasOption("name")        ? hint.getOption("name")        : metadata.getName();
    std::string mDescription = hint.hasOption("description") ? hint.getOption("description") : metadata.getTooltip();
    std::string mValue       = hint.hasOption("value")       ? hint.getOption("value")       : metadata.getValue();
    return PropertyData(mName, mDescription, mValue);
  }

  void ComponentEditor::pushApplicationMetadata(const std::string& section) {
    cMetadata.push_back(cApplicationMetadata[section].get());
  }

  void ComponentEditor::popApplicationMetadata() {
    cMetadata.pop_back();
  }



// ================================ Load component type metadata.

  // void ComponentType::loadMetadata(JSONObject object) {


// ================================ Read module long name from metadata file.

      // Read the long name from the metadata file.
      // std::string mLongName;
      // try {
      //   std::string mMetadataPath = Module::getMetadataPath(mName);
      //   JSONDocument mMetadataDocument(mMetadataPath + ".json", false);
      //   mLongName = mMetadataDocument.hasMember("longName") ? mMetadataDocument.getString("longName") : mName;
      // } catch (const InitException& e) {
      //   std::cout << "ERROR: ModuleChooser::forEachAvailableTreeItem: " << e.getMessage() << std::endl;
      //   mLongName = mName;
      // }

}
