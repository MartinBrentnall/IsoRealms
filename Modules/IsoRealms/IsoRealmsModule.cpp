/*
 * Copyright 2015 Martin Brentnall
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
#include "IsoRealmsModule.h"

const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_BOOLEAN_FIXED                 = "BooleanFixed";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_BOUNDARIES                    = "Boundaries";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_COLOUR_FIXED                  = "ColourFixed";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_ELEMENT_HUD_MODEL             = "ElementHUDModel";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_ELEMENT_HUD_ROUNDED_RECTANGLE = "ElementHUDRoundedRectangle";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_ELEMENT_HUD_STRING            = "ElementHUDString";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_FONT_FILE                     = "FontFile";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_GLOBAL_VARIABLE               = "GlobalVariable";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_INTEGER_FIXED                 = "IntegerFixed";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_INTEGER_TIMER                 = "IntegerTimer";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_LAYER_HUD                     = "LayerHUD";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_MODEL_ELEMENT                 = "ModelElement";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_MODEL_SCRIPTABLE              = "ModelScriptable";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_MODEL_SPRITE                  = "ModelSprite";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_SCRIPT_LUA                    = "ScriptLua";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_SOUND_FILE                    = "SoundFile";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_TEXTURE_FILE                  = "TextureFile";
const std::string IsoRealmsModule::TAG_RESOURCE_TYPE_VERTEX_FIXED                  = "VertexFixed";

const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_BOOLEAN_FIXED                 = "BooleanFixed";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_BOUNDARIES                    = "Boundaries";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_COLOUR_FIXED                  = "Fixed Colour";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_ELEMENT_HUD_MODEL             = "HUD Model Element";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_ELEMENT_HUD_ROUNDED_RECTANGLE = "HUD Rounded Rectangle Element";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_ELEMENT_HUD_STRING            = "HUD String Element";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_FONT_FILE                     = "TrueType Font";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_GLOBAL_VARIABLE               = "Global Variable";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_INTEGER_FIXED                 = "Integer";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_INTEGER_TIMER                 = "Timer";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_LAYER_HUD                     = "Heads-Up Display";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_MODEL_ELEMENT                 = "Element Model";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_MODEL_SCRIPTABLE              = "Scriptable Model";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_MODEL_SPRITE                  = "Sprite Model";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_SCRIPT_LUA                    = "Lua Script";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_SOUND_FILE                    = "Sound File";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_TEXTURE_FILE                  = "Texture File";
const std::string IsoRealmsModule::NAME_RESOURCE_TYPE_VERTEX_FIXED                  = "Vertex Fixed";

bool IsoRealmsModule::cStaticInit = false;

IsoRealmsModule::IsoRealmsModule(IResourceTypeRegistry* resourceTypeRegistry) {
  if (!cStaticInit) {
    int mAudioRate = 44100;
    Uint16 mAudioFormat = AUDIO_S16SYS;
    int mAudioChannels = 2;
    int mAudioBuffers = 1024;
    if (Mix_OpenAudio(mAudioRate, mAudioFormat, mAudioChannels, mAudioBuffers) != 0) {
      std::cout << "Warning: Unable to initialize audio: " <<  Mix_GetError() << std::endl;
    } else {
      Mix_AllocateChannels(32);
    }
    cStaticInit = true;
  }
  
  if (resourceTypeRegistry != NULL) {
    resourceTypeRegistry->addResourceType(&cResourceTypeBooleanFixed,               NAME_RESOURCE_TYPE_BOOLEAN_FIXED);
    resourceTypeRegistry->addResourceType(&cResourceTypeBoundaries,                 NAME_RESOURCE_TYPE_BOUNDARIES);
    resourceTypeRegistry->addResourceType(&cResourceTypeColourFixed,                NAME_RESOURCE_TYPE_COLOUR_FIXED);
    resourceTypeRegistry->addResourceType(&cResourceTypeElementHUDModel,            NAME_RESOURCE_TYPE_ELEMENT_HUD_MODEL);
    resourceTypeRegistry->addResourceType(&cResourceTypeElementHUDRoundedRectangle, NAME_RESOURCE_TYPE_ELEMENT_HUD_ROUNDED_RECTANGLE);
    resourceTypeRegistry->addResourceType(&cResourceTypeElementHUDString,           NAME_RESOURCE_TYPE_ELEMENT_HUD_STRING);
    resourceTypeRegistry->addResourceType(&cResourceTypeFontFile,                   NAME_RESOURCE_TYPE_FONT_FILE);
//    resourceTypeRegistry->addResourceType(&cResourceTypeGlobalVariable,      NAME_RESOURCE_TYPE_GLOBAL_VARIABLE);
    resourceTypeRegistry->addResourceType(&cResourceTypeIntegerFixed,               NAME_RESOURCE_TYPE_INTEGER_FIXED);
    resourceTypeRegistry->addResourceType(&cResourceTypeIntegerTimer,               NAME_RESOURCE_TYPE_INTEGER_TIMER);
    resourceTypeRegistry->addResourceType(&cResourceTypeLayerHUD,                   NAME_RESOURCE_TYPE_LAYER_HUD);
    resourceTypeRegistry->addResourceType(&cResourceTypeModelElement,               NAME_RESOURCE_TYPE_MODEL_ELEMENT);
    resourceTypeRegistry->addResourceType(&cResourceTypeModelScriptable,            NAME_RESOURCE_TYPE_MODEL_SCRIPTABLE);
    resourceTypeRegistry->addResourceType(&cResourceTypeModelSprite,                NAME_RESOURCE_TYPE_MODEL_SPRITE);
    resourceTypeRegistry->addResourceType(&cResourceTypeScriptLua,                  NAME_RESOURCE_TYPE_SCRIPT_LUA);
    resourceTypeRegistry->addResourceType(&cResourceTypeSoundFile,                  NAME_RESOURCE_TYPE_SOUND_FILE);
    resourceTypeRegistry->addResourceType(&cResourceTypeTextureFile,                NAME_RESOURCE_TYPE_TEXTURE_FILE);
    resourceTypeRegistry->addResourceType(&cResourceTypeVertexFixed,                NAME_RESOURCE_TYPE_VERTEX_FIXED);
  }
}

void IsoRealmsModule::load(DOMNodeWrapper* node, IResourceRegistry* runtimeContext) {
  cRuntimeContext = runtimeContext;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if      (mValueAsString == TAG_RESOURCE_TYPE_BOOLEAN_FIXED)                 {cResourceTypeBooleanFixed.loadResource(              mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_BOUNDARIES)                    {cResourceTypeBoundaries.loadResource(                mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_COLOUR_FIXED)                  {cResourceTypeColourFixed.loadResource(               mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_ELEMENT_HUD_MODEL)             {cResourceTypeElementHUDModel.loadResource(           mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_ELEMENT_HUD_ROUNDED_RECTANGLE) {cResourceTypeElementHUDRoundedRectangle.loadResource(mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_ELEMENT_HUD_STRING)            {cResourceTypeElementHUDString.loadResource(          mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_FONT_FILE)                     {cResourceTypeFontFile.loadResource(                  mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_GLOBAL_VARIABLE)               {cResourceTypeGlobalVariable.loadResource(            mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_INTEGER_FIXED)                 {cResourceTypeIntegerFixed.loadResource(              mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_INTEGER_TIMER)                 {cResourceTypeIntegerTimer.loadResource(              mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_LAYER_HUD)                     {cResourceTypeLayerHUD.loadResource(                  mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_MODEL_ELEMENT)                 {cResourceTypeModelElement.loadResource(              mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_MODEL_SCRIPTABLE)              {cResourceTypeModelScriptable.loadResource(           mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_MODEL_SPRITE)                  {cResourceTypeModelSprite.loadResource(               mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_SCRIPT_LUA)                    {cResourceTypeScriptLua.loadResource(                 mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_SOUND_FILE)                    {cResourceTypeSoundFile.loadResource(                 mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_TEXTURE_FILE)                  {cResourceTypeTextureFile.loadResource(               mNode, runtimeContext);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_VERTEX_FIXED)                  {cResourceTypeVertexFixed.loadResource(               mNode, runtimeContext);}
    else                                                                {/* TODO: Throw */}
  }
}

