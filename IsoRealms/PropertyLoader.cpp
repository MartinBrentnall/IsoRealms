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
#include "PropertyLoader.h"

#include <map>
#include <optional>

#include "Assets/Fixed/DigitalInput/KeyboardKey.h"
#include "Assets/Type/IEditable.h"
#include "Condition/Condition.h"
#include "Editing/Property/ITreeSelectorObject.h"
#include "Exception/ArgumentException.h"
#include "IResourceData.h"
#include "Project/Project.h"
#include "Utils.h"

namespace IsoRealms {
  namespace {
    template <typename VALIDITY_CHECKER>
    void requireValidPropertyValue(const std::string& key, const std::string& value, VALIDITY_CHECKER validityChecker) {
      if (!validityChecker(value)) {
        throw ArgumentException("ERROR: PropertyLoader: Invalid value \"" + value + "\" for property \"" + key + "\".");
      }
    }

    template <typename T, typename VALIDITY_CHECKER>
    void requireValidPropertyValue(const std::string& key, const T& value, VALIDITY_CHECKER validityChecker) {
      if (!validityChecker(value)) {
        throw ArgumentException("ERROR: PropertyLoader: Invalid value \"" + Utils::toString(value) + "\" for property \"" + key + "\".");
      }
    }

    void deferDuringLoad(IResourceData& resourceData, std::function<void()> task) {
      if (resourceData.getProject().isLoading() && !resourceData.getProject().areResourcesLoaded()) {
        resourceData.getProject().init(std::move(task));
      } else {
        task();
      }
    }
  }

  PropertyLoader::PropertyLoader(IResourceData& resourceData, JSONObject object) :
            cResourceData(resourceData) {
    cObjects.push_back(object);
  }

  PropertyLoader::PropertyLoader(IResourceData& resourceData, std::vector<JSONObject> objects) :
            cResourceData(resourceData),
            cObjects(std::move(objects)) {
  }

  IResourceData& PropertyLoader::getResourceData() {
    return cResourceData;
  }

  JSONObject& PropertyLoader::currentObject() {
    return cObjects.back();
  }

  const JSONObject& PropertyLoader::currentObject() const {
    return cObjects.back();
  }

  bool PropertyLoader::hasPersistedMember(const std::string& key) const {
    return currentObject().hasMember(key);
  }

  void PropertyLoader::pushObject(JSONObject object) {
    cObjects.push_back(object);
  }

  void PropertyLoader::popObject() {
    cObjects.pop_back();
  }

  void PropertyLoader::loadTreeSelectorAssetProperties(ITreeSelectorObject& item, JSONObject object, const Options& hint) {
    if (hint.getOption(Options::PROPERTY_IMMEDIATE) == "true") {
      pushObject(object);
      item.getAssetProperties(*this);
      popObject();
      return;
    }

    std::vector<JSONObject> mObjectStack = cObjects;
    mObjectStack.push_back(object);
    ITreeSelectorObject* mItem = &item;
    IResourceData* mResourceData = &cResourceData;
    std::function<void()> mLoad = [mResourceData, mObjectStack = std::move(mObjectStack), mItem]() {
      PropertyLoader mLoader(*mResourceData, mObjectStack);
      mItem->getAssetProperties(mLoader);
    };
    deferDuringLoad(cResourceData, std::move(mLoad));
  }

  bool PropertyLoader::loadPropertyArray(const std::string& key, const std::function<void()>& addAndLoadElement) {
    for (JSONValue mValue : currentObject().getArray(key)) {
      JSONObject mObject = mValue.getObject();
      pushObject(mObject);
      addAndLoadElement();
      popObject();
    }
    return true;
  }

  bool PropertyLoader::loadFixedPropertyArray(const std::string& key, unsigned int count, const std::function<unsigned int(const JSONObject&)>& matchIndex, const std::function<void(unsigned int index)>& loadElement) {
    std::map<unsigned int, JSONObject> mSlots;
    for (JSONValue mValue : currentObject().getArray(key)) {
      JSONObject mObject = mValue.getObject();
      mSlots.emplace(matchIndex(mObject), mObject);
    }
    for (unsigned int mIndex = 0; mIndex < count; mIndex++) {
      pushObject(mSlots.at(mIndex));
      loadElement(mIndex);
      popObject();
    }
    return true;
  }

  void PropertyLoader::createPropertyAdd(const std::string& key, const std::string& value, std::function<void()> addPropertyFunction) {
    // Nothing to do.
  }

  void PropertyLoader::createPropertyCode(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<void()> removeFunction) {
    setter(currentObject().getString(key));
  }

  void PropertyLoader::createPropertyColourChannel(const std::string& key, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) {
    confirmationCallback(currentObject().getFloat(key));
  }

  void PropertyLoader::createPropertyColourHue(const std::string& key, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    // This is a calculated property, so we don't need to load it from the JSON.
  }

