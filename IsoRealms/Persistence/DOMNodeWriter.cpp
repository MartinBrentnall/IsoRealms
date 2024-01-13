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
#include "DOMNodeWriter.h"

#include "IsoRealms/System.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  DOMNodeWriter::DOMNodeWriter(xercesc::DOMDocument* document, xercesc::DOMElement* element) {
    cImplementation = nullptr;
    cDocument = document;
    cElement = element;
  }

  DOMNodeWriter::DOMNodeWriter(std::string nodeName) {
    cImplementation = xercesc::DOMImplementationRegistry::getDOMImplementation(xercesc::XMLString::transcode("Range"));
    cDocument = cImplementation->createDocument(nullptr, xercesc::XMLString::transcode(nodeName.c_str()), nullptr);
    cElement = cDocument->getDocumentElement();
  }

  DOMNodeWriter DOMNodeWriter::addBranch(std::string branchName) {
    xercesc::DOMElement* mElement = cDocument->createElement(xercesc::XMLString::transcode(branchName.c_str()));
    cElement->appendChild(mElement);
    return DOMNodeWriter(cDocument, mElement);
  }

  void DOMNodeWriter::addBranch(DOMNodeWriter* node) {
    cElement->appendChild(node->cElement);
  }

  void DOMNodeWriter::addText(std::string text) {
    xercesc::DOMText* mText = cDocument->createTextNode(xercesc::XMLString::transcode(text.c_str()));
    cElement->appendChild(mText);
  }

  void DOMNodeWriter::addText(int text) {
    std::stringstream mIntText;
    mIntText << text;
    std::string mString = mIntText.str();  
    xercesc::DOMText* mText = cDocument->createTextNode(xercesc::XMLString::transcode(mString.c_str()));
    cElement->appendChild(mText);
  }

  void DOMNodeWriter::addAttribute(const std::string& name, const std::string& value, const std::string& defaultValue) {
    if (value != defaultValue) {
      xercesc::DOMAttr* mAttribute = cDocument->createAttribute(xercesc::XMLString::transcode(name.c_str()));
      mAttribute->setValue(xercesc::XMLString::transcode(value.c_str()));
      cElement->setAttributeNode(mAttribute);
    }
  }

  void DOMNodeWriter::addAttribute(const std::string& name, unsigned int value, unsigned int defaultValue) {
    if (value != defaultValue) {
      xercesc::DOMAttr* mAttribute = cDocument->createAttribute(xercesc::XMLString::transcode(name.c_str()));
      mAttribute->setValue(xercesc::XMLString::transcode(Utils::toString(value).c_str()));
      cElement->setAttributeNode(mAttribute);
    }
  }

  void DOMNodeWriter::addAttribute(const std::string& name, int value, int defaultValue) {
    if (value != defaultValue) {
      xercesc::DOMAttr* mAttribute = cDocument->createAttribute(xercesc::XMLString::transcode(name.c_str()));
      mAttribute->setValue(xercesc::XMLString::transcode(Utils::toString(value).c_str()));
      cElement->setAttributeNode(mAttribute);
    }
  }

  void DOMNodeWriter::addAttribute(const std::string& name, bool value, bool defaultValue) {
    if (value != defaultValue) {
      addAttribute(name, std::string(value ? "true" : "false"));
    }
  }

  void DOMNodeWriter::addAttribute(const std::string& name, float value, float defaultValue) {
    if (value != defaultValue) {
      xercesc::DOMAttr* mAttribute = cDocument->createAttribute(xercesc::XMLString::transcode(name.c_str()));
      mAttribute->setValue(xercesc::XMLString::transcode(Utils::toString(value).c_str()));
      cElement->setAttributeNode(mAttribute);
    }
  }

  void DOMNodeWriter::addAttribute(const std::string& name, double value, double defaultValue) {
    if (value != defaultValue) {
      xercesc::DOMAttr* mAttribute = cDocument->createAttribute(xercesc::XMLString::transcode(name.c_str()));
      mAttribute->setValue(xercesc::XMLString::transcode(Utils::toString(value).c_str()));
      cElement->setAttributeNode(mAttribute);
    }
  }

  void DOMNodeWriter::save(std::string filename) {
    if (filename.find('/') != std::string::npos) {
      System::makeUserDataDirectory(filename.substr(0, filename.find_last_of('/')));
    }
    std::string mFilename = System::getPath(filename, true);
    xercesc::DOMImplementationLS* mDOMImplementationLS = (xercesc::DOMImplementationLS*) cImplementation;
    xercesc::DOMLSSerializer* mDOMLSSerializer = mDOMImplementationLS->createLSSerializer();
    xercesc::DOMConfiguration* mOutputConfig = mDOMLSSerializer->getDomConfig();
    mOutputConfig->setParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true); 
    mOutputConfig->setParameter(xercesc::XMLUni::fgDOMWRTXercesPrettyPrint, false);
    xercesc::DOMLSOutput* mDOMLSOutput = mDOMImplementationLS->createLSOutput();
    xercesc::LocalFileFormatTarget mTarget(mFilename.c_str());
    mDOMLSOutput->setByteStream(&mTarget);
    mDOMLSSerializer->write(cDocument, mDOMLSOutput);
    mDOMLSOutput->release();
    mDOMLSSerializer->release();
    cDocument->release();// TODO: Probably should be somewhere else!
    mTarget.flush();
  }

  bool DOMNodeWriter::empty() {
    return !cElement->hasChildNodes();
  }

  void DOMNodeWriter::addAttributeString(std::string name, std::string text) {
    addAttribute(name, text);
  }
}
