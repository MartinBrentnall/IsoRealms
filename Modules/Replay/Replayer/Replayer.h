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

#include "IsoRealms.h"

namespace IsoRealms::Replay {
  class Replay;

  class Replayer final {
    public:
    Replayer(Replay& replay, IResourceData& data);
    Replayer(Replay& replay, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    bool renderIcon() const;
    void hintInUse(bool inUse);
    void getProperties(PropertyMaker& owner, const Metadata& metadata);

    /*********************\
     * Module interfaces *
    \*********************/
    void updateInputs(unsigned int milliseconds);
    void updateRuntime(unsigned int milliseconds);
    void reset();

    /***********************\
     * Scripting Interface *
    \***********************/
    void setRecording(const std::string& file);
    void setReplaying(const std::string& file, bool user);
    void setInactive();

    private:
    union InputType {
      bool cDigital;
      float cAnalogue;
    };

    struct InputEvent {
      unsigned int cID;
      InputType cState;
      unsigned int cTime;
    };

    class DigitalInput : public IBoolean {
      public:
      DigitalInput(Replayer& parent, IResourceData& data);
      DigitalInput(Replayer& parent, IResourceData& data, JSONObject object);
      void registerAssets(ResourceAssetRegistry& assets);
      void save(JSONObject object) const;
      void getProperties(PropertyMaker& owner, const Metadata& metadata);
      void setRecordedState(bool state);
      std::string getName() const;
      
      /***********************\
       * Implements IBoolean *
      \***********************/
      bool getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(PropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;
      
      private:
      
      // External interfaces.
      Replayer& cParent;
      IStateNotifier<IBoolean>* cStateNotifier;

      // Definition data.
      std::string cDefName;
      Boolean cDefActualInput;
      
      // Runtime data.
      unsigned int cRuntimeID;
      bool cRuntimeRecordedInput;
    };
    
    class AnalogueInput : public IFloat {
      public:
      AnalogueInput(Replayer& parent, IResourceData& data);
      AnalogueInput(Replayer& parent, IResourceData& data, JSONObject object);
      void registerAssets(ResourceAssetRegistry& assets);
      void save(JSONObject object) const;
      void getProperties(PropertyMaker& owner, const Metadata& metadata);
      void setRecordedState(float state);
      std::string getName() const;

      /*********************\
       * Implements IFloat *
      \*********************/
      float getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(PropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;
      
      private:
      
      // External interfaces.
      Replayer& cParent;
      IStateNotifier<IFloat>* cStateNotifier;
      
      // Definition data.
      std::string cDefName;
      Float cDefActualInput;
      
      // Runtime data.
      unsigned int cRuntimeID;
      float cRuntimeRecordedInput;
    };
    
    enum class State {
      INACTIVE,
      RECORDING,
      REPLAYING
    };
    
    static const std::string JSON_ANALOGUE_INPUTS;
    static const std::string JSON_DIGITAL_INPUTS;
    static const std::string JSON_NAME;
    static const std::string JSON_VALUE;

    IResourceData& cResource;

    State cState;
    
    unsigned int cElapsedTime;
    std::string cFilename; 
    InputEvent cNextEvent;
    std::ifstream cRecording;
    LiteralString cFilenameString;
    std::ofstream cOutput;
    std::vector<std::unique_ptr<DigitalInput>> cDigitalInputs;
    std::vector<std::unique_ptr<AnalogueInput>> cAnalogueInputs;

    LuaBinding<Replayer> cLuaBinding;
    
    void readEvent();
    void writeEvent(unsigned int id, bool state);
    void writeEvent(unsigned int id, float state);
    bool isInputNameAllowed(DigitalInput& input, const std::string& name) const;
    bool isInputNameAllowed(AnalogueInput& input, const std::string& name) const;
  };
}
