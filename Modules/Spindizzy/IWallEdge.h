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
#ifndef I_WALL_EDGE_H
#define I_WALL_EDGE_H

/**
 * This interface describes the edge of a wall.
 */
class IWallEdge {
  public:

  /**
   * Get the height of the wall at the west / south most point.
   * 
   * @returns  Height of the wall at the west / south most point.
   */
  virtual int getStartHeight() = 0;

  /**
   * Get the height of the wall at the east / north most point.
   * 
   * @returns  Height of the wall at the east / north most point.
   */
  virtual int getEndHeight() = 0;

  virtual ~IWallEdge() {}
};

#endif
