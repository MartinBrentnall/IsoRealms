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

#include <vector>

#include "IsoRealms/Editing/Property/IPropertyMaker.h"
#include "IsoRealms/Persistence/JSONArray.h"
#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Persistence/JSONValue.h"
#include "IsoRealms/Project/Options.h"

namespace IsoRealms {
  class Condition;
  class ConditionElement;
  class IEditable;
  class IOptionalObject;
  class IComponentData;
  class ITreeSelectorObject;

  /**
   * Loads persisted field values by invoking the same property declarations used
   * for editing.  Each property key is used to read from JSON, and the property
   * setter (or equivalent callback) is used to apply the loaded value.
   */
  class PropertyLoader : public IPropertyMaker {
    public:
    PropertyLoader(IComponentData& resourceData, JSONObject object);
    PropertyLoader(IComponentData& resourceData, std::vector<JSONObject> objects);

    IComponentData& getComponentData() override;

    // TODO: Replace this function with a hint or something more elegant.
    bool loadsPersistedValues() const override {
      return true;
    }

    bool hasPersistedMember(const std::string& key) const override;

    void createPropertyAdd(                  const std::string& key, const std::string& value, std::function<void()> addPropertyFunction) override;
    void createPropertyCode(                 const std::string& key, std::function<std::string()>  getter, std::function<void(const std::string&)> setter,             std::function<void()> removeFunction = nullptr) override;
    void createPropertyColourChannel(        const std::string& key, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) override;
    void createPropertyColourHue(            const std::string& key, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) override;
    void createPropertyColourLightness(      const std::string& key, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) override;
    void createPropertyColourSaturation(     const std::string& key, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) override;
    void createPropertyCondition(            const std::string& key, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter) override;
    void createPropertyEditor(               const std::string& key, IEditable* editable) override;
    void createPropertyKey(                  const std::string& key, std::function<std::string()>  getter, std::function<void(sf::Keyboard::Key)>  setter,             std::function<void()> removeFunction = nullptr) override;
    void createPropertyList(                 const std::string& key, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, const std::string& defaultValue, std::function<void()> removeFunction) override;
    void createPropertyNativeBoolean(        const std::string& key, std::function<bool()>         getter, std::function<void(bool)>               setter, bool               defaultValue,                                                          std::function<void()> removeFunction) override;
    void createPropertyNativeFloat(          const std::string& key, std::function<float()>        getter, std::function<void(float)>              setter, float              defaultValue, std::function<bool(float)>              validityChecker, std::function<void()> removeFunction) override;
    void createPropertyNativeInteger(        const std::string& key, std::function<int()>          getter, std::function<void(int)>                setter, int                defaultValue, std::function<bool(int)>                validityChecker, std::function<void()> removeFunction, const Options& hint = Options::EMPTY) override;
    void createPropertyNativeString(         const std::string& key, std::function<std::string()>  getter, std::function<void(const std::string&)> setter, const std::string& defaultValue, std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction, std::function<void(std::function<void()>, std::function<void()>)> confirmCustom) override;
    void createPropertyNativeUnsignedInteger(const std::string& key, std::function<unsigned int()> getter, std::function<void(unsigned int)>       setter, unsigned int       defaultValue, std::function<bool(unsigned int)>       validityChecker, std::function<void()> removeFunction) override;
    void createPropertyOptional(             const std::string& key, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback, std::function<std::string()> valueGetter = nullptr, const Options& hint = Options::EMPTY) override;
    void createPropertyStruct(               const std::string& key, const std::string& value, std::function<void(IPropertyMaker&)> subProperties, std::function<void()> removeFunction = nullptr, const Options& hint = Options::EMPTY) override;
    void createPropertyTreeSelector(         const std::string& key, ITreeSelectorObject& item, const Options& hint = Options::EMPTY, std::function<void()> removeFunction = nullptr) override;

    /**************************************\
     * Implements IComponentAccessManager *
    \**************************************/
    void confirm(const std::string& message, std::function<void()> confirm, std::function<void()> cancel) override;
    bool isComponentReadOnly() const override;
    void promoteComponentToProject() override;

    protected:
    bool loadPropertyArray(const std::string& key, const std::function<void()>& addAndLoadElement) override;
    bool loadFixedPropertyArray(const std::string& key, unsigned int count, const std::function<unsigned int(const JSONObject&)>& matchIndex, const std::function<void(unsigned int index)>& loadElement) override;

    private:
    IComponentData& cComponentData;
    std::vector<JSONObject> cObjects;

    const JSONObject& currentObject() const;
    JSONObject& currentObject();
    void pushObject(JSONObject object);
    void popObject();
    void loadTreeSelectorAssetProperties(ITreeSelectorObject& item, JSONObject object, const Options& hint);
  };
}
