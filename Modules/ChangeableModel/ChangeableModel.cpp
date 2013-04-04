#include "ChangeableModel.h"

ChangeableModel::ChangeableModel() {
  cModel = NULL;
}

void ChangeableModel::setModel(I3DModel* model) {
  cModel = model;
}

void ChangeableModel::update(unsigned int milliseconds) {
  if (cModel != NULL) {
    cModel->update(milliseconds);
  }
}

void ChangeableModel::render() {
  if (cModel != NULL) {
    cModel->render();
  }
}
