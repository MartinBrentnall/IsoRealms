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
#include "MusicPlayer.h"

namespace IsoRealms::Music {
  const std::string MusicPlayer::JSON_ACTION = "action";
  const std::string MusicPlayer::JSON_FADE   = "fade";
  const std::string MusicPlayer::JSON_FILE   = "file";
  const std::string MusicPlayer::JSON_SONGS  = "songs";
  const std::string MusicPlayer::JSON_VOLUME = "volume";
  
  MusicPlayer::MusicPlayer(IProject& project, Music& music, IResourceData& data) :
            cDefVolume(project, 0.0f),
            cRuntimeSelectedSong(0),
            cRuntimePlaying(false),
            cTotalTracks(*this),
            cSelectedTrack(*this),
            cSongLength(*this),
            cSongPosition(*this),
            cTotalLength(*this),
            cTotalPosition(*this),
            cVolume(*this),
            cTrackName(*this),
            cLuaBinding(project, this) {
    project.updateRuntime([this](unsigned int milliseconds) {
      if (cRuntimeSelectedSong < cDefSongs.size()) {
        cDefSongs[cRuntimeSelectedSong]->fadeIn(milliseconds);
        if (cDefSongs[cRuntimeSelectedSong]->isAboutToFinish()) {
          std::cout << "FADE OUT SONG " << cRuntimeSelectedSong << std::endl;
          cRuntimeSongsFadingOut.push_back(cRuntimeSelectedSong);
          if (cRuntimeSelectedSong < cDefSongs.size() - 1) {
            cRuntimeSelectedSong++;
            std::cout << "START NEXT SONG " << cRuntimeSelectedSong << std::endl;
            cDefSongs[cRuntimeSelectedSong]->startPlayback();
          }
        }
      }
      
      std::vector<unsigned int> mIndicesToRemove;
      
      for (unsigned int i = 0; i < cRuntimeSongsFadingOut.size(); ++i) {
        cDefSongs[cRuntimeSongsFadingOut[i]]->fadeOut(milliseconds);
        if (cDefSongs[cRuntimeSongsFadingOut[i]]->hasFinished()) {
          mIndicesToRemove.push_back(i);
        }
      }
      
      for (int i = mIndicesToRemove.size() - 1; i >= 0; i--) {
        cRuntimeSongsFadingOut.erase(cRuntimeSongsFadingOut.begin() + mIndicesToRemove[i]);
      }
    });

    project.reset([this]() {
      cRuntimeSelectedSong = 0;
      cRuntimePlaying = false;
    });
  }
  
  MusicPlayer::MusicPlayer(IProject& project, Music& music, IResourceData& data, JSONObject object, IOptions& options) :
            MusicPlayer(project, music, data) {
    cDefFade = object.getInteger(JSON_FADE);
    cDefVolume.init(object, JSON_VOLUME);
    for (JSONObject mSongObject : object.getArray(JSON_SONGS)) {
      cDefSongs.emplace_back(std::make_unique<Song>(project, *this, mSongObject));
    }
  }

  void MusicPlayer::registerAssets(IAssetRegistry& assets) {
    assets.add(&cLuaBinding, "", "MusicPlayers");    
    assets.add(&cTotalTracks, "TotalTracks", "MusicPlayers");
    assets.add(&cSelectedTrack, "SelectedTrack", "MusicPlayers");
    assets.add(&cSongLength, "SongLength", "MusicPlayers");
    assets.add(&cSongPosition, "SongPosition", "MusicPlayers");
    assets.add(&cTotalLength, "TotalLength", "MusicPlayers");
    assets.add(&cTotalPosition, "TotalPosition", "MusicPlayers");
    assets.add(&cVolume, "Volume", "MusicPlayers");
    assets.add(&cTrackName, "TrackName", "MusicPlayers");
  }

