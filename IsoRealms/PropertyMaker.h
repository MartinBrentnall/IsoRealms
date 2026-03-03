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

#include <memory>
#include <optional>
#include <string>

#include <SFML/Window/Event.hpp>

#include "Editing/IResourceAccessManager.h"
#include "Editing/Property/PropertyTreeSelector.h"
#include "Editing/Property/PropertyOptional.h"

namespace IsoRealms {
  class Condition;
  class ConditionElement;
  class IDialogManager;
  class IEditable;
  class IProperty;
  class IPropertyManager;
  class IResourceData;

  class PropertyMaker : public IResourceAccessManager {
    public:
    PropertyMaker(Application& application, IResourceData& parent, IPropertyManager& properties, IDialogManager& dialogManager);

    IResourceData& getResourceData() {
      return cParent;
    }

    void createPropertyAdd(                  const PropertyData& metadata, const std::string& value, std::function<void()> addPropertyFunction);
    void createPropertyCode(                 const PropertyData& metadata, std::function<std::string()>  getter, std::function<void(const std::string&)> setter,             std::function<void()> removeFunction = nullptr);
    void createPropertyColourChannel(        const PropertyData& metadata, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback);
    void createPropertyColourHue(            const PropertyData& metadata, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback);
    void createPropertyColourLightness(      const PropertyData& metadata, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback);
    void createPropertyColourSaturation(     const PropertyData& metadata, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback);
    void createPropertyCondition(            const PropertyData& metadata, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter);
    void createPropertyEditor(               const PropertyData& metadata, IEditable* editable);
    void createPropertyKey(                  const PropertyData& metadata, std::function<std::string()>  getter, std::function<void(sf::Keyboard::Key)>  setter,             std::function<void()> removeFunction = nullptr);
    void createPropertyList(                 const PropertyData& metadata, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, std::function<void()> removeFunction = nullptr);
    void createPropertyNativeBoolean(        const PropertyData& metadata, std::function<bool()>         getter, std::function<void(bool)>               setter,                                                                                                              std::function<void()> removeFunction = nullptr);
    void createPropertyNativeFloat(          const PropertyData& metadata, std::function<float()>        getter, std::function<void(float)>              setter,             std::function<bool(float)>              validityChecker = [](float)              {return true;}, std::function<void()> removeFunction = nullptr);
    void createPropertyNativeInteger(        const PropertyData& metadata, std::function<int()>          getter, std::function<void(int)>                setter,             std::function<bool(int)>                validityChecker = [](int)                {return true;}, std::function<void()> removeFunction = nullptr);
    void createPropertyNativeString(         const PropertyData& metadata, std::function<std::string()>  getter, std::function<void(const std::string&)> setter,             std::function<bool(const std::string&)> validityChecker = [](const std::string&) {return true;}, std::function<void()> removeFunction = nullptr, std::function<void(std::function<void()>, std::function<void()>)> confirmCustom = nullptr);
    void createPropertyNativeUnsignedInteger(const PropertyData& metadata, std::function<unsigned int()> getter, std::function<void(unsigned int)>       setter,             std::function<bool(unsigned int)>       validityChecker = [](unsigned int)       {return true;}, std::function<void()> removeFunction = nullptr);
    void createPropertyStruct(               const PropertyData& metadata, const std::string& value, std::function<void(PropertyMaker&)> subProperties, std::function<void()> removeFunction = nullptr);

    template <typename CONTAINER, typename VALUE_FUNC, typename PROPERTY_FUNC, typename ADD_FUNC> void createPropertyArray(const PropertyData& metadata, const CONTAINER& container, VALUE_FUNC value, PROPERTY_FUNC createProperty, ADD_FUNC add) {
      for (const auto& mElement : container) {
        createProperty(value(mElement));
      }
      createPropertyAdd(metadata, "Add...", [createProperty, add]() {
        createProperty(add());
      });
    }

    template <typename TREE_ITEM_TYPE> void createPropertyTreeSelector(const PropertyData& metadata, TREE_ITEM_TYPE& item, std::function<void()> removeFunction = nullptr) {
      cProperties.addProperty(std::make_unique<PropertyTreeSelector<TREE_ITEM_TYPE>>(*this, *this, cParent, metadata, item, removeFunction));
    }

    template <typename OPTIONAL_TYPE> void createPropertyOptional(const PropertyData& metadata, std::function<void(const std::string&)> choiceCallback) {
      cProperties.addProperty(std::make_unique<PropertyOptional<OPTIONAL_TYPE>>(*this, *this, cParent, metadata, choiceCallback, cParent.getProject(), cApplication));
    }

    void confirm(const std::string& message, std::function<void()> confirm, std::function<void()> cancel);
    
    /***********************************\
     * Implements IResourceAccessManager *
    \***********************************/
    void confirm(std::function<void()> confirm, std::function<void()> cancel) override;
    bool isResourceReadOnly() const override;
    void promoteResourceToProject() override;

    private:
    Application& cApplication;
    IResourceData& cParent;
    IPropertyManager& cProperties;
    IDialogManager& cDialogManager;
  };
}
