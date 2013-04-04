#ifndef I_MENU_POPUP_H
#define I_MENU_POPUP_H

class IMenuPopup {
  public:
  virtual void popupMenu(IMenuPopup*) = 0;
};

#endif
