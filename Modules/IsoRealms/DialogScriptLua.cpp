/*
 * Copyright 2015 Martin Brentnall
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
#include "DialogScriptLua.h"

DialogScriptLua::DialogScriptLua(IEditingContext* editingContext, LuaScript* script, IResourceAccessor* resources) : ResizableDialog(editingContext->getComponentContainer(), "Modules/IsoRealms/DialogScriptLua", resources) {
  setComponentAction("okButton", new OKCommand(this));
  setComponentAction("cancelButton", new CancelCommand(this));
  cLuaScript = script;
  std::string mCode = cLuaScript->getCode();
  setStringValue("codeEditor", mCode);
}

void DialogScriptLua::addConfirmationListener(IConfirmationListener* listener) {
  cConfirmationListener = listener;
}

LuaScript* DialogScriptLua::getResource() {
  return cLuaScript;
}

std::string DialogScriptLua::getResourceName() {
  return "TODO";
}
  
void DialogScriptLua::confirmSelection() {
  close();
  if (cConfirmationListener != NULL) {
    cConfirmationListener->dialogConfirmed(this);
  }
}

DialogScriptLua::OKCommand::OKCommand(DialogScriptLua* parent) {
  cParent = parent;
}

void DialogScriptLua::OKCommand::execute() {
  cParent->confirmSelection();
}

DialogScriptLua::CancelCommand::CancelCommand(DialogScriptLua* parent) {
  cParent = parent;
}

void DialogScriptLua::CancelCommand::execute() {
  cParent->close();
  if (cParent->cConfirmationListener != NULL) {
    cParent->cConfirmationListener->dialogCancelled(cParent);
  }
}
