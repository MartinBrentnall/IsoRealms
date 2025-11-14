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

#include "IsoRealms.h"

#include "SignalInputID.h"

namespace IsoRealms::UI {
  class Layout;
  class LayoutComponent;

  class LayoutEditor : public IEditableScreen,
                       public IUIStyle {
    public:
    LayoutEditor(Layout& layout, IDialogManager& dialogManager);
    void updateScreen(unsigned int milliseconds);

    bool signal(SignalInputID id);

    /******************************\
     * Implements IEditableScreen *
    \******************************/
    bool input(sf::Event& event) override;
    void resetInput() override;
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;
    void notifyVisible() override;
    void notifyHidden() override;
    void notifyLostFocus() override;
    void notifyGainedFocus() override;
    std::vector<std::string> getDigitalInputs() const override;
    std::vector<std::string> getAnalogueInputs() const override;
    void setDigitalInput(const std::string& name, IBoolean* input) override;
    void setAnalogueInput(const std::string& name, IFloat* input) override;
    void setExitAction(IAction* action) override;
    void setAppearance(IFont* font, float scale) override;
    IScreen* screen() override;
    
    /*******************************\
     * Implements IUIConfiguration *
    \*******************************/
    IFont* getFont() const override;
    float getFontSize() const override;
    IFont* getCodeFont() const override;
    float getCodeFontSize() const override;
    Project& getProject() const override;
      
    private:
    enum class Handle {
      NONE,
      WEST,
      EAST,
      SOUTH,
      NORTH,
      SOUTHWEST,
      SOUTHEAST,
      NORTHWEST,
      NORTHEAST,
      CENTER
    };

    class ILayoutTool {
      public:
      virtual bool input(sf::Event& event) = 0;
      virtual void update(unsigned int milliseconds) = 0;
      virtual void renderSelection() const = 0;
      virtual void renderIcon() const = 0;
      virtual bool isTool(ILayoutTool* tool) const = 0;
    };
    
    class DrawTool : public ILayoutTool {
      public:
      DrawTool(LayoutEditor& parent);
      
      /**************************\
       * Implements ILayoutTool *
      \**************************/
      bool input(sf::Event& event) override;
      void update(unsigned int milliseconds) override;
      void renderSelection() const override;
      void renderIcon() const override;
      bool isTool(ILayoutTool* tool) const override;
      
      private:
      LayoutEditor& cParent;
      float cPinnedX;
      float cPinnedY;
      bool cPinSet;
    };
    
    class PropertiesTool : public ILayoutTool {
      public:
      PropertiesTool(LayoutEditor& parent);
      
      /**************************\
       * Implements ILayoutTool *
      \**************************/
      bool input(sf::Event& event) override;
      void update(unsigned int milliseconds) override;
      void renderSelection() const override;
      void renderIcon() const override;
      bool isTool(ILayoutTool* tool) const override;
      
      private:
      LayoutEditor& cParent;
    };
    
    class LocationTool : public ILayoutTool {
      public:
      LocationTool(LayoutEditor& parent);
      
      /**************************\
       * Implements ILayoutTool *
      \**************************/
      bool input(sf::Event& event) override;
      void update(unsigned int milliseconds) override;
      void renderSelection() const override;
      void renderIcon() const override;
      bool isTool(ILayoutTool* tool) const override;
      
      private:
      LayoutEditor& cParent;
      bool cResizingHorizontal;
      float cResizingFixedHorizontal;
      bool cResizingVertical;
      float cResizingFixedVertical;
      Handle cMovingHandle;
    };
    
    class OffsetTool : public ILayoutTool {
      public:
      OffsetTool(LayoutEditor& parent);
      
      /**************************\
       * Implements ILayoutTool *
      \**************************/
      bool input(sf::Event& event) override;
      void update(unsigned int milliseconds) override;
      void renderSelection() const override;
      void renderIcon() const override;
      bool isTool(ILayoutTool* tool) const override;
      
      private:
      LayoutEditor& cParent;
      bool cResizingHorizontal;
      float cResizingFixedHorizontal;
      bool cResizingVertical;
      float cResizingFixedVertical;
      Handle cMovingHandle;
    };
    
    class DeleteTool : public ILayoutTool {
      public:
      DeleteTool(LayoutEditor& parent);
      
      /**************************\
       * Implements ILayoutTool *
      \**************************/
      bool input(sf::Event& event) override;
      void update(unsigned int milliseconds) override;
      void renderSelection() const override;
      void renderIcon() const override;
      bool isTool(ILayoutTool* tool) const override;
      
      private:
      LayoutEditor& cParent;
    };
    
    class OrderingTool : public ILayoutTool {
      public:
      OrderingTool(LayoutEditor& parent);
      
      /**************************\
       * Implements ILayoutTool *
      \**************************/
      bool input(sf::Event& event) override;
      void update(unsigned int milliseconds) override;
      void renderSelection() const override;
      void renderIcon() const override;
      bool isTool(ILayoutTool* tool) const override;
      
      private:
      LayoutEditor& cParent;
    };

    class CopyTool : public ILayoutTool {
      public:
      CopyTool(LayoutEditor& parent);

      /**************************\
       * Implements ILayoutTool *
      \**************************/
      bool input(sf::Event& event) override;
      void update(unsigned int milliseconds) override;
      void renderSelection() const override;
      void renderIcon() const override;
      bool isTool(ILayoutTool* tool) const override;

      private:
      LayoutEditor& cParent;
    };
    
    static const float ZOOM_LIMIT_MAXIMUM;
    static const float ZOOM_LIMIT_MINIMUM;
    
    static const float ANALOGUE_INPUT_DEAD_ZONE;

    static const float EDIT_HANDLE_RADIUS;

    HatHandler& cHatHandler;
    IDialogManager& cDialogManager;

    IAction* cExitAction;

    bool cHasFocus;

    DrawTool cDrawTool;
    PropertiesTool cPropertiesTool;
    LocationTool cLocationTool;
    OffsetTool cOffsetTool;
    DeleteTool cDeleteTool;
    OrderingTool cOrderingTool;
    CopyTool cCopyTool;

    Layout& cLayout;
    LayoutComponent* cSelectedComponent;
    Handle cSelectedComponentClosestHandle;
    float cSelectedComponentLeft;
    float cSelectedComponentRight;
    float cSelectedComponentTop;
    float cSelectedComponentBottom;
    float cEditHandleRadius;


    EditorToolbar<ILayoutTool*, ILayoutTool*> cToolbar;
    ILayoutTool* cSelectedTool;
    float cCursorSpeedX;
    float cCursorSpeedY;
    float cZoomSpeed;
    
    float cZoomFactorStep;
    AnimatedFloat cZoomFactor;
    AnimatedFloat cPanX;
    AnimatedFloat cPanY;
    float cPreviousMouseX;
    float cPreviousMouseY;
    bool cDragging;
    bool cSelectionOverride;
    bool cCycleSelection;
    bool cDrawComponent;
    float cAspectRatio;
    bool cEditingProperties;
    UIManager cPropertiesUI;
    IFont* cFont;
    float cFontSize;
    
    // Component mouse drawing.
    bool cDrawingStarted;
    float cDrawingStartX;
    float cDrawingStartY;
    float cDrawingEndX;
    float cDrawingEndY;
    
    // Grid configuration.
    float cGridX;
    float cGridY;

    // Handle location functions
    std::vector<std::pair<Handle, std::function<Point2D()>>> cHandleLocationFunctions;

    Point2D transform(const Point2D& normalisedLocation) const;
    float snapX(float x) const;
    float snapY(float y) const;
    void renderGrid(float fromY, float toY, float x) const;
    void renderGrid(float fromX, float toX) const;
    void openProperties();
    void deleteSelectedComponent();
    void moveSelectedComponentToBack();
    void moveSelectedComponentToFront();
    void pickHandle();
    void renderEditingHandles();
    void selectComponent(LayoutComponent* component);
    void resizeComponentHorizontal(float fixed, float x);
    void resizeComponentVertical(float fixed, float y);
    void moveComponent(Handle handle, float x, float y);
    void offsetComponentHorizontal(float fixed, float x);
    void offsetComponentVertical(float fixed, float y);
    void offfsetComponent(Handle handle, float x, float y);
    void updateSelectedComponentEdges();
    float getHandleXOffset(Handle handle) const;
    float getHandleYOffset(Handle handle) const;
  };
}
