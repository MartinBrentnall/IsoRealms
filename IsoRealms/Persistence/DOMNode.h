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

#include <vector>
#include <errno.h>
#include <iostream>
#include <string>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

#include "IsoRealms/Exception/ArgumentException.h"
#include "ParseException.h"

namespace IsoRealms {

  /**
  * Wraps a DOM node and provides a number of general convenience functions.
  */
  class DOMNode {
    private:
    
    xercesc::DOMNode* cNode;
    std::vector<DOMNode> cChildren;
    bool cRoot;

    DOMNode(xercesc::DOMNode*);  

    void initChildren();
    int getChildCount();
    DOMNode* getChild(int); // TODO: Use '[]'
    
    public:
    enum class Type {
      PROGRAM,
      USER
    };
      
    class Iterator {
      private:
      DOMNode* cParent;
      unsigned int cIndex;
      
      public:
      Iterator();
      Iterator(DOMNode*, unsigned int);

      Iterator& operator++();
      bool operator!=(const Iterator&);
      DOMNode& operator*();
    };
    
    DOMNode(const std::string& path, Type type);

    Iterator begin();
    Iterator end();
    
    /**
    * Return the named attribute of this node.
    *
    * @returns  Named attribute of this node.
    */
    std::string getAttribute(const std::string&, const std::string& defaultValue = "") const;

    int getIntegerAttribute(const std::string&, int defaultValue = 0);

    float getFloatAttribute(const std::string&, float defaultValue = 0.0f);
    
    bool getBooleanAttribute(const std::string&, bool defaultValue = false);

    /**
    * Get the name of the node.
    *
    * @returns  Name of the node.
    */
    std::string getName();

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
    
    bool containsNode(const std::string& name);
    DOMNode& getNode(const std::string& name);

    void debug() const;
    
    ~DOMNode();
  };
}
