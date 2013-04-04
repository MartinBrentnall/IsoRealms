#ifndef DIALOG_SPINDIZZY_SPRITES_H
#define DIALOG_SPINDIZZY_SPRITES_H

#include <IsoRealms/GUI/ISelector.h>
#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/GUI/SelectableComponent.h>
#include <IsoRealms/IConfirmationListener.h>

#include "C64SpindizzySpriteSet.h"

class DialogSpindizzySprites:public ResizableDialog {
  private:
  class OKCommand:public ICommand {
    private:
    DialogSpindizzySprites* cParent;
    
    public:
    OKCommand(DialogSpindizzySprites*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class CancelCommand:public ICommand {
    private:
    DialogSpindizzySprites* cParent;
    
    public:
    CancelCommand(DialogSpindizzySprites*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class UndoCommand:public ICommand {
    private:
    DialogSpindizzySprites* cParent;
    
    public:
    UndoCommand(DialogSpindizzySprites*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  class ColourSelector:public ISelector,
                       public IResourceSelectionListener<IColour> {
    private:
    unsigned int cWhich;
    DialogSpindizzySprites* cParent;
    IColour* cBorrowedColour;
    IColour* cColour;
    
    public:
    ColourSelector(DialogSpindizzySprites*, IColour*, unsigned int);
    
    void render(SelectableComponent*);
    void selected();
    void deselected();
    
    void resourceSelected(IColour*);
  };
    
  class TextureIcon:public ISizedComponent {
    private:
    ITexture* cTexture;
    
    public:
    TextureIcon(ITexture*);
        
    /******************************\
     * Implements ISizedComponent *
    \******************************/
    float getWidth();
    float getHeight();
    void render();
    void update(unsigned int);
    bool input(SDL_Event&);
  };
  
  C64SpindizzySpriteSet* cSprites;
  IResourceSelector* cResourceSelector;
  IConfirmationListener* cConfirmationListener;
  IColour* cOriginalColour1;
  IColour* cOriginalColour2;
  IColour* cOriginalColour3;
  IColour* cOriginalColourOutline;
  ColourSelector* cColourSelector1;
  ColourSelector* cColourSelector2;
  ColourSelector* cColourSelector3;
  ColourSelector* cColourSelectorOutline;
  
  void undo();
  
  public:
  DialogSpindizzySprites(IComponentContainer*, C64SpindizzySpriteSet*, IResourceAccessor*, IResourceSelector*, IConfirmationListener* = NULL);
  
  C64SpindizzySpriteSet* getSprites();
};

#endif
