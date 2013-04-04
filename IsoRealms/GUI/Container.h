#ifndef CONTAINER_H
#define CONTAINER_H

#include "IComponentHolder.h"
#include "ISizedComponent.h"

class Container:public ISizedComponent,
                public IComponentHolder {
  private:
  ISizedComponent* cComponent;

  class ContainerBounds:public IComponentBoundsCalculator {
    private:
    Container* cParent;
    
    public:
    ContainerBounds(Container*);
    
    float getLeft();
    float getRight();
    float getTop();
    float getBottom();
  };
  
  public:
  Container();
    
  void render();
  void update(unsigned int);
  bool input(SDL_Event&);

  float getWidth();
  float getHeight();
  
  void addComponent(const std::string&, ISizedComponent*);
  void removeComponent(ISizedComponent*);
};

#endif
