#ifndef I_MENU_STACK_H
#define I_MENU_STACK_H

class FrontEndMenu;

class IMenuStack {
  public:
  virtual void push(FrontEndMenu*) = 0;
};

#endif
