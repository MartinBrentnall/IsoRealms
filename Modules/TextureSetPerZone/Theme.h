#ifndef THEME_H
#define THEME_H

#include <map>

#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/IResourceUseListener.h>
#include <IsoRealms/Resources/Colour/IColour.h>
#include <IsoRealms/Resources/Texture/ITexture.h>

#include "ThemeColour.h"
#include "ThemeTexture.h"
#include "IThemeSource.h"

class Theme:public IResource,
            public IResourceUseListener<ITexture>,
	    public IResourceUseListener<IColour> {
  private:
  IResourceAccessor* cResources;
  std::map<ThemeTexture*, ITexture*> cTextures;
  std::map<ThemeColour*, IColour*> cColours;
  IThemeSource* cThemeSource;

  public:
  Theme(IThemeSource*);
    
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  void save(DOMNodeWriter*, IResourceLocator*);
    
  void registerElement(ThemeTexture*, ITexture*);
  void registerElement(ThemeColour*, IColour*);
  void set();
  void removeTexture(ThemeTexture*);
  void removeColour(ThemeColour*);
  IColour* getColour(ThemeColour*);  
  
  /*********************************************\
   * Implements IResourceUseListener<ITexture> *
  \*********************************************/
  void resourcePendingDestruction(ITexture*, ITexture*);
  void resourceChanged(ITexture*);

  /********************************************\
   * Implements IResourceUseListener<IColour> *
  \********************************************/
  void resourcePendingDestruction(IColour*, IColour*);
  void resourceChanged(IColour*);
};

#endif
