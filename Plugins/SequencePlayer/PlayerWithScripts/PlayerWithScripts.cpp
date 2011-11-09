/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "PlayerWithScripts.h"

PlayerWithScripts::PlayerWithScripts(IRuntimeContext* runtimeContext) {
  cPaused = false;
  cForward = false;
  cLength = 1000;
  cPosition = 0;
  cLeftStartScript = Script::getDummy();
  cLeftEndScript = Script::getDummy();
  cReachedEndScript = Script::getDummy();
  cReachedStartScript = Script::getDummy();
  cRuntimeContext = runtimeContext;
}

void PlayerWithScripts::addSequence(ISequence* sequence) {
  cSequences.insert(sequence);
}

void PlayerWithScripts::removeSequence(ISequence* sequence) {
  cSequences.erase(sequence);
}

void PlayerWithScripts::play() {
  cForward = true;
}

void PlayerWithScripts::rewind() {
  cForward = false;
}

void PlayerWithScripts::pause() {
  cPaused = true;
}

void PlayerWithScripts::unpause() {
  cPaused = false;
}

void PlayerWithScripts::update(int milliseconds) {
  if (!cPaused && (cForward ? cPosition < cLength : cPosition > 0)) {
    if (cForward && cPosition == 0) {
      cLeftStartScript->execute();
    } else if (!cForward && cPosition < cLength) {
      cLeftEndScript->execute();
    }
    cPosition += cForward ? milliseconds : -milliseconds;
    if (cPosition >= cLength || cPosition <= 0) {
      if (cForward) {
        cPosition = cLength;
        cReachedEndScript->execute();
      } else {
        cPosition = 0;
        cReachedStartScript->execute();
      }
    }
    for (std::set<ISequence*>::iterator i = cSequences.begin(); i != cSequences.end(); i++) {
      (*i)->update(float(cPosition) / float(cLength));
    }
  }
}

void PlayerWithScripts::load(DOMNodeWrapper* node) {
  cLength = node->getIntegerAttribute("length");
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "LeftStartScript") {
      cLeftStartScript = cRuntimeContext->getScript(mNode);
    } else if (mValueAsString == "LeftEndScript") {
      cLeftEndScript = cRuntimeContext->getScript(mNode);
    } else if (mValueAsString == "ReachedStartScript") {
      cReachedStartScript = cRuntimeContext->getScript(mNode);
    } else if (mValueAsString == "ReachedEndScript") {
      cReachedEndScript = cRuntimeContext->getScript(mNode);
    }
  }
}

void PlayerWithScripts::save(DOMNodeWriter* node) {
  node->addAttribute("length", cLength);
  cLeftStartScript->save(node, "LeftStartScript");
  cLeftEndScript->save(node, "LeftEndScript");
  cReachedStartScript->save(node, "ReachedStartScript");
  cReachedEndScript->save(node, "ReachedEndScript");
}

std::vector<IDynamicElement*> PlayerWithScripts::getPreLoopCommands() {
  std::vector<IDynamicElement*> mPlayerUpdater;
  mPlayerUpdater.push_back(this);
  return mPlayerUpdater;
}

extern "C" IPlugin* create(IRuntimeContext* runtimeContext) {
  return new PlayerWithScripts(runtimeContext);
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

