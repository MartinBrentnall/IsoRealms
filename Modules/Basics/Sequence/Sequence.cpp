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

namespace IsoRealms::Basics {
  const std::string Sequence::TAG_TRACK         = "Track";

  const std::string Sequence::ATTRIBUTE_LOOP    = "loop";
  const std::string Sequence::ATTRIBUTE_PLAYING = "playing";
  const std::string Sequence::ATTRIBUTE_TYPE    = "type";

  Sequence::Sequence(IProject* project, Basics* basics) :
            cDefPlaying(false),
            cDefLoop(false),
            cLuaBinding(project, this) {
    project->updateRuntime([this](unsigned int milliseconds) {
      skip(milliseconds);
    });    
    
    project->reset([this]() {
      reset(); 
    });
  }
  
  Sequence::Sequence(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            Sequence(project, basics) {
    cDefPlaying = node.getBooleanAttribute(ATTRIBUTE_PLAYING);
    cDefLoop = node.getBooleanAttribute(ATTRIBUTE_LOOP);
    for (DOMNode& mChild : node) {
      std::string mSequenceTrackType = mChild.getName();
      if (mSequenceTrackType == ActionTrack::TYPE_NAME) {
        cDefTracks.emplace_back(std::make_unique<ActionTrack>(mChild, project));
      } else if (mSequenceTrackType == ColourTrack::TYPE_NAME) {
        cDefTracks.emplace_back(std::make_unique<ColourTrack>(mChild, project));
      } else {
        throw ParseException("Unknown type for Basics/Sequence: " + mSequenceTrackType);
      }
    }
  }

  void Sequence::registerAssets(IAssetRegistry* assets) {
    assets->add(&cLuaBinding, "", "Sequences");
    for (std::unique_ptr<ISequenceTrack>& mTrack : cDefTracks) {
      mTrack->registerAssets(assets);
    }
  }
  
  void Sequence::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(&cLuaBinding);
    for (std::unique_ptr<ISequenceTrack>& mTrack : cDefTracks) {
      mTrack->unregisterAssets(assets);
    }
  }
  
  void Sequence::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute(ATTRIBUTE_PLAYING, cDefPlaying);
    node->addAttribute(ATTRIBUTE_LOOP, cDefLoop);
    for (const std::unique_ptr<ISequenceTrack>& mTrack : cDefTracks) {
      mTrack->save(node);
    }
  }

  void Sequence::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool Sequence::renderIcon() {
    return false;
  }

  std::vector<IProperty*> Sequence::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }
  void Sequence::play() {
    cRuntimePlaying = true;
  }

  void Sequence::pause() {
    cRuntimePlaying = false;
  }

  void Sequence::reset() {
    cRuntimePlaying = cDefPlaying;
    for (std::unique_ptr<ISequenceTrack>& mTrack : cDefTracks) {
      mTrack->reset();
    }
  }
  
  void Sequence::skip(unsigned int milliseconds) {
    if (cRuntimePlaying) {
      bool mSequenceFinished = true;
      for (std::unique_ptr<ISequenceTrack>& mTrack : cDefTracks) {
        if (mTrack->play(milliseconds)) {
          mSequenceFinished = false;
        }
      }
      
      if (mSequenceFinished && cDefLoop) {
        for (std::unique_ptr<ISequenceTrack>& mTrack : cDefTracks) {
          mTrack->reset();
        }
      }
    }
  }

  unsigned int Sequence::getTrackCount() const {
    return static_cast<unsigned int>(cDefTracks.size());
  }

  ISequenceTrack* Sequence::getTrack(unsigned int index) const {
    return cDefTracks[index].get();
  }

  unsigned int Sequence::getDuration() const {
    unsigned int mDuration = 0;
    for (const std::unique_ptr<ISequenceTrack>& mTrack : cDefTracks) {
      mDuration = std::max(mDuration, mTrack->getDuration());
    }
    return mDuration;
  }
}
