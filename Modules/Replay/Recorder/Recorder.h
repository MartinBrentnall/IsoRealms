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

#include <fstream>
#include <iomanip>
#include <iostream>

#include "IsoRealms.h"

namespace IsoRealms::Replay {
  class Replay;
  
  class Recorder final : public IScreen,
                         public IInputHandler {
    private:
    static const std::string JSON_INPUT;
    static const std::string JSON_INPUT_CONFIGURATION;
    static const std::string JSON_INPUTS;
    static const std::string JSON_KEY;
    static const std::string JSON_ON_FINISH;
    static const std::string JSON_TYPE;

    static const std::string TYPE_ANALOGUE;
    static const std::string TYPE_DIGITAL;

    // External interfaces.
    IProject& cParentProject;
    
    std::vector<std::unique_ptr<Boolean>> cDefDigitalInputs;
    std::vector<std::unique_ptr<Float>> cDefAnalogueInputs;
    LiteralString cFilenameString;
    std::string cFilename;
    std::string cInputConfiguration;
    std::ofstream cOutput;
    unsigned int cElapsedTime;
    Action cQuitAction;
    std::unique_ptr<Project> cProject;
      
    LuaBinding<Recorder> cLuaBinding;
    
    public:
    Recorder(IProject& project, Replay& replay, IResourceData& data);
    Recorder(IProject& project, Replay& replay, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IPropertyOwner& owner);

    /*********************\
     * Module interfaces *
    \*********************/
    void updateRuntime(unsigned int milliseconds);
    void reset();

    void writeInput(unsigned int id, bool state);
    void writeInput(unsigned int id, float state);

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;

    /****************************\
     * Implements IInputHandler *
    \****************************/
    bool input(sf::Event&) override;
    void resetInput() override;

    /*********************\
     * Implements IAsset *
    \*********************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties(IPropertyOwner& owner) override;
    bool isDefaultConfiguration() const override;
  };
}
