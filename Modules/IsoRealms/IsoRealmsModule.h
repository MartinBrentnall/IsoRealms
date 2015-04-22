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
#include <IsoRealms/IModule.h>
#include <IsoRealms/Resources/Colour/Colour.h>
#include <IsoRealms/Resources/ResourceType.h>
#include <IsoRealms/Resources/Script/IGlobalVariable.h>
#include <IsoRealms/LuaSupport/LuaGlobalVariable.h>

#include "DialogBooleanFixed.h"
#include "DialogBoundaries.h"
#include "DialogColourFixed.h"
#include "DialogFontFile.h"
#include "DialogGlobalVariable.h"
#include "DialogHUDModel.h"
#include "DialogHUDRoundedRectangle.h"
#include "DialogHUDString.h"
#include "DialogIntegerFixed.h"
#include "DialogIntegerTimer.h"
#include "DialogLayerHUD.h"
#include "DialogModelElement.h"
#include "DialogModelScriptable.h"
#include "DialogModelSprite.h"
#include "DialogScriptLua.h"
#include "DialogStringTime.h"
#include "DialogSoundFile.h"
#include "DialogTextureFile.h"
#include "DialogVertexFixed.h"
#include "ResourceBooleanFixed.h"
#include "ResourceBoundaries.h"
#include "ResourceFontFile.h"
#include "ResourceHUDModel.h"
#include "ResourceHUDRoundedRectangle.h"
#include "ResourceHUDString.h"
#include "ResourceIntegerFixed.h"
#include "ResourceIntegerTimer.h"
#include "ResourceLayerHUD.h"
#include "ResourceModelElement.h"
#include "ResourceModelScriptable.h"
#include "ResourceModelSprite.h"
#include "ResourceStringTime.h"
#include "ResourceSoundFile.h"
#include "ResourceTextureFile.h"
#include "ResourceVertexFixed.h"

class IsoRealmsModule:public IModule {
  private:
  static const std::string TAG_RESOURCE_TYPE_BOOLEAN_FIXED;
  static const std::string TAG_RESOURCE_TYPE_BOUNDARIES;
  static const std::string TAG_RESOURCE_TYPE_COLOUR_FIXED;
  static const std::string TAG_RESOURCE_TYPE_FONT_FILE;
  static const std::string TAG_RESOURCE_TYPE_GEOMETRY_PROCESSOR;
  static const std::string TAG_RESOURCE_TYPE_GLOBAL_VARIABLE;
  static const std::string TAG_RESOURCE_TYPE_HUD_MODEL;
  static const std::string TAG_RESOURCE_TYPE_HUD_ROUNDED_RECTANGLE;
  static const std::string TAG_RESOURCE_TYPE_HUD_STRING;
  static const std::string TAG_RESOURCE_TYPE_INTEGER_FIXED;
  static const std::string TAG_RESOURCE_TYPE_INTEGER_TIMER;
  static const std::string TAG_RESOURCE_TYPE_LAYER_HUD;
  static const std::string TAG_RESOURCE_TYPE_MODEL_ELEMENT;
  static const std::string TAG_RESOURCE_TYPE_MODEL_SCRIPTABLE;
  static const std::string TAG_RESOURCE_TYPE_MODEL_SPRITE;
  static const std::string TAG_RESOURCE_TYPE_SCRIPT_LUA;
  static const std::string TAG_RESOURCE_TYPE_SOUND_FILE;
  static const std::string TAG_RESOURCE_TYPE_STRING_TIME;
  static const std::string TAG_RESOURCE_TYPE_TEXTURE_FILE;
  static const std::string TAG_RESOURCE_TYPE_VERTEX_FIXED;

