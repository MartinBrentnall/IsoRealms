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

#include "IsoRealms/Resources/Type/IStateNotifier.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Project/Options.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/Project/Registry/IResourceProvider.h"

#include "Editing.h"
#include "IDialogManager.h"
#include "Property/IPropertyManager.h"
#include "Metadata.h"
#include "PropertyData.h"

namespace IsoRealms {
  ComponentEditor::ComponentEditor(Application& application, IComponentData& parent, IPropertyManager& properties, IDialogManager& dialogManager) :
            cApplication(application),
            cParent(parent),
            cProperties(properties),
            cDialogManager(dialogManager) {
  }

  void ComponentEditor::propertyAdd(const std::string& key, const std::string& value, std::function<void()> addPropertyFunction) {
    cProperties.addProperty(std::make_unique<PropertyAdd>(cMetadata.getPropertyData(key), *this, value, addPropertyFunction));
  }

  void ComponentEditor::propertyBoolean(const std::string& key, std::function<bool()> getter, std::function<void(bool)> setter, bool defaultValue, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyNativeBoolean>(cMetadata.getPropertyData(key), *this, cParent, getter, setter, cParent.getProject(), removeFunction));
  }

  void ComponentEditor::propertyCode(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyCode>(cParent.getProject(), cMetadata.getPropertyData(key), *this, getter, setter, removeFunction));
  }

  void ComponentEditor::propertyColourChannel(const std::string& key, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourChannel>(cParent, key, cMetadata, cMetadata.getPropertyData(key), *this, valueFunction, minRed, minGreen, minBlue, minAlpha, maxRed, maxGreen, maxBlue, maxAlpha, confirmationCallback));
  }

  void ComponentEditor::propertyColourHue(const std::string& key, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourHue>(cParent, key, cMetadata, cMetadata.getPropertyData(key), *this, valueFunction, saturation, lightness, alpha, confirmationCallback));
  }

  void ComponentEditor::propertyColourLightness(const std::string& key, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourLightness>(cParent, key, cMetadata, cMetadata.getPropertyData(key), *this, valueFunction, hue, saturation, alpha, confirmationCallback));
  }

  void ComponentEditor::propertyColourSaturation(const std::string& key, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourSaturation>(cParent, key, cMetadata, cMetadata.getPropertyData(key), *this, valueFunction, hue, lightness, alpha, confirmationCallback));
  }

  void ComponentEditor::propertyCondition(const std::string& key, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter, const Options& hint) {
    cProperties.addProperty(std::make_unique<PropertyCondition>(cMetadata.getPropertyData(key), *this, availableElements, getter, setter));
  }

  void ComponentEditor::propertyEditor(const std::string& key, IEditable* editable) {
    cProperties.addProperty(std::make_unique<PropertyEditor>(cMetadata.getPropertyData(key), *this, editable));
  }

  void ComponentEditor::propertyFloat(const std::string& key, std::function<float()> getter, std::function<void(float)> setter, float defaultValue, std::function<bool(float)> validityChecker, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyNativeFloat>(cMetadata.getPropertyData(key), *this, getter, validityChecker, setter, removeFunction));
  }

  void ComponentEditor::propertyInteger(const std::string& key, std::function<int()> getter, std::function<void(int)> setter, int defaultValue, std::function<bool(int)> validityChecker, std::function<void()> removeFunction, const Options& hint) {
    if (hint.getOption(Options::PROPERTY_NO_EDIT) == "true") {
      return;
    }
    cProperties.addProperty(std::make_unique<PropertyNativeInteger>(cMetadata.getPropertyData(key), *this, getter, setter, validityChecker, removeFunction));
  }

  void ComponentEditor::propertyKey(const std::string& key, std::function<std::string()> getter, std::function<void(sf::Keyboard::Key)> setter, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyKey>(cMetadata.getPropertyData(key), *this, getter, setter, removeFunction));
  }

  void ComponentEditor::propertyList(const std::string& key, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, const std::string& defaultValue, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyList>(*this, cParent, cParent.getProject(), cMetadata.getPropertyData(key), options, getter, setter, removeFunction));
  }

  void ComponentEditor::propertyResource(const std::string& key, ITreeSelectorObject& item, const Options& hint, std::function<void()> removeFunction) {
    if (hint.getOption(Options::PROPERTY_NO_EDIT) == "true") {
      return;
    }
    cProperties.addProperty(std::make_unique<PropertyTreeSelector>(*this, cParent, mergePropertyMetadata(cMetadata.getPropertyData(key), hint), item, removeFunction));
  }

  void ComponentEditor::propertyString(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, const std::string& defaultValue, std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction, std::function<void(std::function<void()>, std::function<void()>)> confirmCustom) {
    cProperties.addProperty(std::make_unique<PropertyNativeString>(cMetadata.getPropertyData(key), *this, getter, setter, validityChecker, removeFunction, confirmCustom));
  }

  void ComponentEditor::propertyOptional(const std::string& key, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback, std::function<std::string()> valueGetter, const Options& hint) {
    cProperties.addProperty(std::make_unique<PropertyOptional>(*this, cParent, cMetadata.getPropertyData(key), choiceCallback, cParent.getProject(), cApplication, optionalSource, noneLabel, noneIcon, valueGetter));
  }

  void ComponentEditor::propertyUnsignedInteger(const std::string& key, std::function<unsigned int()> getter, std::function<void(unsigned int)> setter, unsigned int defaultValue, std::function<bool(unsigned int)> validityChecker, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyNativeUnsignedInteger>(cMetadata.getPropertyData(key), *this, getter, setter, validityChecker, removeFunction));
  }

  void ComponentEditor::scope(const std::string& key, const std::string& value, std::function<void(IComponentDefiner&)> subProperties, std::function<void()> removeFunction, const Options& hint) {
    if (hint.getOption(Options::PROPERTY_NO_EDIT) == "true") {
      return;
    }
    cProperties.addProperty(std::make_unique<PropertyStruct>(cParent, cMetadata.getPropertyData(key), *this, value, subProperties, removeFunction));
  }

  bool ComponentEditor::isComponentReadOnly() const {
    return cParent.isReadOnly();
  }

  void ComponentEditor::confirm(const std::string& message, std::function<void()> confirm, std::function<void()> cancel) {
    cDialogManager.confirm(message, confirm, cancel);
  }

  void ComponentEditor::promoteComponentToProject() {
    cParent.setOwner(cParent.getProject().getProjectFile());
  }
  
  PropertyData ComponentEditor::mergePropertyMetadata(const PropertyData& metadata, const Options& hint) {
    std::string mName = hint.getOption("name");
    std::string mDescription = hint.getOption("description");
    return PropertyData(mName.empty() ? metadata.getName() : mName, mDescription.empty() ? metadata.getTooltip() : mDescription);
  }



