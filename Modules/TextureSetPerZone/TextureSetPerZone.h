/*
 * Copyright 2009 Martin Brentnall
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
#ifndef TEXTURE_SET_PER_ZONE_H
#define TEXTURE_SET_PER_ZONE_H

#include <map>
#include <sstream>
#include <vector>

#include <IsoRealms/DefaultCommandInfo.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/IHUDComponent.h>
#include <IsoRealms/IPluginRegistry.h>
#include <IsoRealms/IZoneContextListener.h>
#include <IsoRealms/Map.h>
#include <IsoRealms/OpenDialogCommand.h>

#include "IThemeSource.h"
#include "TextureSetChooserComponent.h"
#include "Theme.h"
#include "ThemeTexture.h"

class TextureSetPerZone:public IPlugin,
                        public IZoneContextListener,
                        public IComponentSource,
                        public IThemeSource,
                        public IElementType,
			public IResourceType<ITexture> {
  private:
    
  // TODO: This sub-class was intended to set the theme to that of the current zone
  // immediately after all zones had been renderered, so the HUD elements would use
  // the current zone theme.
  // 
  // Maybe this should be replaced with a Lua-based mechanism.
  class DefaultTextureSetCommand:public IDynamicElement {
    private:
    TextureSetPerZone* cParent;
    
    public:
    DefaultTextureSetCommand(TextureSetPerZone*);
    
    /******************************\
     * Implements IDynamicElement *
    \******************************/
    void update(unsigned int);
  };

  std::map<IZone*, Theme*> cZoneThemes;
  std::map<std::string, Theme*> cThemes;
  std::map<std::string, ThemeTexture*> cTextures;
  DefaultTextureSetCommand* cDefaultTextureSetCommand;
  IMap* cCurrentMap;
  IZone* cCurrentZone;
  BlockLocation* cBlockLocation;
  IComponentContainer* cComponentContainer;

  void createThemeTexture(const std::string&, IRuntimeContext*);
  void loadTheme(DOMNodeWrapper*, Theme*);
  void createActualResources(DOMNodeWrapper*, IRuntimeContext*);
  
  public:
  TextureSetPerZone();
    
  void createResources(DOMNodeWrapper*, IRuntimeContext*);

  ThemeTexture* getThemeTexture(const std::string&);
  std::string getThemeElement(ThemeTexture*);
  std::string getThemeName(Theme*);

  /***********************************\
   * Implements IZoneContextListener *
  \***********************************/
  void zoneContextChanged(IZone*);

  /**********************\
   * Implements IPlugin *
  \**********************/
  void renderPreZone(IZone*);
  void zoneContextChanged(IMap*, IZone*);
  void setEditingContext(IEditingContext*);
  void saveData(DOMNodeWriter*, IMap*, IZone*);
  void loadData(DOMNodeWrapper*, IPluginRegistry*, IZone*);
  void save(DOMNodeWriter*, IResourceLocator*);
  std::vector<IDynamicElement*> getPreLoopCommands();
  std::vector<IDynamicElement*> getPostLoopCommands();
  
  /*******************************\
   * Implements IComponentSource *
  \*******************************/
  IHUDComponent* createComponent();
  
  /***************************\
   * Implements IElementType *
  \***************************/
  IPlugin* getElementSet();
  IElement* getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);
  void setEditingContext(IEditingContext*, IResourceManager*);
  void configureElement();
  bool input(SDL_Event&);
  void renderEditingPreview();
  void renderIcon();
  void updateIcon(int);
  void destroy(IElement*);
  IElementHandler* getElementHandler();
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  /***********************************\
   * Implements ITextureResourceType *
  \***********************************/
  void createResource(IResourceAccessor*);
  void editResource(ITexture*, IResourceAccessor*);
  void removeResource(ITexture*, IResourceAccessor*, IResourceInstanceListener<ITexture>*);
};

#endif
