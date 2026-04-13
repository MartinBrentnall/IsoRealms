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
#include "Jewel.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const std::string Jewel::JSON_COLOUR        = "colour";
  const std::string Jewel::JSON_CYCLE_COLOURS = "cycleColours";
  const std::string Jewel::JSON_CYCLE_SPEED   = "cycleSpeed";
  const std::string Jewel::JSON_FRAME         = "frame";

  Jewel::Jewel(Spindizzy& spindizzy, IResourceData& data) :
            cProject(data.getProject()),
            cColourFrame(data, 1.0f, 1.0f, 0.0f) {
    cSampleModel = std::make_unique<Instance>(*this, cProject);
    cColoursCycle.emplace_back(std::make_unique<CycleColour>(*this, data));
  }
  
  Jewel::Jewel(Spindizzy& spindizzy, IResourceData& data, JSONObject object) :
            Jewel(spindizzy, data) {
    cCycleSpeed = object.getFloat(JSON_CYCLE_SPEED);
    cColourFrame.init(object, JSON_FRAME);
    cColoursCycle.clear();
    for (JSONValue mCycleColourValue : object.getArray(JSON_CYCLE_COLOURS)) {
      cColoursCycle.emplace_back(std::make_unique<CycleColour>(*this, data, mCycleColourValue.getObject()));
    }
  }

  void Jewel::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    for (unsigned int i = 0; i < cColoursCycle.size(); i++) {
      cColoursCycle[i]->getProperties(owner, metadata);
    }
    owner.createPropertyTreeSelector(metadata.getPropertyData("Outline"), cColourFrame);
  }

  void Jewel::removed() {
    // Nothing to do.
  }

  void Jewel::updateEditing(unsigned int milliseconds) {
    cSampleModel->update(milliseconds);
  }
  
  void Jewel::save(JSONObject object) const {
    cColourFrame.save(object, JSON_FRAME);
    object.addFloat(JSON_CYCLE_SPEED, cCycleSpeed);
    JSONArray mCycleColoursArray = object.addArray(JSON_CYCLE_COLOURS);
    for (const std::unique_ptr<CycleColour>& mCycleColour : cColoursCycle) {
      JSONObject mCycleColourObject =  mCycleColoursArray.addObject();
      mCycleColour->save(mCycleColourObject);
    }
  }

  IModelInstance* Jewel::createModel() {
    return cInstances.emplace_back(std::make_unique<Instance>(*this, cProject)).get();
  }

  bool Jewel::renderIcon() const {
    glTranslatef(0.0f, -0.75f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_YAW,   0.0f, 0.0f, 1.0f);
    glScalef(2.5f, 2.5f, 2.5f);
    return renderPreview();
  }

  bool Jewel::renderPreview() const {
    cSampleModel->render();
    return true;
  }

  bool Jewel::renderAssetIcon() const {
    return renderIcon();
  }

  void Jewel::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void Jewel::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool Jewel::isDefaultConfiguration() const {
    return true;
  }

  void Jewel::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  void Jewel::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IModel>(this, "", "Spindizzy Jewel Models");
  }
    
  // TODO: Handle removal of a colour
