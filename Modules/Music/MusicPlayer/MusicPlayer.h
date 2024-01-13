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

#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <vector>

#include <SFML/Audio.hpp>

#include "IsoRealms/Lua.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms::Music {
  class Music;

  class MusicPlayer {
    private:
    static const std::string TAG_SONG;
    static const std::string TAG_ACTION;
      
    static const std::string ATTRIBUTE_FADE;
    static const std::string ATTRIBUTE_FILE;
    static const std::string ATTRIBUTE_VOLUME;

    class Song {
      private:
      MusicPlayer* cParent;
      std::string cDefFile;
      Action cDefAction;
      
      sf::Music cMusic;
      
      int cRuntimeVolume;
      float cRuntimeActualVolume;
      
      void updateVolume();
      
      public:
      Song(IProject* project, MusicPlayer* parent, DOMNode& node);
      
      void startPlayback();
      void jump(int milliseconds);
      void pause();
      void unpause();
      
      void fadeIn(unsigned int milliseconds);
      void fadeOut(unsigned int milliseconds);
      bool hasFinished();
      bool isAboutToFinish();
      int getLength();
      int getPosition();
      std::string getName();
    };
    
    class TotalTracks : public IInteger {
      private:
      MusicPlayer* cParent;
        
      public:
      TotalTracks(MusicPlayer* parent);
      
      /***********************\
       * Implements IInteger *
      \***********************/
      int getValue() const override;
      bool renderAssetIcon() const override;
    };
    
    class SelectedTrack : public IInteger {
      private:
      MusicPlayer* cParent;
        
      public:
      SelectedTrack(MusicPlayer* parent);
      
      /***********************\
       * Implements IInteger *
      \***********************/
      int getValue() const override;
      bool renderAssetIcon() const override;
    };
    
    class SongLength : public IString {
      private:
      MusicPlayer* cParent;

      public:
      SongLength(MusicPlayer* parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
    };

    class SongPosition : public IString {
      private:
      MusicPlayer* cParent;

      public:
      SongPosition(MusicPlayer* parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
    };

    class TotalLength : public IString {
      private:
      MusicPlayer* cParent;

      public:
      TotalLength(MusicPlayer* parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
    };

    class TotalPosition : public IString {
      private:
      MusicPlayer* cParent;

      public:
      TotalPosition(MusicPlayer* parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
    };

    class Volume : public IString {
      private:
      MusicPlayer* cParent;

      public:
      Volume(MusicPlayer* parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
    };

    class TrackName : public IString {
      private:
      MusicPlayer* cParent;

      public:
      TrackName(MusicPlayer* parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
    };

    std::vector<std::unique_ptr<Song>> cDefSongs;
    int cDefFade;
    Float cDefVolume;
    
    std::vector<unsigned int> cRuntimeSongsFadingOut;
    unsigned int cRuntimeSelectedSong;
    bool cRuntimePlaying;
    
    // Expose our data
    TotalTracks cTotalTracks;
    SelectedTrack cSelectedTrack;
    SongLength cSongLength;
    SongPosition cSongPosition;
    TotalLength cTotalLength;
    TotalPosition cTotalPosition;
    Volume cVolume;
    TrackName cTrackName;
    
    LuaBinding<MusicPlayer> cLuaBinding;
    
    public:
        
    /**********************\
     * Resource Interface *
    \**********************/
    MusicPlayer(IProject* project, Music* music);
    MusicPlayer(IProject* project, Music* music, DOMNode& node, IOptions* options, IResourceData* data);
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    void registerAssets(IAssetRegistry* assets);  
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    bool renderIcon();
    
    /***********************\
     * Scripting Interface *
    \***********************/
    void play();
    void jump(int milliseconds);
    void next();
    void previous();
    void togglePause();

    virtual ~MusicPlayer() {
    }
  };
}
