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
#include <vector>

#include "IsoRealms/IComponentDefiner.h"
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
  class JSONDocument;
  class JSONObject;

  /**
   * Loads persisted field values by invoking the same property declarations used
   * for editing.  Each property key is used to read from JSON, and the property
   * setter (or equivalent callback) is used to apply the loaded value.
   */
  class ComponentLoader : public IComponentDefiner {
    public:
    ComponentLoader(IComponentData& resourceData, JSONObject object);
    ComponentLoader(IComponentData& resourceData, const std::string& file, bool user);

    void propertyAdd(             const std::string& key, const std::string& value, std::function<void()> addPropertyFunction, const Options& hint = Options::EMPTY) override;
    void propertyBoolean(         const std::string& key, std::function<bool()>         getter, std::function<void(bool)>               setter, bool               defaultValue, std::function<void()> removeFunction, PropertyBooleanConfirmCallback confirmCustom = nullptr) override;
    void propertyCode(            const std::string& key, std::function<std::string()>  getter, std::function<void(const std::string&)> setter,             std::function<void()> removeFunction = nullptr) override;
    void propertyColourChannel(   const std::string& key, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) override;
    void propertyColourHue(       const std::string& key, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) override;
    void propertyColourLightness( const std::string& key, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) override;
    void propertyColourSaturation(const std::string& key, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) override;
    void propertyCondition(       const std::string& key, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter, const Options& hint = Options::EMPTY) override;
    void propertyEditor(          const std::string& key, IEditable* editable) override;
    void propertyFloat(           const std::string& key, std::function<float()>        getter, std::function<void(float)>              setter, float              defaultValue, std::function<bool(float)>              validityChecker, std::function<void()> removeFunction) override;
    void propertyInteger(         const std::string& key, std::function<int()>          getter, std::function<void(int)>                setter, int                defaultValue, std::function<bool(int)>                validityChecker, std::function<void()> removeFunction, const Options& hint = Options::EMPTY) override;
    void propertyKey(             const std::string& key, std::function<std::string()>  getter, std::function<void(sf::Keyboard::Key)>  setter,             std::function<void()> removeFunction = nullptr) override;
    void propertyList(            const std::string& key, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, const std::string& defaultValue, std::function<void()> removeFunction) override;
    void propertyOptional(        const std::string& key, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback, std::function<std::string()> valueGetter = nullptr, const Options& hint = Options::EMPTY) override;
    void propertyResource(        const std::string& key, ITreeSelectorObject& item, const Options& hint = Options::EMPTY, std::function<void()> removeFunction = nullptr) override;
    void propertyString(          const std::string& key, std::function<std::string()>  getter, std::function<void(const std::string&)> setter, const std::string& defaultValue, std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction, PropertyConfirmCallback confirmCustom, const Options& hint = Options::EMPTY) override;
    void propertyUnsignedInteger( const std::string& key, std::function<unsigned int()> getter, std::function<void(unsigned int)>       setter, unsigned int       defaultValue, std::function<bool(unsigned int)>       validityChecker, std::function<void()> removeFunction) override;

    void scopeModule(Module& module, std::function<void()> removeFunction) override;
    void scope(const std::string& key, const std::string& value, std::function<void()> subProperties, std::function<void()> removeFunction = nullptr, const Options& hint = Options::EMPTY, std::function<bool()> icon = nullptr) override;
    void spacer(float height) override;

    void keyedArray(const std::string& key, const std::string& addKey, IKeyedArraySource& source, const Options& hint = Options::EMPTY) override;
    void array(const std::string& key, const std::string& addKey, IArraySource& source, const Options& hint = Options::EMPTY) override;
    void fixedArray(const std::string& key, IFixedArraySource& source, const Options& hint = Options::EMPTY) override;

    void onInitialised(std::function<void()> callback, const Options& hint = Options::EMPTY) override;

    private:
    bool loadPropertyArray(const std::string& key, const std::function<void()>& addAndLoadElement, const Options& hint = Options::EMPTY);
    void loadKeyedArray(const std::string& key, const std::function<void(const std::string& memberKey, bool isNull)>& loadMember, const Options& hint = Options::EMPTY);
    bool loadFixedPropertyArray(const std::string& key, unsigned int count, const std::function<void(unsigned int index)>& loadElement);
    bool loadKeyedMembers(const std::function<void(const std::string& key, bool isNull)>& loadMember);

    IComponentData& cComponentData;
    std::vector<std::unique_ptr<JSONDocument>> cDocuments;
    std::vector<JSONObject> cObjects;

    const JSONObject& currentObject() const;
    JSONObject& currentObject();
    void pushObject(JSONObject object);
    void popObject();
    void loadTreeSelectorResourceProperties(ITreeSelectorObject& item, JSONObject object, const Options& hint);
  };
}
