/*
 * Copyright 2009 Martin Brentnall
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
#include "System.h"

std::string System::getConfigurationFileLocation() {
  return getProgramDataDirectory() + getConfigurationFileName();
}

std::string System::getSettingsFileLocation() {
  return getUserDataDirectory() + getSettingsFileName();
}

std::string System::getUserDataDirectory() {
  return getenv("HOME") + getDirectorySeparator() + getUserDataDirectoryName() + getDirectorySeparator();
}

void System::makeDirectory(const std::string& path) {
  struct stat mUserDataLocationInfo;
  std::vector<std::string> mFolders = Utils::splitWords(path, '/');
  std::string mUserDataLocation = "";
  for (unsigned int i = 0; i < mFolders.size(); i++) {
    mUserDataLocation += "/" + mFolders[i];
    if (stat(mUserDataLocation.c_str(), &mUserDataLocationInfo) == 0) {
      if (!S_ISDIR(mUserDataLocationInfo.st_mode)) {
        std::cout << "Exists, but is not a directory!" << std::endl;
        exit(1);
      }
    } else if (mkdir(mUserDataLocation.c_str(), 0700)) {
      std::cout << "Couldn't create user data directory" << std::endl;
      exit(1);
    }
  }
}

void System::makeUserDataDirectory(const std::string& path) {
  struct stat mUserDataLocationInfo;
  std::string mUserDataLocation = getenv("HOME") + getDirectorySeparator() + getUserDataDirectoryName();
  std::vector<std::string> mFolders = Utils::splitWords(path, '/');
  for (unsigned int i = 0; i < mFolders.size(); i++) {
    mUserDataLocation += "/" + mFolders[i];
    if (stat(mUserDataLocation.c_str(), &mUserDataLocationInfo) == 0) {
      if (!S_ISDIR(mUserDataLocationInfo.st_mode)) {
        std::cout << "Exists, but is not a directory!" << std::endl;
        exit(1);
      }
    } else if (mkdir(mUserDataLocation.c_str(), 0700)) {
      std::cout << "Couldn't create user data directory" << std::endl;
      exit(1);
    }
  }
}

void System::checkUserDataDirectory() {
  struct stat mUserDataLocationInfo;
  std::string mUserDataLocation = getenv("HOME") + getDirectorySeparator() + getUserDataDirectoryName();
  if (stat(mUserDataLocation.c_str(), &mUserDataLocationInfo) == 0) {
    if (!S_ISDIR(mUserDataLocationInfo.st_mode)) {
      std::cout << "It's not a directory!" << std::endl;
      exit(1);
    }
  } else if (mkdir(mUserDataLocation.c_str(), 0700)) {
    std::cout << "Couldn't create user data directory" << std::endl;
    exit(1);
  }
}

std::string System::convertToSystemFormat(const std::string& filename) {
  return filename;
}

bool System::configurationFileExists(const std::string& filename) {
  struct stat mFileInfo;
  std::string mFileLocation = getUserDataDirectory() + convertToSystemFormat(filename);
  return stat(mFileLocation.c_str(), &mFileInfo) == 0;
}

bool System::fileExists(const std::string& filename) {
  struct stat mFileInfo;
  return stat(filename.c_str(), &mFileInfo) == 0;
}

std::string System::getDirectorySeparator() {
  return "/";
}

std::string System::getUserDataDirectoryName() {
  return ".isorealms";
}

std::string System::getConfigurationFileName() {
  return "configuration.xml";
}

std::string System::getSettingsFileName() {
  return "settings.xml";
}

// TODO: Rename function "getDataResource"
std::string System::getProgramDataDirectory() {
  return "/usr/share/IsoRealms/";
}

std::string System::getProgramResource(const std::string& filename) {
  return getProgramDataDirectory() + convertToSystemFormat(filename);
}

std::string System::getUserResource(const std::string& filename) {
  return getUserDataDirectory() + convertToSystemFormat(filename);
}

std::string System::getUserProjectResource(const std::string& projectName, const std::string& filename) {
  return getUserDataDirectory() + "projectConfig" + getDirectorySeparator() + projectName + getDirectorySeparator() + convertToSystemFormat(filename);
}

std::string System::getConfigurationResource(const std::string& filename) {
  return getProgramDataDirectory() + convertToSystemFormat(filename) + ".so";
}

std::vector<std::string>* System::getFileList(const std::string& filename, bool files) {
  std::vector<std::string>* mList = new std::vector<std::string>();
  DIR *dp;
  struct dirent *dirp;
  if ((dp = opendir(filename.c_str())) == nullptr) {
    std::cout << "WARNING(" << errno << "); Directory \"" << filename << "\" could not be opened to list files" << std::endl;
    return mList;
  }

  struct stat mFileInfo;
  while ((dirp = readdir(dp)) != nullptr) {
    if (dirp->d_name[0] != '.' && dirp->d_type) {
      std::string mFullPath = filename + dirp->d_name;
      if (lstat(mFullPath.c_str(), &mFileInfo) < 0) {
        perror(mFullPath.c_str());
      } else {
        if (S_ISDIR(mFileInfo.st_mode) != files) {
          mList->push_back(std::string(dirp->d_name));
        }
      }
    }
  }
  closedir(dp);
  return mList;
}

unsigned int System::getCPUCores() {
  return sysconf(_SC_NPROCESSORS_ONLN);
}
