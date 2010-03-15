#ifndef DUMMY_COLLECTABLES_H
#define DUMMY_COLLECTABLES_H

#include "../../Global/DummyPlugin.h"

#include "ICollectables.h"

class DummyCollectables:public ICollectables,
                        public DummyPlugin {
  public:
  void registerCollectable(ICollectable*);
  void collect(ICollector*, Vertex&, Vertex&);
  void reinitialise();
};

#endif
