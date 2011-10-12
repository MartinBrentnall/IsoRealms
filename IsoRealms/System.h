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
#ifndef SYSTEM_H
#define SYSTEM_H

#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

/**
 * This class defines Operating System dependent functionality.
 */
class System {
  private:

  /**
   * Test the specified string against the filter string.
   *
   * @param string  The string to test.
   * @param string  The filter string.
   * @returns       true if the test passes, otherwise false.
   */
  static bool passesFilter(std::string, std::string);

  public:

  /**
   * Return the entire path of the file storing the engine configuration.
   *
   * @returns  The entire path of the file storing the engine configuration.
   */
  static std::string getConfigurationFileLocation();

  /**
   * Return the entire path of the file storing the user settings.
   *
   * @returns  The entire path of the file storing the user settings.
   */
  static std::string getSettingsFileLocation();

  /**
   * Return the directory containing user data for the engine.
   *
   * @return  The entire path of the directory containing user data.
   */
  static std::string getUserDataDirectory();

  /**
   * Convert the specified string to the format of the OS.
   *
   * @return  The string formatted for the OS.
   */
  static std::string convertToSystemFormat(std::string);

  /**
   * Ensure that the user data directory exists.  If it doesn't exist, this
   * function will attempt to create it.
   */
  static void checkUserDataDirectory();

  /**
   * Check whether the configuration file exists.
   *
   * @returns  true if the configuration file exists, otherwise false.
   */
  static bool configurationFileExists();

  /**
   * Check whether the data file exists at the specified location.
   *
   * @param string  Location to check for the a file.
   * @returns  true if the configuration file exists, otherwise false.
   */
  static bool configurationFileExists(std::string);

  /**
   * Return the directory separator used on the current OS.
   *
   * @returns  The directory separator character for the current OS.
   */
  static std::string getDirectorySeparator();

  /**
   * Return the directory name storing user data for Iso Realms.
   *
   * @returns  The directory name storing user data for Iso Realms.
   */
  static std::string getUserDataDirectoryName();

  /**
   * Return the file name storing the engine configuration.
   *
   * @returns  The file name storing the engine configuration.
   */
  static std::string getConfigurationFileName();

  /**
   * Return the file name storing the user settings.
   *
   * @returns  The file name storing the user configuration.
   */
  static std::string getSettingsFileName();

  /**
   * Return the file name for the specified resource.
   *
   * @return  The file name for the specified resource.
   */
  static std::string getResource(std::string);

  /**
   * Return the project-specific file name for the specified resource.
   * 
   * @return  The project-specific file name for the specified resource.
   */
  static std::string getProjectResource(const std::string&, const std::string&);
  
  /**
   * Return the file name for the specified resource.
   *
   * @return  The file name for the specified resource.
   */
  static std::string getConfigurationResource(std::string);

  /**
   * Return a list of filenames from the specified resource.
   *
   * @param string  The resource to list files for.
   * @return        The list of filenames.
   */
  static std::vector<std::string>* getFileList(std::string);

  /**
   * Return a list of filenames from the specified resource.
   *
   * @param string  The resource to list files for.
   * @param string  Filter on filename.
   * @return        The list of filenames.
   */
  static std::vector<std::string>* getFileList(const std::string&, const std::string&);
};

#endif
