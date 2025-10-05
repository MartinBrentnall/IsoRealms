/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
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

#include <SFML/Window/Event.hpp>

#include "IsoRealms/Assets/Type/IFont.h"
#include "IsoRealms/Exception/ArgumentException.h"

namespace IsoRealms {
  class HatHandler;
  class IDigitalInputMapping;
  class IColour;
  class ITexture;
  class IVisualElement;
  class LiteralVertex;

  class Utils {
    public:
    class Point2D {
      public:
      Point2D(float x, float y) :
                cX(x),
                cY(y) {
      }

      // Get a point in 3D space on a straight line from A to B.
      // Interpolation is from 0 (point A) to 1 (point B).
      Point2D midPoint(const Point2D& other, float weight) const {
        return Point2D(cX + (other.cX - cX) * weight, cY + (other.cY - cY) * weight);
      }

      float getX() const {
        return cX;
      }

      float getY() const {
        return cY;
      }

      private:
      float cX;
      float cY;
    };

    /**
    * Convert the specified integer value to a string.
    */
    static std::string toString(int);
    static std::string toString(unsigned int);
    static std::string toString(long);

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
    static void renderIconAdd();
    static void renderIconTick();
    static void renderIconTerminal();
    static void renderIconLock();
    
    static void renderRingSection(float x, float y, float innerRadius, float outerRadius, float startAngle, float endAngle);
    static void renderRing(float x, float y, float innerRadius, float outerRadius);
    static void renderCircle(float x, float y, float radius);
    static void renderOval(float x, float y, float width, float height, float startAngle, float endAngle);
    static void renderCurve(float x, float y, float radius, float startAngle, float endAngle);
    static void renderCurveLine(float x, float y, float radius, float startAngle, float endAngle);
    static void renderRectangle(float left, float bottom, float right, float top);
    static void renderBar(float left, float bottom, float right, float top);
    static void renderRoundedRectangleLines(float left, float bottom, float right, float top, float curveSize);
    static void renderRoundedRectangle(float left, float bottom, float right, float top, float curveSize);
    static void renderBezier(const Point2D& start, const Point2D& controlA, const Point2D& controlB, const Point2D& end, int resolution);

    template <typename TYPE, typename TYPEB> static int getIndex(std::vector<std::unique_ptr<TYPE>>& vector, const TYPEB* element) {
      for (std::size_t i = 0; i < vector.size(); i++) {
        if (vector[i].get() == element) {
          return i;
        }
      }
      return -1;
    }

    template <typename TYPE> static int removeElement(std::vector<TYPE>& vector, const TYPE& element) {
      int mRemoved = 0;
      for (std::size_t i = vector.size(); i > 0; i--) {
        if (vector[i - 1] == element) {
          vector.erase(vector.begin() + (i - 1));
          mRemoved++;
        }
      }
      return mRemoved;
    }
    
    template <typename TYPE, typename TYPEB> static std::unique_ptr<TYPE> removeElementUnique(std::vector<std::unique_ptr<TYPE>>& vector, const TYPEB* element) {
      for (std::size_t i = vector.size(); i > 0; i--) {
        if (vector[i - 1].get() == element) {
          std::unique_ptr<TYPE> mElement = std::move(vector[i - 1]);
          vector.erase(vector.begin() + (i - 1));
          return mElement;
        }
      }
      return nullptr;
    }

    template <typename KEY, typename VALUE> static int removeByValue(std::map<KEY, VALUE>& map, VALUE& value) {
      int mRemoved = 0;
      for (std::pair<const KEY, VALUE>& mPair : map) {
        if (mPair.second == value) {
          map.erase(mPair.first);
          mRemoved++;
        }
      }
      return mRemoved;
    }

    template <typename KEY, typename VALUE> static KEY reverseLookupUnique(const std::map<KEY, std::unique_ptr<VALUE>>& map, const VALUE& value) {
      for (const std::pair<const KEY, std::unique_ptr<VALUE>>& mPair : map) {
        if (mPair.second.get() == &value) {
          return mPair.first;
        }
      }
      throw ArgumentException("ERROR: Utils::reverseLookup: Specified value not found in specified map");
    }

    template <typename KEY, typename VALUE> static KEY reverseLookup(const std::map<KEY, VALUE>& map, const VALUE& value) {
      for (const std::pair<const KEY, VALUE>& mPair : map) {
        if (mPair.second == value) {
          return mPair.first;
        }
      }
      throw ArgumentException("ERROR: Utils::reverseLookup: Specified value not found in specified map");
    }

    template <typename MAP> static std::string getAvailableKey(MAP& map, const std::string proposedName) {
      std::string mProposedName = proposedName;
      int mCount = 1;
      while (map.find(mProposedName) != map.end()) {
        mProposedName = proposedName + " " + toString(mCount++);
      }
      return mProposedName;
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
    static double distance(double x1, double y1, double x2, double y2);

    static double round(double value, double alignTo, double direction) {
      return direction == 0.0 ? std::round(value / alignTo) * alignTo
           : direction >  0.0 ? std::ceil( value / alignTo) * alignTo
           :                    std::floor(value / alignTo) * alignTo;
    }
    
    static long round(double value, long alignTo, long direction) {
      return direction == 0 ? std::round(value / static_cast<double>(alignTo)) * alignTo
           : direction >  0 ? std::ceil( value / static_cast<double>(alignTo)) * alignTo
           :                  std::floor(value / static_cast<double>(alignTo)) * alignTo;
    }

    static std::unique_ptr<IDigitalInputMapping> toDigitalInputMapping(HatHandler& hatHandler, sf::Event& event);
    
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
