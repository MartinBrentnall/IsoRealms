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
#include "System.h"

#include "Application.h"
#include "Utils.h"

namespace IsoRealms {
#if _WIN32
  const std::string System::USER_DATA_DIRECTORY    = std::string(getenv("USERPROFILE")) + "/.isorealms/";
  const std::string System::MODULE_EXTENSION       = ".dll";
#elif __linux__
  const std::string System::USER_DATA_DIRECTORY    = std::string(getenv("HOME")) + "/.isorealms/";
  const std::string System::MODULE_EXTENSION       = ".so";
#endif
  
  std::string System::getPath(const std::string& filename, bool user) {
    return (user ? USER_DATA_DIRECTORY : "") + convertToSystemFormat(filename);
  }

  std::string System::getModulePath(const std::string& filename, bool user) {
    return getPath(filename, user) + MODULE_EXTENSION;
  }

  bool System::fileExists(const std::string& filename, bool user) {
    struct stat mFileInfo;
    std::string mFilename = getPath(filename, user);
    return stat(mFilename.c_str(), &mFileInfo) == 0;
  }
  
  bool System::moduleExists(const std::string& filename, bool user) {
    std::string mFilename = getModulePath(filename, user);
    return std::filesystem::exists(mFilename);
  }
  
  void System::makeUserDataDirectory(const std::string& path) {
    std::string mUserDataLocation = USER_DATA_DIRECTORY;
    std::vector<std::string> mFolders = Utils::splitWords(path, '/');

    // Create the data folder itself if it doesn't already exist.
    if (!std::filesystem::exists(mUserDataLocation)) {
      if (!std::filesystem::create_directory(mUserDataLocation)) {
        throw ArgumentException("ERROR: System::makeUserDataDirectory: Failed to create data directory \"" + mUserDataLocation + "\", mkdir function failed.");
      }
    }

    for (unsigned int i = 0; i < mFolders.size(); i++) {
      mUserDataLocation += (i == 0 ? "" : "/") + mFolders[i];
      if (std::filesystem::exists(mUserDataLocation)) {
        if (!std::filesystem::is_directory(mUserDataLocation)) {
          throw ArgumentException("ERROR: System::makeUserDataDirectory: Failed to create data directory, \"" + mUserDataLocation + "\" already exists as something other than a directory.");
        }
      } else {
        if (!std::filesystem::create_directory(mUserDataLocation)) {
          throw ArgumentException("ERROR: System::makeUserDataDirectory: Failed to create data directory \"" + mUserDataLocation + "\", mkdir function failed.");
        }
      }
    }
  }
  
  void System::renameUserDataDirectory(const std::string& oldPath, const std::string& newPath) {
    std::string mFullOldPath = USER_DATA_DIRECTORY + "/" + oldPath;
    std::string mFullNewPath = USER_DATA_DIRECTORY + "/" + newPath;
    if (std::filesystem::exists(mFullOldPath) && !std::filesystem::exists(mFullNewPath)) {
      std::filesystem::rename(mFullOldPath, mFullNewPath);
      std::cout << "Rename from \"" << mFullOldPath << "\" to \"" << mFullNewPath << "\" succeeded!" << std::endl;
    } else {
      std::cout << "Rename from \"" << mFullOldPath << "\" to \"" << mFullNewPath << "\" could not be done!" << std::endl;
    }
  }  

  std::ofstream System::openOutputStream(const std::string& path) {
    std::ofstream mOutput;
    std::string mFullPath = getPath(path, true);
    if (path.find('/') != std::string::npos) {
      makeUserDataDirectory(path.substr(0, path.find_last_of('/')));
    }
    mOutput.open(mFullPath, std::ios::out | std::ios::binary);
    return mOutput;
  }
  
  std::ifstream System::openInputStream(const std::string& path, bool user) {
    std::ifstream mInput;
    std::string mFullPath = getPath(path, user);
    mInput.open(mFullPath, std::ios::in);
    return mInput;
  }

  std::vector<std::string> System::getFileList(const std::string& filename, bool files) {
    std::vector<std::string> mList;
    if (std::filesystem::exists(filename)) {
      for (auto const& mDirEntry : std::filesystem::directory_iterator{filename}) {
        std::string mFileName = mDirEntry.path().filename().string();
        if (mFileName[0] != '.') {
          if (mDirEntry.is_directory() != files) {
            mList.push_back(mFileName);
          }
        }
      }
    }
    return mList;
  }
  
  /***********\
   * Private *
  \***********/
  std::string System::convertToSystemFormat(const std::string& filename) {
    return filename;
  }
}
