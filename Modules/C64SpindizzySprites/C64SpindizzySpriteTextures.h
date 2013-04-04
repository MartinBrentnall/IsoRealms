#ifndef C64_SPINDIZZY_SPRITE_TEXTURES_H
#define C64_SPINDIZZY_SPRITE_TEXTURES_H

#include <IsoRealms/IPlugin.h>
#include <IsoRealms/Resources/IRuntimeContext.h>

#include "C64SpindizzySpriteSet.h"
#include "DialogSpindizzySprites.h"

class C64SpindizzySpriteTextures:public IPlugin,
                                 public IResourceType<ITexture>,
				 public IConfirmationListener {
  private:
  IRuntimeContext* cRuntimeContext;
  IResourceSelector* cResourceSelector;
  std::vector<C64SpindizzySpriteSet*> cSpriteSets;
  std::vector<DialogSpindizzySprites*> cCreateDialogs;
  IComponentContainer* cComponentContainer;
  
  
  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  void save(DOMNodeWriter*, IResourceLocator*);
  void setEditingContext(IEditingContext*, IResourceManager*);
  
  /***********************************\
   * Implements ITextureResourceType *
  \***********************************/
  void createResource(IResourceAccessor*);
  void editResource(ITexture*, IResourceAccessor*);
  void removeResource(ITexture*, IResourceAccessor*, IResourceInstanceListener<ITexture>*);
  
  /************************************\
   * Implements IConfirmationListener *
  \************************************/
  void dialogConfirmed(IRectangularComponent*);
  void dialogCancelled(IRectangularComponent*);
};

#endif
