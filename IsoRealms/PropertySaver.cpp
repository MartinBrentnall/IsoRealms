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
#include "PropertySaver.h"

#include "Assets/Type/IEditable.h"
#include "Condition/Condition.h"
#include "Editing/Property/ITreeSelectorObject.h"
#include "IResourceData.h"

namespace IsoRealms {
  PropertySaver::PropertySaver(IResourceData& resourceData, JSONObject object) :
            cResourceData(resourceData) {
    cObjects.push_back(object);
  }

  IResourceData& PropertySaver::getResourceData() {
    return cResourceData;
  }

  JSONObject& PropertySaver::currentObject() {
    return cObjects.back();
  }

  const JSONObject& PropertySaver::currentObject() const {
    return cObjects.back();
  }

  void PropertySaver::pushObject(JSONObject object) {
    cObjects.push_back(object);
  }

  void PropertySaver::popObject() {
    cObjects.pop_back();
  }

  bool PropertySaver::beginSavePropertyArray(const std::string& key) {
    currentObject().addArray(key);
    cSaveArrayKeys.push_back(key);
    return true;
  }

  void PropertySaver::beginSavePropertyArrayElement() {
    pushObject(currentObject().getArray(cSaveArrayKeys.back()).addObject());
  }

  void PropertySaver::endSavePropertyArrayElement() {
    popObject();
  }

  void PropertySaver::endSavePropertyArray() {
    cSaveArrayKeys.pop_back();
  }

  void PropertySaver::saveTreeSelectorAssetProperties(const ITreeSelectorObject& item, JSONObject object, const Options& hint) const {
    if (item.hasConfiguration()) {
      PropertySaver mSaver(cResourceData, object);
      const_cast<ITreeSelectorObject&>(item).getAssetProperties(mSaver);
    }
  }

  void PropertySaver::createPropertyAdd(const std::string& key, const std::string& value, std::function<void()> addPropertyFunction) {
    // Nothing to do.
  }

  void PropertySaver::createPropertyCode(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<void()> removeFunction) {
    currentObject().addString(key, getter());
  }

  void PropertySaver::createPropertyColourChannel(const std::string& key, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) {
    currentObject().addFloat(key, valueFunction());
  }

  void PropertySaver::createPropertyColourHue(const std::string& key, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    // This is a calculated property, so we don't need to save it to JSON.
  }

  void PropertySaver::createPropertyColourLightness(const std::string& key, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) {
    // This is a calculated property, so we don't need to save it to JSON.
  }

  void PropertySaver::createPropertyColourSaturation(const std::string& key, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    // This is a calculated property, so we don't need to save it to JSON.
  }

  void PropertySaver::createPropertyCondition(const std::string& key, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter) {
    std::optional<Condition>& mCondition = getter();
    if (mCondition.has_value()) {
      mCondition->save(currentObject().addObject(key));
    }
  }

  void PropertySaver::createPropertyEditor(const std::string& key, IEditable* editable) {
    editable->save(cResourceData, currentObject());
  }

  void PropertySaver::createPropertyKey(const std::string& key, std::function<std::string()> getter, std::function<void(sf::Keyboard::Key)> setter, std::function<void()> removeFunction) {
    currentObject().addString(key, getter());
  }

  void PropertySaver::createPropertyList(const std::string& key, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, const std::string& defaultValue, std::function<void()> removeFunction) {
    currentObject().addString(key, getter(), defaultValue);
  }

  void PropertySaver::createPropertyNativeBoolean(const std::string& key, std::function<bool()> getter, std::function<void(bool)> setter, bool defaultValue, std::function<void()> removeFunction) {
    currentObject().addBoolean(key, getter(), defaultValue);
  }

  void PropertySaver::createPropertyNativeFloat(const std::string& key, std::function<float()> getter, std::function<void(float)> setter, float defaultValue, std::function<bool(float)> validityChecker, std::function<void()> removeFunction) {
    currentObject().addFloat(key, getter(), defaultValue);
  }

  void PropertySaver::createPropertyNativeInteger(const std::string& key, std::function<int()> getter, std::function<void(int)> setter, int defaultValue, std::function<bool(int)> validityChecker, std::function<void()> removeFunction, const Options& hint) {
    if (hint.getOption(Options::PROPERTY_NO_EDIT) == "true") {
      return;
    }
    currentObject().addInteger(key, getter(), defaultValue);
  }

  void PropertySaver::createPropertyNativeString(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, const std::string& defaultValue, std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction, std::function<void(std::function<void()>, std::function<void()>)> confirmCustom) {
    currentObject().addString(key, getter(), defaultValue);
  }

  void PropertySaver::createPropertyNativeUnsignedInteger(const std::string& key, std::function<unsigned int()> getter, std::function<void(unsigned int)> setter, unsigned int defaultValue, std::function<bool(unsigned int)> validityChecker, std::function<void()> removeFunction) {
    currentObject().addInteger(key, static_cast<int>(getter()), static_cast<int>(defaultValue));
  }

  void PropertySaver::createPropertyOptional(const std::string& key, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback, std::function<std::string()> valueGetter, const Options& hint) {
    if (hint.getOption(Options::PROPERTY_NO_PERSIST) == "true") {
      return;
    }
    if (valueGetter) {
      currentObject().addString(key, valueGetter());
    }
  }

  void PropertySaver::createPropertyStruct(const std::string& key, const std::string& value, std::function<void(IPropertyMaker&)> subProperties, std::function<void()> removeFunction, const Options& hint) {
    if (hint.getOption(Options::PROPERTY_NO_EDIT) == "true") {
      subProperties(*this);
      return;
    }
    if (hint.getOption(Options::PROPERTY_SCOPED) == "true") {
      pushObject(currentObject().addObject(key));
      subProperties(*this);
      popObject();
    } else {
      subProperties(*this);
    }
  }

  void PropertySaver::createPropertyTreeSelector(const std::string& key, ITreeSelectorObject& item, const Options& hint, std::function<void()> removeFunction) {
    if (hint.getOption(Options::PROPERTY_NO_EDIT) == "true") {
      return;
    }
    if (hint.getOption(Options::PROPERTY_INLINE) == "true") {
      item.saveToProperty(currentObject(), hint);
      saveTreeSelectorAssetProperties(item, currentObject(), hint);
    } else {
      item.saveToProperty(currentObject(), key, hint);
      if (item.hasConfiguration()) {
        saveTreeSelectorAssetProperties(item, currentObject().getObject(key), hint);
      }
    }
  }

  void PropertySaver::confirm(const std::string& message, std::function<void()> confirm, std::function<void()> cancel) {
    // Nothing to do.
  }

  bool PropertySaver::isResourceReadOnly() const {
    return false;
  }

  void PropertySaver::promoteResourceToProject() {
    // Nothing to do.
  }
}
