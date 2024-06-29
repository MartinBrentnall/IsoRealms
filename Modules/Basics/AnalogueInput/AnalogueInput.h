/*
 * Copyright 2023 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <set>
#include <queue>

#include "IsoRealms/Exception/ActionException.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/LocalAssetRegistry.h"
#include "IsoRealms/Input.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"
#include "IsoRealms/Utils.h"

#include "DigitalToAnalogueMapping.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for an analogue input.  Each analogue input may be
   * bound to multiple physical inputs.  The analogue input is implemented as
   * an Input Handler and its state is exposed as a Float and via a state
   * notifier.
   *
   * Analogue input provides functions to facilitate overriding the defined
   * inputs with user-defined physical input mappings, as well as persistence
   * of the user-defined input mappings.
   *
   * All functions operate on the basis of user-defined physical input mappings
   * when they exist, and default physical input mappings when user-defined
   * physicalinput mappings do not exist.
   */
  class AnalogueInput final : public IFloat,
                              public IInputHandler {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    AnalogueInput(IProject* project, Basics* basics);
    AnalogueInput(IProject* project, Basics* basics, JSONObject object, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(JSONObject object, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    /*********************\
     * Implements IFloat *
    \*********************/
    float getValue() const override;

    /****************************\
     * Implements IInputHandler *
    \****************************/
    bool input(sf::Event& event) override;
    void resetInput() override;

    /*********************\
     * Implements IAsset *
    \*********************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;

    /***********************\
     * Scripting Interface *
    \***********************/
    /**
     * Obtain a string representation of the physical input mappings to be unsigned
     * for displaying in menus (etc.).
     *
     * @return String representation of the current input mapping.
     */
    std::string getInputsString() const;

    /**
     * Return the number of physical inputs bound to this analogue input.
     *
     * @return The number of physical inputs currently bound to this analogue
     *         input.
     */
    unsigned int getMappingCount() const;

    /**
     * Return the physical input at the specified index, which is expected to
     * fall between 0 and getMappingCount() - 1.
     *
     * @param index Index of the desired input mapping.
     * @return The physical input mapping at the specified index.
     */
    std::shared_ptr<IAnalogueInputMapping> getMapping(unsigned int index) const;

    /**
     * Bind the specified analogue input mapping as a user-mapping to this
     * analogue input.
     *
     * @param input The input to bind.
     */
    void addCustomInput(std::shared_ptr<IAnalogueInputMapping> input);

    /**
     * Remove all user-bound physical input mappings.
     */
    void clearCustomInputs();

    /********************\
     * Module Interface *
    \********************/
    /**
     * Read physical input mapping from the specified node and bind them as
     * user-defined inputs to this analogue input.
     *
     * @param node The node from which to read physical input mappings.
     */
    void loadCustomMapping(JSONObject object);

    /**
     * Write user-defined physical input mappings to the specified node.  If
     * there are no user-define physical input mappings, this function has
     * no effect.
     *
     * @param node The node to which to write user-defined physical input
     *         mappings.
     */
    void saveCustomMapping(JSONObject object) const;

    private:

    // JSON members.
    static const std::string JSON_ID;
    static const std::string JSON_INPUT;
    static const std::string JSON_MAPPINGS;
    static const std::string JSON_NAME;
    static const std::string JSON_TYPE;

    // Definition data.
    class InputMapping {
      public:
      InputMapping(std::shared_ptr<IAnalogueInputMapping> input, const std::string& name);

      bool matches(sf::Event& event) const;
      float input(sf::Event& event);
      std::string getShortName() const;
      std::shared_ptr<IAnalogueInputMapping> getInput() const;
      void save(JSONObject object) const;
      void loadCustomMapping(JSONObject object);
      void registerAssets(IAssetRegistry* assets);
      void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
      std::string getName();
      void reset();

      private:
      std::string cName;
      std::shared_ptr<IAnalogueInputMapping> cPhysicalInput;
      float cState;
    };
    std::vector<std::unique_ptr<InputMapping>> cDefMapping; /// Default input mapping.

    // Runtime data.
    std::vector<std::unique_ptr<InputMapping>> cRuntimeMapping; /// User input mapping.
    float cRuntimeState;                                        /// Current state of this input.

    // Scripting Interface.
    LuaBinding<AnalogueInput> cLuaBinding;

    // Misc.
    IStateNotifier<IFloat>* cStateNotifier;
  };
}
