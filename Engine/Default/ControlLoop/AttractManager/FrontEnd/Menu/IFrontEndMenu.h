#ifndef I_FRONT_END_MENU_H
#define I_FRONT_END_MENU_H

class IFrontEndMenu {
  public:
  virtual bool input(SDL_Event&) = 0;
  virtual void render(float, IFont*) = 0;
  virtual void update(unsigned int) = 0;
};

#endif
