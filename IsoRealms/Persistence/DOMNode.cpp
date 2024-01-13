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
#include "DOMNode.h"

#include "IsoRealms/System.h"

namespace IsoRealms {
  DOMNode::DOMNode(const std::string& filename, DOMNode::Type type) {
    std::string mFilename = System::getPath(filename, type == Type::USER);
//    std::cout << "Opening DOMNode: \"" << mFilename << "\" (from \"" << filename << "\")..." << std::endl;
    xercesc::XercesDOMParser mParser;
    mParser.setValidationScheme(xercesc::XercesDOMParser::Val_Always);
    xercesc::HandlerBase mErrorHandler;
    mParser.setErrorHandler(&mErrorHandler);
    try {
      mParser.parse(mFilename.c_str());
    } catch (const xercesc::XMLException& mException) {
      char* mMessage = xercesc::XMLString::transcode(mException.getMessage());
      ParseException mThrowException("XML parsing exception in " + mFilename + ": " + std::string(mMessage));
      xercesc::XMLString::release(&mMessage);
      throw mThrowException;
    } catch (const xercesc::DOMException& mException) {
      char* mMessage = xercesc::XMLString::transcode(mException.msg);
      ParseException mThrowException("XML parsing exception: " + mFilename + ": "  + std::string(mMessage));
      xercesc::XMLString::release(&mMessage);
      throw mThrowException;
    } catch (const xercesc::SAXException& mException) {
      char* mMessage = xercesc::XMLString::transcode(mException.getMessage());
      ParseException mThrowException("XML parsing exception: " + mFilename + ": "  + std::string(mMessage));
      xercesc::XMLString::release(&mMessage);
      throw mThrowException;
    }
    cNode = mParser.adoptDocument();
    cRoot = true;
    
    initChildren();
  }

  DOMNode::DOMNode(xercesc::DOMNode* node) {
    cNode = node;
    cRoot = false;
    initChildren();
  }

  void DOMNode::initChildren() {
    xercesc::DOMNodeList *mChildNodes = cNode->getChildNodes();
    unsigned int mLength = static_cast<unsigned int>(mChildNodes->getLength());
    for (unsigned int i = 0; i < mLength; i++) {
      xercesc::DOMNode* mItem = mChildNodes->item(i);
      if (mItem->getNodeType() == xercesc::DOMNode::NodeType::ELEMENT_NODE) {
        cChildren.push_back(mItem);
      }
    }
  }

  DOMNode::Iterator DOMNode::begin() {
    return Iterator(this, 0);
  }

  DOMNode::Iterator DOMNode::end() {
    return Iterator(this, getChildCount());
  }

  DOMNode::Iterator::Iterator() {
    cParent = nullptr;
    cIndex  = 0;
  }

  DOMNode::Iterator::Iterator(DOMNode* parent, unsigned int index) {
    cParent = parent;
    cIndex  = index;
  }

  DOMNode::Iterator& DOMNode::Iterator::operator++() {
    cIndex++;
    return *this;
  }

  bool DOMNode::Iterator::operator!=(const DOMNode::Iterator& node) {
    return cParent != node.cParent || cIndex != node.cIndex;
  }

  DOMNode& DOMNode::Iterator::operator*() {
    return *(cParent->getChild(cIndex));
  }

  std::string DOMNode::getAttribute(const std::string& attribute, const std::string& defaultValue) const {
    xercesc::DOMNamedNodeMap* mNamedNodeMap = cNode->getAttributes();
    if (mNamedNodeMap != nullptr) {
      XMLCh* mAttribute = xercesc::XMLString::transcode(attribute.c_str());
      xercesc::DOMNode* mAttributesNode = mNamedNodeMap->getNamedItem(mAttribute);
      xercesc::XMLString::release(&mAttribute);
      if (mAttributesNode != nullptr) {
        char *mTextValue = xercesc::XMLString::transcode(mAttributesNode->getChildNodes()->item(0)->getNodeValue());
        std::string mTextValueString(mTextValue);
        xercesc::XMLString::release(&mTextValue);
        return mTextValueString;
      }
    } else {
      std::cout << "Something bad happened!" << std::endl;
    }
    return defaultValue;
  }

