#include "TimeLimits.h"

void TimeLimits::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  cLocks = 0;
  IArgumentSource* mModuleArgumentSource = new ArgumentSourceCustom<TimeLimits>(this);
  runtimeContext->registerArgument("Module", "Module", mModuleArgumentSource);
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "TimeLimit") {
      TimeLimit* mTimeLimit = new TimeLimit(this);
      cTimers.push_back(mTimeLimit);
      std::string mName = mNode->getAttribute("name");
      runtimeContext->add(mTimeLimit, mName, mNode);
      runtimeContext->addDynamicElement(mTimeLimit);
    }
  }
}

void TimeLimits::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cTimers.size(); i++) {
    DOMNodeWriter* mCameraNode = node->addBranch("TimeLimit");
    std::string mResourceName = resourceLocator->getPath(cTimers[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mCameraNode->addAttribute("name", mResourceName);
    cTimers[i]->save(mCameraNode, resourceLocator);
  }
}

void TimeLimits::addLock() {
  cLocks++;
}

void TimeLimits::removeLock() {
  cLocks--;
}

bool TimeLimits::isLocked() {
  return cLocks > 0;
}

extern "C" IPlugin* create() {
  return new TimeLimits();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
