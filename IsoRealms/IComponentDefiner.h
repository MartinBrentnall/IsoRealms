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

#include "IsoRealms/IComponentArraySource.h"
#include "IsoRealms/Project/Options.h"

namespace IsoRealms {
  class Condition;
  class ConditionElement;
  class IComponentAccessManager;
  class IEditable;
  class IOptionalObject;
  class ITreeSelectorObject;
  class Module;

  using PropertyConfirmCallback        = std::function<void(      std::function<void()>, std::function<void()>, IComponentAccessManager&)>;
  using PropertyBooleanConfirmCallback = std::function<void(bool, std::function<void()>, std::function<void()>, IComponentAccessManager&)>;

  class IComponentDefiner {
    public:
    virtual ~IComponentDefiner() = default;

    /** Hint option: value is persisted inline in the current load object (see ComponentLoader). */
    inline static const std::string PROPERTY_INLINE = "inline";

    /** Hint option: load reference and properties immediately instead of deferring to project init. */
    inline static const std::string PROPERTY_IMMEDIATE = "immediate";

    /** Hint option: property is not loaded or saved via the property system. */
    inline static const std::string PROPERTY_NO_PERSIST = "noPersist";

    /** Hint option: property is not shown or editable in the editor. */
    inline static const std::string PROPERTY_HIDDEN = "hidden";

    /** Hint option: suppress the default array add control; use a separate add property instead. */
    inline static const std::string PROPERTY_NO_ADD = "noAdd";

    /** Hint option: JSON member name under which nested struct properties are loaded and saved. */
    inline static const std::string PROPERTY_SCOPED = "scoped";

    /** Hint option: scope loads from a project file; value is the file path (see PROPERTY_USER). ComponentLoader only. */
    inline static const std::string PROPERTY_FILE = "file";

    /** Hint option: used with PROPERTY_FILE; "true" if the file is in user space. ComponentLoader only. */
    inline static const std::string PROPERTY_USER = "user";

    /** Hint option: scope properties are deferred until components are loaded, using a captured object stack. */
    inline static const std::string PROPERTY_DEFER = "defer";

    inline static const Options HINT_IMMEDIATE        {{PROPERTY_IMMEDIATE,  "true"}};
    inline static const Options HINT_INLINE           {{PROPERTY_INLINE,     "true"}};
    inline static const Options HINT_DEFER            {{PROPERTY_DEFER,      "true"}};
    inline static const Options HINT_HIDDEN           {{PROPERTY_HIDDEN,     "true"}};
    inline static const Options HINT_SCOPED           {{PROPERTY_SCOPED,     "true"}};
    inline static const Options HINT_NO_ADD           {{PROPERTY_NO_ADD,     "true"}};
    inline static const Options HINT_NO_PERSIST       {{PROPERTY_NO_PERSIST, "true"}};
    inline static const Options HINT_INLINE_IMMEDIATE {{PROPERTY_INLINE,     "true"}, {PROPERTY_IMMEDIATE, "true"}};

    virtual bool loadsPersistedValues() const {
      return false;
    }

    virtual bool savesPersistedValues() const {
      return false;
    }

    virtual void onInitialised(std::function<void()> callback) {
      // Only invoked after persisted values have been applied during loading.
    }

    virtual void onPersisted(std::function<void()> callback) {
      // Only invoked after persisted values have been written during saving.
    }

    // TODO: Replace this function with a hint.
    virtual bool hasPersistedMember(const std::string& key) const {
      return false;
    }

