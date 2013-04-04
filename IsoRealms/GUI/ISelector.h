#ifndef I_SELECTOR_H
#define I_SELECTOR_H

class SelectableComponent;

class ISelector {
  public:
  virtual void render(SelectableComponent*) = 0;
  virtual void selected() = 0;
  virtual void deselected() = 0;
};

#endif
