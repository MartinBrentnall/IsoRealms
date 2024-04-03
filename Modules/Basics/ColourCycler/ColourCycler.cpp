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
#include "ColourCycler.h"

namespace IsoRealms::Basics {
  const std::string ColourCycler::TAG_CYCLE_SPEED   = "CycleSpeed";
  const std::string ColourCycler::TAG_INPUT_COLOUR  = "InputColour";
  const std::string ColourCycler::TAG_IO            = "IO";
  const std::string ColourCycler::TAG_OUTPUT_COLOUR = "OutputColour";

  const std::string ColourCycler::ATTRIBUTE_OFFSET           = "offset";
  const std::string ColourCycler::ATTRIBUTE_SPEED_MULTIPLIER = "speedMultiplier";

  ColourCycler::ColourCycler(IProject* project, Basics* basics) :
            cDefCycleSpeed(project, 0.0f) {
    project->updateRuntime([this](unsigned int milliseconds) {
      update(milliseconds);
    });
    
    project->updateEditing([this](unsigned int milliseconds) {
      update(milliseconds);
    });    
    
    project->reset([this]() {
      for (std::unique_ptr<ColourCycle>& mColourCycle : cDefOutputColours) {
        mColourCycle->reset();
      }
    });
  }

  ColourCycler::ColourCycler(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            ColourCycler(project, basics) {
    unsigned int mOutputColourCount = 0;
    for (DOMNode& mChild : node.getNode(TAG_IO)) {
      std::string mChildName = mChild.getName();
      if (mChildName == TAG_INPUT_COLOUR) {
        cDefInputColours.emplace_back(std::make_unique<Colour>(project, 0.0f, 0.0f, 0.0f));
        cDefInputColours.back()->init(mChild);
      } else if (mChildName == TAG_OUTPUT_COLOUR) {
        cDefOutputColours.push_back(std::make_unique<ColourCycle>(this, mChild.getFloatAttribute(ATTRIBUTE_OFFSET), mChild.getFloatAttribute(ATTRIBUTE_SPEED_MULTIPLIER)));
        mOutputColourCount++;
      } else {
        throw ResourceInitException("ERROR: ColourCycler::ColourCycler: Unknown tag in IO: " + mChildName);
      }
    }
    cDefCycleSpeed.init(node.getNode(TAG_CYCLE_SPEED));
  }

  void ColourCycler::registerAssets(IAssetRegistry* assets) {
    for (unsigned int i = 0; i < cDefOutputColours.size(); i++) {
      std::string mOutputColourName = std::to_string(i);
      cDefOutputColours[i]->registerAssets(assets, mOutputColourName);
    }
  }
  
  void ColourCycler::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    for (std::unique_ptr<ColourCycle>& mOutputColour :cDefOutputColours) {
      mOutputColour->unregisterAssets(assets);
    }
  }
  
