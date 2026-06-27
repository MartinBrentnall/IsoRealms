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
#include "ComponentLoader.h"

#include <optional>

#include "IsoRealms/Condition/Condition.h"
#include "IsoRealms/Editing/Property/ITreeSelectorObject.h"
#include "IsoRealms/Exception/ArgumentException.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Persistence/JSONDocument.h"
#include "IsoRealms/Persistence/JSONArray.h"
#include "IsoRealms/Persistence/JSONThing.h"
#include "IsoRealms/Persistence/JSONValue.h"
#include "IsoRealms/Project/ComponentType.h"
#include "IsoRealms/Project/ComponentType.h"
#include "IsoRealms/Project/Module.h"
#include "IsoRealms/Resources/Fixed/DigitalInput/KeyboardKey.h"
#include "IsoRealms/Resources/Type/IEditable.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  namespace {
    template <typename VALIDITY_CHECKER>
    void requireValidPropertyValue(const std::string& key, const std::string& value, VALIDITY_CHECKER validityChecker) {
      if (validityChecker && !validityChecker(value)) {
        throw ArgumentException("ERROR: ComponentLoader: Invalid value \"" + value + "\" for property \"" + key + "\".");
      }
    }

    template <typename T, typename VALIDITY_CHECKER>
    void requireValidPropertyValue(const std::string& key, const T& value, VALIDITY_CHECKER validityChecker) {
      if (validityChecker && !validityChecker(value)) {
        throw ArgumentException("ERROR: ComponentLoader: Invalid value \"" + Utils::toString(value) + "\" for property \"" + key + "\".");
      }
    }

    void deferDuringLoad(IComponentData& resourceData, std::function<void()> task) {
      if (resourceData.getProject().isLoading() && !resourceData.getProject().areComponentsLoaded()) {
        resourceData.getProject().init(std::move(task));
      } else {
        task();
      }
    }
  }

  ComponentLoader::ComponentLoader(IComponentData& resourceData, JSONObject object) :
            cComponentData(resourceData) {
    cObjects.push_back(object);
  }

  ComponentLoader::ComponentLoader(IComponentData& resourceData, const std::string& file, bool user) :
            cComponentData(resourceData) {
    cDocuments.push_back(std::make_unique<JSONDocument>(file, user));
    cObjects.push_back(cDocuments.back()->getObject("project"));
  }

  JSONObject& ComponentLoader::currentObject() {
    return cObjects.back();
  }

  const JSONObject& ComponentLoader::currentObject() const {
    return cObjects.back();
  }

  void ComponentLoader::pushObject(JSONObject object) {
    cObjects.push_back(object);
  }

  void ComponentLoader::popObject() {
    cObjects.pop_back();
  }

  void ComponentLoader::loadTreeSelectorResourceProperties(ITreeSelectorObject& item, JSONObject object, const Options& hint) {
    if (hint.getOption(IComponentDefiner::HINT_KEY_IMMEDIATE) == "true") {
      pushObject(object);
      item.defineTreeItem(*this);
      popObject();
      return;
    }

    std::vector<JSONObject> mObjectStack = cObjects;
    mObjectStack.push_back(object);
    ITreeSelectorObject* mItem = &item;
    deferDuringLoad(cComponentData, [this, mItem, object]() {
      pushObject(object);
      mItem->defineTreeItem(*this);
      popObject();
    });
  }

  bool ComponentLoader::loadPropertyArray(const std::string& key, const std::function<void()>& addAndLoadElement, const Options& hint) {
    if (!currentObject().hasMember(key)) {
      return true;
    }
    for (JSONValue mValue : currentObject().getArray(key)) {
      JSONObject mObject = mValue.getObject();
      pushObject(mObject);
      addAndLoadElement();
      popObject();
    }
    return true;
  }

  void ComponentLoader::loadKeyedArray(const std::string& key, const std::function<void(const std::string& memberKey, bool isNull)>& loadMember, const Options& hint) {
    if (key.empty()) {
      loadKeyedMembers(loadMember);
      return;
    }
    if (!currentObject().hasMember(key)) {
      return;
    }
    pushObject(currentObject().getObject(key));
    loadKeyedMembers(loadMember);
    popObject();
  }

  bool ComponentLoader::loadKeyedMembers(const std::function<void(const std::string& key, bool isNull)>& loadMember) {
    for (JSONThing mMember : currentObject()) {
      if (mMember.isNull()) {
        loadMember(mMember.getName(), true);
      } else {
        pushObject(mMember.getValue());
        loadMember(mMember.getName(), false);
        popObject();
      }
    }
    return true;
  }

  bool ComponentLoader::loadFixedPropertyArray(const std::string& key, unsigned int count, const std::function<void(unsigned int index)>& loadElement) {
    if (!currentObject().hasMember(key) || !currentObject().isArray(key)) {
      return false;
    }
    unsigned int mIndex = 0;
    for (JSONValue mValue : currentObject().getArray(key)) {
      if (mIndex >= count) {
        break;
      }
      pushObject(mValue.getObject());
      loadElement(mIndex);
      popObject();
      mIndex++;
    }
    return true;
  }

  void ComponentLoader::propertyAdd(const std::string& key, const std::string& value, std::function<void()> addPropertyFunction, const Options& hint) {
    // Nothing to do.
  }

  void ComponentLoader::propertyBoolean(const std::string& key, std::function<bool()> getter, std::function<void(bool)> setter, bool defaultValue, std::function<void()> removeFunction, PropertyBooleanConfirmCallback confirmCustom) {
    setter(currentObject().getBoolean(key, defaultValue));
  }

  void ComponentLoader::propertyCode(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<void()> removeFunction) {
    setter(currentObject().getString(key));
  }

  void ComponentLoader::propertyColourChannel(const std::string& key, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) {
    confirmationCallback(currentObject().getFloat(key));
  }

  void ComponentLoader::propertyColourHue(const std::string& key, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    // This is a calculated property, so we don't need to load it from the JSON.
  }

  void ComponentLoader::propertyColourLightness(const std::string& key, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) {
    // This is a calculated property, so we don't need to load it from the JSON.
  }

  void ComponentLoader::propertyColourSaturation(const std::string& key, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) {
    // This is a calculated property, so we don't need to load it from the JSON.
  }

  void ComponentLoader::propertyCondition(const std::string& key, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter, const Options& hint) {
    if (!currentObject().hasMember(key)) {
      return;
    }
    pushObject(currentObject().getObject(key));
    std::optional<Condition>& mCondition = getter();
    mCondition.emplace(true);
    mCondition->define(*this, availableElements);
    setter(mCondition);
    popObject();
  }

  void ComponentLoader::propertyEditor(const std::string& key, IEditable* editable) {
    // Nothing to do.
  }

  void ComponentLoader::propertyFloat(const std::string& key, std::function<float()> getter, std::function<void(float)> setter, float defaultValue, std::function<bool(float)> validityChecker, std::function<void()> removeFunction) {
    float mValue = currentObject().getFloat(key, defaultValue);
    requireValidPropertyValue(key, mValue, validityChecker);
    setter(mValue);
  }

  void ComponentLoader::propertyInteger(const std::string& key, std::function<int()> getter, std::function<void(int)> setter, int defaultValue, std::function<bool(int)> validityChecker, std::function<void()> removeFunction, const Options& hint) {
    int mValue = currentObject().getInteger(key, defaultValue);
    requireValidPropertyValue(key, mValue, validityChecker);
    setter(mValue);
  }

  void ComponentLoader::propertyKey(const std::string& key, std::function<std::string()> getter, std::function<void(sf::Keyboard::Key)> setter, std::function<void()> removeFunction) {
    setter(KeyboardKey::getKey(currentObject().getString(key)));
  }

  void ComponentLoader::propertyList(const std::string& key, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, const std::string& defaultValue, std::function<void()> removeFunction) {
    std::string mValue = currentObject().getString(key, defaultValue);
    for (const std::string& mOption : options) {
      if (mOption == mValue) {
        setter(mValue);
        return;
      }
    }
    throw ArgumentException("ERROR: ComponentLoader::propertyList: Value \"" + mValue + "\" is not a valid option for property \"" + key + "\".");
  }

  void ComponentLoader::propertyOptional(const std::string& key, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback, std::function<std::string()> valueGetter, const Options& hint) {
    if (hint.getOption(IComponentDefiner::HINT_KEY_TRANSIENT) == "true") {
      return;
    }
    choiceCallback(currentObject().getString(key));
  }

  void ComponentLoader::propertyResource(const std::string& key, ITreeSelectorObject& item, const Options& hint, std::function<void()> removeFunction) {
    if (hint.getOption(IComponentDefiner::HINT_KEY_TRANSIENT) == "true") {
      return;
    }
    if (hint.getOption(IComponentDefiner::HINT_KEY_INLINE) == "true") {
      loadTreeSelectorResourceProperties(item, currentObject(), hint);
    } else {
      if (hint.getOption(IComponentDefiner::HINT_KEY_OPTIONAL) != "true" || currentObject().hasMember(key)) {
        loadTreeSelectorResourceProperties(item, currentObject().getObject(key), hint);
      }
    }
  }

  void ComponentLoader::propertyString(const std::string& key, std::function<std::string()> getter, std::function<void(const std::string&)> setter, const std::string& defaultValue, std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction, PropertyConfirmCallback confirmCustom, const Options& hint) {
    if (hint.getOption(IComponentDefiner::HINT_KEY_TRANSIENT) == "true") {
      return;
    }
    std::string mValue = currentObject().getString(key, defaultValue);
    requireValidPropertyValue(key, mValue, validityChecker);
    setter(mValue);
  }

  void ComponentLoader::propertyUnsignedInteger(const std::string& key, std::function<unsigned int()> getter, std::function<void(unsigned int)> setter, unsigned int defaultValue, std::function<bool(unsigned int)> validityChecker, std::function<void()> removeFunction) {
    unsigned int mValue = static_cast<unsigned int>(currentObject().getInteger(key, static_cast<int>(defaultValue)));
    requireValidPropertyValue(key, mValue, validityChecker);
    setter(mValue);
  }

  void ComponentLoader::scopeModule(Module& module, std::function<void()> removeFunction) {
    std::vector<ComponentType*> mComponentTypes = module.getComponentTypes();
    for (ComponentType* mComponentType : mComponentTypes) {
      const std::string mTypeName = module.getName(mComponentType);
      if (currentObject().hasMember(mTypeName)) {
        pushObject(currentObject().getObject(mTypeName));
        mComponentType->define(*this);
        popObject();
      }
    }
  }

  void ComponentLoader::scope(const std::string& key, const std::string& value, std::function<void()> subProperties, std::function<void()> removeFunction, const Options& hint, std::function<bool()> icon) {
    if (hint.getOption(IComponentDefiner::HINT_KEY_TRANSIENT) == "true") {
      return;
    }
    if (hint.getOption(IComponentDefiner::HINT_KEY_DEFER) == "true") {
      JSONObject mObject = currentObject();
      deferDuringLoad(cComponentData, [this, mObject, mSubProperties = std::move(subProperties)]() {
        pushObject(mObject);
        mSubProperties();
        popObject();
      });
      return;
    }
    const std::string mFilePath = hint.getOption(IComponentDefiner::HINT_KEY_DOCUMENT);
    if (!mFilePath.empty()) {
      const bool mUser = hint.getOption(IComponentDefiner::HINT_KEY_USER_DOCUMENT) == "true";
      cDocuments.push_back(std::make_unique<JSONDocument>(mFilePath, mUser));
      pushObject(cDocuments.back()->getObject("project"));
      subProperties();
      popObject();
      return;
    }
    if (hint.getOption(IComponentDefiner::HINT_KEY_NESTED) == "true") {
      pushObject(currentObject().getObject(key));
      subProperties();
      popObject();
    } else {
      subProperties();
    }
  }

  void ComponentLoader::spacer(float height) {
    // Nothing to do.
  }

  void ComponentLoader::keyedArray(const std::string& key, const std::string& addKey, IKeyedArraySource& source, const Options& hint) {
    loadKeyedArray(key, [&source](const std::string& memberKey, bool isNull) {
      if (isNull) {
        assert(false);
      }
      source.defineNewMember(memberKey);
    }, hint);
  }

  void ComponentLoader::array(const std::string& key, const std::string& addKey, IArraySource& source, const Options& hint) {
    loadPropertyArray(key, [&source]() {
      source.defineNewMember();
    }, hint);
  }

  void ComponentLoader::fixedArray(const std::string& key, IFixedArraySource& source, const Options& hint) {
    loadFixedPropertyArray(key, source.getCount(), [&source](unsigned int index) {
      source.defineAtIndex(index);
    });
  }

  void ComponentLoader::onInitialised(std::function<void()> callback, const Options& hint) {
    if (hint.getOption(IComponentDefiner::HINT_KEY_IMMEDIATE) == "true") {
      callback();
    } else {
      deferDuringLoad(cComponentData, std::move(callback));
    }
  }
}
