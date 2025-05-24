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
#include <iostream>
#include <vector>

#include "IsoRealms/IAssetOverride.h"
#include "IsoRealms/Project.h"
#include "IsoRealms/Options/LocalOptions.h"
#include "IsoRealms/Options/WrappedOptions.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Replay {
  class Replay;
  
  class Player : public IAssetOverride,
                 public IScreen,
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
      private:
      bool cState;
      std::vector<IStateListener<IBoolean*>*> cListeners;
        
      public:
      DigitalInput();
        
      void setValue(bool state);
      void addListener(IStateListener<IBoolean*>* listener);
      
      /***********************\
       * Implements IBoolean *
      \***********************/
      bool getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;
    };

    class AnalogueInput : public IFloat {
      private:
      float cState;

      public:
      AnalogueInput();

      void setValue(float state);

      /*********************\
       * Implements IFloat *
      \*********************/
      float getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;
    };

    IProject& cParentProject;
    std::string cFilename; 
    Action cQuitAction;
    std::ifstream cRecording;
    std::unique_ptr<Project> cProject;
    std::vector<std::unique_ptr<DigitalInput>> cDigitalInputs;
    std::vector<std::unique_ptr<AnalogueInput>> cAnalogueInputs;
    std::map<std::string, DigitalInput*> cOverriddenDigitalInputs;
    std::map<std::string, IFloat*> cOverriddenAnalogueInputs;
    unsigned int cElapsedTime;
    InputEvent cNextEvent;
    bool cFinished;
      
    void readNextEvent();
    void finish();
    
    public:
    Player(IProject& project, Replay& replay, IResourceData& data);
    Player(IProject& project, Replay& replay, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish);
    void save(JSONObject object, IAssetIdentifier& identifier) const;
    bool renderIcon() const;
    void hintInUse(bool inUse);
    std::vector<std::unique_ptr<IProperty>> getProperties(IAssetBrowser& browser, IAssetRegistry& assets);
    
    /*********************\
    * Implements IScreen *
    \*********************/
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
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    /*****************************\
     * Implements IAssetOverride *
    \*****************************/
    IActionType*     getActionType(    JSONObject object, IStateListener<IActionType*>*     listener) const override;
    IAssets*         getAssets(        JSONObject object, IStateListener<IAssets*>*         listener) const override;
    IBinding*        getBinding(       JSONObject object, IStateListener<IBinding*>*        listener) const override;
    IBindingType*    getBindingType(   JSONObject object, IStateListener<IBindingType*>*    listener) const override;
    IBoolean*        getBoolean(       JSONObject object, IStateListener<IBoolean*>*        listener) const override;
    IColour*         getColour(        JSONObject object, IStateListener<IColour*>*         listener) const override;
    IEditable*       getEditable(      JSONObject object, IStateListener<IEditable*>*       listener) const override;
    IFloat*          getFloat(         JSONObject object, IStateListener<IFloat*>*          listener) const override;
    IFont*           getFont(          JSONObject object, IStateListener<IFont*>*           listener) const override;
    IInputHandler*   getInputHandler(  JSONObject object, IStateListener<IInputHandler*>*   listener) const override;
    IInteger*        getInteger(       JSONObject object, IStateListener<IInteger*>*        listener) const override;
    IModel*          getModel(         JSONObject object, IStateListener<IModel*>*          listener) const override;
    IProjectOptions* getProjectOptions(JSONObject object, IStateListener<IProjectOptions*>* listener) const override;
    IScreen*         getScreen(        JSONObject object, IStateListener<IScreen*>*         listener) const override;
    IString*         getString(        JSONObject object, IStateListener<IString*>*         listener) const override;
    ITexture*        getTexture(       JSONObject object, IStateListener<ITexture*>*        listener) const override;
    IVertex*         getVertex(        JSONObject object, IStateListener<IVertex*>*         listener) const override;
  };
}
