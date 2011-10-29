#include "RuntimeContext.h"

RuntimeContext::RuntimeContext(IMap* map, ICommandRegistry* commandRegistry) {
  cMap = map;
  cCommandRegistry = commandRegistry;
}

IMap* RuntimeContext::getMap() {
  return cMap;
}

ICommandRegistry* RuntimeContext::getCommandRegistry() {
  return cCommandRegistry;
}
