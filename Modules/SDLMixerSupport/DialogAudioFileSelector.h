#ifndef DIALOG_AUDIO_FILE_SELECTOR_H
#define DIALOG_AUDIO_FILE_SELECTOR_H

#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/GUI/Dialogs/ComponentFileSelector.h>
#include <IsoRealms/GUI/ResizableDialog.h>

class DialogAudioFileSelector:public ResizableDialog,
                              public IFileSelectionListener {
  private:
  ComponentFileSelector* cFileSelector;
  IConfirmationListener* cConfirmationListener;
  std::string cFile;
  
  class OKCommand:public ICommand {
    private:
    DialogAudioFileSelector* cParent;
    
    public:
    OKCommand(DialogAudioFileSelector*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class CancelCommand:public ICommand {
    private:
    DialogAudioFileSelector* cParent;
    
    public:
    CancelCommand(DialogAudioFileSelector*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  void confirmSelection();
  
  public:
  DialogAudioFileSelector(IComponentContainer*, IResourceAccessor*, IConfirmationListener*);
  
  std::string getFileName();
  
  /*************************************\
   * Implements IFileSelectionListener *
  \*************************************/
  void fileSelectionAsserted(const std::string&);
};

#endif
