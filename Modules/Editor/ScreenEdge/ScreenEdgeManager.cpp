#include "ScreenEdgeManager.h"

void ScreenEdgeManager::update(unsigned int update) {
  cScreenEdgeBottom.update(update);
  cScreenEdgeLeft.update(update);
  cScreenEdgeRight.update(update);
  cScreenEdgeTop.update(update);
}

void ScreenEdgeManager::render() {
  cScreenEdgeBottom.render();
  cScreenEdgeLeft.render();
  cScreenEdgeRight.render();
  cScreenEdgeTop.render();
}

bool ScreenEdgeManager::input(SDL_Event& event) {
  return cScreenEdgeBottom.input(event)
      || cScreenEdgeLeft.input(event)
      || cScreenEdgeRight.input(event)
      || cScreenEdgeTop.input(event);
}

bool ScreenEdgeManager::contains(float x, float y) {
  return cScreenEdgeBottom.contains(x, y)
      || cScreenEdgeLeft.contains(x, y)
      || cScreenEdgeRight.contains(x, y)
      || cScreenEdgeTop.contains(x, y);
}

void ScreenEdgeManager::add(Dialog* resizableDialog) {
  cScreenEdgeRight.add(resizableDialog);
}

