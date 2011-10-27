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
#ifndef DOM_NODE_WRITER_H
#define DOM_NODE_WRITER_H

#include <cstdio>
#include <iostream>
#include <string>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/util/XMLString.hpp>

using namespace xercesc;

class DOMNodeWriter {
  private:
  DOMImplementation* cImplementation;
  DOMDocument* cDocument;
  DOMElement* cElement;

  /**
   * This is used by us to create branch nodes.
   * 
   * @param DOMDocument* The document to refer to.
   * @param DOMElement* The element represented.
   */
  DOMNodeWriter(DOMDocument*, DOMElement*);

  public:

  /**
   * Construct the root node with the specified name.
   * 
   * @param std::string  The name of the root node.
   */
  DOMNodeWriter(std::string);
 
  /**
   * Add a branch of the specified name.
   * 
   * @param std::string  The name of the branch.
   * @returns  The branch.
   */
  DOMNodeWriter* addBranch(std::string);

  /**
   * Create a branch of the specified name, but do not add it.
   * 
   * @param std::string  The name of the branch.
   * @returns  The branch.
   */
  DOMNodeWriter* createBranch(std::string);

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

  /**
   * Add the specified attribute of the specified name.
   * 
   * @param std::string  Attribute name.
   * @param std::string  Attribute content.
   */
  void addAttribute(std::string, std::string);

  /**
   * Add the specified attribute of the specified name.
   * 
   * @param std::string  Attribute name.
   * @param int          Attribute content.
   */
  void addAttribute(std::string, int);
  
  /**
   * Add the specified attribute of the specified name.
   * 
   * @param std::string  Attribute name.
   * @param int          Attribute content.
   */
  void addAttribute(std::string, unsigned int);
  
  /**
   * Add the specified attribute of the specified name.
   * 
   * @param std::string&  Attribute name.
   * @param float         Attribute content.
   */
  void addAttribute(const std::string&, float);

  /**
   * Add the specified attribute of the specified name.
   * 
   * @param std::string&  Attribute name.
   * @param double        Attribute content.
   */
  void addAttribute(const std::string&, double);

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
};

#endif
