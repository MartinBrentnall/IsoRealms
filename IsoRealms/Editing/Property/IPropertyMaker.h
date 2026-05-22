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
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <SFML/Window/Event.hpp>

#include "IsoRealms/Editing/IResourceAccessManager.h"
#include "IsoRealms/PropertyData.h"

namespace IsoRealms {
  class Condition;
  class ConditionElement;
  class IEditable;
  class IOptionalObject;
  class IResourceData;
  class ITreeSelectorObject;

  class IPropertyMaker : public IResourceAccessManager {
    public:
    virtual ~IPropertyMaker() = default;

    virtual IResourceData& getResourceData() = 0;

    virtual void createPropertyAdd(                  const PropertyData& metadata, const std::string& value, std::function<void()> addPropertyFunction) = 0;
    virtual void createPropertyCode(                 const PropertyData& metadata, std::function<std::string()>  getter, std::function<void(const std::string&)> setter,             std::function<void()> removeFunction = nullptr) = 0;
    virtual void createPropertyColourChannel(        const PropertyData& metadata, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) = 0;
    virtual void createPropertyColourHue(            const PropertyData& metadata, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) = 0;
    virtual void createPropertyColourLightness(      const PropertyData& metadata, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) = 0;
    virtual void createPropertyColourSaturation(     const PropertyData& metadata, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) = 0;
    virtual void createPropertyCondition(            const PropertyData& metadata, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter) = 0;
    virtual void createPropertyEditor(               const PropertyData& metadata, IEditable* editable) = 0;
    virtual void createPropertyKey(                  const PropertyData& metadata, std::function<std::string()>  getter, std::function<void(sf::Keyboard::Key)>  setter,             std::function<void()> removeFunction = nullptr) = 0;
    virtual void createPropertyList(                 const PropertyData& metadata, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, std::function<void()> removeFunction = nullptr) = 0;
    virtual void createPropertyNativeBoolean(        const PropertyData& metadata, std::function<bool()>         getter, std::function<void(bool)>               setter,                                                                                                              std::function<void()> removeFunction = nullptr) = 0;
    virtual void createPropertyNativeFloat(          const PropertyData& metadata, std::function<float()>        getter, std::function<void(float)>              setter,             std::function<bool(float)>              validityChecker = [](float)              {return true;}, std::function<void()> removeFunction = nullptr) = 0;
    virtual void createPropertyNativeInteger(        const PropertyData& metadata, std::function<int()>          getter, std::function<void(int)>                setter,             std::function<bool(int)>                validityChecker = [](int)                {return true;}, std::function<void()> removeFunction = nullptr) = 0;
    virtual void createPropertyNativeString(         const PropertyData& metadata, std::function<std::string()>  getter, std::function<void(const std::string&)> setter,             std::function<bool(const std::string&)> validityChecker = [](const std::string&) {return true;}, std::function<void()> removeFunction = nullptr, std::function<void(std::function<void()>, std::function<void()>)> confirmCustom = nullptr) = 0;
    virtual void createPropertyNativeUnsignedInteger(const PropertyData& metadata, std::function<unsigned int()> getter, std::function<void(unsigned int)>       setter,             std::function<bool(unsigned int)>       validityChecker = [](unsigned int)       {return true;}, std::function<void()> removeFunction = nullptr) = 0;
    virtual void createPropertyOptional(             const PropertyData& metadata, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback) = 0;
    virtual void createPropertyStruct(               const PropertyData& metadata, const std::string& value, std::function<void(IPropertyMaker&)> subProperties, std::function<void()> removeFunction = nullptr) = 0;
    virtual void createPropertyTreeSelector(         const PropertyData& metadata, ITreeSelectorObject& item, std::function<void()> removeFunction = nullptr) = 0;

    template <typename CONTAINER, typename VALUE_FUNC, typename PROPERTY_FUNC, typename ADD_FUNC>
    void createPropertyArray(const PropertyData& metadata, const CONTAINER& container, VALUE_FUNC value, PROPERTY_FUNC createProperty, ADD_FUNC add) {
      for (const auto& mElement : container) {
        createProperty(value(mElement));
      }
      createPropertyAdd(metadata, "Add...", [createProperty, add]() {
        createProperty(add());
      });
    }
  };
}
