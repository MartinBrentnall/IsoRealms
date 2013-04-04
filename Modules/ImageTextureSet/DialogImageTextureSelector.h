#ifndef DIALOG_IMAGE_TEXTURE_SELECTOR_H
#define DIALOG_IMAGE_TEXTURE_SELECTOR_H

#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/GUI/Dialogs/ComponentFileSelector.h>
#include <IsoRealms/GUI/ResizableDialog.h>

class DialogImageTextureSelector:public ResizableDialog,
                                 public IFileSelectionListener {
  private:
  ComponentFileSelector* cFileSelector;
  IConfirmationListener* cConfirmationListener;
  std::string cFile;
  
  class OKCommand:public ICommand {
    private:
    DialogImageTextureSelector* cParent;
    
    public:
    OKCommand(DialogImageTextureSelector*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class CancelCommand:public ICommand {
    private:
    DialogImageTextureSelector* cParent;
    
    public:
    CancelCommand(DialogImageTextureSelector*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  void confirmSelection();
  
  public:
  DialogImageTextureSelector(IComponentContainer*, IResourceAccessor*, IConfirmationListener*);
  
  std::string getFileName();
  
  /*************************************\
   * Implements IFileSelectionListener *
  \*************************************/
  void fileSelectionAsserted(const std::string&);
};

#endif
