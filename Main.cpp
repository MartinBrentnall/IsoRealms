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
#include "Main.h"

int main(int argc, char **argv) {
  // Initialise SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
    std::cout << "ERROR: " << std::endl << SDL_GetError() << std::endl;
    return 1;
  }
  SDL_EnableKeyRepeat(250, SDL_DEFAULT_REPEAT_INTERVAL); 

  // Initialise Xerces-C++
  try {
    XMLPlatformUtils::Initialize();
  } catch (const XMLException &mXMLException) {
    SDL_Quit();
    std::cout << "ERROR: " << std::endl << "Could not initialise xerces-c" << std::endl;
    return 1;
  }
  
  // Initialise our engine configuration
  bool mSuccess = 0;
  try {
    KeyStates::init();
    Configuration* mGlobalConfiguration = Configuration::getInstance();
    mGlobalConfiguration->getScreenConfiguration();
    IEngine* mEngine = mGlobalConfiguration->getEngine();
    mEngine->run();
  } catch (InitException &e) {
    std::cout << "Fatal: " << std::endl << e.getMessage() << std::endl;
    mSuccess = 1;
  } catch (std::exception &e) {
    std::cout << "Fatal: " << std::endl << "Unhandled exception thrown: " << e.what() << std::endl;
    mSuccess = 1;
  } catch (DOMException &e) {
    std::cout << "Fatal: " << std::endl << e.code << ": " << e.msg << std::endl;
  } catch (PluginSupportException* e) {
    std::cout << "Fatal: " << std::endl << e->getMessage() << std::endl;
  } catch (...) {
    std::cout << "Fatal: " << std::endl << "Something unknown was thrown" << std::endl;
    mSuccess = 1;
  }
  Configuration* mConfiguration = Configuration::getInstance();
  mConfiguration->save();
  XMLPlatformUtils::Terminate();
  SDL_Quit();
  return mSuccess;
}


