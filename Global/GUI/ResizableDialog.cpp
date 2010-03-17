#include "ResizableDialog.h"

const float ResizableDialog::RESIZE_HANDLE_SIZE = 0.05f;

ResizableDialog::ResizableDialog(IComponentContainer* componentContainer, const std::string& title, float x, float y, float width, float height) : Dialog(componentContainer, title, x, y, width, height) {
  cResizing = false;
}

ResizableDialog::ResizableDialog(IComponentContainer* componentContainer, const std::string& dialogDescriptionFile) : Dialog(componentContainer, dialogDescriptionFile) {
  cResizing = false;
}

void ResizableDialog::renderContent() {
  float mRight = getRight();
  float mBottom = getBottom();
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();

  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_LINE_STRIP);
  glColor3f(0.8f, 0.6f, 1.0f);
  // Resize handle
  glVertex2f(mRight,                                     mBottom + RESIZE_HANDLE_SIZE);
  glVertex2f(mRight - 0.01f * mAspectRatio,              mBottom + RESIZE_HANDLE_SIZE);
  glVertex2f(mRight - 0.01f * mAspectRatio,              mBottom + 0.01f); 
  glVertex2f(mRight - RESIZE_HANDLE_SIZE * mAspectRatio, mBottom + 0.01f); 
  glVertex2f(mRight - RESIZE_HANDLE_SIZE * mAspectRatio, mBottom);
  glEnd();

  renderResizableDialogContent();
}

void ResizableDialog::updateContent(int ticks) {
  updateResizableDialogContent(ticks);
}

bool ResizableDialog::mouseButtonDown(SDL_Event& event) {
  float mRight = getRight();
  float mBottom = getBottom();
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  if (contains(mX, mY)) {
    if (mX >= mRight - RESIZE_HANDLE_SIZE * mAspectRatio && mX <= mRight && mY >= mBottom && mY <= mBottom + RESIZE_HANDLE_SIZE) {
      cResizing = true;
      // TODO: Bring container to front?
      return true;
    }
  }
  return false;
}

bool ResizableDialog::mouseMotion(SDL_Event& event) {
  if (cResizing) {
    Configuration* mConfiguration = Configuration::getInstance();
    ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
    float mX = mScreen->getXLocation(event.motion.xrel) + 1.0f;
    float mY = mScreen->getYLocation(event.motion.yrel) - 1.0f;
    resize(mX, -mY);
    translate(0, mY);
    return true;
  }
  return false;
}

bool ResizableDialog::inputContent(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      return mouseButtonDown(event);
    }

    case SDL_MOUSEBUTTONUP: {
      bool cWasResizing = cResizing;
      cResizing = false;
      return cWasResizing;
    }

    case SDL_MOUSEMOTION: {
      return mouseMotion(event);
    }

    default: {
      break;
    }
  }
  return inputResizableDialogContent(event);
}