  static const std::string NAME_RESOURCE_TYPE_BOOLEAN_FIXED;
  static const std::string NAME_RESOURCE_TYPE_BOUNDARIES;
  static const std::string NAME_RESOURCE_TYPE_COLOUR_FIXED;
  static const std::string NAME_RESOURCE_TYPE_FONT_FILE;
  static const std::string NAME_RESOURCE_TYPE_GEOMETRY_PROCESSOR;
  static const std::string NAME_RESOURCE_TYPE_GLOBAL_VARIABLE;
  static const std::string NAME_RESOURCE_TYPE_HUD_MODEL;
  static const std::string NAME_RESOURCE_TYPE_HUD_ROUNDED_RECTANGLE;
  static const std::string NAME_RESOURCE_TYPE_HUD_STRING;
  static const std::string NAME_RESOURCE_TYPE_INTEGER_FIXED;
  static const std::string NAME_RESOURCE_TYPE_INTEGER_TIMER;
  static const std::string NAME_RESOURCE_TYPE_LAYER_HUD;
  static const std::string NAME_RESOURCE_TYPE_MODEL_ELEMENT;
  static const std::string NAME_RESOURCE_TYPE_MODEL_SCRIPTABLE;
  static const std::string NAME_RESOURCE_TYPE_MODEL_SPRITE;
  static const std::string NAME_RESOURCE_TYPE_SCRIPT_LUA;
  static const std::string NAME_RESOURCE_TYPE_SOUND_FILE;
  static const std::string NAME_RESOURCE_TYPE_STRING_TIME;
  static const std::string NAME_RESOURCE_TYPE_TEXTURE_FILE;
  static const std::string NAME_RESOURCE_TYPE_VERTEX_FIXED;
  
  static bool cStaticInit;
  
  ResourceType<IBoolean,             ResourceBooleanFixed,        DialogBooleanFixed>        cResourceTypeBooleanFixed;
  ResourceType<IBoundaries,          ResourceBoundaries,          DialogBoundaries>          cResourceTypeBoundaries;
  ResourceType<IColour,              Colour,                      DialogColourFixed>         cResourceTypeColourFixed;
  ResourceType<IFont,                ResourceFontFile,            DialogFontFile>            cResourceTypeFontFile;
  ResourceType<IGlobalVariable,      LuaGlobalVariable,           DialogGlobalVariable>      cResourceTypeGlobalVariable;
  ResourceType<IHUDComponentType,    ResourceHUDModel,            DialogHUDModel>            cResourceTypeHUDModel;
  ResourceType<IHUDComponentType,    ResourceHUDRoundedRectangle, DialogHUDRoundedRectangle> cResourceTypeHUDRoundedRectangle;
  ResourceType<IHUDComponentType,    ResourceHUDString,           DialogHUDString>           cResourceTypeHUDString;
  ResourceType<IInteger,             ResourceIntegerFixed,        DialogIntegerFixed>        cResourceTypeIntegerFixed;
  ResourceType<IInteger,             ResourceIntegerTimer,        DialogIntegerTimer>        cResourceTypeIntegerTimer;
  ResourceType<ILayerType,           ResourceLayerHUD,            DialogLayerHUD>            cResourceTypeLayerHUD;
  ResourceType<I3DModelType,         ResourceModelElement,        DialogModelElement>        cResourceTypeModelElement;
  ResourceType<I3DModelType,         ResourceModelScriptable,     DialogModelScriptable>     cResourceTypeModelScriptable;
  ResourceType<I3DModelType,         ResourceModelSprite,         DialogModelSprite>         cResourceTypeModelSprite;
  ResourceType<IScript,              LuaScript,                   DialogScriptLua>           cResourceTypeScriptLua;
  ResourceType<ISound,               ResourceSoundFile,           DialogSoundFile>           cResourceTypeSoundFile;
  ResourceType<IString,              ResourceStringTime,          DialogStringTime>          cResourceTypeStringTime;
  ResourceType<ITexture,             ResourceTextureFile,         DialogTextureFile>         cResourceTypeTextureFile;
  ResourceType<IVertex,              ResourceVertexFixed,         DialogVertexFixed>         cResourceTypeVertexFixed;
    
  IResourceRegistry* cRuntimeContext;
  IComponentContainer* cComponentContainer;

  public:
  IsoRealmsModule(IResourceTypeRegistry*);
    
  /**********************\
   * Implements IPlugin *
  \**********************/
  void load(DOMNodeWrapper*, IResourceRegistry*);
  void save(DOMNodeWriter*, IResourceLocator*);
};
