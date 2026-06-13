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
#pragma once

#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <SFML/Window/Event.hpp>

#include "IsoRealms/Editing/IResourceAccessManager.h"
#include "IsoRealms/Project/Options.h"

namespace IsoRealms {
  class Condition;
  class ConditionElement;
  class IEditable;
  class IOptionalObject;
  class IResourceData;
  class ITreeSelectorObject;
  class JSONObject;

  class IPropertyMaker : public IResourceAccessManager {
    public:
    virtual ~IPropertyMaker() = default;

    virtual IResourceData& getResourceData() = 0;

    virtual bool loadsPersistedValues() const {
      return false;
    }

    // TODO: Replace this function with a hint.
    virtual bool hasPersistedMember(const std::string& key) const {
      return false;
    }

    virtual void createPropertyAdd(                  const std::string& key, const std::string& value, std::function<void()> addPropertyFunction) = 0;
    virtual void createPropertyCode(                 const std::string& key, std::function<std::string()>  getter, std::function<void(const std::string&)> setter,             std::function<void()> removeFunction = nullptr) = 0;
    virtual void createPropertyColourChannel(        const std::string& key, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) = 0;
    virtual void createPropertyColourHue(            const std::string& key, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) = 0;
    virtual void createPropertyColourLightness(      const std::string& key, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) = 0;
    virtual void createPropertyColourSaturation(     const std::string& key, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) = 0;
    virtual void createPropertyCondition(            const std::string& key, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter) = 0;
    virtual void createPropertyEditor(               const std::string& key, IEditable* editable) = 0;
    virtual void createPropertyKey(                  const std::string& key, std::function<std::string()>  getter, std::function<void(sf::Keyboard::Key)>  setter,             std::function<void()> removeFunction = nullptr) = 0;
    virtual void createPropertyList(                 const std::string& key, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, const std::string& defaultValue = "", std::function<void()> removeFunction = nullptr) = 0;
    virtual void createPropertyNativeBoolean(        const std::string& key, std::function<bool()>         getter, std::function<void(bool)>               setter, bool               defaultValue = false,                                                                                                  std::function<void()> removeFunction = nullptr) = 0;
    virtual void createPropertyNativeFloat(          const std::string& key, std::function<float()>        getter, std::function<void(float)>              setter, float              defaultValue = 0.0f,  std::function<bool(float)>              validityChecker = [](float)              {return true;}, std::function<void()> removeFunction = nullptr) = 0;
    virtual void createPropertyNativeInteger(        const std::string& key, std::function<int()>          getter, std::function<void(int)>                setter, int                defaultValue = 0,     std::function<bool(int)>                validityChecker = [](int)                {return true;}, std::function<void()> removeFunction = nullptr, const Options& hint = Options::EMPTY) = 0;
    virtual void createPropertyNativeString(         const std::string& key, std::function<std::string()>  getter, std::function<void(const std::string&)> setter, const std::string& defaultValue = "",    std::function<bool(const std::string&)> validityChecker = [](const std::string&) {return true;}, std::function<void()> removeFunction = nullptr, std::function<void(std::function<void()>, std::function<void()>)> confirmCustom = nullptr) = 0;
    virtual void createPropertyNativeUnsignedInteger(const std::string& key, std::function<unsigned int()> getter, std::function<void(unsigned int)>       setter, unsigned int       defaultValue = 0,     std::function<bool(unsigned int)>       validityChecker = [](unsigned int)       {return true;}, std::function<void()> removeFunction = nullptr) = 0;
    virtual void createPropertyOptional(             const std::string& key, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback, std::function<std::string()> valueGetter = nullptr, const Options& hint = Options::EMPTY) = 0;
    virtual void createPropertyStruct(               const std::string& key, const std::string& value, std::function<void(IPropertyMaker&)> subProperties, std::function<void()> removeFunction = nullptr, const Options& hint = Options::EMPTY) = 0;
    virtual void createPropertyTreeSelector(         const std::string& key, ITreeSelectorObject& item, const Options& hint = Options::EMPTY, std::function<void()> removeFunction = nullptr) = 0;

    template <typename CONTAINER, typename VALUE_FUNC, typename PROPERTY_FUNC, typename ADD_FUNC>
    void createPropertyArray(const std::string& key, const CONTAINER& container, VALUE_FUNC value, PROPERTY_FUNC createProperty, ADD_FUNC add) {
      if (loadPropertyArray(key, [&]() {
        createProperty(add());
      })) {
        return;
      }
      for (const auto& mElement : container) {
        createProperty(value(mElement));
      }
      createPropertyAdd(key, "Add...", [createProperty, add]() {
        createProperty(add());
      });
    }

    template <typename CONTAINER, typename VALUE_FUNC, typename PROPERTY_FUNC, typename MATCH_INDEX_FUNC>
    void createPropertyFixedArray(const std::string& key, const CONTAINER& container, VALUE_FUNC value, PROPERTY_FUNC createProperty, MATCH_INDEX_FUNC matchIndex) {
      const unsigned int mCount = static_cast<unsigned int>(std::distance(std::begin(container), std::end(container)));
      std::function<void(unsigned int)> mCreateAtIndex = [&](unsigned int mIndex) {
        auto mElement = std::begin(container);
        std::advance(mElement, mIndex);
        createProperty(value(*mElement), mIndex);
      };
      if (loadFixedPropertyArray(key, mCount, matchIndex, mCreateAtIndex)) {
        return;
      }
      unsigned int mIndex = 0;
      for (typename CONTAINER::const_reference mElement : container) {
        createProperty(value(mElement), mIndex++);
      }
    }

    protected:
    virtual bool loadPropertyArray(const std::string& key, const std::function<void()>& addAndLoadElement) {
      return false;
    }

    virtual bool loadFixedPropertyArray(const std::string& key, unsigned int count, const std::function<unsigned int(const JSONObject&)>& matchIndex, const std::function<void(unsigned int index)>& loadElement) {
      return false;
    }
  };
}
