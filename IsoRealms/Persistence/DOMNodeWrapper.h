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
#ifndef DOM_NODE_WRAPPER_H
#define DOM_NODE_WRAPPER_H

#include "ParseException.h"

#include <errno.h>
#include <iostream>
#include <string>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

// TODO: Remove this
using namespace xercesc;

/**
 * Wraps a DOM node and provides a number of general convenience functions.
 */
class DOMNodeWrapper {
  private:
  DOMNode* cNode;
  int cIndex;

  DOMNodeWrapper(DOMNode*);  

  public:
  DOMNodeWrapper(std::string);
  int getChildCount();

  /**
   * Return 
   */
  DOMNodeWrapper* getChild(int); // TODO: Use '[]'
  DOMNodeWrapper* next();

  /**
   * Return the named attribute of this node.
   *
   * @returns  Named attribute of this node.
   */
  std::string getAttribute(const std::string&);

  int getIntegerAttribute(const std::string&);

  float getFloatAttribute(const std::string&);
  
  bool getBooleanAttribute(const std::string&);

  /**
   * Get the name of the node.
   *
   * @returns  Name of the node.
   */
  std::string getNodeName();

  float getFloatValue();
  
  /**
   * Parse and return an integer value from the node.
   *
   * @returns                 Parsed integer value.
   * @throws  ParseException  If the value is not a valid integer.
   */
  int getIntegerValue();

  /**
   * Parse and return a boolean value from the node.
   *
   * @returns                 Parsed boolean value.
   * @throws  ParseException  If the value is not a valid boolean.
   */
  bool getBooleanValue();

  /**
   * Parse and return a string value from the node.
   *
   * @returns                 Parsed string value.
   * @throws  ParseException  If the value is not a valid string.
   */
  std::string getStringValue();
};

#endif
