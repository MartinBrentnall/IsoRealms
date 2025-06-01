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
  const std::string ColourCycler::JSON_COLOUR           = "colour";
  const std::string ColourCycler::JSON_INPUTS           = "inputs";
  const std::string ColourCycler::JSON_OFFSET           = "offset";
  const std::string ColourCycler::JSON_OUTPUTS          = "outputs";
  const std::string ColourCycler::JSON_SPEED            = "speed";
  const std::string ColourCycler::JSON_SPEED_MULTIPLIER = "speedMultiplier";

  ColourCycler::ColourCycler(IProject& project, Basics& basics, IResourceData& data) :
            cProject(project),
            cDefCycleSpeed(project, 0.0f) {
    project.updateRuntime([this](unsigned int milliseconds) {
      update(milliseconds);
    });
    
    project.updateEditing([this](unsigned int milliseconds) {
      update(milliseconds);
    });
    
    project.reset([this]() {
      for (std::unique_ptr<ColourCycle>& mColourCycle : cDefOutputColours) {
        mColourCycle->reset();
      }
    });

    // There must be at least one input.
    cDefInputColours.emplace_back(std::make_unique<Colour>(project, 0.0f, 0.0f, 0.0f));
    cDefOutputColours.emplace_back(std::make_unique<ColourCycle>(*this, 0.0f, 1.0f));
  }

  ColourCycler::ColourCycler(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            ColourCycler(project, basics, data) {
    cDefInputColours.clear();
    cDefOutputColours.clear();
    for (JSONObject mInputObject : object.getArray(JSON_INPUTS)) {
      cDefInputColours.emplace_back(std::make_unique<Colour>(project, 0.0f, 0.0f, 0.0f));
      cDefInputColours.back()->init(mInputObject, JSON_COLOUR);
    }
    for (JSONObject mOutputObject : object.getArray(JSON_OUTPUTS)) {
      cDefOutputColours.emplace_back(std::make_unique<ColourCycle>(*this, mOutputObject.getFloat(JSON_OFFSET), mOutputObject.getFloat(JSON_SPEED_MULTIPLIER)));
    }
    cDefCycleSpeed.init(object, JSON_SPEED);
  }

  void ColourCycler::registerAssets(IAssetRegistry& assets) {
    for (unsigned int i = 0; i < cDefOutputColours.size(); i++) {
      std::string mOutputColourName = std::to_string(i);
      cDefOutputColours[i]->registerAssets(assets, mOutputColourName);
    }
  }
  
  void ColourCycler::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    for (std::unique_ptr<ColourCycle>& mOutputColour : cDefOutputColours) {
      mOutputColour->unregisterAssets(assets, relinquish);
    }
  }
  
  void ColourCycler::save(JSONObject object, IAssetIdentifier& identifier) const {
    cDefCycleSpeed.save(object, JSON_SPEED);
    JSONArray mInputsArray = object.addArray(JSON_INPUTS);
    for (const std::unique_ptr<Colour>& mInputColour : cDefInputColours) {
      JSONObject mInputObject = mInputsArray.addObject();
      mInputColour->save(mInputObject, JSON_COLOUR);
    }
    JSONArray mOutputsArray = object.addArray(JSON_OUTPUTS);
    for (const std::unique_ptr<ColourCycle>& mOutputColour : cDefOutputColours) {
      JSONObject mOutputObject = mOutputsArray.addObject();
      mOutputColour->save(mOutputObject);
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

  std::vector<std::unique_ptr<IProperty>> ColourCycler::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Float>>("Cycle Speed", "TODO", cDefCycleSpeed));
    for (unsigned int i = 0; i < cDefInputColours.size(); i++) {
      mProperties.emplace_back(std::make_unique<PropertyAsset<Colour>>("Colour " + Utils::toString(i + 1), "TODO", *cDefInputColours[i].get(), [this, i]() {
        cDefInputColours.erase(cDefInputColours.begin() + i);
      }));
    }
    mProperties.emplace_back(std::make_unique<PropertyAdd>("Colour ?", "TODO", "Add...", [this]() {
      cDefInputColours.emplace_back(std::make_unique<Colour>(cProject, 0.0f, 0.0f, 0.0f));
      return std::make_unique<PropertyAsset<Colour>>("Colour " + Utils::toString(static_cast<int>(cDefInputColours.size() + 1)), "TODO", *cDefInputColours[cDefInputColours.size() - 1].get());
    }));
    for (unsigned int i = 0; i < cDefOutputColours.size(); i++) {
      mProperties.emplace_back(std::make_unique<PropertyStruct>("Output " + Utils::toString(i + 1), "TODO", "Edit...", [this, i]() {
        return cDefOutputColours[i]->getProperties();
      }, [this, i]() {
        cDefOutputColours.erase(cDefOutputColours.begin() + i);
//        mOutputColour->unregisterAssets(assets); // TODO
      }));
    }
    mProperties.emplace_back(std::make_unique<PropertyAdd>("Output ?", "TODO", "Add...", [this]() {
      cDefOutputColours.emplace_back(std::make_unique<ColourCycle>(*this, 0.0f, 1.0f));
      return std::make_unique<PropertyStruct>("Output " + Utils::toString(static_cast<int>(cDefOutputColours.size() + 1)), "TODO", "Edit...", [this]() {
        return cDefOutputColours[cDefOutputColours.size() + 1]->getProperties();
      });
    }));
    return mProperties;
  }

  void ColourCycler::update(unsigned int milliseconds) {
    for (std::unique_ptr<ColourCycle>& mOutputColour : cDefOutputColours) {
      mOutputColour->update(milliseconds);
    }
  }

  ColourCycler::ColourCycle::ColourCycle(ColourCycler& parent, float startPosition, float speedMultiplier) :
          cParent(parent),
          cDefStartPosition(startPosition),
          cDefSpeedMultiplier(speedMultiplier == 0.0f ? 1.0f : speedMultiplier),
          cRuntimePosition(cDefStartPosition * cParent.cDefInputColours.size()),
          cStateNotifier(nullptr) {
  }

  void ColourCycler::ColourCycle::registerAssets(IAssetRegistry& assets, const std::string& name) {
    cStateNotifier = assets.add(this, name, "Cycling Colours");
  }
  
  void ColourCycler::ColourCycle::unregisterAssets(IAssetRemover& assets, bool relinquish) {
    assets.remove(this, relinquish);
  }
  
  void ColourCycler::ColourCycle::save(JSONObject object) const {
    object.addFloat(JSON_OFFSET, cDefStartPosition);
    object.addFloat(JSON_SPEED_MULTIPLIER, cDefSpeedMultiplier);
  }

  void ColourCycler::ColourCycle::update(unsigned int milliseconds) {
    cRuntimePosition += milliseconds * std::max(0.0f, cParent.cDefCycleSpeed->getValue() * cDefSpeedMultiplier);
    while (cRuntimePosition >= cParent.cDefInputColours.size()) {
      cRuntimePosition -= cParent.cDefInputColours.size();
    }
    calculateColour();
  }

  void ColourCycler::ColourCycle::reset() {
    cRuntimePosition = cDefStartPosition * cParent.cDefInputColours.size();
    calculateColour();
  }

  std::vector<std::unique_ptr<IProperty>> ColourCycler::ColourCycle::getProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Phase Offset",     "TODO", [this]() {return cDefStartPosition;},   [this](float value) {cDefStartPosition   = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Speed Multiplier", "TODO", [this]() {return cDefSpeedMultiplier;}, [this](float value) {cDefSpeedMultiplier = value; return true;}));
    return mProperties;
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

  void ColourCycler::ColourCycle::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> ColourCycler::ColourCycle::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ColourCycler::ColourCycle::isDefaultConfiguration() const {
    return true;
  }

  void ColourCycler::ColourCycle::set() const {
    glColor4f(cRuntimeRed, cRuntimeGreen, cRuntimeBlue, cRuntimeAlpha);
  }

  void ColourCycler::ColourCycle::calculateColour() {
    unsigned int mPrevious = static_cast<unsigned int>(cRuntimePosition);
    unsigned int mNext     = mPrevious != cParent.cDefInputColours.size() - 1 ? mPrevious + 1 : 0;
    std::unique_ptr<Colour>& mFrom = cParent.cDefInputColours[mPrevious];
    std::unique_ptr<Colour>& mTo   = cParent.cDefInputColours[mNext];
    float mWeight          = static_cast<float>(cRuntimePosition - mPrevious);
    cRuntimeRed            = (*mFrom.get())->getRed()   + ((*mTo.get())->getRed()   - (*mFrom.get())->getRed())   * mWeight;
    cRuntimeGreen          = (*mFrom.get())->getGreen() + ((*mTo.get())->getGreen() - (*mFrom.get())->getGreen()) * mWeight;
    cRuntimeBlue           = (*mFrom.get())->getBlue()  + ((*mTo.get())->getBlue()  - (*mFrom.get())->getBlue())  * mWeight;
    cRuntimeAlpha          = (*mFrom.get())->getAlpha() + ((*mTo.get())->getAlpha() - (*mFrom.get())->getAlpha()) * mWeight;
    if (cStateNotifier != nullptr) { // TODO: State notifier should be set after editor adds another, so this shouldn't be needed.'
      cStateNotifier->stateChanged(this);
    }
  }
}
