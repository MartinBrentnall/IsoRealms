#ifndef MULTIPLE_CLICK_DETECTOR_H
#define MULTIPLE_CLICK_DETECTOR_H

#include <iostream>
#include <SDL/SDL.h>

class MultipleClickDetector {
  private:
  static const unsigned int CLICK_INTERVAL_THRESHOLD;

  unsigned int cClicks;
  Uint8 cButton;
 
  unsigned int cTimeSinceLastClick;

  public:
  static const unsigned int SINGLE_CLICK;
  static const unsigned int DOUBLE_CLICK;
  static const unsigned int TRIPLE_CLICK;
    
  MultipleClickDetector();
  void input(SDL_Event&);
  void update(unsigned int);
  unsigned int getClicks();
};

#endif
