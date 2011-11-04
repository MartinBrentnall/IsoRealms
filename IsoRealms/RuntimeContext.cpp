#include "RuntimeContext.h"

RuntimeContext::RuntimeContext(IMap* map, ICommandRegistry* commandRegistry, bool editing) {
  cMap = map;
  cCommandRegistry = commandRegistry;
  cEditing = editing;
}

IMap* RuntimeContext::getMap() {
  return cMap;
}

ICommandRegistry* RuntimeContext::getCommandRegistry() {
  return cCommandRegistry;
}

bool RuntimeContext::isEditing() {
  return cEditing;
}

