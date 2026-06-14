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

#include "Resources/Type/IStateNotifier.h"
#include "Editing.h"
#include "Editing/IDialogManager.h"
#include "Editing/Property/IPropertyManager.h"
#include "IComponentData.h"
#include "Metadata.h"
#include "Project/Options.h"
#include "Project/Project.h"
#include "Project/Registry/IResourceProvider.h"
#include "PropertyData.h"

namespace IsoRealms {
  ComponentEditor::ComponentEditor(Application& application, IComponentData& parent, const Metadata& metadata, IPropertyManager& properties, IDialogManager& dialogManager) :
            cApplication(application),
            cParent(parent),
            cMetadata(metadata),
            cProperties(properties),
            cDialogManager(dialogManager) {
  }

  IComponentData& ComponentEditor::getComponentData() {
    return cParent;
  }

  void ComponentEditor::propertyAdd(const std::string& key, const std::string& value, std::function<void()> addPropertyFunction) {
    cProperties.addProperty(std::make_unique<PropertyAdd>(cMetadata.getPropertyData(key), *this, value, addPropertyFunction));
  }

  void ComponentEditor::propertyBoolean(const std::string& key, std::function<bool()> getter, std::function<void(bool)> setter, bool defaultValue, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyNativeBoolean>(*this, cMetadata.getPropertyData(key), *this, getter, setter, cParent.getProject(), removeFunction));
  }

  void ComponentEditor::propertyCode(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyCode>(cParent.getProject(), cMetadata.getPropertyData(key), *this, getter, setter, removeFunction));
  }

  void ComponentEditor::propertyColourChannel(const std::string& key, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourChannel>(*this, key, cMetadata, cMetadata.getPropertyData(key), *this, valueFunction, minRed, minGreen, minBlue, minAlpha, maxRed, maxGreen, maxBlue, maxAlpha, confirmationCallback));
  }

  void ComponentEditor::propertyColourHue(const std::string& key, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourHue>(*this, key, cMetadata, cMetadata.getPropertyData(key), *this, valueFunction, saturation, lightness, alpha, confirmationCallback));
  }

  void ComponentEditor::propertyColourLightness(const std::string& key, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourLightness>(*this, key, cMetadata, cMetadata.getPropertyData(key), *this, valueFunction, hue, saturation, alpha, confirmationCallback));
  }

  void ComponentEditor::propertyColourSaturation(const std::string& key, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    cProperties.addProperty(std::make_unique<PropertyColourSaturation>(*this, key, cMetadata, cMetadata.getPropertyData(key), *this, valueFunction, hue, lightness, alpha, confirmationCallback));
  }

  void ComponentEditor::propertyCondition(const std::string& key, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter) {
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
    cProperties.addProperty(std::make_unique<PropertyList>(*this, *this, cParent.getProject(), cMetadata.getPropertyData(key), options, getter, setter, removeFunction));
  }

  void ComponentEditor::propertyResource(const std::string& key, ITreeSelectorObject& item, const Options& hint, std::function<void()> removeFunction) {
    if (hint.getOption(Options::PROPERTY_NO_EDIT) == "true") {
      return;
    }
    cProperties.addProperty(std::make_unique<PropertyTreeSelector>(*this, *this, cParent, mergePropertyMetadata(cMetadata.getPropertyData(key), hint), item, removeFunction));
  }

  void ComponentEditor::propertyString(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, const std::string& defaultValue, std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction, std::function<void(std::function<void()>, std::function<void()>)> confirmCustom) {
    cProperties.addProperty(std::make_unique<PropertyNativeString>(cMetadata.getPropertyData(key), *this, getter, setter, validityChecker, removeFunction, confirmCustom));
  }

  void ComponentEditor::propertyOptional(const std::string& key, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback, std::function<std::string()> valueGetter, const Options& hint) {
    cProperties.addProperty(std::make_unique<PropertyOptional>(*this, *this, cParent, cMetadata.getPropertyData(key), choiceCallback, cParent.getProject(), cApplication, optionalSource, noneLabel, noneIcon, valueGetter));
  }

  void ComponentEditor::propertyUnsignedInteger(const std::string& key, std::function<unsigned int()> getter, std::function<void(unsigned int)> setter, unsigned int defaultValue, std::function<bool(unsigned int)> validityChecker, std::function<void()> removeFunction) {
    cProperties.addProperty(std::make_unique<PropertyNativeUnsignedInteger>(cMetadata.getPropertyData(key), *this, getter, setter, validityChecker, removeFunction));
  }

  void ComponentEditor::scope(const std::string& key, const std::string& value, std::function<void(IComponentDefiner&)> subProperties, std::function<void()> removeFunction, const Options& hint) {
    if (hint.getOption(Options::PROPERTY_NO_EDIT) == "true") {
      subProperties(*this);
      return;
    }
    cProperties.addProperty(std::make_unique<PropertyStruct>(*this, cMetadata.getPropertyData(key), *this, value, subProperties, removeFunction));
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
}