  void PropertyLoader::createPropertyColourLightness(const std::string& key, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) {
    // This is a calculated property, so we don't need to load it from the JSON.
  }

  void PropertyLoader::createPropertyColourSaturation(const std::string& key, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    // This is a calculated property, so we don't need to load it from the JSON.
  }

  void PropertyLoader::createPropertyCondition(const std::string& key, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter) {
    std::optional<Condition>& mCondition = getter();
    mCondition = Condition(currentObject().getObject(key), availableElements);
    setter(mCondition);
  }

  void PropertyLoader::createPropertyEditor(const std::string& key, IEditable* editable) {
    editable->load(cResourceData, currentObject());
  }

  void PropertyLoader::createPropertyKey(const std::string& key, std::function<std::string()> getter, std::function<void(sf::Keyboard::Key)> setter, std::function<void()> removeFunction) {
    setter(KeyboardKey::getKey(currentObject().getString(key)));
  }

  void PropertyLoader::createPropertyList(const std::string& key, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, const std::string& defaultValue, std::function<void()> removeFunction) {
    std::string mValue = currentObject().getString(key, defaultValue);
    for (const std::string& mOption : options) {
      if (mOption == mValue) {
        setter(mValue);
        return;
      }
    }
    throw ArgumentException("ERROR: PropertyLoader::createPropertyList: Value \"" + mValue + "\" is not a valid option for property \"" + key + "\".");
  }

  void PropertyLoader::createPropertyNativeBoolean(const std::string& key, std::function<bool()> getter, std::function<void(bool)> setter, bool defaultValue, std::function<void()> removeFunction) {
    setter(currentObject().getBoolean(key, defaultValue));
  }

  void PropertyLoader::createPropertyNativeFloat(const std::string& key, std::function<float()> getter, std::function<void(float)> setter, float defaultValue, std::function<bool(float)> validityChecker, std::function<void()> removeFunction) {
    float mValue = currentObject().getFloat(key, defaultValue);
    requireValidPropertyValue(key, mValue, validityChecker);
    setter(mValue);
  }

  void PropertyLoader::createPropertyNativeInteger(const std::string& key, std::function<int()> getter, std::function<void(int)> setter, int defaultValue, std::function<bool(int)> validityChecker, std::function<void()> removeFunction, const Options& hint) {
    int mValue = currentObject().getInteger(key, defaultValue);
    requireValidPropertyValue(key, mValue, validityChecker);
    setter(mValue);
  }

  void PropertyLoader::createPropertyNativeString(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, const std::string& defaultValue, std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction, std::function<void(std::function<void()>, std::function<void()>)> confirmCustom) {
    std::string mValue = currentObject().getString(key, defaultValue);
    requireValidPropertyValue(key, mValue, validityChecker);
    setter(mValue);
  }

  void PropertyLoader::createPropertyNativeUnsignedInteger(const std::string& key, std::function<unsigned int()> getter, std::function<void(unsigned int)> setter, unsigned int defaultValue, std::function<bool(unsigned int)> validityChecker, std::function<void()> removeFunction) {
    unsigned int mValue = static_cast<unsigned int>(currentObject().getInteger(key, static_cast<int>(defaultValue)));
    requireValidPropertyValue(key, mValue, validityChecker);
    setter(mValue);
  }

  void PropertyLoader::createPropertyOptional(const std::string& key, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback, std::function<std::string()> valueGetter, const Options& hint) {
    if (hint.getOption(Options::PROPERTY_NO_PERSIST) == "true") {
      return;
    }
    choiceCallback(currentObject().getString(key));
  }

  void PropertyLoader::createPropertyStruct(const std::string& key, const std::string& value, std::function<void(IPropertyMaker&)> subProperties, std::function<void()> removeFunction, const Options& hint) {
    std::string mScoped = hint.getOption(Options::PROPERTY_SCOPED);
    if (mScoped == "true") {
      pushObject(currentObject().getObject(key));
      subProperties(*this);
      popObject();
    } else {
      subProperties(*this);
    }
  }

  void PropertyLoader::createPropertyTreeSelector(const std::string& key, ITreeSelectorObject& item, const Options& hint, std::function<void()> removeFunction) {
    if (hint.getOption(Options::PROPERTY_INLINE) == "true") {
      item.loadFromProperty(currentObject(), hint);
      loadTreeSelectorAssetProperties(item, currentObject(), hint);
    } else {
      item.loadFromProperty(currentObject(), key, hint);
      loadTreeSelectorAssetProperties(item, currentObject().getObject(key), hint);
    }
  }

  void PropertyLoader::confirm(const std::string& message, std::function<void()> confirm, std::function<void()> cancel) {
    confirm();
  }

  bool PropertyLoader::isResourceReadOnly() const {
    return false;
  }

  void PropertyLoader::promoteResourceToProject() {
    // Nothing to do.
  }
}
