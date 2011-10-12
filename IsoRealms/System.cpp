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

bool System::passesFilter(std::string test, std::string filter) {
  // TODO: This is absolutely awful...  Must find and learn regular expressions library
  if (filter == "*") {
    return true;
  }
  if (filter[0] == '*' && test.size() > filter.size() + 1) {
    std::string mFilterMatch = filter.substr(1);
    std::string mTestEnd = test.substr(test.size() - (filter.size() - 1));
    if (mTestEnd == mFilterMatch) {
      return true;
    }
  }
  return false;
}

std::string System::getConfigurationFileLocation() {
  return getUserDataDirectory() + getConfigurationFileName();
}

std::string System::getSettingsFileLocation() {
  return getUserDataDirectory() + getSettingsFileName();
}

std::string System::getUserDataDirectory() {
  return getenv("HOME") + getDirectorySeparator() + getUserDataDirectoryName() + getDirectorySeparator();
}

void System::checkUserDataDirectory() {
  struct stat mUserDataLocationInfo;
  std::string mUserDataLocation = getenv("HOME") + getDirectorySeparator() + getUserDataDirectoryName();
  if (stat(mUserDataLocation.c_str(), &mUserDataLocationInfo) == 0) {
    if (!S_ISDIR(mUserDataLocationInfo.st_mode)) {
      std::cout << "It's not a directory!" << std::endl;
      exit(1);
    }
  } else {
    // TODO: Create an actual, real, user data directory instead of this hackish symlink
    if (symlink("/usr/share/IsoRealms/default_setup", mUserDataLocation.c_str())) {
//    if (mkdir(mUserDataLocation.c_str(), 0700)) {
      std::cout << "Couldn't create user data directory" << std::endl;
      exit(1);
    }
  }
}

bool System::configurationFileExists() {
  struct stat mConfigurationFileInfo;
  std::string mConfigurationFileLocation = getConfigurationFileLocation();
  return stat(mConfigurationFileLocation.c_str(), &mConfigurationFileInfo) == 0;
}

std::string System::convertToSystemFormat(std::string filename) {
  return filename;
}

bool System::configurationFileExists(std::string filename) {
  struct stat mFileInfo;
  std::string mFileLocation = getUserDataDirectory() + convertToSystemFormat(filename);
  return stat(mFileLocation.c_str(), &mFileInfo) == 0;
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
std::string System::getResource(std::string filename) {
  return getUserDataDirectory() + convertToSystemFormat(filename);
}

std::string System::getProjectResource(const std::string& projectName, const std::string& filename) {
  return getUserDataDirectory() + "projectConfig" + getDirectorySeparator() + projectName + getDirectorySeparator() + convertToSystemFormat(filename);
}

std::string System::getConfigurationResource(std::string filename) {
  return "/usr/share/IsoRealms/" + convertToSystemFormat(filename) + ".so";
}

std::vector<std::string>* System::getFileList(std::string filename) {
  std::vector<std::string>* mList = new std::vector<std::string>();
  DIR *dp;
  struct dirent *dirp;
  if ((dp = opendir(filename.c_str())) == NULL) {
    std::cout << "Error(" << errno << ") opening " << filename << std::endl;
  }

  while ((dirp = readdir(dp)) != NULL) {
    mList->push_back(std::string(dirp->d_name));
  }
  closedir(dp);
  return mList;
}

std::vector<std::string>* System::getFileList(const std::string& filename, const std::string& filter) {
  std::vector<std::string>* mList = new std::vector<std::string>();
  DIR *dp;
  struct dirent *dirp;
  if ((dp = opendir(filename.c_str())) == NULL) {
    std::cout << "Error(" << errno << ") opening " << filename << std::endl;
  }

  while ((dirp = readdir(dp)) != NULL) {
    std::string mFileName(dirp->d_name);
    std::string mCompletePath = filename + mFileName;
    bool mIsDir = opendir(mCompletePath.c_str()) != NULL;
    if (passesFilter(mFileName, filter) && mFileName[0] != '.' && (mIsDir || filter == "*.isorealms" || filter == "*.ogg")) {
      mList->push_back(mFileName);
    }
  }
  closedir(dp);
  return mList;
}


