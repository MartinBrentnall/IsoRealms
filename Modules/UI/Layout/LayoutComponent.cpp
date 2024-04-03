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
#include "LayoutComponent.h"

#include "Layout.h"

namespace IsoRealms::UI {
  const float LayoutComponent::EDIT_HANDLE_RADIUS = 0.01f;

  const std::string LayoutComponent::TAG_BOTTOM   = "Bottom";
  const std::string LayoutComponent::TAG_LEFT     = "Left";
  const std::string LayoutComponent::TAG_LOCATION = "Location";
  const std::string LayoutComponent::TAG_OFFSET   = "Offset";
  const std::string LayoutComponent::TAG_RIGHT    = "Right";
  const std::string LayoutComponent::TAG_SCREEN   = "Screen";
  const std::string LayoutComponent::TAG_TOP      = "Top";

  const std::string LayoutComponent::ATTRIBUTE_TYPE   = "type";

  const std::string LayoutComponent::LOCATION_TYPE_ABSOLUTE = "Absolute";
  const std::string LayoutComponent::LOCATION_TYPE_RELATIVE = "Relative";

  const std::string LayoutComponent::OFFSET_TYPE_ABSOLUTE = "Absolute";
  const std::string LayoutComponent::OFFSET_TYPE_LINKED   = "Linked";

  LayoutComponent::LayoutComponent(IProject* project, Layout& layout) :
            cDefLayout(layout),
            cDefScreen(project),
            cDefLeftEdge(*this, -1.0f),
            cDefRightEdge(*this, 1.0f),
            cDefBottomEdge(*this, -1.0f),
            cDefTopEdge(*this, 1.0f),
            cRuntimeScreen(nullptr),
            cEditingSelectedHandle(Handle::NONE),
            cEditingDragging(false),
            cLuaBinding(project, this) {
    project->reset([this]() {
      cRuntimeScreen = *cDefScreen;
    });
  }

  LayoutComponent::LayoutComponent(IProject* project, Layout& layout, DOMNode& node) :
            cDefLayout(layout),
            cDefScreen(project),
            cDefLeftEdge(project, *this, node, TAG_LEFT, true, -1.0f),
            cDefRightEdge(project, *this, node, TAG_RIGHT, true, 1.0f),
            cDefBottomEdge(project, *this, node, TAG_BOTTOM, false, -1.0f),
            cDefTopEdge(project, *this, node, TAG_TOP, false, 1.0f),
            cRuntimeScreen(nullptr),
            cEditingSelectedHandle(Handle::NONE),
            cEditingDragging(false),
            cLuaBinding(project, this) {
    if (node.containsNode(TAG_SCREEN)) {
      cDefScreen.init(node.getNode(TAG_SCREEN));
    }

    project->reset([this]() {
      cRuntimeScreen = *cDefScreen;
    });
  }

  void LayoutComponent::registerAssets(IAssetRegistry* assets, const std::string& name) {
    assets->add(&cLuaBinding, name, "Layout Components");
  }
  
  void LayoutComponent::unregisterAssets(IAssetRemover* assets) {
    assets->remove(&cLuaBinding);
  }
  
  void LayoutComponent::render(float scale, float aspectRatio) {
    if (cRuntimeScreen != nullptr) {
      float mLeft        = cDefLeftEdge.getLocation(aspectRatio);
      float mRight       = cDefRightEdge.getLocation(aspectRatio);
      float mBottom      = cDefBottomEdge.getLocation(1.0f);
      float mTop         = cDefTopEdge.getLocation(1.0f);
      float mWidth       = mRight - mLeft;
      float mHeight      = mTop - mBottom;
      float mScale       = mHeight / 2.0f;
      float mAspectRatio = mWidth / mHeight;
      float mXCenter     = mLeft + mWidth / 2.0f;
      float mYCenter     = mBottom + mHeight / 2.0f;
      glPushMatrix();
      glTranslatef(mXCenter, mYCenter, 0.0f);
      glScalef(mScale, mScale, mScale);
//       glBindTexture(GL_TEXTURE_2D, 0);
//       glColor3f(1.0f, 1.0f, 1.0f);
//       glBegin(GL_LINE_LOOP);
//       glVertex2f(-1.0f * mAspectRatio, -1.0f);
//       glVertex2f( 1.0f * mAspectRatio, -1.0f);
//       glVertex2f( 1.0f * mAspectRatio,  1.0f);
//       glVertex2f(-1.0f * mAspectRatio,  1.0f);
//       glEnd();
      cRuntimeScreen->renderScreen(mScale * scale, mAspectRatio);
      glPopMatrix();
    }
  }

