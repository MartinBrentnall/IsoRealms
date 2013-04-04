#ifndef COLOUR_SELECTOR_H
#define COLOUR_SELECTOR_H

#include <IsoRealms/GUI/Dialogs/ComponentColourSelector.h>
#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/Resources/Colour/Colour.h>
#include <IsoRealms/Resources/IResourceUseListener.h>

class DialogColourSelector:public ResizableDialog {
  private:
  ComponentColourSelector* cColourSelector;
  IConfirmationListener* cConfirmationListener;
  Colour* cColour;
  float cRed;
  float cGreen;
  float cBlue;
  float cAlpha;

  class OKCommand:public ICommand {
    private:
    DialogColourSelector* cParent;
    
    public:
    OKCommand(DialogColourSelector*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class CancelCommand:public ICommand {
    private:
    DialogColourSelector* cParent;
    
    public:
    CancelCommand(DialogColourSelector*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class UndoCommand:public ICommand {
    private:
    DialogColourSelector* cParent;
    
    public:
    UndoCommand(DialogColourSelector*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  public:
  DialogColourSelector(IComponentContainer*, Colour*, IResourceAccessor*, IConfirmationListener* = NULL);
  
  Colour* getColour();
  void undo();
};

#endif
