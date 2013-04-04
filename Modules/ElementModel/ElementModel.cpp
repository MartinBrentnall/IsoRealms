#include "ElementModel.h"

ElementModel::ElementModel(IElement** element, Vertex* location, float scale) {
  cElement = element;
  cLocation = location;
  cScale = scale;
}

void ElementModel::update(unsigned int milliseconds) {
  for (unsigned int i = 0; i < cDynamics.size(); i++) {
    cDynamics[i]->update(milliseconds);
  }
}

void ElementModel::render() {
  glPushMatrix();
  glTranslatef(cLocation->x, cLocation->y, cLocation->z * IsoRealmsConstants::BLOCK_HEIGHT);
  glScalef(cScale, cScale, cScale);
  (*cElement)->renderStatic();
  for (unsigned int i = 0; i < cVisuals.size(); i++) {
    cVisuals[i]->render();
  }
  glPopMatrix();
}

