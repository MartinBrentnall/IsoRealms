#include "ModelCycleable.h"

ModelCycleable::ModelCycleable(IModelTypeCycleable* modelType, std::vector<I3DModelType*> modelTypes, Vertex* location, float scale) {
  cModelType = modelType;
  cLocation = location;
  cScale = scale;
  for (I3DModelType* mModelType : modelTypes) {
    cModels.push_back(mModelType != nullptr ? mModelType->createModel(location, scale) : nullptr);
  }
}

void ModelCycleable::render() {
  cModels[cModelType->getCycleIndex()]->render();
}

void ModelCycleable::update(unsigned int milliseconds) {
  cModels[cModelType->getCycleIndex()]->update(milliseconds);
}

void ModelCycleable::setModel(I3DModelType* modelType, unsigned int index) {
  cModels[index] = modelType->createModel(cLocation, cScale);
}