//   void Jewel::relinquish(IColour* destroyee) {
//     bool mChanged = false;
//     if (destroyee == cColourFrame) {
//       cColourFrame = assets->createLiteralColour(this, 1.0f, 1.0f, 0.0f);
//       mChanged = true;
//     }
//     for (int i = cColoursCycle.size() - 1; i >= 0; i--) {
//       if (destroyee == cColoursCycle[i]->getColour()) {
//         cColoursCycle.erase(cColoursCycle.begin() + i);
//         mChanged = true;
//       }
//     }
//     if (mChanged) {
//       for (unsigned int i = 0; i < cInstances.size(); i++) {
//         cInstances[i]->randomize();
//       }
//     }
//   }
  
  Jewel::CycleColour::CycleColour(Jewel& parent, IResourceData& data) :
            cParent(parent),
            cDefColour(data, 1.0f, 0.0f, 1.0f) {
  }

  Jewel::CycleColour::CycleColour(Jewel& parent, IResourceData& data, JSONObject object) :
            CycleColour(parent, data) {
    cDefColour.init(object, JSON_COLOUR);
  }

  void Jewel::CycleColour::save(JSONObject object) const {
    cDefColour.save(object, JSON_COLOUR);
  }

  const Colour* Jewel::CycleColour::getColour() const {
    return &cDefColour;
  }
  
  bool Jewel::CycleColour::operator==(const CycleColour& cycleColour) const {
    return false;
  }
  
  void Jewel::CycleColour::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector(metadata.getPropertyData("PanelPhase"), cDefColour);
  }

  unsigned int Jewel::Instance::cReferenceCount = 0;
  GLuint Jewel::Instance::cPanelDisplayList;
  GLuint Jewel::Instance::cFrameDisplayList;

  /****************\
   * Constructors *
  \****************/
  Jewel::Instance::Instance(Jewel& parent, Project& project) :
            cDefParent(parent) {
    
    // If this is the first model instance, we need to create some display lists.
    if (cReferenceCount == 0) {
      project.getApplication().mainThreadInit([this]() {
        float mRadius    = 0.5f;
        float mLineWidth = 0.05f;
        
        LiteralVertex mBottom(0.0f,    0.0f,    0.0f);
        LiteralVertex mTop(   0.0f,    0.0f,    0.5f * 1.6f);
        LiteralVertex mNorth( 0.0f,    mRadius, 0.5f * 0.8f);
        LiteralVertex mEast(  mRadius, 0.0f,    0.5f * 0.8f);
        LiteralVertex mSouth( 0.0f,   -mRadius, 0.5f * 0.8f);
        LiteralVertex mWest( -mRadius, 0.0f,    0.5f * 0.8f);

        cPanelDisplayList = glGenLists(1);
        glNewList(cPanelDisplayList, GL_COMPILE);
        glBegin(GL_TRIANGLES);
        Utils::renderInnerTriangle(mBottom, mNorth, mEast,  mLineWidth);
        Utils::renderInnerTriangle(mBottom, mEast,  mSouth, mLineWidth);
        Utils::renderInnerTriangle(mBottom, mSouth, mWest,  mLineWidth);
        Utils::renderInnerTriangle(mBottom, mWest,  mNorth, mLineWidth);
        Utils::renderInnerTriangle(mTop,    mNorth, mWest,  mLineWidth);
        Utils::renderInnerTriangle(mTop,    mWest,  mSouth, mLineWidth);
        Utils::renderInnerTriangle(mTop,    mSouth, mEast,  mLineWidth);
        Utils::renderInnerTriangle(mTop,    mEast,  mNorth, mLineWidth);
        glEnd();
        glEndList();
      
        cFrameDisplayList = glGenLists(1);
        glNewList(cFrameDisplayList, GL_COMPILE);
        glBegin(GL_TRIANGLES);
        Utils::renderOuterTriangle(mBottom, mNorth, mEast,  mLineWidth);
        Utils::renderOuterTriangle(mBottom, mEast,  mSouth, mLineWidth);
        Utils::renderOuterTriangle(mBottom, mSouth, mWest,  mLineWidth);
        Utils::renderOuterTriangle(mBottom, mWest,  mNorth, mLineWidth);
        Utils::renderOuterTriangle(mTop,    mNorth, mWest,  mLineWidth);
        Utils::renderOuterTriangle(mTop,    mWest,  mSouth, mLineWidth);
        Utils::renderOuterTriangle(mTop,    mSouth, mEast,  mLineWidth);
        Utils::renderOuterTriangle(mTop,    mEast,  mNorth, mLineWidth);
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
        glEndList();
      });
    }
    cReferenceCount++;
    
    randomize();
  }

  void Jewel::Instance::randomize() {
    cProgress = cDefParent.cColoursCycle.empty() ? 0.0f : (std::rand() % (cDefParent.cColoursCycle.size() * 1000)) / 1000.0f;
  }

  void Jewel::Instance::update(unsigned int milliseconds) {
    cProgress += milliseconds * cDefParent.cCycleSpeed;
    while (cProgress >= cDefParent.cColoursCycle.size()) {
      cProgress -= cDefParent.cColoursCycle.size();
    }
  }

  void Jewel::Instance::render() const {
    glBindTexture(GL_TEXTURE_2D, 0);
    cDefParent.cColourFrame->set();
    glCallList(cFrameDisplayList);
    unsigned int mPrevious = cProgress;
    unsigned int mNext     = mPrevious != cDefParent.cColoursCycle.size() - 1 ? cProgress + 1 : 0;
    float mCurrent         = cProgress - mPrevious;
    LiteralColour mColourPanel(***cDefParent.cColoursCycle[mPrevious]->getColour(), ***cDefParent.cColoursCycle[mNext]->getColour(), mCurrent);
    mColourPanel.set();
    glCallList(cPanelDisplayList);
    glColor3f(1.0, 1.0, 1.0);
  }

  Jewel::Instance::~Instance() {
    cReferenceCount--;
    if (cReferenceCount == 0) {
      glDeleteLists(cFrameDisplayList, 1);
      glDeleteLists(cPanelDisplayList, 1);
    }
  }
}
