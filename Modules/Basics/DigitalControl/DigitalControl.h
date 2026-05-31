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
#include <set>
#include <queue>

#include "IsoRealms.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a digital input.  Each digital input may be bound
   * to multiple physical inputs.  The digital input is implemented as an Input
   * Handler and its state is exposed as a Boolean and via a state notifier.
   *
   * Digital input provides functions to facilitate overriding the defined
   * inputs with user-defined physical input mappings, as well as persistence
   * of the user-defined input mappings.
   *
   * All functions operate on the basis of user-defined physical input mappings
   * when they exist, and default physical input mappings when user-defined
   * physicalinput mappings do not exist.
   */
  class DigitalControl final : public IBoolean,
                               public IInputHandler {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    DigitalControl(Basics& basics, IResourceData& data);
    DigitalControl(Basics& basics, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void registerAssets(ResourceAssetRegistry& assets, const std::string& parentID);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    DigitalControl(IResourceData& owner);
    DigitalControl(IResourceData& owner, JSONObject object);

    /*********************\
     * Module interfaces *
    \*********************/
    void reset();

    /***********************\
     * Implements IBoolean *
    \***********************/
    bool getValue() const override;

    /****************************\
     * Implements IInputHandler *
    \****************************/
    bool input(sf::Event& event) override;
    void resetInput() override;

    /***********************\
     * Implements multiple *
    \***********************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

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
     * Return the number of physical inputs bound to this digital input.
     *
     * @return The number of physical inputs currently bound to this digital
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
    std::shared_ptr<DigitalInput> getMapping(unsigned int index) const;

    /**
     * Bind the specified digital input mapping as a user-mapping to this
     * digital input.
     *
     * @param input The input to bind.
     */
    void addCustomInput(std::shared_ptr<DigitalInput> input);

    /**
     * Remove all user-bound physical input mappings.
     */
    void clearCustomInputs();

    /********************\
     * Module Interface *
    \********************/
    /**
     * Read physical input mapping from the specified node and bind them as
     * user-defined inputs to this digital input.
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
    inline static const std::string JSON_TYPE     = "type";
    inline static const std::string JSON_MAPPINGS = "mappings";

    class InputMapping {
      public:
      InputMapping(std::shared_ptr<DigitalInput> input);

      bool matches(sf::Event& event) const;
      bool input(sf::Event& event);
      void reset();
      std::string getShortName() const;
      std::shared_ptr<DigitalInput> getInput() const;
      void save(JSONObject object) const;
      void getProperties(IPropertyMaker& owner, const Metadata& metadata, std::function<void()> removeFunction);

      private:
      std::shared_ptr<DigitalInput> cInput;
      bool cState;
    };

    // External interfaces.
    Project& cProject;
    IResourceData& cResourceData;

    // Definition data.
    std::vector<std::unique_ptr<InputMapping>> cDefMapping; /// Default input mapping.

    // Runtime data.
    std::vector<std::unique_ptr<InputMapping>> cRuntimeMapping; /// User input mapping.
    bool cRuntimeState;                                         /// Current state of this input.

    // Scripting Interface.
    LuaBinding<DigitalControl> cLuaBinding;

    // Misc.
    IStateNotifier* cStateNotifier;
  };
}
