#include "ZoneRepresentationByModel.h"

ZoneRepresentationByModel::ZoneRepresentationByModel(IMap* map) : MapRenderer(map) {
}

void ZoneRepresentationByModel::update(unsigned int ticks) {
  IMap* mMap = getMap();
  std::vector<IZone*> mZones = mMap->getZones();
  for (unsigned int i = 0; i < mZones.size(); i++) {
    if (!isFiltered(mZones[i])) {
      mModel[i]->update(ticks);
    }
  }
}

void ZoneRepresentationByModel::render() {
  IMap* mMap = getMap();
  std::vector<IZone*> mZones = mMap->getZones();
  for (unsigned int i = 0; i < mZones.size(); i++) {
    if (!isFiltered(mZones[i])) {
      mModel[i]->render();
    }
  }
}


