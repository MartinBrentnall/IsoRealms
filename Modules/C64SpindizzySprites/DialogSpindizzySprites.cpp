#include "DialogSpindizzySprites.h"

DialogSpindizzySprites::DialogSpindizzySprites(IComponentContainer* container, C64SpindizzySpriteSet* textureSet, IResourceAccessor* resources, IResourceSelector* resourceSelector, IConfirmationListener* confirmationListener) : ResizableDialog(container, "Plugins/C64SpindizzySprites/DialogSpindizzySprites", resources) {
  cSprites = textureSet;
  cResourceSelector = resourceSelector;
  cConfirmationListener = confirmationListener;
  setComponentAction("okButton", new OKCommand(this));
  setComponentAction("cancelButton", new CancelCommand(this));
  setComponentAction("undoButton", new UndoCommand(this));
  cOriginalColour1       = cSprites->getColour1();
  cOriginalColour2       = cSprites->getColour2();
  cOriginalColour3       = cSprites->getColour3();
  cOriginalColourOutline = cSprites->getColourOutline();
  cColourSelector1       = new ColourSelector(this, cOriginalColour1,       0);
  cColourSelector2       = new ColourSelector(this, cOriginalColour2,       1);
  cColourSelector3       = new ColourSelector(this, cOriginalColour3,       2);
  cColourSelectorOutline = new ColourSelector(this, cOriginalColourOutline, 3);
  setSelectable("colour1",       cColourSelector1);
  setSelectable("colour2",       cColourSelector2);
  setSelectable("colour3",       cColourSelector3);
  setSelectable("outlineColour", cColourSelectorOutline);
  std::vector<ITexture*> mTextures = cSprites->getTextures();
  for (unsigned int i = 0; i < mTextures.size(); i++) {
    addComponent("previewPane", new TextureIcon(mTextures[i]));
  }
}

void DialogSpindizzySprites::undo() {
  cColourSelector1->resourceSelected(cOriginalColour1);
  cColourSelector2->resourceSelected(cOriginalColour2);
  cColourSelector3->resourceSelected(cOriginalColour3);
  cColourSelectorOutline->resourceSelected(cOriginalColourOutline);
}

C64SpindizzySpriteSet* DialogSpindizzySprites::getSprites() {
  return cSprites;
}

DialogSpindizzySprites::OKCommand::OKCommand(DialogSpindizzySprites* parent) {
  cParent = parent;
}

void DialogSpindizzySprites::OKCommand::execute() {
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogConfirmed(cParent);
  }
}

DialogSpindizzySprites::CancelCommand::CancelCommand(DialogSpindizzySprites* parent) {
  cParent = parent;
}

void DialogSpindizzySprites::CancelCommand::execute() {
  cParent->undo();
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogCancelled(cParent);
  }
}

DialogSpindizzySprites::UndoCommand::UndoCommand(DialogSpindizzySprites* parent) {
  cParent = parent;
}

void DialogSpindizzySprites::UndoCommand::execute() {
  cParent->undo();
}

DialogSpindizzySprites::ColourSelector::ColourSelector(DialogSpindizzySprites* parent, IColour* colour, unsigned int which) {
  cWhich = which;
  cParent = parent;
  cColour = colour;
  cBorrowedColour = NULL;
}
    
void DialogSpindizzySprites::ColourSelector::render(SelectableComponent* component) {
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

void DialogSpindizzySprites::ColourSelector::selected() {
  cParent->cResourceSelector->addResourceSelectionListener(this);
}

void DialogSpindizzySprites::ColourSelector::deselected() {
  cParent->cResourceSelector->removeResourceSelectionListener(this);
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceOwned(cBorrowedColour);
    cBorrowedColour = NULL;
  }
}

void DialogSpindizzySprites::ColourSelector::resourceSelected(IColour* colour) {
  if (cBorrowedColour != NULL) {
    cParent->cResourceSelector->notifyResourceReleased(cBorrowedColour);
  }
  cColour = colour;
  cBorrowedColour = colour;
  switch (cWhich) {
    case 0: cParent->cSprites->setColour1(cColour);       break;
    case 1: cParent->cSprites->setColour2(cColour);       break;
    case 2: cParent->cSprites->setColour3(cColour);       break;
    case 3: cParent->cSprites->setColourOutline(cColour); break;
  }
}

DialogSpindizzySprites::TextureIcon::TextureIcon(ITexture* texture) {
  cTexture = texture;
}

float DialogSpindizzySprites::TextureIcon::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return 0.1f * mAspectRatio;
}

float DialogSpindizzySprites::TextureIcon::getHeight() {
  return 0.1f;
}

void DialogSpindizzySprites::TextureIcon::render() {
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

void DialogSpindizzySprites::TextureIcon::update(unsigned int milliseconds) {
  // Nothing to do
}

bool DialogSpindizzySprites::TextureIcon::input(SDL_Event& event) {
  return false;
}



