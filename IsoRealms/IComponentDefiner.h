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
#include "IsoRealms/Project/ProjectFile.h"

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

    /** Hint keys and presets describe how a definer should treat a property or scope.
     *  Individual definers interpret only the hints they support; unsupported hints are ignored. */

    /** Sub-properties are stored directly in the parent record instead of under a nested record. */
    inline static const std::string HINT_KEY_INLINE = "inline";

    /** The property may be absent; definers should not require a value to be present. */
    inline static const std::string HINT_KEY_OPTIONAL = "optional";

    /** Apply nested resource properties during the current definition pass. */
    inline static const std::string HINT_KEY_IMMEDIATE = "immediate";

    /** Property participates in editing but is not written to external storage. */
    inline static const std::string HINT_KEY_TRANSIENT = "transient";

    /** Property is omitted from property menus. */
    inline static const std::string HINT_KEY_HIDDEN = "hidden";

    /** Suppress the default array add control; use propertyAdd separately. */
    inline static const std::string HINT_KEY_NO_ADD = "noAdd";

    /** Sub-properties are grouped under a named nested record keyed by the property name. */
    inline static const std::string HINT_KEY_NESTED = "nested";

    /** Scope sub-properties are rooted in another document; value is the document path. */
    inline static const std::string HINT_KEY_DOCUMENT = "document";

    /** Used with HINT_KEY_DOCUMENT; "true" if the document is in user space. */
    inline static const std::string HINT_KEY_USER_DOCUMENT = "userDocument";

    /** Used with HINT_KEY_DOCUMENT; "true" if the document should be written during saving. */
    inline static const std::string HINT_KEY_WRITABLE = "writable";

    /** Defer scope sub-properties until after the current definition pass completes. */
    inline static const std::string HINT_KEY_DEFER = "defer";

    /** Module that owns the resource metadata for this scope; empty for application resources. */
    inline static const std::string HINT_KEY_METADATA_MODULE = "metadataModule";

    /** Resource type name used to look up metadata for this scope. */
    inline static const std::string HINT_KEY_METADATA_RESOURCE_TYPE = "metadataResourceType";

    inline static const Options HINT_IMMEDIATE    {{HINT_KEY_IMMEDIATE,    "true"}};
    inline static const Options HINT_INLINE       {{HINT_KEY_INLINE,       "true"}};
    inline static const Options HINT_DEFER        {{HINT_KEY_DEFER,        "true"}};
    inline static const Options HINT_HIDDEN       {{HINT_KEY_HIDDEN,       "true"}};
    inline static const Options HINT_NESTED       {{HINT_KEY_NESTED,       "true"}};
    inline static const Options HINT_OPTIONAL     {{HINT_KEY_OPTIONAL,     "true"}};
    inline static const Options HINT_NO_ADD       {{HINT_KEY_NO_ADD,       "true"}};
    inline static const Options HINT_TRANSIENT    {{HINT_KEY_TRANSIENT,    "true"}};
    inline static const Options HINT_INLINE_IMMEDIATE = HINT_INLINE + HINT_IMMEDIATE;

    static Options externalScopeHint(const ProjectFile& file, bool writable = true) {
      return {{HINT_KEY_DOCUMENT, file.cFile.getRelativePath()}, {HINT_KEY_USER_DOCUMENT, file.cFile.isUser() ? "true" : "false"}, {HINT_KEY_WRITABLE, writable ? "true" : "false"}};
    }

    static Options resourceMetadataHint(const std::string& moduleName, const std::string& resourceType) {
      return {{HINT_KEY_METADATA_MODULE, moduleName}, {HINT_KEY_METADATA_RESOURCE_TYPE, resourceType}};
    }

    virtual void onInitialised(std::function<void()> callback, const Options& hint = Options::EMPTY) {
      // Only invoked after persisted values have been applied during loading.
    }

    virtual void onResourceLoaded(std::function<void()> callback) {
      // Only invoked after a propertyResource value has been applied during loading.
    }

    virtual void onPersisted(std::function<void()> callback) {
      // Only invoked after persisted values have been written during saving.
    }

    virtual void finish() {
      // Invoked once at the end of a successful define pass.
    }

    virtual void scopeOwnedResource(ProjectFile* ownerProjectFile, ProjectFile* loadingProjectFile, std::function<void()> scopeMember) {
      scopeMember();
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
    virtual void propertyString(          const std::string& key, std::function<std::string()>  getter, std::function<void(const std::string&)> setter, const std::string& defaultValue = "",    std::function<bool(const std::string&)> validityChecker = [](const std::string&) {return true;}, std::function<void()> removeFunction = nullptr, PropertyConfirmCallback confirmCustom = nullptr, const Options& hint = Options::EMPTY) = 0;
    virtual void propertyOptional(        const std::string& key, IOptionalObject& optionalSource, const std::string& noneLabel, std::function<bool()> noneIcon, std::function<void(const std::string&)> choiceCallback, std::function<std::string()> valueGetter = nullptr, const Options& hint = Options::EMPTY) = 0;
    virtual void propertyResource(        const std::string& key, ITreeSelectorObject& item, const Options& hint = Options::EMPTY, std::function<void()> removeFunction = nullptr) = 0;
    virtual void propertyUnsignedInteger( const std::string& key, std::function<unsigned int()> getter, std::function<void(unsigned int)>       setter, unsigned int       defaultValue = 0,     std::function<bool(unsigned int)>       validityChecker = [](unsigned int)       {return true;}, std::function<void()> removeFunction = nullptr) = 0;

    void propertyBoolean(const std::string& key, bool& value, bool defaultValue = false, std::function<void()> removeFunction = nullptr, PropertyBooleanConfirmCallback confirmCustom = nullptr) {
      propertyBoolean(key, [&]() {return value;}, [&](bool v) {value = v;}, defaultValue, removeFunction, confirmCustom);
    }

    void propertyFloat(const std::string& key, float& value, float defaultValue = 0.0f, std::function<bool(float)> validityChecker = [](float) {return true;}, std::function<void()> removeFunction = nullptr) {
      propertyFloat(key, [&]() {return value;}, [&](float v) {value = v;}, defaultValue, validityChecker, removeFunction);
    }

    void propertyInteger(const std::string& key, int& value, int defaultValue = 0, std::function<bool(int)> validityChecker = [](int) {return true;}, std::function<void()> removeFunction = nullptr, const Options& hint = Options::EMPTY) {
      propertyInteger(key, [&]() {return value;}, [&](int v) {value = v;}, defaultValue, validityChecker, removeFunction);
    }

    void propertyString(const std::string& key, std::string& value, const std::string& defaultValue = "", std::function<bool(const std::string&)> validityChecker = [](const std::string&) {return true;}, std::function<void()> removeFunction = nullptr, PropertyConfirmCallback confirmCustom = nullptr, const Options& hint = Options::EMPTY) {
      propertyString(key, [&]() {return value;}, [&](const std::string& v) {value = v;}, defaultValue, validityChecker, removeFunction, confirmCustom);
    }

    void propertyUnsignedInteger(const std::string& key, unsigned int& value, unsigned int defaultValue = 0, std::function<bool(unsigned int)> validityChecker = [](unsigned int) {return true;}, std::function<void()> removeFunction = nullptr) {
      propertyUnsignedInteger(key, [&]() {return value;}, [&](unsigned int v) {value = v;}, defaultValue, validityChecker, removeFunction);
    }

    virtual void scopeModule(Module& module, std::function<void()> removeFunction) = 0;
    virtual void scope(const std::string& key, const std::string& value, std::function<void()> subProperties, std::function<void()> removeFunction = nullptr, const Options& hint = Options::EMPTY, std::function<bool()> icon = nullptr) = 0;
    virtual void spacer(float height) = 0;

    virtual void keyedArray(const std::string& key, const std::string& addKey, IKeyedArraySource& source, const Options& hint = Options::EMPTY) = 0;
    virtual void scopeComponents(const std::string& key, const std::string& addKey, IComponentKeyedArraySource& source, std::function<void(IComponent& component)> scopeMember, const Options& hint = Options::EMPTY) = 0;
    virtual void scopeModules(const std::string& key, const std::string& addKey, IModuleKeyedArraySource& source, std::function<void(Module& module)> scopeMember, const Options& hint = Options::EMPTY) = 0;
    virtual void scopeOwnedKeyedArray(const std::string& key, const std::string& addKey, IOwnedKeyedArraySource& source, std::function<void(IOwnedKeyedMember& member)> scopeMember, const Options& hint = Options::EMPTY) = 0;
    virtual void array(const std::string& key, const std::string& addKey, IArraySource& source, const Options& hint = Options::EMPTY) = 0;
    virtual void fixedArray(const std::string& key, IFixedArraySource& source, const Options& hint = Options::EMPTY) = 0;

    template <typename CONTAINER, typename VALUE_FUNC, typename PROPERTY_FUNC, typename ADD_FUNC>
    void keyedArray(const std::string& key, const std::string& addKey, const CONTAINER& container, VALUE_FUNC value, PROPERTY_FUNC createProperty, ADD_FUNC add, const Options& hint = Options::EMPTY) {
      KeyedArraySource<CONTAINER, VALUE_FUNC, PROPERTY_FUNC, ADD_FUNC> mSource(container, value, createProperty, add);
      keyedArray(key, addKey, mSource, hint);
    }

    template <typename CONTAINER, typename GET_COMPONENT_FUNC, typename SCOPE_MEMBER_FUNC, typename ADD_FUNC>
    void scopeComponents(const std::string& key, const std::string& addKey, const CONTAINER& container, GET_COMPONENT_FUNC getComponent, SCOPE_MEMBER_FUNC scopeMember, ADD_FUNC add, const Options& hint = Options::EMPTY) {
      ComponentKeyedArraySource<CONTAINER, GET_COMPONENT_FUNC, ADD_FUNC> mSource(container, getComponent, add);
      scopeComponents(key, addKey, mSource, scopeMember, hint);
    }

    template <typename CONTAINER, typename GET_KEY_FUNC, typename GET_MODULE_FUNC, typename SCOPE_MEMBER_FUNC, typename ADD_FUNC>
    void scopeModules(const std::string& key, const std::string& addKey, const CONTAINER& container, GET_KEY_FUNC getKey, GET_MODULE_FUNC getModule, SCOPE_MEMBER_FUNC scopeMember, ADD_FUNC add, const Options& hint = Options::EMPTY) {
      ModuleKeyedArraySource<CONTAINER, GET_KEY_FUNC, GET_MODULE_FUNC, ADD_FUNC> mSource(container, getKey, getModule, add);
      scopeModules(key, addKey, mSource, scopeMember, hint);
    }

    template <typename CONTAINER, typename GET_MEMBER_FUNC, typename SCOPE_MEMBER_FUNC, typename ADD_FUNC>
    void scopeOwnedKeyedArray(const std::string& key, const std::string& addKey, const CONTAINER& container, GET_MEMBER_FUNC getMember, SCOPE_MEMBER_FUNC scopeMember, ADD_FUNC add, const Options& hint = Options::EMPTY) {
      OwnedKeyedArraySource<CONTAINER, GET_MEMBER_FUNC, ADD_FUNC> mSource(container, getMember, add);
      scopeOwnedKeyedArray(key, addKey, mSource, scopeMember, hint);
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
