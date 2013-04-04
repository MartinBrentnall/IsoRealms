#include "ZoneModelRenderer.h"

I3DModel* ZoneModelRenderer::createModel(IZone* zone) {
  BlockArea* mBlockArea = zone->getZoneArea();
  float mXDifference = mBlockArea->getEast() - mBlockArea->getWest();
  float mYDifference = mBlockArea->getNorth() - mBlockArea->getSouth();
  float mZDifference = mBlockArea->getTop() - mBlockArea->getBottom();
  float mX = mBlockArea->getWest() + (mXDifference / 2.0f);
  float mY = mBlockArea->getSouth() + (mYDifference / 2.0f);
  float mZ = (mBlockArea->getBottom() + (mZDifference / 2.0f) - 1.0f) * IsoRealmsConstants::BLOCK_HEIGHT;
  float mScale = std::min(mXDifference, std::min(mYDifference, mZDifference)) + 1;
  Vertex* mLocation = new Vertex(mX, mY, mZ);
  I3DModel* mModel = cModelType->createModel(mLocation, mScale);
  cZoneModels[zone] = mModel;
  return mModel;
}

void ZoneModelRenderer::render(std::vector<IZone*>& zones, IProject* project) {
  for (unsigned int i = 0; i < zones.size(); i++) {
    I3DModel* mModel = cZoneModels[zones[i]];
    if (mModel == NULL) {
      mModel = createModel(zones[i]);
    }
    mModel->render();
  }
}

void ZoneModelRenderer::update(std::vector<IZone*>& zones, unsigned int ticks) {
  // Nothing to do
}

void ZoneModelRenderer::updateRuntime(std::vector<IZone*>& zones, unsigned int ticks) {
  for (unsigned int i = 0; i < zones.size(); i++) {
    I3DModel* mModel = cZoneModels[zones[i]];
    if (mModel == NULL) {
      mModel = createModel(zones[i]);
    }
    mModel->update(ticks);
  }
}

void ZoneModelRenderer::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  std::string mModel = node->getAttribute("model");
  cModelType = resources->getModelType(mModel);
}

void ZoneModelRenderer::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("model", resourceLocator->getPath(cModelType));
}

