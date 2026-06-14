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
#include "ComponentSaver.h"

#include "Resources/Type/IEditable.h"
#include "Condition/Condition.h"
#include "Editing/Property/ITreeSelectorObject.h"
#include "IComponentData.h"

namespace IsoRealms {
  ComponentSaver::ComponentSaver(IComponentData& resourceData, JSONObject object) :
            cComponentData(resourceData) {
    cObjects.push_back(object);
  }

  IComponentData& ComponentSaver::getComponentData() {
    return cComponentData;
  }

  JSONObject& ComponentSaver::currentObject() {
    return cObjects.back();
  }

  const JSONObject& ComponentSaver::currentObject() const {
    return cObjects.back();
  }

  void ComponentSaver::pushObject(JSONObject object) {
    cObjects.push_back(object);
  }

  void ComponentSaver::popObject() {
    cObjects.pop_back();
  }

  bool ComponentSaver::beginSavePropertyArray(const std::string& key) {
    currentObject().addArray(key);
    cSaveArrayKeys.push_back(key);
    return true;
  }

  void ComponentSaver::beginSavePropertyArrayElement() {
    pushObject(currentObject().getArray(cSaveArrayKeys.back()).addObject());
  }

  void ComponentSaver::endSavePropertyArrayElement() {
    popObject();
  }

  void ComponentSaver::endSavePropertyArray() {
    cSaveArrayKeys.pop_back();
  }

  void ComponentSaver::saveTreeSelectorResourceProperties(const ITreeSelectorObject& item, JSONObject object, const Options& hint) const {
    if (item.hasConfiguration()) {
      ComponentSaver mSaver(cComponentData, object);
      const_cast<ITreeSelectorObject&>(item).getTreeItemProperties(mSaver);
    }
  }

  void ComponentSaver::propertyAdd(const std::string& key, const std::string& value, std::function<void()> addPropertyFunction) {
    // Nothing to do.
  }

  void ComponentSaver::propertyBoolean(const std::string& key, std::function<bool()> getter, std::function<void(bool)> setter, bool defaultValue, std::function<void()> removeFunction) {
    currentObject().addBoolean(key, getter(), defaultValue);
  }

  void ComponentSaver::propertyCode(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<void()> removeFunction) {
    currentObject().addString(key, getter());
  }

  void ComponentSaver::propertyColourChannel(const std::string& key, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) {
    currentObject().addFloat(key, valueFunction());
  }

  void ComponentSaver::propertyColourHue(const std::string& key, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    // This is a calculated property, so we don't need to save it to JSON.
  }

  void ComponentSaver::propertyColourLightness(const std::string& key, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) {
    // This is a calculated property, so we don't need to save it to JSON.
  }

  void ComponentSaver::propertyColourSaturation(const std::string& key, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    // This is a calculated property, so we don't need to save it to JSON.
  }

  void ComponentSaver::propertyCondition(const std::string& key, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter) {
    std::optional<Condition>& mCondition = getter();
    if (mCondition.has_value()) {
      mCondition->save(currentObject().addObject(key));
    }
  }

  void ComponentSaver::propertyEditor(const std::string& key, IEditable* editable) {
    editable->save(cComponentData, currentObject());
  }

  void ComponentSaver::propertyFloat(const std::string& key, std::function<float()> getter, std::function<void(float)> setter, float defaultValue, std::function<bool(float)> validityChecker, std::function<void()> removeFunction) {
    currentObject().addFloat(key, getter(), defaultValue);
  }

  void ComponentSaver::propertyInteger(const std::string& key, std::function<int()> getter, std::function<void(int)> setter, int defaultValue, std::function<bool(int)> validityChecker, std::function<void()> removeFunction, const Options& hint) {
    if (hint.getOption(Options::PROPERTY_NO_EDIT) == "true") {
      return;
    }
    currentObject().addInteger(key, getter(), defaultValue);
  }

  void ComponentSaver::propertyKey(const std::string& key, std::function<std::string()> getter, std::function<void(sf::Keyboard::Key)> setter, std::function<void()> removeFunction) {
    currentObject().addString(key, getter());
  }

  void ComponentSaver::propertyList(const std::string& key, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, const std::string& defaultValue, std::function<void()> removeFunction) {
    currentObject().addString(key, getter(), defaultValue);
  }

  void ComponentSaver::propertyOptional(const std::string& key, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback, std::function<std::string()> valueGetter, const Options& hint) {
    if (hint.getOption(Options::PROPERTY_NO_PERSIST) == "true") {
      return;
    }
    if (valueGetter) {
      currentObject().addString(key, valueGetter());
    }
  }

  void ComponentSaver::propertyResource(const std::string& key, ITreeSelectorObject& item, const Options& hint, std::function<void()> removeFunction) {
    if (hint.getOption(Options::PROPERTY_INLINE) == "true") {
      item.saveToProperty(currentObject(), hint);
      saveTreeSelectorResourceProperties(item, currentObject(), hint);
    } else {
      item.saveToProperty(currentObject(), key, hint);
      if (item.hasConfiguration()) {
        saveTreeSelectorResourceProperties(item, currentObject().getObject(key), hint);
      }
    }
  }

  void ComponentSaver::propertyString(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, const std::string& defaultValue, std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction, std::function<void(std::function<void()>, std::function<void()>)> confirmCustom) {
    currentObject().addString(key, getter(), defaultValue);
  }

  void ComponentSaver::propertyUnsignedInteger(const std::string& key, std::function<unsigned int()> getter, std::function<void(unsigned int)> setter, unsigned int defaultValue, std::function<bool(unsigned int)> validityChecker, std::function<void()> removeFunction) {
    currentObject().addInteger(key, static_cast<int>(getter()), static_cast<int>(defaultValue));
  }

  void ComponentSaver::scope(const std::string& key, const std::string& value, std::function<void(IComponentDefiner&)> subProperties, std::function<void()> removeFunction, const Options& hint) {
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

  void ComponentSaver::confirm(const std::string& message, std::function<void()> confirm, std::function<void()> cancel) {
    // Nothing to do.
  }

  bool ComponentSaver::isComponentReadOnly() const {
    return false;
  }

  void ComponentSaver::promoteComponentToProject() {
    // Nothing to do.
  }
}
