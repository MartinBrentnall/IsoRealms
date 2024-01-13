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
#include "Jewel.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const std::string Jewel::TAG_COLOUR_CYCLE = "ColourCycle";
  const std::string Jewel::TAG_FRAME_COLOUR = "FrameColour";

  const std::string Jewel::ATTRIBUTE_CYCLE_SPEED = "cycleSpeed";

  Jewel::Jewel(IProject* project, Spindizzy* spindizzy) :
            cColourFrame(project, 1.0f, 1.0f, 0.0f) {
    cEngine = project;
    cSampleModel = std::make_unique<Instance>(this, cEngine);

    project->updateEditing([this](unsigned int milliseconds) {
      cSampleModel->update(milliseconds);
    });
  }
  
  Jewel::Jewel(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data) :
            Jewel(project, spindizzy) {
    cCycleSpeed = node.getFloatAttribute(ATTRIBUTE_CYCLE_SPEED);
    cColourFrame.init(node.getNode(TAG_FRAME_COLOUR));
    for (DOMNode& mNode : node) {
      std::string mChildName = mNode.getName();
      if (mChildName == TAG_COLOUR_CYCLE) {
        cColoursCycle.emplace_back(std::make_unique<CycleColour>(this, project, mNode));
      } else if (mChildName == TAG_FRAME_COLOUR) {
        // Nothing to do.
      } else {
        throw ResourceInitException("Unknown child: " + mChildName);
      }
    }
  }

  std::vector<IProperty*> Jewel::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }
  
  void Jewel::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cColourFrame.save(node, TAG_FRAME_COLOUR);
    node->addAttribute(ATTRIBUTE_CYCLE_SPEED, cCycleSpeed);
    for (const std::unique_ptr<CycleColour>& mCycleColour : cColoursCycle) {
      mCycleColour->save(node, identifier);
    }
  }

  I3DModel* Jewel::createModel() {
    return cInstances.emplace_back(std::make_unique<Instance>(this, cEngine)).get();
  }

  bool Jewel::renderIcon() const {
    glTranslatef(0.0f, -0.75f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_YAW,   0.0f, 0.0f, 1.0f);
    glScalef(2.5f, 2.5f, 2.5f);
    cSampleModel->render();
    return true;
  }

  bool Jewel::renderAssetIcon() const {
    return renderIcon();
  }

  void Jewel::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  void Jewel::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Spindizzy Jewel Models");
  }
    
  void Jewel::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
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
  
  Jewel::CycleColour::CycleColour(Jewel* parent, IProject* project, DOMNode& node) :
            cParent(parent),
            cDefColour(project, 1.0f, 0.0f, 1.0f) {
    cDefColour.init(node);
  }
  
  void Jewel::CycleColour::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefColour.save(node, TAG_COLOUR_CYCLE);
  }
  
  const IColour* Jewel::CycleColour::getColour() const {
    return &cDefColour;
  }
  
  bool Jewel::CycleColour::operator==(const CycleColour& cycleColour) const {
    return false;
  }
  
  unsigned int Jewel::Instance::cReferenceCount = 0;
  GLuint Jewel::Instance::cPanelDisplayList;
  GLuint Jewel::Instance::cFrameDisplayList;

  /****************\
   * Constructors *
  \****************/
  Jewel::Instance::Instance(Jewel* parent, IProject* engine) :
            cDefParent(parent) {
    
    // If this is the first model instance, we need to create some display lists.
    if (cReferenceCount == 0) {
      engine->mainThreadInit([this]() {
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
    cProgress = cDefParent->cColoursCycle.empty() ? 0.0f : (std::rand() % (cDefParent->cColoursCycle.size() * 1000)) / 1000.0f;
  }

  void Jewel::Instance::update(unsigned int milliseconds) {
    cProgress += milliseconds * cDefParent->cCycleSpeed;
    while (cProgress >= cDefParent->cColoursCycle.size()) {
      cProgress -= cDefParent->cColoursCycle.size();
    }
  }

  void Jewel::Instance::render() const {
    glBindTexture(GL_TEXTURE_2D, 0);
    cDefParent->cColourFrame.set();
    glCallList(cFrameDisplayList);
    unsigned int mPrevious = cProgress;
    unsigned int mNext     = mPrevious != cDefParent->cColoursCycle.size() - 1 ? cProgress + 1 : 0;
    float mCurrent         = cProgress - mPrevious;
    LiteralColour mColourPanel(*cDefParent->cColoursCycle[mPrevious]->getColour(), *cDefParent->cColoursCycle[mNext]->getColour(), mCurrent);
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