  void ColourCycler::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefCycleSpeed.save(node, TAG_CYCLE_SPEED);
    DOMNodeWriter mIONode = node->addBranch(TAG_IO);
    for (const std::unique_ptr<Colour>& mInputColour : cDefInputColours) {
      mInputColour->save(&mIONode, TAG_INPUT_COLOUR);
    }
    for (const std::unique_ptr<ColourCycle>& mOutputColour : cDefOutputColours) {
      mOutputColour->save(&mIONode);
    }
  }

  void ColourCycler::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool ColourCycler::renderIcon() {
    float mCircleResolution = static_cast<float>(  5.0f * (M_PI / 180.0f));
    float mStartAngle       = static_cast<float>(  0.0f * (M_PI / 180.0f));
    float mEndAngle         = static_cast<float>(360.0f * (M_PI / 180.0f));
    glBegin(GL_TRIANGLE_FAN);
    cDefOutputColours[0]->set();
    glVertex2f(0.0f, 0.0f);
    for (float angle = mEndAngle; angle >= mStartAngle; angle -= mCircleResolution) {
      glVertex2f(std::sin(angle) * 1.0f, std::cos(angle) * 1.0f);
    }
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (float angle = mEndAngle; angle >= mStartAngle; angle -= mCircleResolution) {
      glVertex2f(std::sin(angle) * 1.0f, std::cos(angle) * 1.0f);
    }
    glEnd();
    return true;
  }

  std::vector<IProperty*> ColourCycler::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  void ColourCycler::update(unsigned int milliseconds) {
    for (std::unique_ptr<ColourCycle>& mOutputColour : cDefOutputColours) {
      mOutputColour->update(milliseconds);
    }
  }

  ColourCycler::ColourCycle::ColourCycle(ColourCycler* parent, float startPosition, float speedMultiplier) :
          cParent(parent),
          cDeffStartPosition(startPosition),
          cDefSpeedMultiplier(speedMultiplier == 0.0f ? 1.0f : speedMultiplier),
          cRuntimePosition(cDeffStartPosition * cParent->cDefInputColours.size()),
          cStateNotifier(nullptr) {
  }

  void ColourCycler::ColourCycle::registerAssets(IAssetRegistry* assets, const std::string& name) {
    cStateNotifier = assets->add(this, name, "Cycling Colours");
  }
  
  void ColourCycler::ColourCycle::unregisterAssets(IAssetRemover* assets) {
    assets->remove(this);
  }
  
  void ColourCycler::ColourCycle::save(DOMNodeWriter* node) {
    DOMNodeWriter mOutputColourNode = node->addBranch(TAG_OUTPUT_COLOUR);
    mOutputColourNode.addAttribute(ATTRIBUTE_OFFSET, cDeffStartPosition);
    mOutputColourNode.addAttribute(ATTRIBUTE_SPEED_MULTIPLIER, cDefSpeedMultiplier);
  }

  void ColourCycler::ColourCycle::update(unsigned int milliseconds) {
    cRuntimePosition += milliseconds * std::max(0.0f, cParent->cDefCycleSpeed->getValue() * cDefSpeedMultiplier);
    while (cRuntimePosition >= cParent->cDefInputColours.size()) {
      cRuntimePosition -= cParent->cDefInputColours.size();
    }
    calculateColour();
  }

  void ColourCycler::ColourCycle::reset() {
    cRuntimePosition = cDeffStartPosition * cParent->cDefInputColours.size();
    calculateColour();
  }

  float ColourCycler::ColourCycle::getRed() const {
    return cRuntimeRed;
  }

  float ColourCycler::ColourCycle::getGreen() const {
    return cRuntimeGreen;
  }

  float ColourCycler::ColourCycle::getBlue() const {
    return cRuntimeBlue;
  }

  float ColourCycler::ColourCycle::getAlpha() const {
    return cRuntimeAlpha;
  }
  
  void ColourCycler::ColourCycle::set() const {
    glColor4f(cRuntimeRed, cRuntimeGreen, cRuntimeBlue, cRuntimeAlpha);
  }

  void ColourCycler::ColourCycle::calculateColour() {
    unsigned int mPrevious = static_cast<unsigned int>(cRuntimePosition);
    unsigned int mNext     = mPrevious != cParent->cDefInputColours.size() - 1 ? mPrevious + 1 : 0;
    std::unique_ptr<Colour>& mFrom = cParent->cDefInputColours[mPrevious];
    std::unique_ptr<Colour>& mTo   = cParent->cDefInputColours[mNext];
    float mWeight          = static_cast<float>(cRuntimePosition - mPrevious);
    cRuntimeRed            = mFrom->getRed()   + (mTo->getRed()   - mFrom->getRed())   * mWeight;
    cRuntimeGreen          = mFrom->getGreen() + (mTo->getGreen() - mFrom->getGreen()) * mWeight;
    cRuntimeBlue           = mFrom->getBlue()  + (mTo->getBlue()  - mFrom->getBlue())  * mWeight;
    cRuntimeAlpha          = mFrom->getAlpha() + (mTo->getAlpha() - mFrom->getAlpha()) * mWeight;
    cStateNotifier->stateChanged(this);
  }
}
