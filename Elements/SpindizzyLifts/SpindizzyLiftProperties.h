#ifndef SPINDIZZY_LIFT_PROPERTIES_H
#define SPINDIZZY_LIFT_PROPERTIES_H

#include "../../Global/DOMNodeWrapper.h"

class SpindizzyLiftProperties {
  private:
  unsigned int cTopDelay;
  unsigned int cBottomDelay;
  unsigned int cUpSpeed;
  unsigned int cDownSpeed;

  public:
  SpindizzyLiftProperties();

  unsigned int getTopDelay();
  unsigned int getBottomDelay();
  unsigned int getUpSpeed();
  unsigned int getDownSpeed();

  void setTopDelay(unsigned int);
  void setBottomDelay(unsigned int);
  void setUpSpeed(unsigned int);
  void setDownSpeed(unsigned int);

  void setup(DOMNodeWrapper*);

  void reset();
};

#endif
