#ifndef DIALOG_SPINDIZZY_TEXTURE_SET_H
#define DIALOG_SPINDIZZY_TEXTURE_SET_H

#include <IsoRealms/GUI/ISelector.h>
#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/GUI/SelectableComponent.h>
#include <IsoRealms/IConfirmationListener.h>

#include "C64SpindizzyTextureSet.h"

class DialogSpindizzyTextureSet:public ResizableDialog {
  private:
  class OKCommand:public ICommand {
    private:
    DialogSpindizzyTextureSet* cParent;
    
    public:
    OKCommand(DialogSpindizzyTextureSet*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class CancelCommand:public ICommand {
    private:
    DialogSpindizzyTextureSet* cParent;
    
    public:
    CancelCommand(DialogSpindizzyTextureSet*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class UndoCommand:public ICommand {
    private:
    DialogSpindizzyTextureSet* cParent;
    
    public:
    UndoCommand(DialogSpindizzyTextureSet*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  class ColourSelector:public ISelector,
                       public IResourceSelectionListener<IColour> {
    private:
    unsigned int cWhich;
    DialogSpindizzyTextureSet* cParent;
    IColour* cBorrowedColour;
    IColour* cColour;
    
    public:
    ColourSelector(DialogSpindizzyTextureSet*, IColour*, unsigned int);
    
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
  
  C64SpindizzyTextureSet* cTextureSet;
  IResourceSelector* cResourceSelector;
  IConfirmationListener* cConfirmationListener;
  IColour* cOriginalFloorColour;
  IColour* cOriginalWallColour;
  IColour* cOriginalGridColour;
  IColour* cOriginalDetailColour;
  ColourSelector* cColourSelectorFloor;
  ColourSelector* cColourSelectorWall;
  ColourSelector* cColourSelectorGrid;
  ColourSelector* cColourSelectorDetail;
  
  void undo();
  
  public:
  DialogSpindizzyTextureSet(IComponentContainer*, C64SpindizzyTextureSet*, IResourceAccessor*, IResourceSelector*, IConfirmationListener* = NULL);
  
  C64SpindizzyTextureSet* getTextureSet();
};

#endif
