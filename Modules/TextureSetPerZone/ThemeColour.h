#ifndef THEME_COLOUR_H
#define THEME_COLOUR_H

#include <IsoRealms/Resources/Colour/IColour.h>

class ThemeColour:public IColour {
  private:
  IColour* cColour;

  public:
  void set(IColour*);

  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);

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
  float luminance();
  void set();
  void setBackground();
};

#endif
