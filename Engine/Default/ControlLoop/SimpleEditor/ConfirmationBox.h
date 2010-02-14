#ifndef CONFIRMATION_BOX_H
#define CONFIRMATION_BOX_H

#include <string>

#include "../../../../Global/ComponentEdgeLayout.h"
#include "../../../../Global/EdgeRelation.h"
#include "../../../../Global/ICommand.h"
#include "../../../../Global/ResizableDialog.h"

#include "Button.h"
#include "TextLabelComponent.h"

class ConfirmationBox:public ResizableDialog {
  private:
  std::string cMessage;

  class OKCommand:public ICommand {
    private:
    ConfirmationBox* cParent;
    ICommand* cCommand;

    public:
    OKCommand(ConfirmationBox*, ICommand*);

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  class CancelCommand:public ICommand {
    private:
    ConfirmationBox* cParent;

    public:
    CancelCommand(ConfirmationBox*);

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  public:

  /**
   * 
   */
  ConfirmationBox(IComponentContainer*, ICommand*, std::string&, std::string&);

  /******************************\
   * Implements ResizableDialog *
  \******************************/
  void updateResizableDialogContent(int);
  void renderResizableDialogContent();
  bool inputResizableDialogContent(SDL_Event&);
};

#endif
