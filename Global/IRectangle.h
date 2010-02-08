#ifndef I_RECTANGLE_H
#define I_RECTANGLE_H

class IRectangle {
  public:

  /**
   * Get the location of the left edge of the rectangle.
   */
  virtual float getLeft() = 0;

  /**
   * Get the location of the right edge of the rectangle.
   */
  virtual float getRight() = 0;

  /**
   * Get the location of the top edge of the rectangle.
   */
  virtual float getTop() = 0;

  /**
   * Get the location of the bottom edge of the rectangle.
   */
  virtual float getBottom() = 0;
};

#endif
