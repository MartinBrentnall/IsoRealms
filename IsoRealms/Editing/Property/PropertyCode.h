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

#include <limits>
#include <functional>
#include <stack>
#include <string>

#include <GL/glew.h>

#include "IsoRealms/AnimatedFloat.h"
#include "IsoRealms/Editing/Choice.h"

#include "IPropertyEditor.h"
#include "Property.h"

namespace IsoRealms {
  class IProject;

  class PropertyCode : public Property {
    public:
    PropertyCode(IProject& project, const std::string& name, const std::string& tooltip, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<void()> removeFunction = nullptr);

    /************************\
     * Implements IProperty *
    \************************/
    void renderValue(IUIStyle& style, float y, float x, float aspectRatio) const override;
    float getValueWidth(IUIStyle& style) const override;
    void confirm(IPropertyManager& manager, float y) override;
    bool hasConfiguration() const override;
    void configure(IPropertyManager& manager) override;

    private:
    class Editor : public IPropertyEditor {
      public:
      Editor(PropertyCode& parent);

      /******************************\
       * Implements IPropertyEditor *
      \******************************/
      void render(IUIStyle& style, float y, float x, float aspectRatio) const override;
      bool update(unsigned int milliseconds) override;
      bool input(UISignalID id, IUIStyle& style) override;
      bool input(sf::Event& event, IUIStyle& style) override;

      private:
      class Edit {
        public:
        Edit(Editor& parent, bool addition, const std::string& content, unsigned int position);
        void undo(IUIStyle& style);
        void redo(IUIStyle& style);
        void remove(IUIStyle& style);
        void add(IUIStyle& style);

        private:
        Editor& cParent;
        bool cAddition;
        std::string cContent;
        unsigned int cPosition;
      };

      static int cDelayUntilBlinkChange;
      static bool cBlinkShowing;

      PropertyCode& cParent;
      std::string cEditingCode;
      unsigned int cCaret;
      bool cMouseSelecting;

      // Cached calculated values.
      unsigned int cLineStartIndex;
      unsigned int cLine;
      float cCaretXPosition;        /// Remembers cursor X position when navigating up and down.
      unsigned int cSelection;
      float cCaretOffsetX;
      
      AnimatedFloat cScrollX;
      AnimatedFloat cScrollY;      

      // Editing code.
      std::string cClipboard;
      std::stack<Edit> cUndoStack;
      std::stack<Edit> cRedoStack;

      int cOpenness;
      bool cClosing;
      std::unique_ptr<Choice> cConfirmationSelection;
      std::unique_ptr<Choice> cClosedConfirmationSelection;

      void updateValues(IUIStyle& style);
      void updateCaretColumnPosition(IUIStyle& style);
      std::string getLine(unsigned int i);
      void clearSelection(IUIStyle& style);
      void cut(IUIStyle& style);
      void copy();
      void paste(IUIStyle& style);
      void undo(IUIStyle& style);
      void redo(IUIStyle& style);
      int getCaretPosition(IUIStyle& style, int x, int y);
    };

    static const unsigned int BLINK_DELAY = 200;

    IProject& cProject;

    std::function<std::string()> cGetter;
    std::function<void(const std::string&)> cSetter;
  };
}
