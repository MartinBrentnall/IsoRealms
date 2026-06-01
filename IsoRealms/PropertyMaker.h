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

#include "IsoRealms/Editing/Property/IPropertyMaker.h"

namespace IsoRealms {
  class Application;
  class Condition;
  class ConditionElement;
  class IDialogManager;
  class IEditable;
  class IOptionalObject;
  class IPropertyManager;
  class IResourceData;
  class ITreeSelectorObject;

  class PropertyMaker : public IPropertyMaker {
    public:
    PropertyMaker(Application& application, IResourceData& parent, IPropertyManager& properties, IDialogManager& dialogManager);

    IResourceData& getResourceData() override;

    void createPropertyAdd(                  const PropertyData& metadata, const std::string& value, std::function<void()> addPropertyFunction) override;
    void createPropertyCode(                 const PropertyData& metadata, std::function<std::string()>  getter, std::function<void(const std::string&)> setter,             std::function<void()> removeFunction = nullptr) override;
    void createPropertyColourChannel(        const PropertyData& metadata, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) override;
    void createPropertyColourHue(            const PropertyData& metadata, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) override;
    void createPropertyColourLightness(      const PropertyData& metadata, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) override;
    void createPropertyColourSaturation(     const PropertyData& metadata, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) override;
    void createPropertyCondition(            const PropertyData& metadata, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter) override;
    void createPropertyEditor(               const PropertyData& metadata, IEditable* editable) override;
    void createPropertyKey(                  const PropertyData& metadata, std::function<std::string()>  getter, std::function<void(sf::Keyboard::Key)>  setter,             std::function<void()> removeFunction = nullptr) override;
    void createPropertyList(                 const PropertyData& metadata, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, std::function<void()> removeFunction = nullptr) override;
    void createPropertyNativeBoolean(        const PropertyData& metadata, std::function<bool()>         getter, std::function<void(bool)>               setter,                                                                                                              std::function<void()> removeFunction = nullptr) override;
    void createPropertyNativeFloat(          const PropertyData& metadata, std::function<float()>        getter, std::function<void(float)>              setter,             std::function<bool(float)>              validityChecker, std::function<void()> removeFunction) override;
    void createPropertyNativeInteger(        const PropertyData& metadata, std::function<int()>          getter, std::function<void(int)>                setter,             std::function<bool(int)>                validityChecker, std::function<void()> removeFunction) override;
    void createPropertyNativeString(         const PropertyData& metadata, std::function<std::string()>  getter, std::function<void(const std::string&)> setter,             std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction, std::function<void(std::function<void()>, std::function<void()>)> confirmCustom) override;
    void createPropertyNativeUnsignedInteger(const PropertyData& metadata, std::function<unsigned int()> getter, std::function<void(unsigned int)>       setter,             std::function<bool(unsigned int)>       validityChecker, std::function<void()> removeFunction) override;
    void createPropertyOptional(             const PropertyData& metadata, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback, std::function<std::string()> valueGetter = nullptr) override;
    void createPropertyStruct(               const PropertyData& metadata, const std::string& value, std::function<void(IPropertyMaker&)> subProperties, std::function<void()> removeFunction = nullptr) override;
    void createPropertyTreeSelector(         const PropertyData& metadata, ITreeSelectorObject& item, std::function<void()> removeFunction = nullptr) override;

    /*************************************\
     * Implements IResourceAccessManager *
    \*************************************/
    void confirm(const std::string& message, std::function<void()> confirm, std::function<void()> cancel) override;
    bool isResourceReadOnly() const override;
    void promoteResourceToProject() override;

    private:
    Application& cApplication;
    IResourceData& cParent;
    IPropertyManager& cProperties;
    IDialogManager& cDialogManager;
  };
}