  int DOMNode::getIntegerAttribute(const std::string& attribute, int defaultValue) {
    xercesc::DOMNamedNodeMap* mNamedNodeMap = cNode->getAttributes();
    char *mEndPointer;
    if (mNamedNodeMap != nullptr) {
      XMLCh* mAttribute = xercesc::XMLString::transcode(attribute.c_str());
      xercesc::DOMNode* mAttributesNode = mNamedNodeMap->getNamedItem(mAttribute);
      xercesc::XMLString::release(&mAttribute);
      if (mAttributesNode != nullptr) {
        char *mTextValue = xercesc::XMLString::transcode(mAttributesNode->getChildNodes()->item(0)->getNodeValue());
        std::string mTextValueString(mTextValue);
        long mIntegerValue = strtol(mTextValue, &mEndPointer, 10);
        xercesc::XMLString::release(&mTextValue);
        // TODO: THERE IS SOMETHING WRONG HERE...  THIS ERROR PASSES ON VALID VALUES OF 0 CAUSING EXCEPTION TO BE THROWN!!!
  /*      if ((errno == ERANGE && (mIntegerValue == LONG_MAX || mIntegerValue == LONG_MIN)) || (errno != 0 && mIntegerValue == 0)) {
          throw ParseException("Whilst reading value for node \"" + getNodeName() + "\"");
        }*/
        return mIntegerValue;
      }
    } else {
      std::cout << "Something bad happened!" << std::endl;
    }
    return defaultValue;
  }

  float DOMNode::getFloatAttribute(const std::string& attribute, float defaultValue) {
    xercesc::DOMNamedNodeMap* mNamedNodeMap = cNode->getAttributes();
    char *mEndPointer;
    if (mNamedNodeMap != nullptr) {
      XMLCh* mAttribute = xercesc::XMLString::transcode(attribute.c_str());
      xercesc::DOMNode* mAttributesNode = mNamedNodeMap->getNamedItem(mAttribute);
      xercesc::XMLString::release(&mAttribute);
      if (mAttributesNode != nullptr) {
        char *mTextValue = xercesc::XMLString::transcode(mAttributesNode->getChildNodes()->item(0)->getNodeValue());
        std::string mTextValueString(mTextValue);
        float mFloatValue = strtof(mTextValue, &mEndPointer);
        xercesc::XMLString::release(&mTextValue);
        // TODO: Work out what this was for and enable it
  /*      if ((errno == ERANGE && (mIntegerValue == LONG_MAX || mIntegerValue == LONG_MIN)) || (errno != 0 && mIntegerValue == 0)) {
          throw ParseException("Whilst reading value for node \"" + getNodeName() + "\"");
        }*/
        return mFloatValue;
      }
    } else {
      std::cout << "Something bad happened!" << std::endl;
    }
    return defaultValue;
  }

  bool DOMNode::getBooleanAttribute(const std::string& attribute, bool defaultValue) {
    xercesc::DOMNamedNodeMap* mNamedNodeMap = cNode->getAttributes();
    if (mNamedNodeMap != nullptr) {
      XMLCh* mAttribute = xercesc::XMLString::transcode(attribute.c_str());
      xercesc::DOMNode* mAttributesNode = mNamedNodeMap->getNamedItem(mAttribute);
      xercesc::XMLString::release(&mAttribute);
      if (mAttributesNode != nullptr) {
        char *mTextValue = xercesc::XMLString::transcode(mAttributesNode->getChildNodes()->item(0)->getNodeValue());
        std::string mTextValueString(mTextValue);
        bool mBoolValue = mTextValueString == "true";
        xercesc::XMLString::release(&mTextValue);
        return mBoolValue;
      }
    } else {
      std::cout << "Something bad happened!" << std::endl;
    }
    return defaultValue;
  }

  int DOMNode::getChildCount() {
    return static_cast<int>(cChildren.size());
  }

  DOMNode* DOMNode::getChild(int index) {
    return &cChildren[index];
  }

  std::string DOMNode::getName() {
    char *mValue = xercesc::XMLString::transcode(cNode->getNodeName());
    std::string mStringValue = mValue;
    xercesc::XMLString::release(&mValue);
    return mStringValue;
  }

  float DOMNode::getFloatValue() {
    xercesc::DOMNodeList *mChildNodes = cNode->getChildNodes();
    char *mEndPointer;
    for (unsigned int i = 0; i < mChildNodes->getLength(); i++) {
      xercesc::DOMNode *mNode = mChildNodes->item(i);
      xercesc::DOMNode::NodeType mType = mNode->getNodeType();
      if (mType == (xercesc::DOMNode::TEXT_NODE)) {
        char *mTextValue = xercesc::XMLString::transcode(mNode->getNodeValue());
        std::string mTextValueString(mTextValue);
        float mFloatValue = strtof(mTextValue, &mEndPointer);
        xercesc::XMLString::release(&mTextValue);
        // TODO: Work out what this was for and enable it
  /*      if ((errno == ERANGE && (mIntegerValue == LONG_MAX || mIntegerValue == LONG_MIN)) || (errno != 0 && mIntegerValue == 0)) {
          throw ParseException("Whilst reading value for node \"" + getNodeName() + "\"");
        }*/
        return mFloatValue;
      }
    }
    throw ParseException("No value specified for node \"" + getName() + "\" (expected integer)");
  }

