#ifndef THEME_H
#define THEME_H

#include <map>

#include <IsoRealms/Resources/IResourceUseListener.h>
#include <IsoRealms/Resources/Texture/ITexture.h>

#include "ThemeTexture.h"
#include "IThemeSource.h"

class Theme:public IResource,
            public IResourceUseListener<ITexture> {
  private:
  IResourceAccessor* cResources;
  std::map<ThemeTexture*, ITexture*> cTextures;
  IThemeSource* cThemeSource;

  public:
  Theme(IThemeSource*);
    
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  void save(DOMNodeWriter*, IResourceLocator*);
    
  void registerElement(ThemeTexture*, ITexture*);
  void set();
  void removeTexture(ThemeTexture*);
  
  /*******************************\
   * Implements ITextureListener *
  \*******************************/
  void resourcePendingDestruction(ITexture*, ITexture*);
  void resourceChanged(ITexture*);
};

#endif
