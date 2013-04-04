#include "DummyFont.h"

void DummyFont::initialiseResource(DOMNodeWrapper*, IResourceAccessor*) {
}

void DummyFont::print(float, float, float, int, const char*, ...) {
}

float DummyFont::getWidth(float, const char*, ...) {
  return 0.0f;
}

