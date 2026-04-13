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
#include "LiftType.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const std::string LiftType::JSON_APPEARANCE = "appearance";
  const std::string LiftType::JSON_ON_TICK    = "onTick";
  const std::string LiftType::JSON_STATE      = "state";

  LiftType::LiftType(Spindizzy& spindizzy, IResourceData& data) :
            cSpindizzy(spindizzy),
            cAssets(spindizzy),
            cDefModel(data),
            cDefActive(data, true),
            cDefTickAction(data.getDummyActionClient()) {
  }
  
  LiftType::LiftType(Spindizzy& spindizzy, IResourceData& data, JSONObject object) :
            LiftType(spindizzy, data) {
    cDefModel.init(object, JSON_APPEARANCE);
    cDefActive.init(object, JSON_STATE);
    cDefTickAction.init(object, JSON_ON_TICK);
  }

  void LiftType::registerAssets(ResourceAssetRegistry& assets) {
    // Nothing to do.
  }
    
  void LiftType::save(JSONObject object) const {
    cDefModel.save(object, JSON_APPEARANCE);
    cDefActive.save(object, JSON_STATE);
    cDefTickAction.save(object, JSON_ON_TICK);
  }

  void LiftType::hintInUse(bool inUse) {
    // Nothing to do. TODO?
  }
  
  bool LiftType::renderIcon() const {
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
    glScalef(0.8f, 0.8f, 0.8f);
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1.0f, 1.0f, 1.0f);
    cDefModel.renderIcon();
    return true;
  }

  void LiftType::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector(metadata.getPropertyData("Appearance"), cDefModel);
    owner.createPropertyTreeSelector(metadata.getPropertyData("State"),      cDefActive);
    owner.createPropertyTreeSelector(metadata.getPropertyData("OnMove"),     cDefTickAction);
  }

  void LiftType::removed() {
    cSpindizzy.removeAll(this);
  }

  bool LiftType::hasReadOnlyReferences() const {
    return cSpindizzy.isUsedInReadOnlyWorld(*this);
  }

  void LiftType::overrideReadOnlyReferences() {
    cSpindizzy.overrideReadOnlyWorlds(*this);
  }

  void LiftType::registerAssets(const std::string& parentID) {
    cAssets.add<IWorldEditorTool>(this, parentID, "Lift Types");
  }  
  
  bool LiftType::isActive() {
    return cDefActive->getValue();
  }
  
  void LiftType::executeTickAction() {
    cDefTickAction.execute();
  }  

  std::unique_ptr<ModelInstance> LiftType::createModel() {
    return cDefModel.createInstance();
  }

  IWorldEditorToolInstance* LiftType::createToolInstance(WorldEditor& editor, IResourceData& owner) {
    return cEditingPens.emplace_back(std::make_unique<Pen>(*this, editor)).get();
  }

  bool LiftType::renderAssetIcon() const {
    return renderIcon();
  }

  void LiftType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void LiftType::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool LiftType::isDefaultConfiguration() const {
    return true;
  }

  LiftType::Pen::Pen(LiftType& parent, WorldEditor& editor) :
            cParent(parent),
            cEditor(editor),
            cPinnedZone(nullptr) {
  }

  bool LiftType::Pen::isTool(IWorldEditorTool* tool) const {
    return tool == &cParent;
  }

  bool LiftType::Pen::renderIcon(float yaw) const {
    return cParent.renderIcon();
  }

  void LiftType::Pen::renderEditingPreview() const {
    if (cPinnedZone != nullptr) {
      glPushMatrix();
  //    glTranslatef(cEditor.getCursorX(), cEditor.getCursorY(), cEditor.getCursorZ());
      if (!cPinnedRange.has_value()) {
        glColor3f(1.0f, 0.5f, 0.5f);
      } else {
        glColor3f(0.5f, 1.0f, 0.5f);
      }
      glLineWidth(3.0);
      glBegin(GL_LINES);
      Utils::renderVolumeLines(cEditor.getCursorX() - 0.5f, cEditor.getCursorX() + 0.5f,
                               cEditor.getCursorY() - 0.5f, cEditor.getCursorY() + 0.5f,
                               cEditor.getCursorZ() * 0.5f, cEditor.getCursorZ() * 0.5f);
      glEnd();
      glColor3f(1.0f, 1.0f, 1.0f);
      glTranslatef(cPinnedLocation.cDefX, cPinnedLocation.cDefY, cPinnedLocation.cDefZ * 0.5f + (0.5f * 0.05));
      cParent.cDefModel.renderPreview();
      glPopMatrix();
      glPushMatrix();
      glTranslatef(cPinnedLocation.cDefX, cPinnedLocation.cDefY, 0);
      glBindTexture(GL_TEXTURE_2D, 0);
      glColor3f(1.0, 1.0, 0.0);
      glLineStipple(1, 255);
      glEnable(GL_LINE_STIPPLE);
      float mRangeA = cEditor.getCursorZ() * 0.5f;
      float mRangeB = (cPinnedRange.has_value() ? cPinnedRange.value() : cEditor.getCursorZ()) * 0.5f;
      renderArrowLines(mRangeA, mRangeB);
      glColor3f(1.0, 0.0, 0.0);
      glDisable(GL_LINE_STIPPLE);
      glLineWidth(5.0);
      renderArrowLines(mRangeA, mRangeB);
      glColor3f(1.0, 1.0, 1.0);
      glLineWidth(1.0);
      glPopMatrix();
    } else {
      glPushMatrix();
      glTranslatef(cEditor.getCursorX(), cEditor.getCursorY(), cEditor.getCursorZ() * 0.5f + (0.5f * 0.05));
      glColor3f(1.0f, 1.0f, 1.0f);
      cParent.cDefModel.renderPreview();
      glPopMatrix();
    }
  }

  void LiftType::Pen::renderUI(float aspectRatio) const {
    // Nothing to do.
  }

  void LiftType::Pen::updateUI(unsigned int milliseconds) {
    // Nothing to do.
  }

  bool LiftType::Pen::inputTool(sf::Event& event) {
    return false;
  }

  bool LiftType::Pen::inputTool(SignalInputID id, double yaw) {
    switch (id) {
      case SignalInputID::USE_TOOL: draw();   return true;
      case SignalInputID::CANCEL:   cancel(); return true;
      default:                                break;
    }
    return false;
  }

  bool LiftType::Pen::isCursorLocked() const {
    return false;
  }

  void LiftType::Pen::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
    if (end == nullptr) {
      cancel();
    } else if (cPinnedZone != nullptr) {
      end->x = cPinnedLocation.cDefX;
      end->y = cPinnedLocation.cDefY;
    }
  }

  double LiftType::Pen::getSnapInterval() const {
    return 1.0;
  }

  void LiftType::Pen::renderArrowLines(float rangeA, float rangeB) const {
    float mLineRadius = 0.5f * 0.5;
    float mArrowOffset = 0.5f * 0.5;
    float mTop    = rangeA > rangeB ? rangeA : rangeB;
    float mBottom = rangeA > rangeB ? rangeB : rangeA;
    glBegin(GL_LINES);
    glVertex3f(0, 0, mTop);
    glVertex3f(0, 0, mBottom);

    // Top point
    glVertex3f( 0,           0, mTop);
    glVertex3f( mLineRadius, 0, mTop - mArrowOffset);
    glVertex3f( 0,           0, mTop);
    glVertex3f(-mLineRadius, 0, mTop - mArrowOffset);

    // Bottom point
    glVertex3f( 0,           0, mBottom);
    glVertex3f( mLineRadius, 0, mBottom + mArrowOffset);
    glVertex3f( 0,           0, mBottom);
    glVertex3f(-mLineRadius, 0, mBottom + mArrowOffset);
    glEnd();
  }

  void LiftType::Pen::draw() {
    WorldEditorCursorCell mCursorCell = cEditor.getCursorCell();
    if (cPinnedZone == nullptr) {
      cPinnedZone = cEditor.getWorld().getOrDrawZone(mCursorCell, cEditor, nullptr);
      cPinnedLocation = mCursorCell;
    } else if (!cPinnedRange.has_value()) {
      cPinnedRange = mCursorCell.cDefZ;
    } else {
      int mSecondRange = mCursorCell.cDefZ;
      int mTopRange    = *cPinnedRange > mSecondRange ? *cPinnedRange : mSecondRange;
      int mBottomRange = *cPinnedRange > mSecondRange ? mSecondRange : *cPinnedRange;
      Lift* mLift = cEditor.getWorld().draw(cParent, cPinnedLocation, mBottomRange, mTopRange, cEditor);
      if (mLift != nullptr) {
        mLift->initialise();
        cPinnedZone = nullptr;
        cPinnedRange.reset();
      }
    }
  }

  bool LiftType::Pen::cancel() {
    if (cPinnedZone != nullptr) {
      if (cPinnedZone->empty()) {
        cEditor.getWorld().remove(cPinnedZone);
      }
      cPinnedZone = nullptr;
      cPinnedRange.reset();
      return true;
    }
    return false;
  }
}
