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
#include "Project/Project.h"
#include "Project/Registry/IAssetProvider.h"

namespace IsoRealms {
  PropertyMaker::PropertyMaker(Application& application, IResourceData& parent, IPropertyManager& properties, IDialogManager& dialogManager) :
            cApplication(application),
            cParent(parent),
            cProperties(properties),
            cDialogManager(dialogManager) {
  }

  void PropertyMaker::createPropertyAdd(const PropertyData& metadata, const std::string& value, std::function<void()> addPropertyFunction) {
    cProperties.addProperty(std::make_unique<PropertyAdd>(metadata, *this, value, addPropertyFunction));
  }

  void PropertyMaker::createPropertyCode(const PropertyData& metadata, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyCode>(cParent.getProject(), metadata, *this, getter, setter, removeFunction));
  }
  
  void PropertyMaker::createPropertyColourChannel(const PropertyData& metadata, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourChannel>(*this, metadata, *this, valueFunction, minRed, minGreen, minBlue, minAlpha, maxRed, maxGreen, maxBlue, maxAlpha, confirmationCallback));
  }
  
  void PropertyMaker::createPropertyColourHue(const PropertyData& metadata, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourHue>(*this, metadata, *this, valueFunction, saturation, lightness, alpha, confirmationCallback));
  }
  
  void PropertyMaker::createPropertyColourLightness(const PropertyData& metadata, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourLightness>(*this, metadata, *this, valueFunction, hue, saturation, alpha, confirmationCallback));
  }
  
  void PropertyMaker::createPropertyColourSaturation(const PropertyData& metadata, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourSaturation>(*this, metadata, *this, valueFunction, hue, lightness, alpha, confirmationCallback));
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
  
  void PropertyMaker::createPropertyStruct(const PropertyData& metadata, const std::string& value, std::function<void(PropertyMaker&)> subProperties, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyStruct>(*this, metadata, *this, value, subProperties, removeFunction));
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
}

