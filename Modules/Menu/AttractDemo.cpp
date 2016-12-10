/*
 * Copyright 2009 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "AttractDemo.h"

AttractDemo::AttractDemo(DOMNodeWrapper* node) {
  std::string mProjectName = node->getAttribute("project");
  bool mProjectUser = node->getBooleanAttribute("user");
  IProjectOptions* mProjectOptions = new ProjectOptions(node);
  cProject = new Project(mProjectName, mProjectUser, nullptr, false, mProjectOptions);
  init();
}

void AttractDemo::init() {
}

void AttractDemo::update(int milliseconds) {
  if (cProject != nullptr) {
    cProject->updateRuntime(milliseconds);
  }
}

void AttractDemo::render() {
  glPushMatrix();
  if (cProject != nullptr) {
    cProject->renderRuntime();
  }
  glPopMatrix();
}

bool AttractDemo::hasFinished() {
  if (cProject != nullptr) {
    return cProject->hasCompleted();
  }
  return false;
}

void AttractDemo::frontEndActive(bool active) {
  // TODO: Implement this
}
