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
#include <IsoRealms/Resources/ResourceTypeSet.h>
#include <IsoRealms/Resources/Script/IGlobalVariable.h>
#include <IsoRealms/LuaSupport/LuaGlobalVariable.h>

#include "DialogBooleanFixed.h"
#include "DialogColourFixed.h"
#include "DialogElementHUDModel.h"
#include "DialogElementHUDRoundedRectangle.h"
#include "DialogElementHUDString.h"
#include "DialogFontFile.h"
#include "DialogGlobalVariable.h"
#include "DialogIntegerFixed.h"
#include "DialogIntegerTimer.h"
#include "DialogLayerHUD.h"
#include "DialogModelElement.h"
#include "DialogModelScriptable.h"
#include "DialogModelSetCycleable.h"
#include "DialogModelSprite.h"
#include "DialogScriptLua.h"
#include "DialogSoundFile.h"
#include "DialogStringFixed.h"
#include "DialogTextureFile.h"
#include "DialogVertexFixed.h"
#include "ResourceBooleanFixed.h"
#include "ResourceElementHUDModel.h"
#include "ResourceElementHUDRoundedRectangle.h"
#include "ResourceElementHUDString.h"
#include "ResourceFontFile.h"
#include "ResourceIntegerFixed.h"
#include "ResourceIntegerTimer.h"
#include "ResourceLayerHUD.h"
#include "ResourceModelElement.h"
#include "ResourceModelScriptable.h"
#include "ResourceModelSetCycleable.h"
#include "ResourceModelSprite.h"
#include "ResourceSoundFile.h"
#include "ResourceStringFixed.h"
#include "ResourceTextureFile.h"
#include "ResourceVertexFixed.h"

