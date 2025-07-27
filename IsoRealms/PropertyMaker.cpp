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

#include "Assets/Registry/IAssetProvider.h"
#include "Assets/Type/IStateNotifier.h"
#include "Editing.h"
#include "Editing/IDialogManager.h"
#include "Editing/Property/IPropertyManager.h"
#include "IResourceData.h"
#include "Project.h"

namespace IsoRealms {
  PropertyMaker::PropertyMaker(IApplication& application, IResourceData& parent, IPropertyManager& properties, IDialogManager& dialogManager) :
            cApplication(application),
            cParent(parent),
            cProperties(properties),
            cDialogManager(dialogManager) {
  }

  void PropertyMaker::createPropertyAdd(const std::string& metadataKey, const std::string& value, std::function<void()> addPropertyFunction) {
    cProperties.addProperty(std::make_unique<PropertyAdd>(cParent.getPropertyData(metadataKey), *this, value, addPropertyFunction));
  }

  void PropertyMaker::createPropertyCode(const std::string& metadataKey, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyCode>(cParent.getProject(), cParent.getPropertyData(metadataKey), *this, getter, setter, removeFunction));
  }
  
  void PropertyMaker::createPropertyColourChannel(const std::string& metadataKey, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourChannel>(*this, cParent.getPropertyData(metadataKey), *this, valueFunction, minRed, minGreen, minBlue, minAlpha, maxRed, maxGreen, maxBlue, maxAlpha, confirmationCallback));
  }
  
  void PropertyMaker::createPropertyColourHue(const std::string& metadataKey, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourHue>(*this, cParent.getPropertyData(metadataKey), *this, valueFunction, saturation, lightness, alpha, confirmationCallback));
  }
  
  void PropertyMaker::createPropertyColourLightness(const std::string& metadataKey, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourLightness>(*this, cParent.getPropertyData(metadataKey), *this, valueFunction, hue, saturation, alpha, confirmationCallback));
  }
  
  void PropertyMaker::createPropertyColourSaturation(const std::string& metadataKey, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourSaturation>(*this, cParent.getPropertyData(metadataKey), *this, valueFunction, hue, lightness, alpha, confirmationCallback));
  }

  void PropertyMaker::createPropertyCondition(const std::string& metadataKey, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter) {
    cProperties.addProperty(std::make_unique<PropertyCondition>(cParent.getPropertyData(metadataKey), *this, availableElements, getter, setter));
  }

  void PropertyMaker::createPropertyEditor(const std::string& metadataKey, IEditable* editable) {
    cProperties.addProperty(std::make_unique<PropertyEditor>(cParent.getPropertyData(metadataKey), *this, editable));
  }

  void PropertyMaker::createPropertyKey(const std::string& metadataKey, std::function<std::string()> getter, std::function<void(sf::Keyboard::Key)>  setter, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyKey>(cParent.getPropertyData(metadataKey), *this, getter, setter, removeFunction));
  }
  
  void PropertyMaker::createPropertyList(const std::string& metadataKey, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyList>(*this, *this, cParent.getProject(), cParent.getPropertyData(metadataKey), options, getter, setter, removeFunction));
  }
  
  void PropertyMaker::createPropertyNativeBoolean(const std::string& metadataKey, std::function<bool()>  getter, std::function<void(bool)>  setter, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyNativeBoolean>(*this, cParent.getPropertyData(metadataKey), *this, getter, setter, cParent.getProject(), removeFunction));
  }

  void PropertyMaker::createPropertyNativeFloat(const std::string& metadataKey, std::function<float()> getter, std::function<void(float)> setter, std::function<bool(float)> validityChecker, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyNativeFloat>(cParent.getPropertyData(metadataKey), *this, getter, validityChecker, setter, removeFunction));
  }

  void PropertyMaker::createPropertyNativeInteger(const std::string& metadataKey, std::function<int()> getter, std::function<void(int)> setter, std::function<bool(int)> validityChecker, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyNativeInteger>(cParent.getPropertyData(metadataKey), *this, getter, setter, validityChecker, removeFunction));
  }

  void PropertyMaker::createPropertyNativeString(const std::string& metadataKey, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyNativeString>(cParent.getPropertyData(metadataKey), *this, getter, setter, validityChecker, removeFunction));
  }
  
  void PropertyMaker::createPropertyNativeUnsignedInteger(const std::string& metadataKey, std::function<unsigned int()> getter, std::function<void(unsigned int)> setter, std::function<bool(unsigned int)> validityChecker, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyNativeUnsignedInteger>(cParent.getPropertyData(metadataKey), *this, getter, setter, validityChecker, removeFunction));
  }
  
  void PropertyMaker::createPropertyStruct(const std::string& metadataKey, const std::string& value, std::function<void(PropertyMaker&)> subProperties, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyStruct>(*this, cParent.getPropertyData(metadataKey), *this, value, subProperties, removeFunction));
  }

  bool PropertyMaker::isResourceReadOnly() const {
    return cParent.isReadOnly();
  }

  void PropertyMaker::confirm(std::function<void()> confirm, std::function<void()> cancel) {
    cDialogManager.confirm("TODO: The resource you're modifying is read-only.  In order to modify this resource, it will need to be promoted to the main project file.", confirm, cancel);
  }

  void PropertyMaker::promoteResourceToProject() {
    cParent.setOwner(cParent.getProject().getFile());
  }
}

