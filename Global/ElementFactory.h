#ifndef ELEMENT_FACTORY_H
#define ELEMENT_FACTORY_H

#include "IElementFactory.h"

template<class T = IElementSet> class ElementFactory:public IElementFactory {
  private:
  T* cElementSet;

  public:
  ElementFactory(T* elementSet) {
    cElementSet = elementSet;
  }

  /******************************\
   * Implements IElementFactory *
  \******************************/
  T* getElementSet() {
    return cElementSet;
  }
};

#endif