class IsoRealmsModule:public IModule,
                      public IResource {
  private:
  static const std::string TAG_RESOURCE_TYPE_BOOLEAN_FIXED;
  static const std::string TAG_RESOURCE_TYPE_COLOUR_FIXED;
  static const std::string TAG_RESOURCE_TYPE_ELEMENT_HUD_MODEL;
  static const std::string TAG_RESOURCE_TYPE_ELEMENT_HUD_ROUNDED_RECTANGLE;
  static const std::string TAG_RESOURCE_TYPE_ELEMENT_HUD_STRING;
  static const std::string TAG_RESOURCE_TYPE_FONT_FILE;
  static const std::string TAG_RESOURCE_TYPE_GLOBAL_VARIABLE;
  static const std::string TAG_RESOURCE_TYPE_INTEGER_FIXED;
  static const std::string TAG_RESOURCE_TYPE_INTEGER_TIMER;
  static const std::string TAG_RESOURCE_TYPE_LAYER_HUD;
  static const std::string TAG_RESOURCE_TYPE_MODEL_ELEMENT;
  static const std::string TAG_RESOURCE_TYPE_MODEL_SCRIPTABLE;
  static const std::string TAG_RESOURCE_TYPE_MODEL_SET_CYCLEABLE;
  static const std::string TAG_RESOURCE_TYPE_MODEL_SPRITE;
  static const std::string TAG_RESOURCE_TYPE_SCRIPT_LUA;
  static const std::string TAG_RESOURCE_TYPE_SOUND_FILE;
  static const std::string TAG_RESOURCE_TYPE_STRING_FIXED;
  static const std::string TAG_RESOURCE_TYPE_TEXTURE_FILE;
  static const std::string TAG_RESOURCE_TYPE_VERTEX_FIXED;

  static const std::string NAME_RESOURCE_TYPE_BOOLEAN_FIXED;
  static const std::string NAME_RESOURCE_TYPE_COLOUR_FIXED;
  static const std::string NAME_RESOURCE_TYPE_ELEMENT_HUD_MODEL;
  static const std::string NAME_RESOURCE_TYPE_ELEMENT_HUD_ROUNDED_RECTANGLE;
  static const std::string NAME_RESOURCE_TYPE_ELEMENT_HUD_STRING;
  static const std::string NAME_RESOURCE_TYPE_FONT_FILE;
  static const std::string NAME_RESOURCE_TYPE_GLOBAL_VARIABLE;
  static const std::string NAME_RESOURCE_TYPE_INTEGER_FIXED;
  static const std::string NAME_RESOURCE_TYPE_INTEGER_TIMER;
  static const std::string NAME_RESOURCE_TYPE_LAYER_HUD;
  static const std::string NAME_RESOURCE_TYPE_MODEL_ELEMENT;
  static const std::string NAME_RESOURCE_TYPE_MODEL_SCRIPTABLE;
  static const std::string NAME_RESOURCE_TYPE_MODEL_SET_CYCLEABLE;
  static const std::string NAME_RESOURCE_TYPE_MODEL_SPRITE;
  static const std::string NAME_RESOURCE_TYPE_SCRIPT_LUA;
  static const std::string NAME_RESOURCE_TYPE_SOUND_FILE;
  static const std::string NAME_RESOURCE_TYPE_STRING_FIXED;
  static const std::string NAME_RESOURCE_TYPE_TEXTURE_FILE;
  static const std::string NAME_RESOURCE_TYPE_VERTEX_FIXED;
  
  static bool cStaticInit;
  
  ResourceType<IBoolean,             ResourceBooleanFixed,               DialogBooleanFixed>               cResourceTypeBooleanFixed;
  ResourceType<IColour,              Colour,                             DialogColourFixed>                cResourceTypeColourFixed;
  ResourceType<IElementType,         ResourceElementHUDModel,            DialogElementHUDModel>            cResourceTypeElementHUDModel;
  ResourceType<IElementType,         ResourceElementHUDRoundedRectangle, DialogElementHUDRoundedRectangle> cResourceTypeElementHUDRoundedRectangle;
  ResourceType<IElementType,         ResourceElementHUDString,           DialogElementHUDString>           cResourceTypeElementHUDString;
  ResourceType<IFont,                ResourceFontFile,                   DialogFontFile>                   cResourceTypeFontFile;
  ResourceType<IGlobalVariable,      LuaGlobalVariable,                  DialogGlobalVariable>             cResourceTypeGlobalVariable;
  ResourceType<IInteger,             ResourceIntegerFixed,               DialogIntegerFixed>               cResourceTypeIntegerFixed;
  ResourceType<IInteger,             ResourceIntegerTimer,               DialogIntegerTimer>               cResourceTypeIntegerTimer;
  ResourceType<ILayerType,           ResourceLayerHUD,                   DialogLayerHUD>                   cResourceTypeLayerHUD;
  ResourceType<I3DModelType,         ResourceModelElement,               DialogModelElement>               cResourceTypeModelElement;
  ResourceType<I3DModelType,         ResourceModelScriptable,            DialogModelScriptable>            cResourceTypeModelScriptable;
  ResourceTypeSet<I3DModelType,      ResourceModelSetCycleable,          DialogModelSetCycleable>          cResourceTypeModelSetCycleable;
  ResourceType<I3DModelType,         ResourceModelSprite,                DialogModelSprite>                cResourceTypeModelSprite;
  ResourceType<IScript,              LuaScript,                          DialogScriptLua>                  cResourceTypeScriptLua;
  ResourceType<ISound,               ResourceSoundFile,                  DialogSoundFile>                  cResourceTypeSoundFile;
  ResourceType<IString,              ResourceStringFixed,                DialogStringFixed>                cResourceTypeStringFixed;
  ResourceType<ITexture,             ResourceTextureFile,                DialogTextureFile>                cResourceTypeTextureFile;
  ResourceType<IVertex,              ResourceVertexFixed,                DialogVertexFixed>                cResourceTypeVertexFixed;
    
  IResourceRegistry* cRuntimeContext;
  IComponentContainer* cComponentContainer;
  IInteger* cLocks;

  public:
  IsoRealmsModule(IResourceTypeRegistry*);

  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  /**********************\
   * Implements IPlugin *
  \**********************/
  void load(DOMNodeWrapper*, IResourceRegistry*, DOMNodeWrapper*);
  void save(DOMNodeWriter*, IResourceLocator*);
};
