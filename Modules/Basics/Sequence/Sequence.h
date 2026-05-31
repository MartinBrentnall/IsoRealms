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
#pragma once

#include <iostream>
#include <vector>

#include "IsoRealms.h"

#include "Modules/Basics/Assets/Client/SequenceTrack.h"

#include "Editor/SequenceEditor.h"
#include "SequenceInstance.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a sequence.  The Sequence supports various types
   * of track that play in tandem, each of which may do something different.
   */
  class Sequence final : public IEditable {
    public:
    Sequence(Basics& basics, IResourceData& data);
    Sequence(Basics& basics, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    /***************************\
     * Asset client interfaces *
    \***************************/
    Basics& getAssetManager();
    IsoRealms::Project& getProject() const;
    bool isReadOnly() const;
    void setOwner(ProjectFile* owner);

    /*********************\
     * Module interfaces *
    \*********************/
    void updateRuntime(unsigned int milliseconds);
    void updateEditing(unsigned int milliseconds);
    void reset();

    Basics& getBasics() const;
    IResourceData& getResourceData();
    bool isPlaying() const;
    bool isLooped() const;
    std::string getInstanceName(SequenceInstance& instance) const;
    void setInstanceName(SequenceInstance& instance, const std::string& name);
    bool isInstanceNameAllowed(SequenceInstance& instance, const std::string& name);

    /************************\
     * Implements IEditable *
    \************************/
    IEditableScreen* createEditableScreen(IsoRealms::Project* project, IDialogManager& dialogManager) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    void resetSequence();
    void stopPreview();
    void setPreviewPosition(long position);
    void preview(unsigned int milliseconds);

    /*********************\
     * Editing Interface *
    \*********************/
    unsigned int getTrackCount() const;
    unsigned int getTrackIndex(SequenceTrack& track) const;
    SequenceTrack& getTrack(unsigned int track) const;
    unsigned int getDuration() const;
    void addTrack();
    void refreshAssetRegistration();
    void deleteTrack(unsigned int track);
    void trackStateChanged(SequenceTrack& track);

    private:

    class Length : public IInteger {
      public:
      Length(Sequence& parent);

      /***********************\
        * Implements IInteger *
      \***********************/
      int getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(IPropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;

      private:
      Sequence& cParent;
    };

    // JSON members.
    inline static const std::string JSON_INSTANCES = "instances";
    inline static const std::string JSON_LOOP      = "loop";
    inline static const std::string JSON_NAME      = "name";
    inline static const std::string JSON_PLAYING   = "playing";
    inline static const std::string JSON_SPEED     = "speed";
    inline static const std::string JSON_TRACKS    = "tracks";
    inline static const std::string JSON_TRACK     = "track";
    inline static const std::string JSON_TYPE      = "type";

    // External interfaces.
    Basics& cBasics;
    IResourceData& cResourceData;

    // Definition data.
    std::vector<std::unique_ptr<SequenceTrack>> cDefTracks; /// Tracks in this sequence.
    bool cDefPlaying;                                       /// Initial state of this sequence.
    bool cDefLoop;                                          /// Sequence should loop upon reaching the end.
    Float cDefSpeed;                                        /// Play at multiple speed of specified value.
    std::map<std::string, std::unique_ptr<SequenceInstance>> cDefInstances;      /// Instances of this sequence.

    // Runtime data.
    float cRuntimePositionFraction;

    // Editing data.
    bool cEditorPlaying; /// Current editor state of this sequence.
    std::map<IEditableScreen*, std::unique_ptr<SequenceEditor>> cEditors;

    // Exposed data.
    Length cExposedLength;

    // Scripting interface.
    LuaBinding<Sequence> cLuaBinding;

    // Private functions.
    void process(unsigned int milliseconds);
  };
}
