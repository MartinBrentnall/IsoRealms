#ifndef COLOUR_PROXY_H
#define COLOUR_PROXY_H

#include "IColour.h"

class ColourProxy:public IColour {
  private:
  IColour* cColour;
  
  public:

  void setImplementation(IColour*);
    
  /**********************\
   * Implements IColour *
  \**********************/
  float getRed() const;
  float getGreen() const;
  float getBlue() const;
  float getAlpha() const;
  int getIntRed() const;
  int getIntGreen() const;
  int getIntBlue() const;
  int getIntAlpha() const;
  void save(DOMNodeWriter*, const std::string&);
};

#endif
