#include "EngineArguments.h"

void EngineArguments::set(const std::string& arguments) {
  cArguments = arguments;
}

std::string EngineArguments::get() {
  return cArguments;
}