// ================================ Load component type metadata.

  // void ComponentType::loadMetadata(JSONObject object) {
  //   cSingular    = object.getString(JSON_SINGULAR);
  //   cPlural      = object.getString(JSON_PLURAL);
  //   cCategory    = object.getString(JSON_CATEGORY);
  //   cDescription = object.getString(JSON_DESCRIPTION);
  //   JSONObject mPropertiesObject = object.getObject(JSON_PROPERTIES);
  //   cMetadata.load(mPropertiesObject);
  //   cMetadata.setParent(&cParent.getProject().getApplication().getMetadata("Component"));
  // }

    // Load the component type metadata.  This needs to be done after the module is created.
    // JSONObject mComponentTypesObject = mMetadataDocument.getObject(JSON_COMPONENTS);
    // for (std::pair<const std::string, std::unique_ptr<ComponentType>>& mComponentType : cComponentTypes) {
    //   JSONObject mComponentTypeObject = mComponentTypesObject.getObject(mComponentType.first);
    //   mComponentType.second->loadMetadata(mComponentTypeObject);
    // }



// ================================ Obtain module metadata path. 

    // std::string Module::getMetadataPath(const std::string& name) {
    //   std::locale mLocale("");
    //   std::string mMetadataPath = "Metadata/" + name + "/" + name + "." + mLocale.name();
    //   std::string::size_type mLastExtensionIndex = mMetadataPath.find_last_of('.');
    //   std::string::size_type mLastDashIndex = mMetadataPath.find_last_of('_');
    //   std::string::size_type mLastSeparatorIndex = (mLastExtensionIndex != std::string::npos && (mLastDashIndex == std::string::npos || mLastExtensionIndex > mLastDashIndex))
    //                                              ? mLastExtensionIndex
    //                                              : mLastDashIndex;
    //   while (!System::fileExists(mMetadataPath + ".json", false) && mLastSeparatorIndex != std::string::npos) {
    //     mMetadataPath = mMetadataPath.substr(0, mLastSeparatorIndex);
    //     mLastExtensionIndex = mMetadataPath.find_last_of('.');
    //     mLastDashIndex = mMetadataPath.find_last_of('_');
    //     mLastSeparatorIndex = (mLastExtensionIndex != std::string::npos && (mLastDashIndex == std::string::npos || mLastExtensionIndex > mLastDashIndex))
    //                         ? mLastExtensionIndex
    //                         : mLastDashIndex;
    //   }
  
    //   if (!System::fileExists(mMetadataPath + ".json", false)) {
    //     throw InitException("ERROR: Module::getMetadataPath: No metadata file found for module \"" + name + "\".");
    //   }
    //   return mMetadataPath;
    // }
  
  


