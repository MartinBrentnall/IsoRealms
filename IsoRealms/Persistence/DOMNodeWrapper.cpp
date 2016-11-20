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
#include "DOMNodeWrapper.h"

DOMNodeWrapper::DOMNodeWrapper(std::string filename) {
  XercesDOMParser* mParser = new XercesDOMParser();
  mParser->setValidationScheme(XercesDOMParser::Val_Always);
  ErrorHandler* mErrorHandler = (ErrorHandler*) new HandlerBase();
  mParser->setErrorHandler(mErrorHandler);
  try {
    mParser->parse(filename.c_str());
  } catch (const XMLException& mException) {
    char* mMessage = XMLString::transcode(mException.getMessage());
    ParseException mException("XML parsing exception: " + std::string(mMessage));
    XMLString::release(&mMessage);
    throw mException;
  } catch (const DOMException& mException) {
    char* mMessage = XMLString::transcode(mException.msg);
    ParseException mException("XML parsing exception: " + std::string(mMessage));
    XMLString::release(&mMessage);
    throw mException;
  } catch (const SAXException& mException) {
    char* mMessage = XMLString::transcode(mException.getMessage());
    ParseException mException("XML parsing exception: " + std::string(mMessage));
    XMLString::release(&mMessage);
    throw mException;
  }
  cNode = mParser->getDocument(); 
  cIndex = 0;
}

DOMNodeWrapper::DOMNodeWrapper(DOMNode* node) {
  cNode = node;
  cIndex = 0;
}

std::string DOMNodeWrapper::getAttribute(const std::string& attribute) {
  DOMNamedNodeMap* mNamedNodeMap = cNode->getAttributes();
  if (mNamedNodeMap != NULL) {
    XMLCh* mAttribute = XMLString::transcode(attribute.c_str());
    DOMNode* mAttributesNode = mNamedNodeMap->getNamedItem(mAttribute);
    XMLString::release(&mAttribute);
    if (mAttributesNode != NULL) {
      char *mTextValue = XMLString::transcode(mAttributesNode->getChildNodes()->item(0)->getNodeValue());
      std::string mTextValueString(mTextValue);
      XMLString::release(&mTextValue);
      return mTextValueString;
    }
  } else {
    std::cout << "Something bad happened!" << std::endl;
  }
  return "";
}

int DOMNodeWrapper::getIntegerAttribute(const std::string& attribute) {
  DOMNamedNodeMap* mNamedNodeMap = cNode->getAttributes();
  char *mEndPointer;
  if (mNamedNodeMap != NULL) {
    XMLCh* mAttribute = XMLString::transcode(attribute.c_str());
    DOMNode* mAttributesNode = mNamedNodeMap->getNamedItem(mAttribute);
    XMLString::release(&mAttribute);
    if (mAttributesNode != NULL) {
      char *mTextValue = XMLString::transcode(mAttributesNode->getChildNodes()->item(0)->getNodeValue());
      std::string mTextValueString(mTextValue);
      long mIntegerValue = strtol(mTextValue, &mEndPointer, 10);
      XMLString::release(&mTextValue);
      // TODO: THERE IS SOMETHING WRONG HERE...  THIS ERROR PASSES ON VALID VALUES OF 0 CAUSING EXCEPTION TO BE THROWN!!!
/*      if ((errno == ERANGE && (mIntegerValue == LONG_MAX || mIntegerValue == LONG_MIN)) || (errno != 0 && mIntegerValue == 0)) {
        throw ParseException("Whilst reading value for node \"" + getNodeName() + "\"");
      }*/
      return mIntegerValue;
    }
  } else {
    std::cout << "Something bad happened!" << std::endl;
  }
  // TODO: Throw something
  return 0;
}

float DOMNodeWrapper::getFloatAttribute(const std::string& attribute) {
  DOMNamedNodeMap* mNamedNodeMap = cNode->getAttributes();
  char *mEndPointer;
  if (mNamedNodeMap != NULL) {
    XMLCh* mAttribute = XMLString::transcode(attribute.c_str());
    DOMNode* mAttributesNode = mNamedNodeMap->getNamedItem(mAttribute);
    XMLString::release(&mAttribute);
    if (mAttributesNode != NULL) {
      char *mTextValue = XMLString::transcode(mAttributesNode->getChildNodes()->item(0)->getNodeValue());
      std::string mTextValueString(mTextValue);
      float mFloatValue = strtof(mTextValue, &mEndPointer);
      XMLString::release(&mTextValue);
      // TODO: Work out what this was for and enable it
/*      if ((errno == ERANGE && (mIntegerValue == LONG_MAX || mIntegerValue == LONG_MIN)) || (errno != 0 && mIntegerValue == 0)) {
        throw ParseException("Whilst reading value for node \"" + getNodeName() + "\"");
      }*/
      return mFloatValue;
    }
  } else {
    std::cout << "Something bad happened!" << std::endl;
  }
  // TODO: Throw something
  return 0.0f;
}