void IsoRealmsModule::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  cResourceTypeBooleanFixed.saveResources(              node, resourceLocator, TAG_RESOURCE_TYPE_BOOLEAN_FIXED);
  cResourceTypeBoundaries.saveResources(                node, resourceLocator, TAG_RESOURCE_TYPE_BOUNDARIES);
  cResourceTypeColourFixed.saveResources(               node, resourceLocator, TAG_RESOURCE_TYPE_COLOUR_FIXED);
  cResourceTypeFontFile.saveResources(                  node, resourceLocator, TAG_RESOURCE_TYPE_FONT_FILE);
  cResourceTypeGlobalVariable.saveResources(            node, resourceLocator, TAG_RESOURCE_TYPE_GLOBAL_VARIABLE);
  cResourceTypeElementHUDModel.saveResources(           node, resourceLocator, TAG_RESOURCE_TYPE_ELEMENT_HUD_MODEL);
  cResourceTypeElementHUDRoundedRectangle.saveResources(node, resourceLocator, TAG_RESOURCE_TYPE_ELEMENT_HUD_ROUNDED_RECTANGLE);
  cResourceTypeElementHUDString.saveResources(          node, resourceLocator, TAG_RESOURCE_TYPE_ELEMENT_HUD_STRING);
  cResourceTypeIntegerFixed.saveResources(              node, resourceLocator, TAG_RESOURCE_TYPE_INTEGER_FIXED);
  cResourceTypeIntegerTimer.saveResources(              node, resourceLocator, TAG_RESOURCE_TYPE_INTEGER_TIMER);
  cResourceTypeLayerHUD.saveResources(                  node, resourceLocator, TAG_RESOURCE_TYPE_LAYER_HUD);
  cResourceTypeModelElement.saveResources(              node, resourceLocator, TAG_RESOURCE_TYPE_MODEL_ELEMENT);
  cResourceTypeModelScriptable.saveResources(           node, resourceLocator, TAG_RESOURCE_TYPE_MODEL_SCRIPTABLE);
  cResourceTypeModelSprite.saveResources(               node, resourceLocator, TAG_RESOURCE_TYPE_MODEL_SPRITE);
  cResourceTypeScriptLua.saveResources(                 node, resourceLocator, TAG_RESOURCE_TYPE_SCRIPT_LUA);
  cResourceTypeSoundFile.saveResources(                 node, resourceLocator, TAG_RESOURCE_TYPE_SOUND_FILE);
  cResourceTypeTextureFile.saveResources(               node, resourceLocator, TAG_RESOURCE_TYPE_TEXTURE_FILE);
  cResourceTypeVertexFixed.saveResources(               node, resourceLocator, TAG_RESOURCE_TYPE_VERTEX_FIXED);
}

extern "C" IModule* create(IResourceTypeRegistry* resourceTypeRegistry) {
  return new IsoRealmsModule(resourceTypeRegistry);
}

extern "C" void destroy(IModule* module) {
  delete module;
}
