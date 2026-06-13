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
  Jewel::Jewel(Spindizzy& spindizzy, IComponentData& data) :
            cProject(data.getProject()),
            cDefColourFrame(data, 1.0f, 1.0f, 0.0f),
            cDefCycleSpeed(DEFAULT_CYCLE_SPEED) {
    cSampleModel = std::make_unique<Instance>(*this, cProject);
  }
  
  void Jewel::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyArray("cycleColours", cDefColoursCycle, [](const std::unique_ptr<CycleColour>& cycleColour) -> CycleColour& {return *cycleColour;}, [this, &owner, &metadata](CycleColour& cycleColour) {
      cycleColour.getProperties(owner, metadata, [this, &cycleColour]() {
        if (cDefColoursCycle.size() > 1) {
          Utils::removeElementUnique(cDefColoursCycle, &cycleColour);
          randomizeInstances();
        }
      });
    }, [this, &owner]() -> CycleColour& {
      IComponentData& mData = owner.getComponentData();
      CycleColour& mCycleColour = *cDefColoursCycle.emplace_back(std::make_unique<CycleColour>(*this, mData));
      randomizeInstances();
      return mCycleColour;
    });
    owner.createPropertyTreeSelector("frame", cDefColourFrame);
    owner.createPropertyNativeFloat("cycleSpeed", [this]() {return cDefCycleSpeed;}, [this](float value) {cDefCycleSpeed = value;}, DEFAULT_CYCLE_SPEED);

    // If we are loading persisted values, we need to create a default cycle colour if none exists.
    if (owner.loadsPersistedValues()) {
      if (cDefColoursCycle.size() == 0) {
        cDefColoursCycle.emplace_back(std::make_unique<CycleColour>(*this, owner.getComponentData()));
      }
    }
  }

  void Jewel::randomizeInstances() {
    cSampleModel->randomize();
    for (const std::unique_ptr<Instance>& mInstance : cInstances) {
      mInstance->randomize();
    }
  }

  void Jewel::removed() {
    // Nothing to do.
  }

  void Jewel::updateEditing(unsigned int milliseconds) {
    cSampleModel->update(milliseconds);
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

  void Jewel::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool Jewel::isDefaultConfiguration() const {
    return true;
  }

  void Jewel::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  void Jewel::registerAssets(ComponentAssetRegistry& assets) {
    assets.add<IModel>(this, "", "Spindizzy Jewel Models");
  }
  
  Jewel::CycleColour::CycleColour(Jewel& parent, IComponentData& data) :
            cParent(parent),
            cDefColour(data, 1.0f, 0.0f, 1.0f) {
  }

  Jewel::CycleColour::CycleColour(Jewel& parent, IComponentData& data, JSONObject object) :
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
  
  void Jewel::CycleColour::getProperties(IPropertyMaker& owner, const Metadata& metadata, std::function<void()> removeFunction) {
    owner.createPropertyTreeSelector(JSON_COLOUR, cDefColour, Options::EMPTY, removeFunction);
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
    cProgress = cDefParent.cDefColoursCycle.empty() ? 0.0f : (std::rand() % (cDefParent.cDefColoursCycle.size() * 1000)) / 1000.0f;
  }

  void Jewel::Instance::update(unsigned int milliseconds) {
    cProgress += milliseconds * cDefParent.cDefCycleSpeed;
    while (cProgress >= cDefParent.cDefColoursCycle.size()) {
      cProgress -= cDefParent.cDefColoursCycle.size();
    }
  }

  void Jewel::Instance::render() const {
    glBindTexture(GL_TEXTURE_2D, 0);
    cDefParent.cDefColourFrame->set();
    glCallList(cFrameDisplayList);
    unsigned int mPrevious = cProgress;
    unsigned int mNext     = mPrevious != cDefParent.cDefColoursCycle.size() - 1 ? cProgress + 1 : 0;
    float mCurrent         = cProgress - mPrevious;
    LiteralColour mColourPanel(***cDefParent.cDefColoursCycle[mPrevious]->getColour(), ***cDefParent.cDefColoursCycle[mNext]->getColour(), mCurrent);
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
