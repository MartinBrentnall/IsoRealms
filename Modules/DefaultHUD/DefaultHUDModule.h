#ifndef DEFAULT_HUD_MODULE_H
#define DEFAULT_HUD_MODULE_H

#include <IsoRealms/IPlugin.h>
#include <IsoRealms/Resources/ElementType/IElementType.h>

#include "DefaultHUD.h"

class DefaultHUDModule:public IPlugin,
                       public IDefaultHUDType {
  private:
  IResourceAccessor* cResourceAccessor;
    
  public:
  /**********************\
   * Implements IPlugin *
  \**********************/
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);  
    
  /***************************\
   * Implements IElementType *
  \***************************/
  IPlugin* getElementSet();
  IElement* getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);
  void configureElement();
  void renderEditingPreview();
  void renderIcon();
  void updateIcon(unsigned int);
  void destroy(IElement*);
  IElementHandler* getElementHandler();
};

#endif
