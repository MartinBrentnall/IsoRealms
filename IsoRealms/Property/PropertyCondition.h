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
#include <functional>
#include <limits>
#include <set>
#include <stack>
#include <GL/glew.h>

#include "IsoRealms/AnimatedFloat.h"
#include "IsoRealms/Condition/Condition.h"
#include "IsoRealms/Editing/ConfiguratorSignalID.h"
#include "IsoRealms/IApplication.h"
#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Utils.h"

#include "Property.h"

namespace IsoRealms {
  class PropertyCondition : public Property {
    private:
    class IConditionChar {
      public:
      virtual void render(IPropertyAppearance* appearance, float xOffset) const = 0;
      virtual float getWidth(IPropertyAppearance* appearance) const = 0;

      virtual ~IConditionChar() {
      }
    };

    class TextChar : public IConditionChar {
      private:
      char cCharacter;

      public:
      TextChar(char character);

      /*****************************\
       * Implements IConditionChar *
      \*****************************/
      void render(IPropertyAppearance* appearance, float xOffset) const override;
      float getWidth(IPropertyAppearance* appearance) const override;
    };

    class ElementChar : public IConditionChar {
      private:
      ConditionElement::Clause* cCharacter;

      public:
      ElementChar(ConditionElement::Clause* cCharacter);

      /*****************************\
       * Implements IConditionChar *
      \*****************************/
      void render(IPropertyAppearance* appearance, float xOffset) const override;
      float getWidth(IPropertyAppearance* appearance) const override;
    };

    class IFunctionOutput;

    class IFunctionInput {
      public:
      virtual void render(IPropertyAppearance* appearance, bool tick) const = 0;
      virtual int getElementCount() const = 0;
      virtual bool input(ConfiguratorSignalID id) = 0;
      virtual IFunctionOutput* getOutput() const = 0;
      virtual IFunctionInput* getInput(int index) const = 0;
      virtual bool setAnd(bool isAnd) = 0;
      virtual std::optional<Condition> createCondition() = 0;
      virtual float getSelectedHeight() const = 0;
      virtual int getMaxDepth() const = 0;
      virtual ~IFunctionInput() {}
    };

    class IFunctionOutput {
      public:
      virtual int getIndex(IFunctionInput* input) const = 0;
      virtual IFunctionInput* getInput(int index) const = 0;
      virtual void setInput(int index, std::unique_ptr<IFunctionInput> input) = 0;
      virtual IFunctionInput* removeInput(int index) = 0;
      virtual IFunctionInput* getAsInput() = 0;
      virtual float getSelectedHeight(const IFunctionInput* input) const = 0;
      virtual bool isAnd() const = 0;
      virtual ~IFunctionOutput() {}
    };

    class FunctionBlock : public IFunctionOutput,
                          public IFunctionInput {
      private:
      PropertyCondition* cParent;
      std::vector<std::unique_ptr<IFunctionInput>> cInputs;
      IFunctionOutput* cOutput;
      bool cAnd;
      bool cNot;

      public:
      FunctionBlock(PropertyCondition* parent, bool isAnd, bool isNot, IFunctionOutput* output);

      void addInput(std::unique_ptr<IFunctionInput> input);

      // Input functions
      void confirm();

      /*****************************\
       * Implements IFunctionInput *
      \*****************************/
      void render(IPropertyAppearance* appearance, bool tick) const override;
      int getElementCount() const override;
      bool input(ConfiguratorSignalID id) override;
      IFunctionOutput* getOutput() const override;
      IFunctionInput* getInput(int index) const override;
      bool setAnd(bool isAnd) override;
      std::optional<Condition> createCondition() override;
      float getSelectedHeight() const override;
      int getMaxDepth() const override;

      /******************************\
       * Implements IFunctionOutput *
      \******************************/
      int getIndex(IFunctionInput* input) const override;
      void setInput(int index, std::unique_ptr<IFunctionInput> input) override;
      IFunctionInput* removeInput(int index) override;
      IFunctionInput* getAsInput() override;
      float getSelectedHeight(const IFunctionInput* input) const override;
      bool isAnd() const override;
    };

