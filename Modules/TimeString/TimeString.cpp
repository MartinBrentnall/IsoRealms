#include "TimeString.h"

void TimeString::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  std::string mIntegerPath = node->getAttribute("value");
  cValue = resources->getInteger(mIntegerPath);
}

std::string TimeString::getValue() {
  int mValue = cValue->getValue();
  int mMilliseconds = mValue % 1000;
  int mSeconds = mValue / 1000;
  int mMinutes = mSeconds / 60;
  mSeconds = mSeconds % 60;
  std::stringstream mStringStream;
  mStringStream << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setw(1) << mMilliseconds;
  return mStringStream.str();
}

void TimeString::setValue(const std::string&) {
  // Read only; cannot be set
}

void TimeString::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("value", resourceLocator->getPath(cValue));
}

extern "C" IPlugin* create() {
  return new SingleResourceTypeModule<TimeString>("String");
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
