#ifndef I_COLOUR_H
#define I_COLOUR_H

#include <string>

#include <IsoRealms/Resources/IResource.h>

class IColour:public IResource {
  public:
  
  /**
   * Returns the cRed intensity of this colour.
   *
   * @returns  The cRed intensity of this colour.
   */
  virtual float getRed() const = 0;

  /**
   * Returns the green intensity of this colour.
   *
   * @returns  The green intensity of this colour.
   */
  virtual float getGreen() const = 0;

  /**
   * Returns the blue intensity of this colour.
   *
   * @returns  The blue intensity of this colour.
   */
  virtual float getBlue() const = 0;

  /**
   * Returns the alpha intensity of this colour.
   *
   * @returns  The alpha intensity of this colour.
   */
  virtual float getAlpha() const = 0;
  
  virtual int getIntRed() const = 0;
  virtual int getIntGreen() const = 0;
  virtual int getIntBlue() const = 0;
  virtual int getIntAlpha() const = 0;
  virtual float luminance() = 0;
  virtual void set() = 0;
  virtual void setBackground() = 0;
};

#endif
