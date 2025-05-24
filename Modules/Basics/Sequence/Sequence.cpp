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
#include "Sequence.h"

#include "Modules/Basics/Basics.h"

namespace IsoRealms::Basics {
  const std::string Sequence::JSON_LOOP    = "loop";
  const std::string Sequence::JSON_PLAYING = "playing";
  const std::string Sequence::JSON_TRACKS  = "tracks";
  const std::string Sequence::JSON_TRACK  = "track";
  const std::string Sequence::JSON_TYPE    = "type";

  Sequence::Sequence(IProject& project, Basics& basics, IResourceData& data) :
            cBasics(basics),
            cDefPlaying(false),
            cDefLoop(false),
            cExposedLength(*this),
            cExposedPosition(*this),
            cLuaBinding(project, this) {
    project.updateRuntime([this](unsigned int milliseconds) {
      skip(milliseconds);
    });    
    
    project.updateEditing([this](unsigned int milliseconds) {
      for (const std::pair<IEditableScreen* const, std::unique_ptr<SequenceEditor>>& mEditor : cEditors) {
        mEditor.second->updateScreen(milliseconds);
      }

    });

    project.reset([this]() {
      reset(); 
    });
  }
  
  Sequence::Sequence(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            Sequence(project, basics, data) {
    cDefPlaying = object.getBoolean(JSON_PLAYING);
    cDefLoop = object.getBoolean(JSON_LOOP);
    for (JSONObject mTrackObject : object.getArray(JSON_TRACKS)) {
      cDefTracks.emplace_back(std::make_unique<SequenceTrack>(basics, *this));
      cDefTracks.back()->set(mTrackObject, JSON_TRACK);
    }
  }

  void Sequence::registerAssets(IAssetRegistry& assets) {
    assets.add(&cLuaBinding, "", "Sequences");
    assets.add(&cExposedLength, "Length", "Sequences");
    assets.add(&cExposedPosition, "Position", "Sequences");
    for (std::unique_ptr<SequenceTrack>& mTrack : cDefTracks) {
      (*mTrack)->registerAssets(assets);
    }
  }
  
  void Sequence::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    assets.remove(&cLuaBinding, relinquish);
    assets.remove(&cExposedLength, relinquish);
    assets.remove(&cExposedPosition, relinquish);
    for (std::unique_ptr<SequenceTrack>& mTrack : cDefTracks) {
      (*mTrack)->unregisterAssets(assets, relinquish);
    }
  }
  
  void Sequence::save(JSONObject object, IAssetIdentifier& identifier) const {
    object.addBoolean(JSON_PLAYING, cDefPlaying);
    object.addBoolean(JSON_LOOP, cDefLoop);
    JSONArray mTrackArray = object.addArray(JSON_TRACKS);
    for (const std::unique_ptr<SequenceTrack>& mTrack : cDefTracks) {
      JSONObject mTrackObject = mTrackArray.addObject();
      mTrack->save(mTrackObject, JSON_TRACK);
    }
  }

  void Sequence::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool Sequence::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> Sequence::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyEditor>("Content", this));
    return mProperties;
  }

  IEditableScreen* Sequence::createEditableScreen(IsoRealms::Project* project) {
    std::unique_ptr<SequenceEditor> mScreen = std::make_unique<SequenceEditor>(*this);
    IEditableScreen* mReturnValue = mScreen.get();
    cEditors[mReturnValue] = std::move(mScreen);
    return mReturnValue;
  }

  bool Sequence::renderAssetIcon() const {
    return renderIcon();
  }

  void Sequence::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Sequence::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Sequence::isDefaultConfiguration() const {
    return true;
  }

  void Sequence::play() {
    cRuntimePlaying = true;
  }

  void Sequence::pause() {
    cRuntimePlaying = false;
  }

  void Sequence::reset() {
    cRuntimePlaying = cDefPlaying;
    cRuntimePosition = 0;
    for (std::unique_ptr<SequenceTrack>& mTrack : cDefTracks) {
      (*mTrack)->reset();
    }
  }

  void Sequence::stopPreview() {
    for (std::unique_ptr<SequenceTrack>& mTrack : cDefTracks) {
      (*mTrack)->stopPreview();
    }
  }

  void Sequence::setPreviewPosition(long position) {
    for (std::unique_ptr<SequenceTrack>& mTrack : cDefTracks) {
      (*mTrack)->setPreviewPosition(position);
    }
  }
  
  void Sequence::preview(unsigned int milliseconds) {
    for (std::unique_ptr<SequenceTrack>& mTrack : cDefTracks) {
      (*mTrack)->play(milliseconds);
    }
  }

  void Sequence::skip(unsigned int milliseconds) {
    if (cRuntimePlaying) {
      cRuntimePosition += milliseconds;
      bool mSequenceFinished = true;
      for (std::unique_ptr<SequenceTrack>& mTrack : cDefTracks) {
        if ((*mTrack)->play(milliseconds)) {
          mSequenceFinished = false;
        }
      }
      
      if (mSequenceFinished && cDefLoop) {
        for (std::unique_ptr<SequenceTrack>& mTrack : cDefTracks) {
          cRuntimePosition = 0;
          (*mTrack)->reset();
        }
      }
    }
  }

  IProject& Sequence::getProject() const {
    return cBasics.getProject();
  }

  unsigned int Sequence::getTrackCount() const {
    return static_cast<unsigned int>(cDefTracks.size());
  }

  SequenceTrack& Sequence::getTrack(unsigned int track) const {
    return *cDefTracks[track].get();
  }

  unsigned int Sequence::getDuration() const {
    unsigned int mDuration = 0;
    for (const std::unique_ptr<SequenceTrack>& mTrack : cDefTracks) {
      mDuration = std::max(mDuration, (*mTrack)->getDuration());
    }
    return mDuration;
  }

  void Sequence::addTrack() {
    cDefTracks.emplace_back(std::make_unique<SequenceTrack>(cBasics, *this));
  }

  Sequence::Length::Length(Sequence& parent) :
            cParent(parent) {
  }

  std::string Sequence::Length::getValue() const {
    int mLength = cParent.getDuration();
    int mMilliseconds = mLength % 1000;
    int mSeconds = mLength / 1000;
    int mMinutes = mSeconds / 60;
    int mHours   = mMinutes / 60;
    mMinutes = mMinutes % 60;
    mSeconds = mSeconds % 60;
    std::stringstream mStringStream;
    mStringStream << mHours << ":" << std::setfill('0') << std::setw(2) << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setfill('0') << std::setw(2) << (mMilliseconds / 10);
    return mStringStream.str();
  }

  bool Sequence::Length::renderAssetIcon() const {
    return false;
  }

  void Sequence::Length::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Sequence::Length::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Sequence::Length::isDefaultConfiguration() const {
    return true;
  }

  Sequence::Position::Position(Sequence& parent) :
            cParent(parent) {
  }

  std::string Sequence::Position::getValue() const {
    int mMilliseconds = cParent.cRuntimePosition % 1000;
    int mSeconds = cParent.cRuntimePosition / 1000;
    int mMinutes = mSeconds / 60;
    int mHours   = mMinutes / 60;
    mMinutes = mMinutes % 60;
    mSeconds = mSeconds % 60;
    std::stringstream mStringStream;
    mStringStream << mHours << ":" << std::setfill('0') << std::setw(2) << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setfill('0') << std::setw(2) << (mMilliseconds / 10);
    return mStringStream.str();
  }

  bool Sequence::Position::renderAssetIcon() const {
    return false;
  }

  void Sequence::Position::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Sequence::Position::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Sequence::Position::isDefaultConfiguration() const {
    return true;
  }
}
