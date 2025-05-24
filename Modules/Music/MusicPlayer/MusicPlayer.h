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
    public:
        
    /**********************\
     * Resource Interface *
    \**********************/
    MusicPlayer(IProject& project, Music& music, IResourceData& data);
    MusicPlayer(IProject& project, Music& music, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);  
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish);
    void save(JSONObject object, IAssetIdentifier& identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon();
    std::vector<std::unique_ptr<IProperty>> getProperties(IAssetBrowser& browser, IAssetRegistry& assets);
    
    
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

    private:
    static const std::string JSON_ACTION;
    static const std::string JSON_FADE;
    static const std::string JSON_FILE;
    static const std::string JSON_SONGS;
    static const std::string JSON_VOLUME;

    class Song {
      public:
      Song(IProject& project, MusicPlayer& parent);
      Song(IProject& project, MusicPlayer& parent, JSONObject object);
      
      void save(JSONObject object) const;
      std::vector<std::unique_ptr<IProperty>> getProperties();
      
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
      std::string getPath();
      std::string getName();
      
      private:
      MusicPlayer& cParent;
      
      File cDefFile;
      Action cDefAction;
      
      sf::Music cMusic;
      
      int cRuntimeVolume;
      float cRuntimeActualVolume;
      
      void updateVolume();
    };
    
    class TotalTracks : public IInteger {
      public:
      TotalTracks(MusicPlayer& parent);
      
      /***********************\
       * Implements IInteger *
      \***********************/
      int getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      MusicPlayer& cParent;
    };
    
    class SelectedTrack : public IInteger {
      public:
      SelectedTrack(MusicPlayer& parent);
      
      /***********************\
       * Implements IInteger *
      \***********************/
      int getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      MusicPlayer& cParent;
    };
    
    class SongLength : public IString {
      public:
      SongLength(MusicPlayer& parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      MusicPlayer& cParent;
    };

    class SongPosition : public IString {
      public:
      SongPosition(MusicPlayer& parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      MusicPlayer& cParent;
    };

    class TotalLength : public IString {
      public:
      TotalLength(MusicPlayer& parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      MusicPlayer& cParent;
    };

    class TotalPosition : public IString {
      public:
      TotalPosition(MusicPlayer& parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      MusicPlayer& cParent;
    };

    class Volume : public IString {
      public:
      Volume(MusicPlayer& parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      MusicPlayer& cParent;
    };

    class TrackName : public IString {
      public:
      TrackName(MusicPlayer& parent);

      /**********************\
       * Implements IString *
      \**********************/
      std::string getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      MusicPlayer& cParent;
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
  };
}
