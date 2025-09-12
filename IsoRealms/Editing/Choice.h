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

#include <GL/glew.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "IsoRealms/Common/AnimatedFloat.h"

namespace IsoRealms {
  class IUIStyle;
  enum class UISignalID;

  class Choice {
    public:
    Choice(IUIStyle& style, const std::string& message, const std::vector<std::string>& options, std::function<bool(const std::string&)> confirmationCallback);
    void render(IUIStyle& style);
    bool update(unsigned int milliseconds);
    bool input(UISignalID id);
    
    private:
    class Option {
      public:
      Option(const std::string& text, float left, float right);
      
      std::string getText() const;
      float getLeft() const;
      float getRight() const;
      void render(IUIStyle& style) const;
      
      private:
      std::string cText;
      float cLeft;
      float cRight;
    };
    
    // Definition data.
    std::string cMessage;
    std::vector<std::unique_ptr<Option>> cOptions;
    std::function<bool(const std::string&)> cConfirmationCallback;
    float cBoxWidth;

    // Runtime data.
    int cSelected;

    // Nice animation.
    AnimatedFloat cSelectionHighlightLeft;
    AnimatedFloat cSelectionHighlightRight;
    int cOpenness;
    bool cClosing;
    
    void updateAnimationValues();
  };
}
