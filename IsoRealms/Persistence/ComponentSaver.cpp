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

#include "IsoRealms/Condition/Condition.h"
#include "IsoRealms/Editing/Property/ITreeSelectorObject.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Project/Module.h"
#include "IsoRealms/Project/ComponentType.h"
#include "IsoRealms/Resources/Type/IEditable.h"

namespace IsoRealms {
  ComponentSaver::ComponentSaver(IComponentData& resourceData, JSONObject object) :
            cComponentData(resourceData) {
    cObjects.push_back(object);
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

  bool ComponentSaver::beginSaveKeyedArray(const std::string& key) {
    if (!key.empty()) {
      pushObject(currentObject().addObject(key));
      cSaveKeyedArrayKeys.push_back(key);
    }
    return true;
  }

  void ComponentSaver::beginSaveKeyedMember(const std::string& memberKey) {
    pushObject(currentObject().addObject(memberKey));
  }

  void ComponentSaver::endSaveKeyedMember() {
    popObject();
  }

  void ComponentSaver::endSaveKeyedArray() {
    if (!cSaveKeyedArrayKeys.empty()) {
      cSaveKeyedArrayKeys.pop_back();
      popObject();
    }
  }

  void ComponentSaver::saveTreeSelectorResourceProperties(const ITreeSelectorObject& item, JSONObject object, const Options& hint) const {
    if (item.hasConfiguration()) {
      ComponentSaver mSaver(cComponentData, object);
      const_cast<ITreeSelectorObject&>(item).defineTreeItem(mSaver);
    }
  }

  void ComponentSaver::propertyAdd(const std::string& key, const std::string& value, std::function<void()> addPropertyFunction, const Options& hint) {
    // Nothing to do.
  }

  void ComponentSaver::propertyBoolean(const std::string& key, std::function<bool()> getter, std::function<void(bool)> setter, bool defaultValue, std::function<void()> removeFunction, PropertyBooleanConfirmCallback confirmCustom) {
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

  void ComponentSaver::propertyCondition(const std::string& key, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter, const Options& hint) {
    std::optional<Condition>& mCondition = getter();
    if (mCondition.has_value()) {
      pushObject(currentObject().addObject(key));
      mCondition->define(*this, availableElements);
      popObject();
    }
  }

  void ComponentSaver::propertyEditor(const std::string& key, IEditable* editable) {
    // Nothing to do.
  }

  void ComponentSaver::propertyFloat(const std::string& key, std::function<float()> getter, std::function<void(float)> setter, float defaultValue, std::function<bool(float)> validityChecker, std::function<void()> removeFunction) {
    currentObject().addFloat(key, getter(), defaultValue);
  }

  void ComponentSaver::propertyInteger(const std::string& key, std::function<int()> getter, std::function<void(int)> setter, int defaultValue, std::function<bool(int)> validityChecker, std::function<void()> removeFunction, const Options& hint) {
    if (hint.getOption(IComponentDefiner::PROPERTY_HIDDEN) == "true") {
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
    if (hint.getOption(IComponentDefiner::PROPERTY_NO_PERSIST) == "true") {
      return;
    }
    if (valueGetter) {
      currentObject().addString(key, valueGetter());
    }
  }

  void ComponentSaver::propertyResource(const std::string& key, ITreeSelectorObject& item, const Options& hint, std::function<void()> removeFunction) {
    if (hint.getOption(IComponentDefiner::PROPERTY_INLINE) == "true") {
      saveTreeSelectorResourceProperties(item, currentObject(), hint);
    } else {
      if (item.hasConfiguration()) {
        saveTreeSelectorResourceProperties(item, currentObject().getObject(key), hint);
      }
    }
  }

  void ComponentSaver::propertyString(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, const std::string& defaultValue, std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction, PropertyConfirmCallback confirmCustom) {
    currentObject().addString(key, getter(), defaultValue);
  }

  void ComponentSaver::propertyUnsignedInteger(const std::string& key, std::function<unsigned int()> getter, std::function<void(unsigned int)> setter, unsigned int defaultValue, std::function<bool(unsigned int)> validityChecker, std::function<void()> removeFunction) {
    currentObject().addInteger(key, static_cast<int>(getter()), static_cast<int>(defaultValue));
  }

  void ComponentSaver::scopeModule(Module& module, std::function<void()> removeFunction) {
    std::vector<ComponentType*> mComponentTypes = module.getComponentTypes();
    for (ComponentType* mComponentType : mComponentTypes) {
      mComponentType->define(*this);
    }
  }

  void ComponentSaver::scope(const std::string& key, const std::string& value, std::function<void()> subProperties, std::function<void()> removeFunction, const Options& hint, std::function<bool()> icon) {
    if (hint.getOption(IComponentDefiner::PROPERTY_HIDDEN) == "true") {
      subProperties();
      return;
    }
    if (hint.getOption(IComponentDefiner::PROPERTY_SCOPED) == "true") {
      pushObject(currentObject().addObject(key));
      subProperties();
      popObject();
    } else {
      subProperties();
    }
  }

  void ComponentSaver::spacer(float height) {
    // Nothing to do.
  }

  void ComponentSaver::keyedArray(const std::string& key, const std::string& addKey, IKeyedArraySource& source, const Options& hint) {
    if (source.isEmpty()) {
      return;
    }
    if (!beginSaveKeyedArray(key)) {
      return;
    }
    source.forEachMember([this](const std::string& name, const std::function<void()>& define) {
      beginSaveKeyedMember(name);
      define();
      endSaveKeyedMember();
    });
    endSaveKeyedArray();
  }

  void ComponentSaver::array(const std::string& key, const std::string& addKey, IArraySource& source, const Options& hint) {
    if (source.isEmpty()) {
      return;
    }
    if (!beginSavePropertyArray(key)) {
      return;
    }
    source.forEachMember([this](const std::function<void()>& define) {
      beginSavePropertyArrayElement();
      define();
      endSavePropertyArrayElement();
    });
    endSavePropertyArray();
  }

  void ComponentSaver::fixedArray(const std::string& key, IFixedArraySource& source, const Options& hint) {
    if (!beginSavePropertyArray(key)) {
      return;
    }
    source.forEachMember([this](unsigned int, const std::function<void()>& define) {
      beginSavePropertyArrayElement();
      define();
      endSavePropertyArrayElement();
    });
    endSavePropertyArray();
  }

  void ComponentSaver::onPersisted(std::function<void()> callback) {
    callback();
  }
}
