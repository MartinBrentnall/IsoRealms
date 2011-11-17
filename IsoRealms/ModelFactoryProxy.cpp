#include "ModelFactoryProxy.h"

ModelFactoryProxy::ModelFactoryProxy() {
  cModelFactory = NULL;
}

void ModelFactoryProxy::setImplementation(I3DModelFactory* modelFactory) {
  cModelFactory = modelFactory;
}

I3DModel* ModelFactoryProxy::createModel(Vertex* location, float scale) {
  return cModelFactory != NULL ? cModelFactory->createModel(location, scale) : new DummyModel(location, scale);
}

void ModelFactoryProxy::destroyModel(I3DModel* model) {
  if (cModelFactory != NULL) {
    cModelFactory->destroyModel(model);
  }
}

ModelFactoryProxy::DummyModel::DummyModel(Vertex* location, float scale) {
  cLocation = location;
  cScale = scale;
}

void ModelFactoryProxy::DummyModel::update(int ticks) {
  // Nothing to do
}

void ModelFactoryProxy::DummyModel::render() {
  glTranslatef(cLocation->x, cLocation->y, cLocation->z * IsoRealmsConstants::BLOCK_HEIGHT);
  glLineWidth(4.0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_LINES);
  glColor3f(1.0f, 0.0f, 1.0f); glVertex3f( 0.5f, 0.0f, 0.0f);
  glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-0.5f, 0.0f, 0.0f);
  glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 0.0f, 0.5f, 0.0f);
  glColor3f(0.0f, 1.0f, 1.0f); glVertex3f( 0.0f, -0.5f, 0.0f);
  glColor3f(0.0f, 0.0f, 1.0f); glVertex3f( 0.0f, 0.0f, 0.5f);
  glColor3f(1.0f, 1.0f, 0.0f); glVertex3f( 0.0f, 0.0f, -0.5f);
  glEnd();
  glLineWidth(1.0);// TODO: Shouldn't be needed here. 
  glTranslatef(-cLocation->x, -cLocation->y, -(cLocation->z * IsoRealmsConstants::BLOCK_HEIGHT));
}
