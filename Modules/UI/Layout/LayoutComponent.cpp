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
  const std::string LayoutComponent::JSON_BOTTOM = "bottom";
  const std::string LayoutComponent::JSON_LEFT   = "left";
  const std::string LayoutComponent::JSON_RIGHT  = "right";
  const std::string LayoutComponent::JSON_SCREEN = "screen";
  const std::string LayoutComponent::JSON_TOP    = "top";

  LayoutComponent::LayoutComponent(IProject& project, Layout& layout, float x1, float y1, float x2, float y2, float aspectRatio) :
            cProjectCallbackManager(project),
            cLayout(layout),
            cDefScreen(project),
            cDefLeftEdge(*this, aspectRatio, std::min(x1, x2)),
            cDefRightEdge(*this, aspectRatio, std::max(x1, x2)),
            cDefBottomEdge(*this, 1.0f, std::min(y1, y2)),
            cDefTopEdge(*this, 1.0f, std::max(y1, y2)),
            cRuntimeScreen(nullptr),
            cLuaBinding(project, this) {
    cProjectCallbackManager.reset([this]() {
      cRuntimeScreen = *cDefScreen;
    });
  }

  LayoutComponent::LayoutComponent(IProject& project, Layout& layout, JSONObject object) :
            cProjectCallbackManager(project),
            cLayout(layout),
            cDefScreen(project),
            cDefLeftEdge(*this, object, JSON_LEFT),
            cDefRightEdge(*this, object, JSON_RIGHT),
            cDefBottomEdge(*this, object, JSON_BOTTOM),
            cDefTopEdge(*this, object, JSON_TOP),
            cRuntimeScreen(nullptr),
            cLuaBinding(project, this) {
    cDefScreen.init(object, JSON_SCREEN);
    cProjectCallbackManager.reset([this]() {
      cRuntimeScreen = *cDefScreen;
    });
  }

  void LayoutComponent::registerAssets(IAssetRegistry& assets, const std::string& name) {
    assets.add(&cLuaBinding, name, "Layout Components");
  }
  
  void LayoutComponent::unregisterAssets(IAssetRemover& assets, bool relinquish) {
    assets.remove(&cLuaBinding, relinquish);
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
      cRuntimeScreen->renderScreen(mScale * scale, mAspectRatio);
      glPopMatrix();
    }
  }

  void LayoutComponent::renderEditor(float scale, float aspectRatio) {
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
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
    glEnable(GL_BLEND);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-1.0f * mAspectRatio, -1.0f);
    glVertex2f( 1.0f * mAspectRatio, -1.0f);
    glVertex2f( 1.0f * mAspectRatio,  1.0f);
    glVertex2f(-1.0f * mAspectRatio,  1.0f);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(-1.0f * mAspectRatio, -1.0f);
    glVertex2f( 1.0f * mAspectRatio,  1.0f);
    glVertex2f( 1.0f * mAspectRatio, -1.0f);
    glVertex2f(-1.0f * mAspectRatio,  1.0f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    cDefScreen->renderScreen(mScale * scale, mAspectRatio);
    glPopMatrix();
  }

  void LayoutComponent::save(JSONObject object, IAssetIdentifier& identifier) const {
    cDefScreen.save(object, JSON_SCREEN);
    cDefTopEdge.save(object, JSON_TOP, &cLayout, 1.0f);
    cDefBottomEdge.save(object, JSON_BOTTOM, &cLayout, -1.0f);
    cDefLeftEdge.save(object, JSON_LEFT, &cLayout, -1.0f);
    cDefRightEdge.save(object, JSON_RIGHT, &cLayout, 1.0f);
  }

  // bool LayoutComponent::pickHandle(float x, float y, float scale, float aspectRatio) {
  //   float mNorth = cDefTopEdge.getLocation(1.0f);
  //   float mSouth = cDefBottomEdge.getLocation(1.0f);
  //   float mWest  = cDefLeftEdge.getLocation(aspectRatio);
  //   float mEast  = cDefRightEdge.getLocation(aspectRatio);
  //   cEditingSelectedHandle = Handle::NONE;
  //   float mEditHandleRadius = EDIT_HANDLE_RADIUS / scale;
  //   testHandlePick(x, y, mWest - mEditHandleRadius, (mSouth + mNorth) / 2.0f,   mEditHandleRadius, Handle::WEST);
  //   testHandlePick(x, y, mEast + mEditHandleRadius, (mSouth + mNorth) / 2.0f,   mEditHandleRadius, Handle::EAST);
  //   testHandlePick(x, y, (mWest + mEast) / 2.0,     mSouth - mEditHandleRadius, mEditHandleRadius, Handle::SOUTH);
  //   testHandlePick(x, y, (mWest + mEast) / 2.0,     mNorth + mEditHandleRadius, mEditHandleRadius, Handle::NORTH);
  //   testHandlePick(x, y, mWest - mEditHandleRadius, mSouth - mEditHandleRadius, mEditHandleRadius, Handle::SOUTHWEST);
  //   testHandlePick(x, y, mEast + mEditHandleRadius, mSouth - mEditHandleRadius, mEditHandleRadius, Handle::SOUTHEAST);
  //   testHandlePick(x, y, mWest - mEditHandleRadius, mNorth + mEditHandleRadius, mEditHandleRadius, Handle::NORTHWEST);
  //   testHandlePick(x, y, mEast + mEditHandleRadius, mNorth + mEditHandleRadius, mEditHandleRadius, Handle::NORTHEAST);
  //   return cEditingSelectedHandle != Handle::NONE;
  // }

  void LayoutComponent::setLeftEdgeLocation(float value, float aspectRatio) {
    cDefLeftEdge.setLocation(aspectRatio, value);
  }

  void LayoutComponent::setRightEdgeLocation(float value, float aspectRatio) {
    cDefRightEdge.setLocation(aspectRatio, value);
  }

  void LayoutComponent::setBottomEdgeLocation(float value) {
    cDefBottomEdge.setLocation(1.0f, value);
  }

  void LayoutComponent::setTopEdgeLocation(float value) {
    cDefTopEdge.setLocation(1.0f, value);
  }

  void LayoutComponent::setLeftEdgeOffset(float value, float aspectRatio) {
    cDefLeftEdge.setOffset(aspectRatio, value);
  }

  void LayoutComponent::setRightEdgeOffset(float value, float aspectRatio) {
    cDefRightEdge.setOffset(aspectRatio, value);
  }

  void LayoutComponent::setBottomEdgeOffset(float value) {
    cDefBottomEdge.setOffset(1.0f, value);
  }

  void LayoutComponent::setTopEdgeOffset(float value) {
    cDefTopEdge.setOffset(1.0f, value);
  }

  void LayoutComponent::renderAsRelation(float aspectRatio) const {
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
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-1.0f * mAspectRatio, -1.0f);
    glVertex2f( 1.0f * mAspectRatio, -1.0f);
    glVertex2f( 1.0f * mAspectRatio,  1.0f);
    glVertex2f(-1.0f * mAspectRatio,  1.0f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();
  }
  
  void LayoutComponent::renderSelectionHighlight(float aspectRatio) {
    float mNorth = cDefTopEdge.getLocation(1.0f);
    float mSouth = cDefBottomEdge.getLocation(1.0f);
    float mWest  = cDefLeftEdge.getLocation(aspectRatio);
    float mEast  = cDefRightEdge.getLocation(aspectRatio);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 255);
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(mWest, mSouth);
    glVertex2f(mEast, mSouth);
    glVertex2f(mEast, mNorth);
    glVertex2f(mWest, mNorth);
    glEnd();  
    glDisable(GL_LINE_STIPPLE);
  }

  // void LayoutComponent::renderEditingHandles(float aspectRatio, float scale) {
  //   float mNorth = cDefTopEdge.getLocation(1.0f);
  //   float mSouth = cDefBottomEdge.getLocation(1.0f);
  //   float mWest  = cDefLeftEdge.getLocation(aspectRatio);
  //   float mEast  = cDefRightEdge.getLocation(aspectRatio);
  //   glBindTexture(GL_TEXTURE_2D, 0);
  //   float mEditHandleRadius = EDIT_HANDLE_RADIUS / scale;
  //   renderEditingHandle(mWest - mEditHandleRadius, (mSouth + mNorth) / 2.0f,   mEditHandleRadius, Handle::WEST);
  //   renderEditingHandle(mEast + mEditHandleRadius, (mSouth + mNorth) / 2.0f,   mEditHandleRadius, Handle::EAST);
  //   renderEditingHandle((mWest + mEast) / 2.0,     mSouth - mEditHandleRadius, mEditHandleRadius, Handle::SOUTH);
  //   renderEditingHandle((mWest + mEast) / 2.0,     mNorth + mEditHandleRadius, mEditHandleRadius, Handle::NORTH);
  //   renderEditingHandle(mWest - mEditHandleRadius, mSouth - mEditHandleRadius, mEditHandleRadius, Handle::SOUTHWEST);
  //   renderEditingHandle(mEast + mEditHandleRadius, mSouth - mEditHandleRadius, mEditHandleRadius, Handle::SOUTHEAST);
  //   renderEditingHandle(mWest - mEditHandleRadius, mNorth + mEditHandleRadius, mEditHandleRadius, Handle::NORTHWEST);
  //   renderEditingHandle(mEast + mEditHandleRadius, mNorth + mEditHandleRadius, mEditHandleRadius, Handle::NORTHEAST);
  //   switch (cEditingSelectedHandle) {
  //     case Handle::WEST:      cDefLeftEdge.renderRelation(aspectRatio);   break;
  //     case Handle::EAST:      cDefRightEdge.renderRelation(aspectRatio);  break;
  //     case Handle::SOUTH:     cDefBottomEdge.renderRelation(aspectRatio); break;
  //     case Handle::NORTH:     cDefTopEdge.renderRelation(aspectRatio);    break;
  //     case Handle::SOUTHWEST: cDefLeftEdge.renderRelation(aspectRatio);
  //                             cDefBottomEdge.renderRelation(aspectRatio); break;
  //     case Handle::SOUTHEAST: cDefRightEdge.renderRelation(aspectRatio);
  //                             cDefBottomEdge.renderRelation(aspectRatio); break;
  //     case Handle::NORTHWEST: cDefLeftEdge.renderRelation(aspectRatio);
  //                             cDefTopEdge.renderRelation(aspectRatio);    break;
  //     case Handle::NORTHEAST: cDefRightEdge.renderRelation(aspectRatio);
  //                             cDefTopEdge.renderRelation(aspectRatio);    break;
  //     case Handle::NONE:      /* Do nothing */                            break;
  //   }
  // }

  bool LayoutComponent::contains(float x, float y, float aspectRatio) {
    return CollisionUtils::contains(x, y, cDefLeftEdge.getLocation(aspectRatio), cDefRightEdge.getLocation(aspectRatio), cDefBottomEdge.getLocation(1.0f), cDefTopEdge.getLocation(1.0f));
  }
  
  bool LayoutComponent::isRelatedTo(LayoutComponent* component) const {
    return false;
    // FIXME:LayoutEditor
/*    return cDefLeftEdge.isRelatedTo(component)
        || cDefRightEdge.isRelatedTo(component)
        || cDefTopEdge.isRelatedTo(component)
        || cDefBottomEdge.isRelatedTo(component);*/
  }

  bool LayoutComponent::isHorizontalEdge(const LayoutComponentEdge& edge) const {
    return &edge == &cDefLeftEdge || &edge == &cDefRightEdge;
  }
  
  bool LayoutComponent::isPositiveEdge(const LayoutComponentEdge& edge) const {
    return &edge == &cDefTopEdge || &edge == &cDefRightEdge;
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
    
  Layout& LayoutComponent::getLayout() {
    return cLayout;
  }

  std::string LayoutComponent::getName() const {
    return cLayout.getName(this);
  }

  std::vector<std::string> LayoutComponent::getAvailableComponentNames() {
    return cLayout.getAvailableRelativeNames(this);
  }
    
  std::vector<std::unique_ptr<IProperty>> LayoutComponent::getProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeString>("Name", "TODO", [this]() {return getName();}, [this](const std::string& value) {std::cout << "TODO: Set layout component name" << std::endl; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Screen>>("Component", "TODO", cDefScreen));
    mProperties.emplace_back(std::make_unique<PropertyStruct>("Left",   "TODO", "Edit...", [this]() {return cDefLeftEdge.getProperties();}));
    mProperties.emplace_back(std::make_unique<PropertyStruct>("Right",  "TODO", "Edit...", [this]() {return cDefRightEdge.getProperties();}));
    mProperties.emplace_back(std::make_unique<PropertyStruct>("Top",    "TODO", "Edit...", [this]() {return cDefTopEdge.getProperties();}));
    mProperties.emplace_back(std::make_unique<PropertyStruct>("Bottom", "TODO", "Edit...", [this]() {return cDefBottomEdge.getProperties();}));
    return mProperties;
  }
  
  void LayoutComponent::setScreen(IScreen* screen) {
    cRuntimeScreen = screen;
  }
    

  // void LayoutComponent::testHandlePick(float xPick, float yPick, float x, float y, float radius, LayoutComponent::Handle handle) {
  //   if (CollisionUtils::contains(xPick, yPick, x - radius, x + radius, y - radius, y + radius)) {
  //     cEditingSelectedHandle = handle;
  //   }
  // }
}
