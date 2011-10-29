/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef SEQUENCE_PLAYER_COMMANDS_H
#define SEQUENCE_PLAYER_COMMANDS_H

#include "../../SequencePlayer/ISequencePlayer.h"

#include "../IUtilities.h"

/**
 * This utility wraps a Player's functions in user commands, which are exposed
 * in the Command Registry.
 */
class SequencePlayerCommands:public IUtilities {
  private:
  
  /**
   * The wrapped player functions.
   */
  std::vector<IUserCommand*> cPlayerCommands;

  /**
   * The socket to which the Player plugin can connect.
   */   
  std::vector<PlugSocket*> cPlayerSocket;
  
  /**
   * The player instance whose functions to wrap.
   */
  ISequencePlayer* cPlayer;

  /**
   * Wraps the Player play() function in a user command.
   */
  class PlayCommand:public IUserCommand {
    private:
    SequencePlayerCommands* cParent;
    
    public:
    PlayCommand(SequencePlayerCommands*);
    
    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getCommandName();
  };
  
  /**
   * Wraps the Player rewind() function in a user command.
   */
  class RewindCommand:public IUserCommand {
    private:
    SequencePlayerCommands* cParent;
    
    public:
    RewindCommand(SequencePlayerCommands*);
    
    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getCommandName();
  };
  
  /**
   * Wraps the Player pause() function in a user command.
   */
  class PauseCommand:public IUserCommand {
    private:
    SequencePlayerCommands* cParent;
    
    public:
    PauseCommand(SequencePlayerCommands*);
    
    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getCommandName();
  };
  
  /**
   * Wraps the Player unpause() function in a user command.
   */
  class UnpauseCommand:public IUserCommand {
    private:
    SequencePlayerCommands* cParent;
    
    public:
    UnpauseCommand(SequencePlayerCommands*);
    
    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getCommandName();
  };
  
  public:
  SequencePlayerCommands();
  
  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::vector<PlugSocket*> getPlugSockets();
  IPlugin* getPlugin(PlugSocket*);
  void setPlugin(PlugSocket*, IPlugin*);

  void setRuntimeContext(IRuntimeContext*);
};

#endif
