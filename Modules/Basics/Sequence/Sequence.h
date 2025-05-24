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
#include "IsoRealms/Types.h"
#include "IsoRealms/Utils.h"

#include "Modules/Basics/Assets/Client/SequenceTrack.h"

#include "Editor/SequenceEditor.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a sequence.  The Sequence supports various types
   * of track that play in tandem, each of which may do something different.
   */
  class Sequence final : public IEditable {
    public:
    Sequence(IProject& project, Basics& basics, IResourceData& data);
    Sequence(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish);
    void save(JSONObject object, IAssetIdentifier& identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IAssetBrowser& browser, IAssetRegistry& assets);

    /************************\
     * Implements IEditable *
    \************************/
    IEditableScreen* createEditableScreen(IsoRealms::Project* project) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    /***********************\
     * Scripting Interface *
    \***********************/
    void play();
    void pause();
    void reset();
    void stopPreview();
    void setPreviewPosition(long position);
    void preview(unsigned int milliseconds);
    void skip(unsigned int milliseconds);

    /*********************\
     * Editing Interface *
    \*********************/
    IProject& getProject() const;
    unsigned int getTrackCount() const;
    SequenceTrack& getTrack(unsigned int track) const;
    unsigned int getDuration() const;
    void addTrack();
    
    private:

    class Length : public IString {
      public:
      Length(Sequence& parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      Sequence& cParent;
    };

    class Position : public IString {
      public:
      Position(Sequence& parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      Sequence& cParent;
    };

    // JSON members.
    static const std::string JSON_LOOP;
    static const std::string JSON_PLAYING;
    static const std::string JSON_TRACKS;
    static const std::string JSON_TRACK;
    static const std::string JSON_TYPE;

    // External interfaces.
    Basics& cBasics;

    // Definition data.
    std::vector<std::unique_ptr<SequenceTrack>> cDefTracks; /// Tracks in this sequence.
    bool cDefPlaying;                                       /// Initial state of this sequence.
    bool cDefLoop;                                          /// Sequence should loop upon reaching the end.

    // Runtime data.
    bool cRuntimePlaying; /// Current state of this sequence.
    int cRuntimePosition; /// Current position of this sequence.

    // Editing data.
    bool cEditorPlaying; /// Current editor state of this sequence.
    std::map<IEditableScreen*, std::unique_ptr<SequenceEditor>> cEditors;

    // Exposed data.
    Length cExposedLength;
    Position cExposedPosition;

    // Scripting interface.
    LuaBinding<Sequence> cLuaBinding;
  };
}
