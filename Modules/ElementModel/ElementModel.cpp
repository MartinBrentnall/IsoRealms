#include "ElementModel.h"

ElementModel::ElementModel(IElement** element, Vertex* location, float scale) {
  cElement = element;
  cLocation = location;
  cScale = scale;
}

void ElementModel::update(unsigned int milliseconds) {
  if ((*cElement)->isDynamicRuntime()) {
    (*cElement)->updateRuntime(milliseconds);
  }
}

void ElementModel::render() {
  glPushMatrix();
  glTranslatef(cLocation->x, cLocation->y, cLocation->z * IsoRealmsConstants::BLOCK_HEIGHT);
  glScalef(cScale, cScale, cScale);
  (*cElement)->renderStatic();
  if ((*cElement)->isVisualRuntime()) {
    (*cElement)->renderRuntime();
  }
  glPopMatrix();
}