bool DOMNodeWrapper::getBooleanAttribute(const std::string& attribute) {
  DOMNamedNodeMap* mNamedNodeMap = cNode->getAttributes();
  if (mNamedNodeMap != NULL) {
    XMLCh* mAttribute = XMLString::transcode(attribute.c_str());
    DOMNode* mAttributesNode = mNamedNodeMap->getNamedItem(mAttribute);
    XMLString::release(&mAttribute);
    if (mAttributesNode != NULL) {
      char *mTextValue = XMLString::transcode(mAttributesNode->getChildNodes()->item(0)->getNodeValue());
      std::string mTextValueString(mTextValue);
      bool mBoolValue = mTextValueString == "true";
      XMLString::release(&mTextValue);
      return mBoolValue;
    }
  } else {
    std::cout << "Something bad happened!" << std::endl;
  }
  // TODO: Throw something
  return false;
}

int DOMNodeWrapper::getChildCount() {
  return cNode->getChildNodes()->getLength();
}

DOMNodeWrapper* DOMNodeWrapper::getChild(int index) {
  DOMNodeList *mChildNodes = cNode->getChildNodes();
  return new DOMNodeWrapper(mChildNodes->item(index));
}

DOMNodeWrapper* DOMNodeWrapper::next() {
  DOMNodeList *mChildNodes = cNode->getChildNodes();
  if (cIndex < getChildCount()) {
    return new DOMNodeWrapper(mChildNodes->item(cIndex++));
  }
  return nullptr;
}

std::string DOMNodeWrapper::getNodeName() {
  char *mValue = XMLString::transcode(cNode->getNodeName());
  std::string mStringValue = mValue;
  XMLString::release(&mValue);
  return mStringValue;
}

float DOMNodeWrapper::getFloatValue() {
  DOMNodeList *mChildNodes = cNode->getChildNodes();
  char *mEndPointer;
  for (unsigned int i = 0; i < mChildNodes->getLength(); i++) {
    DOMNode *mNode = mChildNodes->item(i);
    DOMNode::NodeType mType = mNode->getNodeType();
    if (mType == (DOMNode::TEXT_NODE)) {
      char *mTextValue = XMLString::transcode(mNode->getNodeValue());
      std::string mTextValueString(mTextValue);
      float mFloatValue = strtof(mTextValue, &mEndPointer);
      XMLString::release(&mTextValue);
      // TODO: Work out what this was for and enable it
/*      if ((errno == ERANGE && (mIntegerValue == LONG_MAX || mIntegerValue == LONG_MIN)) || (errno != 0 && mIntegerValue == 0)) {
        throw ParseException("Whilst reading value for node \"" + getNodeName() + "\"");
      }*/
      return mFloatValue;
    }
  }
  throw ParseException("No value specified for node \"" + getNodeName() + "\" (expected integer)");
}

int DOMNodeWrapper::getIntegerValue() {
  DOMNodeList *mChildNodes = cNode->getChildNodes();
  char *mEndPointer;
  for (unsigned int i = 0; i < mChildNodes->getLength(); i++) {
    DOMNode *mNode = mChildNodes->item(i);
    DOMNode::NodeType mType = mNode->getNodeType();
    if (mType == (DOMNode::TEXT_NODE)) {
      char *mTextValue = XMLString::transcode(mNode->getNodeValue());
      errno = 0;
      long mIntegerValue = strtol(mTextValue, &mEndPointer, 10);
      XMLString::release(&mTextValue);
      if ((errno == ERANGE && (mIntegerValue == LONG_MAX || mIntegerValue == LONG_MIN)) || (errno != 0 && mIntegerValue == 0)) {
        throw ParseException("Whilst reading value for node \"" + getNodeName() + "\"");
      }
      return mIntegerValue;
    }
  }
  throw ParseException("No value specified for node \"" + getNodeName() + "\" (expected integer)");
}

bool DOMNodeWrapper::getBooleanValue() {
  DOMNodeList *mChildNodes = cNode->getChildNodes();
  for (unsigned int i = 0; i < mChildNodes->getLength(); i++) {
    DOMNode *mNode = mChildNodes->item(i);
    DOMNode::NodeType mType = mNode->getNodeType();
    if (mType == (DOMNode::TEXT_NODE)) {
      char *mTextValue = XMLString::transcode(mNode->getNodeValue());
      for (unsigned int i = 0; i < strlen(mTextValue); i++) {
        mTextValue[i] = tolower(mTextValue[i]);
      }
      std::string mTextValueString = mTextValue;
      if (mTextValueString == "true" || mTextValueString == "on" || mTextValueString == "yes") {
        return true;
      } else if (mTextValueString == "false" || mTextValueString == "off" || mTextValueString == "no") {
        return false;
      }
      throw ParseException("Not a recognised boolean value for node \"" + getNodeName() + "\": " + mTextValueString);
    }
  }
  throw ParseException("No value specified for node \"" + getNodeName() + "\" (expected boolean)");
}

std::string DOMNodeWrapper::getStringValue() {
  DOMNodeList *mChildNodes = cNode->getChildNodes();
  for (unsigned int i = 0; i < mChildNodes->getLength(); i++) {
    DOMNode *mNode = mChildNodes->item(i);
    DOMNode::NodeType mType = mNode->getNodeType();
    if (mType == (DOMNode::TEXT_NODE)) {
      char *mTextValue = XMLString::transcode(mNode->getNodeValue());
      std::string mTextValueString = mTextValue;
      XMLString::release(&mTextValue);
      return mTextValueString;
    }
  }
  throw ParseException("No value specified for node \"" + getNodeName() + "\" (expected string)");
}
