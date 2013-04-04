#include "MappedZoneHandler.h"

bool MappedZoneHandler::addHandler(IZone* zone, IZoneHandler* handler) {
  std::set<IZoneHandler*> mHandlers = cMappedHandlers[zone];
  unsigned int mSize = mHandlers.size();
  mHandlers.insert(handler);
  cMappedHandlers[zone] = mHandlers;
  return mSize != mHandlers.size();
}

void MappedZoneHandler::removeHandler(IZone* zone, IZoneHandler* handler) {
  std::set<IZoneHandler*> mHandlers = cMappedHandlers[zone];
  mHandlers.erase(handler);
  cMappedHandlers[zone] = mHandlers;
}

void MappedZoneHandler::render(std::vector<IZone*>& zones, IProject* project) {
  for (std::map<IZone*, std::set<IZoneHandler*> >::iterator i = cMappedHandlers.begin(); i != cMappedHandlers.end(); i++) {
    for (std::set<IZoneHandler*>::iterator j = i->second.begin(); j != i->second.end(); j++) {
      std::vector<IZone*> mZone;
      mZone.push_back(i->first);
      (*j)->render(mZone, project);
    }
  }
}

void MappedZoneHandler::update(std::vector<IZone*>& zones, unsigned int ticks) {
  for (std::map<IZone*, std::set<IZoneHandler*> >::iterator i = cMappedHandlers.begin(); i != cMappedHandlers.end(); i++) {
    for (std::set<IZoneHandler*>::iterator j = i->second.begin(); j != i->second.end(); j++) {
      std::vector<IZone*> mZone;
      mZone.push_back(i->first);
      (*j)->update(mZone, ticks);
    }
  }
}

void MappedZoneHandler::updateRuntime(std::vector<IZone*>& zones, unsigned int ticks) {
  for (std::map<IZone*, std::set<IZoneHandler*> >::iterator i = cMappedHandlers.begin(); i != cMappedHandlers.end(); i++) {
    for (std::set<IZoneHandler*>::iterator j = i->second.begin(); j != i->second.end(); j++) {
      std::vector<IZone*> mZone;
      mZone.push_back(i->first);
      (*j)->updateRuntime(mZone, ticks);
    }
  }
}

void MappedZoneHandler::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "DefaultHandler") {
      std::string mHandlerName = mNode->getAttribute("name");
      IZoneHandler* mZoneHandler = resources->getZoneHandler(mHandlerName);
      cDefaultHandlers.push_back(mZoneHandler);
    }
  }
}

void MappedZoneHandler::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cDefaultHandlers.size(); i++) {
    DOMNodeWriter* mDefaultHandlerNode = node->addBranch("DefaultHandler");
    mDefaultHandlerNode->addAttribute("name", resourceLocator->getPath(cDefaultHandlers[i]));
  }
}

void MappedZoneHandler::setDefaultHandlers(IZone* zone) {
  for (unsigned int i = 0; i < cDefaultHandlers.size(); i++) {
    addHandler(zone, cDefaultHandlers[i]);
  }
}


