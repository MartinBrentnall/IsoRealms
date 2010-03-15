#include "DummyCollectables.h"

void DummyCollectables::registerCollectable(ICollectable*) {
  // Nothing to do.
}

void DummyCollectables::collect(ICollector*, Vertex&, Vertex&) {
  // Nothing to do.
}

void DummyCollectables::reinitialise() {
  // Nothing to do.
}

extern "C" IPlugin* create() {
  return new DummyCollectables();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
