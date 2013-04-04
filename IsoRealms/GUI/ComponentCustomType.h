#ifndef COMPONENT_CUSTOM_TYPE
#define COMPONENT_CUSTOM_TYPE

#include "../IComponentCustomType.h"
#include "IRectangularComponent.h"

template <class T> class ComponentCustomType : public IComponentCustomType {
  public:
  AbstractRectangularComponent* createComponent(DOMNodeWrapper* node, IResourceAccessor* resources) {
    return new T(node, resources);
  }
};

#endif