// ================================ Load module metadata.

    // Load the metadata file.
    // std::string mMetadataPath = getMetadataPath(cName);
    // JSONDocument mMetadataDocument(mMetadataPath + ".json", false);
    
    // // Load the module and resource metadata.  This needs to be done before the module is created.
    // cDescription = mMetadataDocument.getString(JSON_DESCRIPTION);
    // cLongName = mMetadataDocument.hasMember(JSON_LONG_NAME) ? mMetadataDocument.getString(JSON_LONG_NAME) : cName;

    // if (mMetadataDocument.hasMember(JSON_CATEGORIES)) {
    //   JSONObject mCategoriesObject = mMetadataDocument.getObject(JSON_CATEGORIES);
    //   for (JSONThing mCategoryThing : mCategoriesObject) {
    //     std::string mCategoryDescription = mCategoryThing.getValueAsString();
    //     std::string mCategoryName = mCategoryThing.getName();
    //     cCategoryDescriptions[mCategoryName] = mCategoryDescription;
    //   }
    // }

    // JSONObject mResourcesObject = mMetadataDocument.getObject(JSON_RESOURCES);
    // for (JSONThing mResourceThing : mResourcesObject) {
    //   JSONObject mResourceObject = mResourceThing.getValue();
    //   std::string mResourceName = mResourceThing.getName();
    //   cResourceMetadata[mResourceName] = std::make_unique<Metadata>();
    //   JSONObject mPropertiesObject = mResourceObject.getObject(JSON_PROPERTIES);
    //   cResourceMetadata[mResourceName]->load(mPropertiesObject);
    // }



// ================================ Load application metadata.    

    // Load application metadata.
    // std::locale mLocale("");
    // std::string mMetadataPath = "Metadata/IsoRealms." + mLocale.name();
    // std::string::size_type mLastExtensionIndex = mMetadataPath.find_last_of('.');
    // std::string::size_type mLastDashIndex = mMetadataPath.find_last_of('_');
    // std::string::size_type mLastSeparatorIndex = (mLastExtensionIndex != std::string::npos && (mLastDashIndex == std::string::npos || mLastExtensionIndex > mLastDashIndex))
    //                                            ? mLastExtensionIndex
    //                                            : mLastDashIndex;
    // while (!System::fileExists(mMetadataPath + ".json", false) && mLastSeparatorIndex != std::string::npos) {
    //   mMetadataPath = mMetadataPath.substr(0, mLastSeparatorIndex);
    //   mLastExtensionIndex = mMetadataPath.find_last_of('.');
    //   mLastDashIndex = mMetadataPath.find_last_of('_');
    //   mLastSeparatorIndex = (mLastExtensionIndex != std::string::npos && (mLastDashIndex == std::string::npos || mLastExtensionIndex > mLastDashIndex))
    //                       ? mLastExtensionIndex
    //                       : mLastDashIndex;
    // }

    // if (!System::fileExists(mMetadataPath + ".json", false)) {
    //   mMetadataPath = "Metadata/IsoRealms.en";
    // }
    // JSONDocument mMetadataDocument(mMetadataPath + ".json", false);
    // JSONObject mRootObject(mMetadataDocument, mMetadataDocument.getDocument());
    // for (JSONThing mSectionThing : mRootObject) {
    //   std::string mSectionName = mSectionThing.getName();
    //   JSONObject mSectionObject = mSectionThing.getValue();
    //   std::unique_ptr<Metadata> mSectionMetadata = std::make_unique<Metadata>();
    //   mSectionMetadata->load(mSectionObject);
    //   cMetadata[mSectionName] = std::move(mSectionMetadata);
    // }



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
