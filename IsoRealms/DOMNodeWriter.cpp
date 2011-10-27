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
#include "DOMNodeWriter.h"

DOMNodeWriter::DOMNodeWriter(DOMDocument* document, DOMElement* element) {
  cImplementation = NULL;
  cDocument = document;
  cElement = element;
}

DOMNodeWriter::DOMNodeWriter(std::string nodeName) {
  cImplementation = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("Range"));
  cDocument = cImplementation->createDocument(NULL, XMLString::transcode(nodeName.c_str()), NULL);
  cElement = cDocument->getDocumentElement();
}

DOMNodeWriter* DOMNodeWriter::createBranch(std::string branchName) {
  DOMElement* mElement = cDocument->createElement(XMLString::transcode(branchName.c_str()));
  return new DOMNodeWriter(cDocument, mElement);
}

DOMNodeWriter* DOMNodeWriter::addBranch(std::string branchName) {
  DOMElement* mElement = cDocument->createElement(XMLString::transcode(branchName.c_str()));
  cElement->appendChild(mElement);
  return new DOMNodeWriter(cDocument, mElement);
}

void DOMNodeWriter::addBranch(DOMNodeWriter* node) {
  cElement->appendChild(node->cElement);
}

void DOMNodeWriter::addText(std::string text) {
  DOMText* mText = cDocument->createTextNode(XMLString::transcode(text.c_str()));
  cElement->appendChild(mText);
}

void DOMNodeWriter::addAttribute(std::string name, std::string text) {
  DOMAttr* mAttribute = cDocument->createAttribute(XMLString::transcode(name.c_str()));
  mAttribute->setValue(XMLString::transcode(text.c_str()));
  cElement->setAttributeNode(mAttribute);
}

void DOMNodeWriter::addAttribute(std::string name, unsigned int val) {
  DOMAttr* mAttribute = cDocument->createAttribute(XMLString::transcode(name.c_str()));
  char mAttributeValue[16];
  sprintf(mAttributeValue, "%u", val);
  mAttribute->setValue(XMLString::transcode(mAttributeValue));
  cElement->setAttributeNode(mAttribute);
}

void DOMNodeWriter::addAttribute(std::string name, int val) {
  DOMAttr* mAttribute = cDocument->createAttribute(XMLString::transcode(name.c_str()));
  char mAttributeValue[16];
  sprintf(mAttributeValue, "%d", val);
  mAttribute->setValue(XMLString::transcode(mAttributeValue));
  cElement->setAttributeNode(mAttribute);
}

void DOMNodeWriter::addAttribute(const std::string& name, float val) {
  DOMAttr* mAttribute = cDocument->createAttribute(XMLString::transcode(name.c_str()));
  char mAttributeValue[16];
  sprintf(mAttributeValue, "%f", val);
  mAttribute->setValue(XMLString::transcode(mAttributeValue));
  cElement->setAttributeNode(mAttribute);
}

void DOMNodeWriter::addAttribute(const std::string& name, double val) {
  DOMAttr* mAttribute = cDocument->createAttribute(XMLString::transcode(name.c_str()));
  char mAttributeValue[16];
  sprintf(mAttributeValue, "%f", val);
  mAttribute->setValue(XMLString::transcode(mAttributeValue));
  cElement->setAttributeNode(mAttribute);
}

void DOMNodeWriter::save(std::string filename) {
  DOMImplementationLS* mDOMImplementationLS = (DOMImplementationLS*) cImplementation;
  DOMLSSerializer* mDOMLSSerializer = mDOMImplementationLS->createLSSerializer();
  DOMConfiguration* mOutputConfig = mDOMLSSerializer->getDomConfig();
  mOutputConfig->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true); 
  mOutputConfig->setParameter(XMLUni::fgDOMWRTXercesPrettyPrint, false);
  DOMLSOutput* mDOMLSOutput = mDOMImplementationLS->createLSOutput();
  XMLFormatTarget* mTarget = new LocalFileFormatTarget(filename.c_str());
  mDOMLSOutput->setByteStream(mTarget);
  mDOMLSSerializer->write(cDocument, mDOMLSOutput);
  cDocument->release();// TODO: Probably should be somewhere else!
}

bool DOMNodeWriter::empty() {
  return !cElement->hasChildNodes();
}
