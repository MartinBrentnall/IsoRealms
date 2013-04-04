#ifndef ELEMENT_TYPE_ICON_H
#define ELEMENT_TYPE_ICON_H

#include <IsoRealms/Resources/ElementType/IElementType.h>

#include "Icon.h"

class ElementTypeIcon:public Icon<IElementType> {
  private:
  IElementType* cElementType;
  
  public:
  ElementTypeIcon(IResourceBrowser<IElementType>*, IElementType*);
    
  void renderIcon();  
  void updateIcon(unsigned int);
  float getWidth();
  float getHeight();
};

#endif