    virtual void propertyAdd(             const std::string& key, const std::string& value, std::function<void()> addPropertyFunction, const Options& hint = Options::EMPTY) = 0;
    virtual void propertyBoolean(         const std::string& key, std::function<bool()>         getter, std::function<void(bool)>               setter, bool               defaultValue = false, std::function<void()> removeFunction = nullptr, PropertyBooleanConfirmCallback confirmCustom = nullptr) = 0;
    virtual void propertyCode(            const std::string& key, std::function<std::string()>  getter, std::function<void(const std::string&)> setter,             std::function<void()> removeFunction = nullptr) = 0;
    virtual void propertyColourChannel(   const std::string& key, std::function<float()> valueFunction, float* minRed, float* minGreen, float* minBlue, float* minAlpha, float* maxRed, float* maxGreen, float* maxBlue, float* maxAlpha, std::function<void(const float)> confirmationCallback) = 0;
    virtual void propertyColourHue(       const std::string& key, std::function<float()> valueFunction, float* saturation, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) = 0;
    virtual void propertyColourLightness( const std::string& key, std::function<float()> valueFunction, float* hue, float* saturation, float* alpha, std::function<void(const float)> confirmationCallback) = 0;
    virtual void propertyColourSaturation(const std::string& key, std::function<float()> valueFunction, float* hue, float* lightness, float* alpha, std::function<void(const float)> confirmationCallback) = 0;
    virtual void propertyCondition(       const std::string& key, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter, const Options& hint = Options::EMPTY) = 0;
    virtual void propertyEditor(          const std::string& key, IEditable* editable) = 0;
    virtual void propertyFloat(           const std::string& key, std::function<float()>        getter, std::function<void(float)>              setter, float              defaultValue = 0.0f,  std::function<bool(float)>              validityChecker = [](float)              {return true;}, std::function<void()> removeFunction = nullptr) = 0;
    virtual void propertyInteger(         const std::string& key, std::function<int()>          getter, std::function<void(int)>                setter, int                defaultValue = 0,     std::function<bool(int)>                validityChecker = [](int)                {return true;}, std::function<void()> removeFunction = nullptr, const Options& hint = Options::EMPTY) = 0;
    virtual void propertyKey(             const std::string& key, std::function<std::string()>  getter, std::function<void(sf::Keyboard::Key)>  setter,             std::function<void()> removeFunction = nullptr) = 0;
    virtual void propertyList(            const std::string& key, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, const std::string& defaultValue = "", std::function<void()> removeFunction = nullptr) = 0;
    virtual void propertyString(          const std::string& key, std::function<std::string()>  getter, std::function<void(const std::string&)> setter, const std::string& defaultValue = "",    std::function<bool(const std::string&)> validityChecker = [](const std::string&) {return true;}, std::function<void()> removeFunction = nullptr, PropertyConfirmCallback confirmCustom = nullptr) = 0;
    virtual void propertyOptional(        const std::string& key, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback, std::function<std::string()> valueGetter = nullptr, const Options& hint = Options::EMPTY) = 0;
    virtual void propertyResource(        const std::string& key, ITreeSelectorObject& item, const Options& hint = Options::EMPTY, std::function<void()> removeFunction = nullptr) = 0;
    virtual void propertyUnsignedInteger( const std::string& key, std::function<unsigned int()> getter, std::function<void(unsigned int)>       setter, unsigned int       defaultValue = 0,     std::function<bool(unsigned int)>       validityChecker = [](unsigned int)       {return true;}, std::function<void()> removeFunction = nullptr) = 0;

    virtual void scopeModule(Module& module, std::function<void()> removeFunction) = 0;
    virtual void scope(const std::string& key, const std::string& value, std::function<void()> subProperties, std::function<void()> removeFunction = nullptr, const Options& hint = Options::EMPTY, std::function<bool()> icon = nullptr) = 0;
    virtual void spacer(float height) = 0;

    virtual void keyedArray(const std::string& key, const std::string& addKey, IKeyedArraySource& source, const Options& hint = Options::EMPTY) = 0;
    virtual void array(const std::string& key, const std::string& addKey, IArraySource& source, const Options& hint = Options::EMPTY) = 0;
    virtual void fixedArray(const std::string& key, IFixedArraySource& source, const Options& hint = Options::EMPTY) = 0;

    template <typename CONTAINER, typename VALUE_FUNC, typename PROPERTY_FUNC, typename ADD_FUNC>
    void keyedArray(const std::string& key, const std::string& addKey, const CONTAINER& container, VALUE_FUNC value, PROPERTY_FUNC createProperty, ADD_FUNC add, const Options& hint = Options::EMPTY) {
      KeyedArraySource<CONTAINER, VALUE_FUNC, PROPERTY_FUNC, ADD_FUNC> mSource(container, value, createProperty, add);
      keyedArray(key, addKey, mSource, hint);
    }

    template <typename CONTAINER, typename VALUE_FUNC, typename PROPERTY_FUNC, typename ADD_FUNC>
    void array(const std::string& key, const CONTAINER& container, VALUE_FUNC value, PROPERTY_FUNC createProperty, ADD_FUNC add, const Options& hint = Options::EMPTY) {
      ArraySource<CONTAINER, VALUE_FUNC, PROPERTY_FUNC, ADD_FUNC> mSource(container, value, createProperty, add);
      array(key, key, mSource, hint);
    }

    template <typename CONTAINER, typename VALUE_FUNC, typename PROPERTY_FUNC>
    void fixedArray(const std::string& key, const CONTAINER& container, VALUE_FUNC value, PROPERTY_FUNC createProperty, const Options& hint = Options::EMPTY) {
      FixedArraySource<CONTAINER, VALUE_FUNC, PROPERTY_FUNC> mSource(container, value, createProperty);
      fixedArray(key, mSource, hint);
    }
  };
}
