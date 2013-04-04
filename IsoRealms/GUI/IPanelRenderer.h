#ifndef I_PANEL_RENDERER_H
#define I_PANEL_RENDERER_H

class PanelComponent;

class IPanelRenderer {
  public:
  virtual void render(PanelComponent*) = 0;
};

#endif
