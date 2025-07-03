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

#ifdef __linux__
#include <dirent.h>
#include <sys/param.h>
#include <unistd.h>
#endif

#include <errno.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

namespace IsoRealms {
  
  /**
   * This class contains system-level and Operating System dependent
   * functionality.  When using an IsoRealms project, the application must be
   * set in this class.
   */
  class System {
    private:
    
    /**
     * Convert the specified path string to the format of the OS.
     *
     * @return  The path string formatted for the OS.
     */
    static std::string convertToSystemFormat(const std::string& path);

    public:
    static const std::string USER_DATA_DIRECTORY;
    static const std::string MODULE_EXTENSION;

    /**
     * Return the path to the program or user data directory.
     *
     * @param path The path within the program or user data directory.
     * @param user true to get a path in the user data directory, false to get
     *             a path in the program data directory.
     * @return A path in the program or user data directory.
     */
    static std::string getPath(const std::string& path, bool user);

    /**
     * Return the path to a module within the user or program data directory.
     *
     * @param path The path within the program or user data directory.
     * @param user true to get a path in the user data directory, false to get
     *             a path in the program data directory.
     * @return A path in the program or user data directory.
     */
    static std::string getModulePath(const std::string& path, bool user);
    
    /**
     * Check whether a file exists at the specified path, relative to either
     * the program location or the user location.
     * 
     * @param path The path to check for existence of a file.
     * @param user true to check the path relative to the user data directory,
     *             false to check the path relative to the program data
     *             directory.
     * @return true if the file exists, otherwise false.
     */
    static bool fileExists(const std::string& path, bool user);
    
    /**
     * Check whether a module exists at the specified path, relative to either
     * the program location or the user location.
     * 
     * @param path The path to check for existence of a file.
     * @param user true to check the path relative to the user data directory,
     *             false to check the path relative to the program data
     *             directory.
     * @return true if the file exists, otherwise false.
     */
    static bool moduleExists(const std::string& path, bool user);
    
    /**
     * Create a folder of the specified path, relative to the user data
     * directory.
     * 
     * @param path The path relative to the user data directory to create.
     */
    static void makeUserDataDirectory(const std::string& path);
    static void renameUserDataDirectory(const std::string& oldPath, const std::string& newPath);
    
    /**
     * Open an output stream to the specified path, relative to the user data
     * directory.
     * 
     * @param path The path relative to the user data directory.
     * @return Output stream to file at the specified path.
     */ 
    static std::ofstream openOutputStream(const std::string& path); 
    static std::ifstream openInputStream(const std::string& path, bool user);

    /**
     * Return a list of filenames from the specified resource.
     *
     * @param path The path in which to get files.
     * @param files True if the path is user path, otherwise system path.
     * @return The list of filenames.
     */
    static std::vector<std::string> getFileList(const std::string& path, bool files = true);
  };
}