    class ElementInput : public IFunctionInput {
      private:
      PropertyCondition* cParent;
      ConditionElement::Clause* cInput;
      IFunctionOutput* cOutput;

      public:
      ElementInput(PropertyCondition* parent, ConditionElement::Clause* input, IFunctionOutput* output);

      // Input functions
      void confirm();

      /*****************************\
       * Implements IFunctionInput *
      \*****************************/
      void render(IPropertyAppearance* appearance, bool tick) const override;
      int getElementCount() const override;
      bool input(ConfiguratorSignalID id) override;
      IFunctionOutput* getOutput() const override;
      IFunctionInput* getInput(int index) const override;
      bool setAnd(bool isAnd) override;
      std::optional<Condition> createCondition() override;
      float getSelectedHeight() const override;
      int getMaxDepth() const override;
    };

    class ResultOutput : public IFunctionOutput {
      private:
      PropertyCondition* cParent;
      std::unique_ptr<IFunctionInput> cInput;

      public:
      ResultOutput(PropertyCondition* cParent);

      void setInput(std::unique_ptr<IFunctionInput> input);

      void render(IPropertyAppearance* appearance);

      int getMaxDepth() const;
      int getElementCount() const;

      std::optional<Condition> createCondition();

      /******************************\
       * Implements IFunctionOutput *
      \******************************/
      int getIndex(IFunctionInput* input) const override;
      IFunctionInput* getInput(int index) const override;
      void setInput(int index, std::unique_ptr<IFunctionInput> input) override;
      IFunctionInput* removeInput(int index) override;
      IFunctionInput* getAsInput() override;
      float getSelectedHeight(const IFunctionInput* input) const override;
      bool isAnd() const override;
    };

    HatHandler& cHatHandler;
    std::function<std::optional<Condition>&()> cGetter;
    std::function<void(std::optional<Condition>&)> cSetter;
    std::vector<ConditionElement*> cAvailableElements;
    AnimatedFloat cSelectedElement; // TODO: AnimatedInteger
    std::vector<std::unique_ptr<IConditionChar>> cConditionString;
    std::unique_ptr<ResultOutput> cConditionDiagram;
    IFunctionInput* cSelectedInput;
    AnimatedFloat cSelectionHeight;
    AnimatedFloat cSelectionDepth;
    bool cSelectedNotGate;
    bool cShowingConditionPalette;

    static std::vector<std::unique_ptr<PropertyCondition::IConditionChar>> getConditionString(PropertyCondition* parent, std::optional<Condition>& condition);
    static std::unique_ptr<IFunctionInput> getConditionDiagram(PropertyCondition* parent, std::optional<Condition>& condition, IFunctionOutput* output);

    void showConditionPalette(bool isAnd);
    void showConditionPalette(ConditionElement* element);
    void renderConditionPalette(IPropertyAppearance* appearance, bool tick) const;
    void updatePosition(bool init = false);

    // Input events
    void selectFromPalette();
    void selectNextElement();
    void selectPreviousElement();
    void confirmCondition();
    void moveCursorDown();
    void moveCursorUp();
    void moveCursorLeft();
    void moveCursorRight();

    public:
    PropertyCondition(HatHandler& hatHandler, const std::string& label, std::vector<ConditionElement*> availableElements, std::function<std::optional<Condition>&()> getter, std::function<void(std::optional<Condition>&)> setter);

    /***********************\
     * Implements Property *
    \***********************/
    bool updateEditing(unsigned int milliseconds) override;
    void render(IPropertyAppearance* appearance) const override;
    void renderEditing(IPropertyAppearance* appearance) const override;
    IEditable* edit(IAssetRegistry* assets) override;
    void close() override;
    float getWidth(IPropertyAppearance* appearance) const override;
    bool input(ConfiguratorSignalID id) override;
  };
}