  int DOMNode::getIntegerValue() {
    xercesc::DOMNodeList *mChildNodes = cNode->getChildNodes();
    char *mEndPointer;
    for (unsigned int i = 0; i < mChildNodes->getLength(); i++) {
      xercesc::DOMNode *mNode = mChildNodes->item(i);
      xercesc::DOMNode::NodeType mType = mNode->getNodeType();
      if (mType == (xercesc::DOMNode::TEXT_NODE)) {
        char *mTextValue = xercesc::XMLString::transcode(mNode->getNodeValue());
        errno = 0;
        long mIntegerValue = strtol(mTextValue, &mEndPointer, 10);
        xercesc::XMLString::release(&mTextValue);
        if ((errno == ERANGE && (mIntegerValue == LONG_MAX || mIntegerValue == LONG_MIN)) || (errno != 0 && mIntegerValue == 0)) {
          throw ParseException("Whilst reading value for node \"" + getName() + "\"");
        }
        return mIntegerValue;
      }
    }
    throw ParseException("No value specified for node \"" + getName() + "\" (expected integer)");
  }

  bool DOMNode::getBooleanValue() {
    xercesc::DOMNodeList *mChildNodes = cNode->getChildNodes();
    for (unsigned int i = 0; i < mChildNodes->getLength(); i++) {
      xercesc::DOMNode *mNode = mChildNodes->item(i);
      xercesc::DOMNode::NodeType mType = mNode->getNodeType();
      if (mType == (xercesc::DOMNode::TEXT_NODE)) {
        char *mTextValue = xercesc::XMLString::transcode(mNode->getNodeValue());
        for (unsigned int i = 0; i < strlen(mTextValue); i++) {
          mTextValue[i] = tolower(mTextValue[i]);
        }
        std::string mTextValueString = mTextValue;
        if (mTextValueString == "true" || mTextValueString == "on" || mTextValueString == "yes") {
          return true;
        } else if (mTextValueString == "false" || mTextValueString == "off" || mTextValueString == "no") {
          return false;
        }
        throw ParseException("Not a recognised boolean value for node \"" + getName() + "\": " + mTextValueString);
      }
    }
    throw ParseException("No value specified for node \"" + getName() + "\" (expected boolean)");
  }

  std::string DOMNode::getStringValue() {
    xercesc::DOMNodeList *mChildNodes = cNode->getChildNodes();
    for (unsigned int i = 0; i < mChildNodes->getLength(); i++) {
      xercesc::DOMNode *mNode = mChildNodes->item(i);
      xercesc::DOMNode::NodeType mType = mNode->getNodeType();
      if (mType == (xercesc::DOMNode::TEXT_NODE)) {
        char *mTextValue = xercesc::XMLString::transcode(mNode->getNodeValue());
        std::string mTextValueString = mTextValue;
        xercesc::XMLString::release(&mTextValue);
        return mTextValueString;
      }
    }
    throw ParseException("No value specified for node \"" + getName() + "\" (expected string)");
  }

  bool DOMNode::containsNode(const std::string& name) {
    for (DOMNode& mNode : cChildren) {
      std::string mValue = mNode.getName();
      if (mValue == name) {
        return true;
      }
    }
    return false;
  }
    
  DOMNode& DOMNode::getNode(const std::string& name) {
    if (name == "TODO") {
      std::cout << "DEBUG" << std::endl;
    }

    DOMNode* mFoundNode = nullptr;
    for (DOMNode& mNode : cChildren) {
      std::string mValue = mNode.getName();
      if (mValue == name) {
        if (mFoundNode != nullptr) {
          throw ArgumentException("ERROR: DOMNode::getNode: There should be exactly one node of name \"" + name + "\" (multiple nodes found).");
        }
        mFoundNode = &mNode;
      }
    }
    
    if (mFoundNode == nullptr) {
      throw ArgumentException("ERROR: DOMNode::getNode: There should be exactly one node of name \"" + name + "\" (no nodes found).");
    }
    return *mFoundNode;
  }

  void DOMNode::debug() const {
    xercesc::DOMNode* mNode = cNode;
    while (mNode != nullptr) {
      char *mValue = xercesc::XMLString::transcode(mNode->getNodeName());
      std::string mStringValue = mValue;
      xercesc::XMLString::release(&mValue);
      std::cout << "Node stack: \"" << mStringValue << "\"" << std::endl;
      mNode = mNode->getParentNode();
    }
  }

  DOMNode::~DOMNode() {
    if (cRoot) {
      cNode->release();
    }
  }
}