  void MusicPlayer::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    assets.remove(&cLuaBinding,    relinquish);
    assets.remove(&cTotalTracks,   relinquish);
    assets.remove(&cSelectedTrack, relinquish);
    assets.remove(&cSongLength,    relinquish);
    assets.remove(&cSongPosition,  relinquish);
    assets.remove(&cTotalLength,   relinquish);
    assets.remove(&cTotalPosition, relinquish);
    assets.remove(&cVolume,        relinquish);
    assets.remove(&cTrackName,     relinquish);
  }
  
  void MusicPlayer::save(JSONObject object, IAssetIdentifier& identifier) const {
    object.addInteger(JSON_FADE, cDefFade);
    cDefVolume.save(object, JSON_VOLUME);
    JSONArray mSongsArray = object.addArray(JSON_SONGS);
    for (const std::unique_ptr<Song>& mSong : cDefSongs) {
      JSONObject mSongObject = mSongsArray.addObject();
      mSong->save(mSongObject);
    }
  }

  void MusicPlayer::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool MusicPlayer::renderIcon() {
    return false;
  }
  
  std::vector<std::unique_ptr<IProperty>> MusicPlayer::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeInteger>("Fade Duration", [this]() {return cDefFade;}, [this](int value) {cDefFade = value; return true;}));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Float>>( "Volume",        cDefVolume));
    for (std::unique_ptr<Song>& mSong : cDefSongs) {
      mProperties.emplace_back(std::make_unique<PropertyStruct>("Song", mSong->getName(), [this, &mSong]() {return mSong->getProperties();}));
    }
    mProperties.emplace_back(std::make_unique<PropertyAdd>("Song", "Add...", [this, &browser]() {
      cDefSongs.emplace_back(std::make_unique<Song>(browser.getProject(), *this));
      std::unique_ptr<Song>& mSong  = cDefSongs.back();
      return std::make_unique<PropertyStruct>("Song", mSong->getName(), [this, &mSong]() {return mSong->getProperties();});
    }));
    return mProperties;
  }

  void MusicPlayer::play() {
    if (!cRuntimePlaying) {
      cRuntimePlaying = true;
      std::cout << "START PLAYBACK!!!" << std::endl;
      cDefSongs[cRuntimeSelectedSong]->startPlayback();
    }
  }
  
  void MusicPlayer::jump(int milliseconds) {
    if (cRuntimeSelectedSong < cDefSongs.size()) {
      cDefSongs[cRuntimeSelectedSong]->jump(milliseconds);
    }
  }
  
  void MusicPlayer::next() {
    if (cRuntimeSelectedSong < cDefSongs.size() - 1) {
      cRuntimeSongsFadingOut.push_back(cRuntimeSelectedSong);
      cRuntimeSelectedSong++;
      cDefSongs[cRuntimeSelectedSong]->startPlayback();
    }
  }
  
  void MusicPlayer::previous() {
    if (cRuntimeSelectedSong > 0) {
      if (cRuntimeSelectedSong < cDefSongs.size()) {
        cRuntimeSongsFadingOut.push_back(cRuntimeSelectedSong);
      }
      cRuntimeSelectedSong--;
      cDefSongs[cRuntimeSelectedSong]->startPlayback();
    }
  }
  
  void MusicPlayer::togglePause() {
    if (cRuntimePlaying) {
      cRuntimePlaying = false;
      cDefSongs[cRuntimeSelectedSong]->pause();
      for (unsigned int i : cRuntimeSongsFadingOut) {
        cDefSongs[i]->pause();
      }
    } else {
      cRuntimePlaying = true;
      cDefSongs[cRuntimeSelectedSong]->unpause();
      for (unsigned int i : cRuntimeSongsFadingOut) {
        cDefSongs[i]->unpause();
      }
    }
  }
  
  MusicPlayer::Song::Song(IProject& project, MusicPlayer& parent) :
            cParent(parent),
            cDefFile(project),
            cDefAction(project) {
  }
  
  MusicPlayer::Song::Song(IProject& project, MusicPlayer& parent, JSONObject object) :
            cParent(parent),
            cDefFile(project),
            cDefAction(project) {
    cDefAction.init(object, JSON_ACTION);
    cDefFile.load(JSON_FILE, object);
    std::string mResource = cDefFile.getPath();
    if (!cMusic.openFromFile(mResource)) {
      throw ArgumentException("ERROR: MusicPlayer::Song::Song: Specified file \"" + cDefFile.getPath() + "\" could not be opened");
    }
  }
  
  void MusicPlayer::Song::save(JSONObject object) const {
    cDefAction.save(object, JSON_ACTION);
    cDefFile.save(JSON_FILE, object);
  }
  
  std::vector<std::unique_ptr<IProperty>> MusicPlayer::Song::getProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<File>>(  "File",     cDefFile));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("On Start", cDefAction));
    return mProperties;
  }
  
  void MusicPlayer::Song::startPlayback() {
    cRuntimeVolume = 0;
    cMusic.setPlayingOffset(sf::Time::Zero);
    cMusic.play();
    cDefAction.execute();
  }
  
  void MusicPlayer::Song::jump(int milliseconds) {
    cMusic.setPlayingOffset(sf::milliseconds(std::max(0, cMusic.getPlayingOffset().asMilliseconds() + milliseconds)));
  }
  
  void MusicPlayer::Song::pause() {
    cMusic.pause();
  }
  
  void MusicPlayer::Song::unpause() {
    cMusic.play();
  }
  
  void MusicPlayer::Song::updateVolume() {
    float mVal = powf(cParent.cDefVolume->getValue(), 2.0f);
    float mNewVolume = (cRuntimeVolume / static_cast<float>(cParent.cDefFade)) * 100.0f * mVal;
    if (cRuntimeActualVolume != mNewVolume || mNewVolume == 0) {
      cRuntimeActualVolume = mNewVolume;
      cMusic.setVolume(cRuntimeActualVolume);
    }
  }
  
  void MusicPlayer::Song::fadeIn(unsigned int milliseconds) {
    cRuntimeVolume = std::min(cParent.cDefFade, cRuntimeVolume + static_cast<int>(milliseconds));
    updateVolume();
  }
  
  void MusicPlayer::Song::fadeOut(unsigned int milliseconds) {
    cRuntimeVolume = std::max(0, cRuntimeVolume - static_cast<int>(milliseconds));
    updateVolume();
  }
  
  bool MusicPlayer::Song::hasFinished() {
    return cRuntimeVolume == 0;
  }
  
  bool MusicPlayer::Song::isAboutToFinish() {
    return cMusic.getDuration().asMilliseconds() - cMusic.getPlayingOffset().asMilliseconds() < cParent.cDefFade;
  }
  
  int MusicPlayer::Song::getLength() {
    return cMusic.getDuration().asMilliseconds();
  }
  
  int MusicPlayer::Song::getPosition() {
    return cMusic.getPlayingOffset().asMilliseconds();
  }
  
  std::string MusicPlayer::Song::getPath() {
    return cDefFile.getPath();
  }
  
  std::string MusicPlayer::Song::getName() {
    std::string mPath = cDefFile.getPath();
    mPath = mPath.substr(mPath.find_last_of('/') + 1);
    mPath = mPath.substr(0, mPath.find_last_of('.'));
    return mPath;
  }

  MusicPlayer::TotalTracks::TotalTracks(MusicPlayer& parent) :
            cParent(parent) {
  }
  
  int MusicPlayer::TotalTracks::getValue() const {
    return cParent.cDefSongs.size();
  }
  
  bool MusicPlayer::TotalTracks::renderAssetIcon() const {
    return false;
  }

  void MusicPlayer::TotalTracks::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> MusicPlayer::TotalTracks::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool MusicPlayer::TotalTracks::isDefaultConfiguration() const {
    return true;
  }

  MusicPlayer::SelectedTrack::SelectedTrack(MusicPlayer& parent) :
            cParent(parent) {
  }
  
  int MusicPlayer::SelectedTrack::getValue() const {
    return cParent.cRuntimeSelectedSong + 1;
  }
  
  bool MusicPlayer::SelectedTrack::renderAssetIcon() const {
    return false;
  }

  void MusicPlayer::SelectedTrack::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> MusicPlayer::SelectedTrack::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool MusicPlayer::SelectedTrack::isDefaultConfiguration() const {
    return true;
  }

  MusicPlayer::SongLength::SongLength(MusicPlayer& parent) :
            cParent(parent) {    
  }
  
  std::string MusicPlayer::SongLength::getValue() const {
    int cLength = cParent.cDefSongs[cParent.cRuntimeSelectedSong]->getLength();
    int mMilliseconds = cLength % 1000;
    int mSeconds = cLength / 1000;
    int mMinutes = mSeconds / 60;
    mSeconds = mSeconds % 60;
    std::stringstream mStringStream;
    mStringStream << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setfill('0') << std::setw(2) << (mMilliseconds / 10);
    return mStringStream.str();
  }

  bool MusicPlayer::SongLength::renderAssetIcon() const {
    return false;
  }

  void MusicPlayer::SongLength::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> MusicPlayer::SongLength::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool MusicPlayer::SongLength::isDefaultConfiguration() const {
    return true;
  }

  MusicPlayer::SongPosition::SongPosition(MusicPlayer& parent) :
            cParent(parent) {    
  }
  
  std::string MusicPlayer::SongPosition::getValue() const {
    int cLength = cParent.cDefSongs[cParent.cRuntimeSelectedSong]->getPosition();
    int mMilliseconds = cLength % 1000;
    int mSeconds = cLength / 1000;
    int mMinutes = mSeconds / 60;
    mSeconds = mSeconds % 60;
    std::stringstream mStringStream;
    mStringStream << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setfill('0') << std::setw(2) << (mMilliseconds / 10);
    return mStringStream.str();
  }
  
  bool MusicPlayer::SongPosition::renderAssetIcon() const {
    return false;
  }

  void MusicPlayer::SongPosition::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> MusicPlayer::SongPosition::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool MusicPlayer::SongPosition::isDefaultConfiguration() const {
    return true;
  }

  MusicPlayer::TotalLength::TotalLength(MusicPlayer& parent) :
            cParent(parent) {    
  }
  
  std::string MusicPlayer::TotalLength::getValue() const {
    int mLength = 0;
    for (std::unique_ptr<Song>& mSong : cParent.cDefSongs) {
      mLength += mSong->getLength();
    }
    mLength -= cParent.cDefFade * (cParent.cDefSongs.size() - 1);
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

  bool MusicPlayer::TotalLength::renderAssetIcon() const {
    return false;
  }

  void MusicPlayer::TotalLength::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> MusicPlayer::TotalLength::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool MusicPlayer::TotalLength::isDefaultConfiguration() const {
    return true;
  }

  MusicPlayer::TotalPosition::TotalPosition(MusicPlayer& parent) :
            cParent(parent) {    
  }
  
  std::string MusicPlayer::TotalPosition::getValue() const {
    int mLength = 0;
    for (unsigned int i = 0; i < cParent.cRuntimeSelectedSong; ++i) {
      mLength += cParent.cDefSongs[i]->getLength();
    }
    mLength += cParent.cDefSongs[cParent.cRuntimeSelectedSong]->getPosition();
    mLength -= cParent.cDefFade * cParent.cRuntimeSelectedSong;
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
  
  bool MusicPlayer::TotalPosition::renderAssetIcon() const {
    return false;
  }

  void MusicPlayer::TotalPosition::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> MusicPlayer::TotalPosition::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool MusicPlayer::TotalPosition::isDefaultConfiguration() const {
    return true;
  }

  MusicPlayer::Volume::Volume(MusicPlayer& parent) :
            cParent(parent) {
  }

  std::string MusicPlayer::Volume::getValue() const {
    std::stringstream mStringStream;
    mStringStream << std::fixed << std::setprecision(2) << (cParent.cDefVolume->getValue() * 100.0f) << "%%";
    return mStringStream.str();
  }

  bool MusicPlayer::Volume::renderAssetIcon() const {
    return false;
  }

  void MusicPlayer::Volume::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> MusicPlayer::Volume::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool MusicPlayer::Volume::isDefaultConfiguration() const {
    return true;
  }

  MusicPlayer::TrackName::TrackName(MusicPlayer& parent) :
            cParent(parent) {    
  }
  
  std::string MusicPlayer::TrackName::getValue() const {
    return cParent.cDefSongs[cParent.cRuntimeSelectedSong]->getName();
  }

  bool MusicPlayer::TrackName::renderAssetIcon() const {
    return false;
  }

  void MusicPlayer::TrackName::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> MusicPlayer::TrackName::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool MusicPlayer::TrackName::isDefaultConfiguration() const {
    return true;
  }
}