  void LayoutComponent::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefScreen.save(node, TAG_SCREEN);
    cDefTopEdge.save(node, TAG_TOP, &cDefLayout, 1.0f);
    cDefBottomEdge.save(node, TAG_BOTTOM, &cDefLayout, -1.0f);
    cDefLeftEdge.save(node, TAG_LEFT, &cDefLayout, -1.0f);
    cDefRightEdge.save(node, TAG_RIGHT, &cDefLayout, 1.0f);
  }

  bool LayoutComponent::inputEditor(sf::Event& event, IScreen* screen, float x, float y, float aspectRatio, float scale) {
/* FIXME:LayoutEditor
    switch (event.type) {
      case sf::Event::MouseButtonPressed: {
        switch (event.button.button) {
          case SDL_BUTTON_LEFT: {
            float mNorth = cDefTopEdge.getLocation(1.0f);
            float mSouth = cDefBottomEdge.getLocation(1.0f);
            float mWest  = cDefLeftEdge.getLocation(aspectRatio);
            float mEast  = cDefRightEdge.getLocation(aspectRatio);
            cEditingSelectedHandle = LayoutComponent::NONE;
            float mEditHandleRadius = EDIT_HANDLE_RADIUS / scale;
            testHandlePick(x, y, mWest - mEditHandleRadius, (mSouth + mNorth) / 2.0f,   mEditHandleRadius, LayoutComponent::WEST);
            testHandlePick(x, y, mEast + mEditHandleRadius, (mSouth + mNorth) / 2.0f,   mEditHandleRadius, LayoutComponent::EAST);
            testHandlePick(x, y, (mWest + mEast) / 2.0,     mSouth - mEditHandleRadius, mEditHandleRadius, LayoutComponent::SOUTH);
            testHandlePick(x, y, (mWest + mEast) / 2.0,     mNorth + mEditHandleRadius, mEditHandleRadius, LayoutComponent::NORTH);
            testHandlePick(x, y, mWest - mEditHandleRadius, mSouth - mEditHandleRadius, mEditHandleRadius, LayoutComponent::SOUTHWEST);
            testHandlePick(x, y, mEast + mEditHandleRadius, mSouth - mEditHandleRadius, mEditHandleRadius, LayoutComponent::SOUTHEAST);
            testHandlePick(x, y, mWest - mEditHandleRadius, mNorth + mEditHandleRadius, mEditHandleRadius, LayoutComponent::NORTHWEST);
            testHandlePick(x, y, mEast + mEditHandleRadius, mNorth + mEditHandleRadius, mEditHandleRadius, LayoutComponent::NORTHEAST);
            cEditingDragging = true;
            return cEditingSelectedHandle != LayoutComponent::NONE;
          }
        }
      }
      
      case sf::Event::MouseButtonReleased: {
        if (cEditingDragging) {
          cEditingDragging = false;
          return true;
        }
      }

      case sf::Event::MouseMoved: {
        if (cEditingDragging) {
          IApplication* mApplication = project->getApplication();
          float mXPan =  mApplication->normalise(event.motion.xrel) / scale;
          float mYPan = -mApplication->normalise(event.motion.yrel) / scale;
          switch (cEditingSelectedHandle) {
            case Handle::WEST:      cDefLeftEdge.editOffset(mXPan);   break;
            case Handle::EAST:      cDefRightEdge.editOffset(mXPan);  break;
            case Handle::SOUTH:     cDefBottomEdge.editOffset(mYPan); break;
            case Handle::NORTH:     cDefTopEdge.editOffset(mYPan);    break;
            case Handle::SOUTHWEST: cDefLeftEdge.editOffset(mXPan);
                                    cDefBottomEdge.editOffset(mYPan); break;
            case Handle::SOUTHEAST: cDefRightEdge.editOffset(mXPan);
                                    cDefBottomEdge.editOffset(mYPan); break;
            case Handle::NORTHWEST: cDefLeftEdge.editOffset(mXPan);
                                    cDefTopEdge.editOffset(mYPan);    break;
            case Handle::NORTHEAST: cDefRightEdge.editOffset(mXPan);
                                    cDefTopEdge.editOffset(mYPan);    break;
            case Handle::NONE:      cDefLeftEdge.editOffset(mXPan);   
                                    cDefRightEdge.editOffset(mXPan);
                                    cDefBottomEdge.editOffset(mYPan);
                                    cDefTopEdge.editOffset(mYPan);    break;
          }
          return true;
        }
      }    
      */
  //     case sf::Event::KeyPressed: {
  //       switch (event.key.code) {
  //         case sf::Keyboard::Delete: {
  //           cDefLayout.removeRelatableElement(cDefLayout, this);
  //           cDefLayout.removeElement(cElement);
  //           return true;
  //         }
  //         
  //         default: {
  //           return true;
  //         }
  //       }
  //     }
  //  }
    return false;
  }

  bool LayoutComponent::renderSelectionHighlight(float aspectRatio, float scale) {
/* FIXME:LayoutEditor    float mNorth = cDefTopEdge.getLocation(aspectRatio);
    float mSouth = cDefBottomEdge.getLocation(aspectRatio);
    float mWest  = cDefLeftEdge.getLocation(aspectRatio);
    float mEast  = cDefRightEdge.getLocation(aspectRatio);
    glBindTexture(GL_TEXTURE_2D, 0);
    float mEditHandleRadius = EDIT_HANDLE_RADIUS / scale;
    renderEditingHandle(mWest - mEditHandleRadius, (mSouth + mNorth) / 2.0f,   mEditHandleRadius, Handle::WEST);
    renderEditingHandle(mEast + mEditHandleRadius, (mSouth + mNorth) / 2.0f,   mEditHandleRadius, Handle::EAST);
    renderEditingHandle((mWest + mEast) / 2.0,     mSouth - mEditHandleRadius, mEditHandleRadius, Handle::SOUTH);
    renderEditingHandle((mWest + mEast) / 2.0,     mNorth + mEditHandleRadius, mEditHandleRadius, Handle::NORTH);
    renderEditingHandle(mWest - mEditHandleRadius, mSouth - mEditHandleRadius, mEditHandleRadius, Handle::SOUTHWEST);
    renderEditingHandle(mEast + mEditHandleRadius, mSouth - mEditHandleRadius, mEditHandleRadius, Handle::SOUTHEAST);
    renderEditingHandle(mWest - mEditHandleRadius, mNorth + mEditHandleRadius, mEditHandleRadius, Handle::NORTHWEST);
    renderEditingHandle(mEast + mEditHandleRadius, mNorth + mEditHandleRadius, mEditHandleRadius, Handle::NORTHEAST);
    switch (cEditingSelectedHandle) {
      case Handle::WEST:      cDefLeftEdge.renderRelation(aspectRatio);   break;
      case Handle::EAST:      cDefRightEdge.renderRelation(aspectRatio);  break;
      case Handle::SOUTH:     cDefBottomEdge.renderRelation(aspectRatio); break;
      case Handle::NORTH:     cDefTopEdge.renderRelation(aspectRatio);    break;
      case Handle::SOUTHWEST: cDefLeftEdge.renderRelation(aspectRatio);
                              cDefBottomEdge.renderRelation(aspectRatio); break;
      case Handle::SOUTHEAST: cDefRightEdge.renderRelation(aspectRatio);
                              cDefBottomEdge.renderRelation(aspectRatio); break;
      case Handle::NORTHWEST: cDefLeftEdge.renderRelation(aspectRatio);
                              cDefTopEdge.renderRelation(aspectRatio);    break;
      case Handle::NORTHEAST: cDefRightEdge.renderRelation(aspectRatio);
                              cDefTopEdge.renderRelation(aspectRatio);    break;
      case Handle::NONE:      *//* Do nothing *//*                        break;
    }
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 255);
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(mWest, mSouth);
    glVertex2f(mEast, mSouth);
    glVertex2f(mEast, mNorth);
    glVertex2f(mWest, mNorth);
    glEnd();  
    glDisable(GL_LINE_STIPPLE);*/
    return true;
  }

  bool LayoutComponent::contains(float x, float y, float aspectRatio) {
    return CollisionUtils::contains(x, y, cDefLeftEdge.getLocation(aspectRatio), cDefRightEdge.getLocation(aspectRatio), cDefBottomEdge.getLocation(aspectRatio), cDefTopEdge.getLocation(aspectRatio));
  }
  
  bool LayoutComponent::isRelatedTo(LayoutComponent* component) const {
    return false;
    // FIXME:LayoutEditor
/*    return cDefLeftEdge.isRelatedTo(component)
        || cDefRightEdge.isRelatedTo(component)
        || cDefTopEdge.isRelatedTo(component)
        || cDefBottomEdge.isRelatedTo(component);*/
  }

  float LayoutComponent::getLeft(float aspectRatio) const {
    return cDefLeftEdge.getLocation(aspectRatio);
  }
  
  float LayoutComponent::getRight(float aspectRatio) const {
    return cDefRightEdge.getLocation(aspectRatio);
  }
  
  float LayoutComponent::getBottom() const {
    return cDefBottomEdge.getLocation(1.0f);
  }
  
  float LayoutComponent::getTop() const {
    return cDefTopEdge.getLocation(1.0f);
  }
    
  void LayoutComponent::setScreen(IScreen* screen) {
    cRuntimeScreen = screen;
  }
    
  void LayoutComponent::renderEditingHandle(float x, float y, float radius, LayoutComponent::Handle handle) {
    if (cEditingSelectedHandle == handle) {
      glColor3f(1.0f, 1.0f, 0.0f);
    } else {
      glColor3f(1.0f, 1.0f, 1.0f);
    }
    glBegin(GL_QUADS);
    glVertex2f(x - radius, y - radius);
    glVertex2f(x + radius, y - radius);
    glVertex2f(x + radius, y + radius);
    glVertex2f(x - radius, y + radius);
    glEnd();  
    
    if (cEditingSelectedHandle == handle) {
      glColor3f(0.0f, 1.0f, 0.0f);
    } else {
      glColor3f(1.0f, 0.0f, 0.0f);
    }
    glBegin(GL_LINE_LOOP);
    glVertex2f(x - radius, y - radius);
    glVertex2f(x + radius, y - radius);
    glVertex2f(x + radius, y + radius);
    glVertex2f(x - radius, y + radius);
    glEnd();  
  }

  void LayoutComponent::testHandlePick(float xPick, float yPick, float x, float y, float radius, LayoutComponent::Handle handle) {
    if (CollisionUtils::contains(xPick, yPick, x - radius, x + radius, y - radius, y + radius)) {
      cEditingSelectedHandle = handle;
    }
  }
  
  LayoutComponent::Edge::Edge(LayoutComponent& parent, float value) :
            cDefParent(parent),
            cDefLocation(std::make_unique<AbsoluteLocation>(value)),
            cDefOffset(std::make_unique<AbsoluteOffset>(0.0f)) {
  }
  
  LayoutComponent::Edge::Edge(IProject* project, LayoutComponent& parent, DOMNode& node, const std::string& tag, bool horizontal, float defaultValue) :
            Edge(parent, defaultValue) {
    if (node.containsNode(tag)) {
      DOMNode& mEdgeNode = node.getNode(tag);
      if (mEdgeNode.containsNode(TAG_LOCATION)) {
        DOMNode& mLocationNode = mEdgeNode.getNode(TAG_LOCATION);
        std::string mLocationType = mLocationNode.getAttribute(ATTRIBUTE_TYPE, LOCATION_TYPE_ABSOLUTE);
        cDefLocation = mLocationType == LOCATION_TYPE_ABSOLUTE ? static_cast<std::unique_ptr<ILayoutLocation>>(std::make_unique<AbsoluteLocation>(mLocationNode, defaultValue))
                     : mLocationType == LOCATION_TYPE_RELATIVE ? static_cast<std::unique_ptr<ILayoutLocation>>(std::make_unique<RelativeLocation>(project, mLocationNode, &cDefParent.cDefLayout, horizontal, defaultValue))
                     :                                           nullptr;
      }

      if (mEdgeNode.containsNode(TAG_OFFSET)) {
        DOMNode& mOffsetNode = mEdgeNode.getNode(TAG_OFFSET);
        std::string mOffsetType = mOffsetNode.getAttribute(ATTRIBUTE_TYPE, OFFSET_TYPE_ABSOLUTE);
        cDefOffset = mOffsetType == OFFSET_TYPE_ABSOLUTE ? static_cast<std::unique_ptr<ILayoutOffset>>(std::make_unique<AbsoluteOffset>(mOffsetNode))
                   : mOffsetType == OFFSET_TYPE_LINKED   ? static_cast<std::unique_ptr<ILayoutOffset>>(std::make_unique<LinkedOffset>(project, mOffsetNode, &cDefParent.cDefLayout, &cDefParent, (tag == TAG_LEFT || tag == TAG_BOTTOM) ? -1.0f : 1.0f))
                   :                                       nullptr;
      }
    }
  }
  
  float LayoutComponent::Edge::getLocation(float aspectRatio) const {
    return cDefLocation->getLocation(aspectRatio) + cDefOffset->getOffset(aspectRatio);
  }
  
  void LayoutComponent::Edge::save(DOMNodeWriter* node, const std::string& tag, Layout* layout, float defaultValue) const {
    DOMNodeWriter mEdgeNode = node->addBranch(tag);
    DOMNodeWriter mNode = mEdgeNode.addBranch(TAG_LOCATION);
    cDefLocation->save(&mNode, layout, defaultValue);
    mNode = mEdgeNode.addBranch(TAG_OFFSET);
    cDefOffset->save(&mNode, layout);
  }
}
