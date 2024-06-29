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

#include <fstream>
#include <iomanip>
#include <iostream>

#include "IsoRealms/IStateListener.h"
#include "IsoRealms/Literals.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/Options/LocalOptions.h"
#include "IsoRealms/Project.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Replay {
  class Replay;
  
  class Recorder : public IScreen,
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

    IProject* cParentProject;
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
    Recorder(IProject* project, Replay* replay);
    Recorder(IProject* project, Replay* replay, JSONObject object, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(JSONObject object, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

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
  };
}
