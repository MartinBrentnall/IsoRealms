#include "DialogSpindizzyTextureSet.h"

DialogSpindizzyTextureSet::DialogSpindizzyTextureSet(IComponentContainer* container, C64SpindizzyTextureSet* textureSet, IResourceAccessor* resources, IResourceSelector* resourceSelector, IConfirmationListener* confirmationListener) : ResizableDialog(container, "Modules/C64SpindizzyTextureSet/DialogSpindizzyTextureSet", resources) {
  cTextureSet = textureSet;
  cResourceSelector = resourceSelector;
  cConfirmationListener = confirmationListener;
  setComponentAction("okButton", new OKCommand(this));
  setComponentAction("cancelButton", new CancelCommand(this));
  setComponentAction("undoButton", new UndoCommand(this));
  cOriginalFloorColour  = cTextureSet->getFloorColour();
  cOriginalWallColour   = cTextureSet->getWallColour();
  cOriginalGridColour   = cTextureSet->getGridColour();
  cOriginalDetailColour = cTextureSet->getDetailColour();
  cColourSelectorFloor  = new ColourSelector(this, cOriginalFloorColour,  0);
  cColourSelectorWall   = new ColourSelector(this, cOriginalWallColour,   1);
  cColourSelectorGrid   = new ColourSelector(this, cOriginalGridColour,   2);
  cColourSelectorDetail = new ColourSelector(this, cOriginalDetailColour, 3);
  setSelectable("floorColour",  cColourSelectorFloor);
  setSelectable("wallColour"  , cColourSelectorWall);
  setSelectable("gridColour",   cColourSelectorGrid);
  setSelectable("detailColour", cColourSelectorDetail);
  std::vector<ITexture*> mTextures = cTextureSet->getTextures();
  for (unsigned int i = 0; i < mTextures.size(); i++) {
    addComponent("previewPane", new TextureIcon(mTextures[i]));
  }
}

void DialogSpindizzyTextureSet::undo() {
  cColourSelectorFloor->resourceSelected(cOriginalFloorColour);
  cColourSelectorWall->resourceSelected(cOriginalWallColour);
  cColourSelectorGrid->resourceSelected(cOriginalGridColour);
  cColourSelectorDetail->resourceSelected(cOriginalDetailColour);
}

C64SpindizzyTextureSet* DialogSpindizzyTextureSet::getTextureSet() {
  return cTextureSet;
}

DialogSpindizzyTextureSet::OKCommand::OKCommand(DialogSpindizzyTextureSet* parent) {
  cParent = parent;
}

void DialogSpindizzyTextureSet::OKCommand::execute() {
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogConfirmed(cParent);
  }
}

DialogSpindizzyTextureSet::CancelCommand::CancelCommand(DialogSpindizzyTextureSet* parent) {
  cParent = parent;
}

void DialogSpindizzyTextureSet::CancelCommand::execute() {
  cParent->undo();
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogCancelled(cParent);
  }
}

DialogSpindizzyTextureSet::UndoCommand::UndoCommand(DialogSpindizzyTextureSet* parent) {
  cParent = parent;
}

void DialogSpindizzyTextureSet::UndoCommand::execute() {
  cParent->undo();
}

DialogSpindizzyTextureSet::ColourSelector::ColourSelector(DialogSpindizzyTextureSet* parent, IColour* colour, unsigned int which) {
  cWhich = which;
  cParent = parent;
  cColour = colour;
  cBorrowedColour = NULL;
}
    
void DialogSpindizzyTextureSet::ColourSelector::render(SelectableComponent* component) {
  float mLeft =   component->getLeft();
  float mRight =  component->getRight();
  float mTop =    component->getTop();
  float mBottom = component->getBottom();
  cColour->set();
  glBegin(GL_QUADS);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glVertex2f(mLeft,  mTop);
  glEnd();
}

void DialogSpindizzyTextureSet::ColourSelector::selected() {
  cParent->cResourceSelector->addResourceSelectionListener(this);
}

void DialogSpindizzyTextureSet::ColourSelector::deselected() {
  cParent->cResourceSelector->removeResourceSelectionListener(this);
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceOwned(cBorrowedColour);
    cBorrowedColour = NULL;
  }
}

void DialogSpindizzyTextureSet::ColourSelector::resourceSelected(IColour* colour) {
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceReleased(cBorrowedColour);
  }
  cColour = colour;
  cBorrowedColour = colour;
  switch (cWhich) {
    case 0: cParent->cTextureSet->setFloorColour(cColour);  break;
    case 1: cParent->cTextureSet->setWallColour(cColour);   break;
    case 2: cParent->cTextureSet->setGridColour(cColour);   break;
    case 3: cParent->cTextureSet->setDetailColour(cColour); break;
  }
}

DialogSpindizzyTextureSet::TextureIcon::TextureIcon(ITexture* texture) {
  cTexture = texture;
}

float DialogSpindizzyTextureSet::TextureIcon::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return 0.1f * mAspectRatio;
}

float DialogSpindizzyTextureSet::TextureIcon::getHeight() {
  return 0.1f;
}

void DialogSpindizzyTextureSet::TextureIcon::render() {
  cTexture->set();
  float mLeft = getLeft();
  float mRight = getRight();
  float mTop = getTop();
  float mBottom = getBottom();
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 1.0f); glVertex2f(mLeft,  mBottom);
  glTexCoord2f(1.0f, 1.0f); glVertex2f(mRight, mBottom);
  glTexCoord2f(1.0f, 0.0f); glVertex2f(mRight, mTop);
  glTexCoord2f(0.0f, 0.0f); glVertex2f(mLeft,  mTop);
  glEnd();
}

void DialogSpindizzyTextureSet::TextureIcon::update(unsigned int milliseconds) {
  // Nothing to do
}

bool DialogSpindizzyTextureSet::TextureIcon::input(SDL_Event& event) {
  return false;
}



