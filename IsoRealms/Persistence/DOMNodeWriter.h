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

#include <cstdio>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/util/XMLString.hpp>

namespace IsoRealms {
  class DOMNodeWriter {
    private:
    xercesc::DOMImplementation* cImplementation;
    xercesc::DOMDocument* cDocument;

    public:

    /**
    * Construct the root node with the specified name.
    *
    * @param std::string  The name of the root node.
    */
    xercesc::DOMElement* cElement;
    DOMNodeWriter(std::string);

    /**
    * This is used by us to create branch nodes.
    * 
    * @param DOMDocument* The document to refer to.
    * @param DOMElement* The element represented.
    */
    DOMNodeWriter(xercesc::DOMDocument*, xercesc::DOMElement*);
  
    /**
    * Add a branch of the specified name.
    * 
    * @param std::string  The name of the branch.
    * @returns  The branch.
    */
    DOMNodeWriter addBranch(std::string);

    /**
    * Add specified DOMNodeWriter as a branch.
    * 
    * @param DOMNodeWriter  The branch.
    */
    void addBranch(DOMNodeWriter*);
    
    /**
    * Add the specified text to this node.
    * 
    * @param std::string  The text to add.
    */
    void addText(std::string);

    void addText(int);
    
    /**
    * Add the specified attribute of the specified name.
    * 
    * @param std::string  Attribute name.
    * @param std::string  Attribute content.
    */
    void addAttribute(const std::string& name, const std::string& value, const std::string& defaultValue = "");

    /**
    * Add the specified attribute of the specified name.
    * 
    * @param std::string  Attribute name.
    * @param int          Attribute content.
    */
    void addAttribute(const std::string& name, int value, int defaultValue = 0);
    
    /**
    * Add the specified attribute of the specified name.
    * 
    * @param std::string  Attribute name.
    * @param int          Attribute content.
    */
    void addAttribute(const std::string& name, unsigned int value, unsigned int defaultValue = 0U);
    
    /**
    * Add the specified attribute of the specified name.
    *
    * @param std::string  Attribute name.
    * @param bool         Attribute content.
    */
    void addAttribute(const std::string& name, bool value, bool defaultValue = false);

    /**
    * Add the specified attribute of the specified name.
    * 
    * @param std::string&  Attribute name.
    * @param float         Attribute content.
    */
    void addAttribute(const std::string& name, float value, float defaultValue = 0.0f);

    /**
    * Add the specified attribute of the specified name.
    * 
    * @param std::string&  Attribute name.
    * @param double        Attribute content.
    */
    void addAttribute(const std::string& name, double value, double defaultValue = 0.0);

    /**
    * Save the structure to the specified filename.
    * 
    * @param std::string  The filename to save to.
    */
    void save(std::string);
    
    /**
    * See if this node has anything in it.
    * 
    * @returns  True if it does, otherwise false.
    */
    bool empty();
    
    /***********************\
    * Scripting interface *
    \***********************/
    void addAttributeString(std::string, std::string);
  };
}
