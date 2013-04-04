#ifndef I_ELEMENT_HANDLER_H
#define I_ELEMENT_HANDLER_H

class IElementHandler {
  public:
  virtual bool isRenderer() = 0;
  virtual void render() = 0;
  virtual bool isUpdater() = 0;
  virtual void update(int ticks) = 0;
  virtual void updateRuntime(int ticks) = 0;
};

#endif
