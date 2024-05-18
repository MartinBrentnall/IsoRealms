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
#pragma once

#include <iostream>
#include <vector>

#include "IsoRealms/Lua.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Utils.h"

#include "ActionTrack.h"
#include "ColourTrack.h"
#include "ISequenceTrack.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a sequence.  The Sequence supports various types
   * of track that play in tandem, each of which may do something different.
   */
  class Sequence final {
    public:
    Sequence(IProject* project, Basics* basics);
    Sequence(IProject* project, Basics* basics, JSONObject object, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(JSONObject object, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon();
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    /***********************\
     * Scripting Interface *
    \***********************/
    void play();
    void pause();
    void reset();
    void skip(unsigned int milliseconds);

    /*********************\
     * Editing Interface *
    \*********************/
    unsigned int getTrackCount() const;
    ISequenceTrack* getTrack(unsigned int index) const;
    unsigned int getDuration() const;
    
    private:

    // JSON members.
    static const std::string JSON_LOOP;
    static const std::string JSON_PLAYING;
    static const std::string JSON_TRACKS;
    static const std::string JSON_TYPE;

    // Definition data.
    std::vector<std::unique_ptr<ISequenceTrack>> cDefTracks; /// Tracks in this sequence.
    bool cDefPlaying;                                        /// Initial state of this sequence.
    bool cDefLoop;                                           /// Sequence should loop upon reaching the end.

    // Runtime data.
    bool cRuntimePlaying; /// Current state of this sequence.

    // Scripting interface.
    LuaBinding<Sequence> cLuaBinding;
  };
}
