/*
 * Copyright 2016 Martin Brentnall
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
#include "DialogInputConfiguration.h"

DialogInputConfiguration::DialogInputConfiguration(IComponentContainer* componentContainer, IResourceAccessor* resources, IEditor* editor, IDialogParent* parent) : ResizableDialog(componentContainer, "Modules/Editor/DialogInputConfiguration", resources) {
  std::vector<std::string> mColumnHeadings;
  mColumnHeadings.push_back("Name");
  mColumnHeadings.push_back("Mapping");
  mColumnHeadings.push_back("On Activate");
  mColumnHeadings.push_back("On Deactivate");
  cTable = new ComponentTable(mColumnHeadings, 0.0f);
  
  InputCommands* mInputCommands = editor->getInputConfiguration();
  std::map<std::string, DigitalInput*> mDigitalInputs = mInputCommands->getDigitalInputs();

  for (std::pair<std::string, DigitalInput*> mDigitalInput : mDigitalInputs) {
    std::vector<ISizedComponent*> mRow;
    
    IScriptCall* mActivatedScript   = mDigitalInput.second->getActivatedScript();
    IScriptCall* mDeactivatedScript = mDigitalInput.second->getDeactivatedScript();
    std::vector<SDLKey> mKeys       = mDigitalInput.second->getKeys();
    
    TextFieldComponent* mInputName                  = new TextFieldComponent(mDigitalInput.first, true, 1.0f, false);
    ComponentDigitalInputMapping* mInputMapping     = new ComponentDigitalInputMapping(mKeys,           0.4f);
    ComponentScriptCall* mInputActivateScriptCall   = new ComponentScriptCall(mActivatedScript,         0.6f);
    ComponentScriptCall* mInputDeactivateScriptCall = new ComponentScriptCall(mDeactivatedScript,       0.6f);
        
    mRow.push_back(mInputName);
    mRow.push_back(mInputMapping);
    mRow.push_back(mInputActivateScriptCall);
    mRow.push_back(mInputDeactivateScriptCall);
    cTable->addRow(mRow);
    
  }    
  
  addComponent("tabDigital/inputConfiguration", cTable);
}

