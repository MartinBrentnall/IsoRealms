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
    static const std::string TAG_QUIT_ACTION;

    static const std::string ATTRIBUTE_KEY;

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
    };

    IProject* cParentProject;
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
    Player(IProject* project, Replay* replay);
    Player(IProject* project, Replay* replay, DOMNode& node, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    bool renderIcon() const;
    void hintInUse(bool inUse);
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);
    
    /*********************\
    * Implements IScreen *
    \*********************/
    void renderScreen(float scale, float aspectRatio) const override;

    /****************************\
     * Implements IInputHandler *
    \****************************/
    bool input(sf::Event&) override;

    /*********************\
     * Implements IAsset *
    \*********************/
    bool renderAssetIcon() const override;
    
    /*****************************\
     * Implements IAssetOverride *
    \*****************************/
    I3DModelType*    getModelType(     DOMNode& node, IStateListener<I3DModelType*>* listener) const override;
    IActionType*     getActionType(    DOMNode& node, IStateListener<IActionType*>* listener) const override;
    IAssets*         getAssets(        DOMNode& node, IStateListener<IAssets*>* listener) const override;
    IBinding*        getBinding(       DOMNode& node, IStateListener<IBinding*>* listener) const override;
    IBoolean*        getBoolean(       DOMNode& node, IStateListener<IBoolean*>* listener) const override;
    IColour*         getColour(        DOMNode& node, IStateListener<IColour*>* listener) const override;
    IEditable*       getEditable(      DOMNode& node, IStateListener<IEditable*>* listener) const override;
    IFloat*          getFloat(         DOMNode& node, IStateListener<IFloat*>* listener) const override;
    IFont*           getFont(          DOMNode& node, IStateListener<IFont*>* listener) const override;
    IInputHandler*   getInputHandler(  DOMNode& node, IStateListener<IInputHandler*>* listener) const override;
    IInteger*        getInteger(       DOMNode& node, IStateListener<IInteger*>* listener) const override;
    IProjectOptions* getProjectOptions(DOMNode& node, IStateListener<IProjectOptions*>* listener) const override;
    IScreen*         getScreen(        DOMNode& node, IStateListener<IScreen*>* listener) const override;
    IString*         getString(        DOMNode& node, IStateListener<IString*>* listener) const override;
    ITexture*        getTexture(       DOMNode& node, IStateListener<ITexture*>* listener) const override;
    IVertex*         getVertex(        DOMNode& node, IStateListener<IVertex*>* listener) const override;
  };
}
