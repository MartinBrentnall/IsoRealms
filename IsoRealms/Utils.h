/*
 * Copyright 2023 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <cmath>
#include <iomanip>
#include <iostream>
#include <GL/glew.h>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "IsoRealms/Common/IVisualElement.h"
#include "IsoRealms/Exception/ArgumentException.h"
#include "IsoRealms/Input/ButtonMapping.h"
#include "IsoRealms/Input/HatMapping.h"
#include "IsoRealms/Input/IDigitalInputMapping.h"
#include "IsoRealms/Input/KeyMapping.h"
#include "IsoRealms/Types.h"

namespace IsoRealms {
  class IColour;
  class LiteralVertex;

  class Utils {
    public:

    /**
    * Convert the specified integer value to a string.
    */
    static std::string toString(int);
    static std::string toString(unsigned int);

    /**
    * Convert the specified float value to a string.
    */
    static std::string toString(float);
    static std::string toString(double);
    
    static std::vector<std::string> split(const std::string&, char = '\n');  
    static std::vector<std::string> splitWords(const std::string&, char = ' ');
    static bool endsWith(const std::string&, const std::string&);
    
    static void renderVolumeCuboid(float, float, float, float, float, float);
    static void renderVolumeLines(float, float, float, float, float, float);
    static void renderVolumeMarkers(float, float, float, float, float, float, float);
    
    static void renderStaticVisuals(std::vector<std::unique_ptr<IVisualElement>>& visuals);

    static bool replaceTexture(ITexture*&, ITexture*, ITexture*);
    
    static float luminance(const IColour& colour);

    static float restrict(float low, float high, float val);
    static int restrict(int low, int high, int val);
    static float sine(float min, float max, float gradient);

    static void renderIconBranch();
    static void renderIconLeaf();
    static void renderIconCustom();
    static void renderIconNone();
    static void renderIconTick();
    
    static void renderCurve(float x, float y, float radius, float startAngle, float endAngle);
    static void renderCurveLine(float x, float y, float radius, float startAngle, float endAngle);
    static void renderRectangle(float left, float bottom, float right, float top);
    static void renderBar(float left, float bottom, float right, float top);
    static void renderRoundedRectangleLines(float left, float bottom, float right, float top, float curveSize);
    static void renderRoundedRectangle(float left, float bottom, float right, float top, float curveSize);

    template <class TYPE> static int removeElement(std::vector<TYPE>& vector, const TYPE& element) {
      int mRemoved = 0;
      for (std::size_t i = vector.size(); i > 0; i--) {
        if (vector[i - 1] == element) {
          vector.erase(vector.begin() + (i - 1));
          mRemoved++;
        }
      }
      return mRemoved;
    }
    
    template <class TYPE, class TYPEB> static std::unique_ptr<TYPE> removeElementUnique(std::vector<std::unique_ptr<TYPE>>& vector, const TYPEB* element) {
      for (std::size_t i = vector.size(); i > 0; i--) {
        if (vector[i - 1].get() == element) {
          std::unique_ptr<TYPE> mElement = std::move(vector[i - 1]);
          vector.erase(vector.begin() + (i - 1));
          return mElement;
        }
      }
      return nullptr;
    }

    template <class KEY, class VALUE> static int removeByValue(std::map<KEY, VALUE>& map, VALUE& value) {
      int mRemoved = 0;
      for (std::pair<const KEY, VALUE>& mPair : map) {
        if (mPair.second == value) {
          map.erase(mPair.first);
          mRemoved++;
        }
      }
      return mRemoved;
    }

    template <class KEY, class VALUE> static KEY reverseLookup(const std::map<KEY, VALUE>& map, const VALUE& value) {
      for (const std::pair<const KEY, VALUE>& mPair : map) {
        if (mPair.second == value) {
          return mPair.first;
        }
      }
      throw ArgumentException("ERROR: Utils::reverseLookup: Specified value not found in specified map");
    }

    static void calculateColour(float hue, float saturation, float lightness, float& red, float& green, float& blue);
    static float getMiddle(float a, float b, float c);
    static float getLightness(float red, float green, float blue);
    static float getSaturation(float red, float green, float blue);
    static float getHue(float red, float green, float blue);


    /**
      * Calculate the distance between two 2D points.
      *
      * @param double  X location of first point.
      * @param double  Y location of first point.
      * @param double  X location of second point.
      * @param double  Y location of second point.
      * @returns  Distance between two points.
      */
    static double distance(double, double, double, double);

    static double round(double value, double alignTo, double direction) {
      return direction == 0.0 ? std::round(value / alignTo) * alignTo
           : direction >  0.0 ? std::ceil( value / alignTo) * alignTo
           :                    std::floor(value / alignTo) * alignTo;
    }
    
    static std::unique_ptr<IDigitalInputMapping> toDigitalInputMapping(HatHandler& hatHandler, sf::Event& event) {
      switch (event.type) {
        case sf::Event::KeyPressed:            return std::make_unique<KeyMapping>(event.key.code);
        case sf::Event::JoystickButtonPressed: return std::make_unique<ButtonMapping>(event.joystickButton.button);
        case sf::Event::JoystickMoved: {
          if (event.joystickMove.axis == sf::Joystick::Axis::PovX) {
            if (event.joystickMove.position < 0) {
              return std::make_unique<HatMapping>(hatHandler, HatHandler::Direction::HAT_LEFT);
            } else if (event.joystickMove.position > 0) {
              return std::make_unique<HatMapping>(hatHandler, HatHandler::Direction::HAT_RIGHT);
            }
          } else if (event.joystickMove.axis == sf::Joystick::Axis::PovY) {
#if _WIN32
            if (event.joystickMove.position > 0) {
              return std::make_unique<HatMapping>(hatHandler, HatHandler::Direction::HAT_UP);
            } else if (event.joystickMove.position < 0) {
              return std::make_unique<HatMapping>(hatHandler, HatHandler::Direction::HAT_DOWN);
            }
#elif __linux__
            if (event.joystickMove.position < 0) {
              return std::make_unique<HatMapping>(hatHandler, HatHandler::Direction::HAT_UP);
            } else if (event.joystickMove.position > 0) {
              return std::make_unique<HatMapping>(hatHandler, HatHandler::Direction::HAT_DOWN);
            }
#endif
          }
          break;
        }
        default: break;
      }                               
      return nullptr;
    }
    
    static void shadowPrint(float x, float y, IFont& font, float fontSize, const IColour& colour, float shadowOffset, IFont::Alignment alignment, const std::string& text);

    static int nextPowerOfTwo(int value) {
      int mReturnValue = 1;
      while (mReturnValue < value) {
        mReturnValue <<= 1;
      }
      return mReturnValue;
    }

    /**
      * Render a triangle connected by the specified co-ordinates.
      *
      * @param Vertex  First Vertex of the triangle.
      * @param Vertex  Second Vertex of the triangle.
      * @param Vertex  Third Vertex of the triangle.
      */
    static void renderTriangle(LiteralVertex& a, LiteralVertex& b, LiteralVertex& c);

    /**
      * Render a triangle connected by the specified co-ordinates, and indented
      * inwards (towards the triangle's center) by the specified distance.
      *
      * @param Vertex  First Vertex of the triangle.
      * @param Vertex  Second Vertex of the triangle.
      * @param Vertex  Third Vertex of the triangle.
      * @param float  Distance of indentation.
      */
    static void renderInnerTriangle(LiteralVertex& a, LiteralVertex& b, LiteralVertex& c, float lineWidth);

    /**
      * Render a triangle outline connected by the specified co-ordinates of the
      * specified width, inwards (towards the triangle's center) from the points.
      *
      * @param a First Vertex of the triangle.
      * @param b Second Vertex of the triangle.
      * @param c Third Vertex of the triangle.
      * @param lineWidth Distance of indentation.
      */
    static void renderOuterTriangle(LiteralVertex& a, LiteralVertex& b, LiteralVertex& c, float lineWidth);

    /**
      * From three input vertices forming a triangle, calculate three output
      * vertices that form a new triangle, such that the lines of each triangle
      * are parallel and of the specified distance from each other.
      *
      * @param Vertex  First input triangle vertex.
      * @param Vertex  Second input triangle vertex.
      * @param Vertex  Third input triangle vertex.
      * @param Vertex&  First output triangle vertex.
      * @param Vertex&  Second output triangle vertex.
      * @param Vertex&  Third output triangle vertex.
      * @param float  Distance between lines of input and output triangle lines.
      */
    static void calculateTriangle(LiteralVertex, LiteralVertex, LiteralVertex, LiteralVertex&, LiteralVertex&, LiteralVertex&, float);

    static double getValue(LiteralVertex, LiteralVertex, LiteralVertex, LiteralVertex);
    
    private:
    static const float LUMINANCE_BLUE;
    static const float LUMINANCE_GREEN;
    static const float LUMINANCE_RED;
  };
}
