#ifndef I_SLIDER_RENDERER_H
#define I_SLIDER_RENDERER_H

class SliderComponent;

class ISliderRenderer {
  public:
  virtual void render(SliderComponent*) = 0;
};

#endif
