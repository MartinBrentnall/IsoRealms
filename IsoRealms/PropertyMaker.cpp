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
#include "PropertyMaker.h"

#include "Assets/Type/IStateNotifier.h"
#include "Editing.h"
#include "Editing/IDialogManager.h"
#include "Editing/Property/IPropertyManager.h"
#include "IResourceData.h"
#include "Metadata.h"
#include "Project/Options.h"
#include "Project/Project.h"
#include "Project/Registry/IAssetProvider.h"

namespace IsoRealms {
  PropertyMaker::PropertyMaker(Application& application, IResourceData& parent, const Metadata& metadata, IPropertyManager& properties, IDialogManager& dialogManager) :
            cApplication(application),
            cParent(parent),
            cMetadata(metadata),
            cProperties(properties),
            cDialogManager(dialogManager) {
  }

  IResourceData& PropertyMaker::getResourceData() {
    return cParent;
  }

  void PropertyMaker::createPropertyAdd(const PropertyData& metadata, const std::string& value, std::function<void()> addPropertyFunction) {
    cProperties.addProperty(std::make_unique<PropertyAdd>(metadata, *this, value, addPropertyFunction));
  }

  void PropertyMaker::createPropertyCode(const PropertyData& metadata, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyCode>(cParent.getProject(), metadata, *this, getter, setter, removeFunction));
  }
  
  void PropertyMaker::createPropertyColourChannel(const PropertyData& metadata, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourChannel>(*this, "", cMetadata, metadata, *this, valueFunction, minRed, minGreen, minBlue, minAlpha, maxRed, maxGreen, maxBlue, maxAlpha, confirmationCallback));
  }
  
  void PropertyMaker::createPropertyColourHue(const PropertyData& metadata, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourHue>(*this, "", cMetadata, metadata, *this, valueFunction, saturation, lightness, alpha, confirmationCallback));
  }
  
  void PropertyMaker::createPropertyColourLightness(const PropertyData& metadata, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourLightness>(*this, "", cMetadata, metadata, *this, valueFunction, hue, saturation, alpha, confirmationCallback));
  }
  
  void PropertyMaker::createPropertyColourSaturation(const PropertyData& metadata, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourSaturation>(*this, "", cMetadata, metadata, *this, valueFunction, hue, lightness, alpha, confirmationCallback));
  }

  void PropertyMaker::createPropertyCondition(const PropertyData& metadata, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter) {
    cProperties.addProperty(std::make_unique<PropertyCondition>(metadata, *this, availableElements, getter, setter));
  }

  void PropertyMaker::createPropertyEditor(const PropertyData& metadata, IEditable* editable) {
    cProperties.addProperty(std::make_unique<PropertyEditor>(metadata, *this, editable));
  }

  void PropertyMaker::createPropertyKey(const PropertyData& metadata, std::function<std::string()> getter, std::function<void(sf::Keyboard::Key)>  setter, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyKey>(metadata, *this, getter, setter, removeFunction));
  }
  
  void PropertyMaker::createPropertyList(const PropertyData& metadata, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyList>(*this, *this, cParent.getProject(), metadata, options, getter, setter, removeFunction));
  }
  
  void PropertyMaker::createPropertyNativeBoolean(const PropertyData& metadata, std::function<bool()>  getter, std::function<void(bool)>  setter, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyNativeBoolean>(*this, metadata, *this, getter, setter, cParent.getProject(), removeFunction));
  }

  void PropertyMaker::createPropertyNativeFloat(const PropertyData& metadata, std::function<float()> getter, std::function<void(float)> setter, std::function<bool(float)> validityChecker, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyNativeFloat>(metadata, *this, getter, validityChecker, setter, removeFunction));
  }

  void PropertyMaker::createPropertyNativeInteger(const PropertyData& metadata, std::function<int()> getter, std::function<void(int)> setter, std::function<bool(int)> validityChecker, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyNativeInteger>(metadata, *this, getter, setter, validityChecker, removeFunction));
  }

  void PropertyMaker::createPropertyNativeString(const PropertyData& metadata, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction, std::function<void(std::function<void()>, std::function<void()>)> confirmCustom) {
    cProperties.addProperty(std::make_unique<PropertyNativeString>(metadata, *this, getter, setter, validityChecker, removeFunction, confirmCustom));
  }
  
  void PropertyMaker::createPropertyNativeUnsignedInteger(const PropertyData& metadata, std::function<unsigned int()> getter, std::function<void(unsigned int)> setter, std::function<bool(unsigned int)> validityChecker, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyNativeUnsignedInteger>(metadata, *this, getter, setter, validityChecker, removeFunction));
  }
  
  void PropertyMaker::createPropertyOptional(const PropertyData& metadata, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback, std::function<std::string()> valueGetter) {
    cProperties.addProperty(std::make_unique<PropertyOptional>(*this, *this, cParent, metadata, choiceCallback, cParent.getProject(), cApplication, optionalSource, noneLabel, noneIcon, valueGetter));
  }

  void PropertyMaker::createPropertyStruct(const PropertyData& metadata, const std::string& value, std::function<void(IPropertyMaker&)> subProperties, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyStruct>(*this, metadata, *this, value, subProperties, removeFunction));
  }

  void PropertyMaker::createPropertyTreeSelector(const PropertyData& metadata, ITreeSelectorObject& item, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyTreeSelector>(*this, *this, cParent, metadata, item, removeFunction));
  }

  void PropertyMaker::createPropertyAdd(const std::string& key, const std::string& value, std::function<void()> addPropertyFunction) {
    createPropertyAdd(cMetadata.getPropertyData(key), value, addPropertyFunction);
  }

  void PropertyMaker::createPropertyCode(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<void()> removeFunction) {
    createPropertyCode(cMetadata.getPropertyData(key), getter, setter, removeFunction);
  }

  void PropertyMaker::createPropertyColourChannel(const std::string& key, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourChannel>(*this, key, cMetadata, cMetadata.getPropertyData(key), *this, valueFunction, minRed, minGreen, minBlue, minAlpha, maxRed, maxGreen, maxBlue, maxAlpha, confirmationCallback));
  }

  void PropertyMaker::createPropertyColourHue(const std::string& key, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourHue>(*this, key, cMetadata, cMetadata.getPropertyData(key), *this, valueFunction, saturation, lightness, alpha, confirmationCallback));
  }

  void PropertyMaker::createPropertyColourLightness(const std::string& key, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourLightness>(*this, key, cMetadata, cMetadata.getPropertyData(key), *this, valueFunction, hue, saturation, alpha, confirmationCallback));
  }

  void PropertyMaker::createPropertyColourSaturation(const std::string& key, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourSaturation>(*this, key, cMetadata, cMetadata.getPropertyData(key), *this, valueFunction, hue, lightness, alpha, confirmationCallback));
  }

  void PropertyMaker::createPropertyCondition(const std::string& key, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter) {
    createPropertyCondition(cMetadata.getPropertyData(key), availableElements, getter, setter);
  }

  void PropertyMaker::createPropertyEditor(const std::string& key, IEditable* editable) {
    createPropertyEditor(cMetadata.getPropertyData(key), editable);
  }

  void PropertyMaker::createPropertyKey(const std::string& key, std::function<std::string()> getter, std::function<void(sf::Keyboard::Key)> setter, std::function<void()> removeFunction) {
    createPropertyKey(cMetadata.getPropertyData(key), getter, setter, removeFunction);
  }

  void PropertyMaker::createPropertyList(const std::string& key, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, std::function<void()> removeFunction) {
    createPropertyList(cMetadata.getPropertyData(key), options, getter, setter, removeFunction);
  }

  void PropertyMaker::createPropertyNativeBoolean(const std::string& key, std::function<bool()> getter, std::function<void(bool)> setter, std::function<void()> removeFunction) {
    createPropertyNativeBoolean(cMetadata.getPropertyData(key), getter, setter, removeFunction);
  }

  void PropertyMaker::createPropertyNativeFloat(const std::string& key, std::function<float()> getter, std::function<void(float)> setter, std::function<bool(float)> validityChecker, std::function<void()> removeFunction) {
    createPropertyNativeFloat(cMetadata.getPropertyData(key), getter, setter, validityChecker, removeFunction);
  }

  void PropertyMaker::createPropertyNativeInteger(const std::string& key, std::function<int()> getter, std::function<void(int)> setter, std::function<bool(int)> validityChecker, std::function<void()> removeFunction) {
    createPropertyNativeInteger(cMetadata.getPropertyData(key), getter, setter, validityChecker, removeFunction);
  }

  void PropertyMaker::createPropertyNativeString(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction, std::function<void(std::function<void()>, std::function<void()>)> confirmCustom) {
    createPropertyNativeString(cMetadata.getPropertyData(key), getter, setter, validityChecker, removeFunction, confirmCustom);
  }

  void PropertyMaker::createPropertyNativeUnsignedInteger(const std::string& key, std::function<unsigned int()> getter, std::function<void(unsigned int)> setter, std::function<bool(unsigned int)> validityChecker, std::function<void()> removeFunction) {
    createPropertyNativeUnsignedInteger(cMetadata.getPropertyData(key), getter, setter, validityChecker, removeFunction);
  }

  void PropertyMaker::createPropertyOptional(const std::string& key, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback, std::function<std::string()> valueGetter) {
    createPropertyOptional(cMetadata.getPropertyData(key), optionalSource, noneLabel, noneIcon, choiceCallback, valueGetter);
  }

  void PropertyMaker::createPropertyStruct(const std::string& key, const std::string& value, std::function<void(IPropertyMaker&)> subProperties, std::function<void()> removeFunction) {
    createPropertyStruct(cMetadata.getPropertyData(key), value, subProperties, removeFunction);
  }

  void PropertyMaker::createPropertyTreeSelector(const std::string& key, ITreeSelectorObject& item, const Options& hint, std::function<void()> removeFunction) {
    createPropertyTreeSelector(mergePropertyMetadata(cMetadata.getPropertyData(key), hint), item, removeFunction);
  }

  bool PropertyMaker::isResourceReadOnly() const {
    return cParent.isReadOnly();
  }

  void PropertyMaker::confirm(const std::string& message, std::function<void()> confirm, std::function<void()> cancel) {
    cDialogManager.confirm(message, confirm, cancel);
  }

  void PropertyMaker::promoteResourceToProject() {
    cParent.setOwner(cParent.getProject().getProjectFile());
  }
  
  PropertyData PropertyMaker::mergePropertyMetadata(const PropertyData& metadata, const Options& hint) {
    std::string mName = hint.getOption("name");
    std::string mDescription = hint.getOption("description");
    return PropertyData(mName.empty() ? metadata.getName() : mName, mDescription.empty() ? metadata.getTooltip() : mDescription);
  }
}

