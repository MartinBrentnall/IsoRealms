#ifndef SPINDIZZY_LIFT_CONFIGURATION_COMPONENT_H
#define SPINDIZZY_LIFT_CONFIGURATION_COMPONENT_H

#include <string>

#include "../../Global/GUI/Button.h"
#include "../../Global/GUI/CloseDialogCommand.h"
#include "../../Global/GUI/ComponentEdgeLayout.h"
#include "../../Global/GUI/EdgeRelation.h"
#include "../../Global/GUI/FlexibleGridLayoutComponent.h"
#include "../../Global/GUI/ResizableDialog.h"
#include "../../Global/GUI/ScrollableContainer.h"
#include "../../Global/GUI/TextLabelComponent.h"
#include "../../Global/GUI/TextFieldComponent.h"
#include "../../Global/IComponentContainer.h"

#include "SpindizzyLiftProperties.h"

class SpindizzyLiftConfigurationComponent:public ResizableDialog {
  private:
  SpindizzyLiftProperties* cProperties;
  TextFieldComponent* cUpSpeedField;
  TextFieldComponent* cDownSpeedField;
  TextFieldComponent* cTopDelayField;
  TextFieldComponent* cBottomDelayField;
  
  std::string toString(unsigned int);

  class ApplyCommand:public ICommand {
    private:
    SpindizzyLiftConfigurationComponent* cParent;

    public:
    ApplyCommand(SpindizzyLiftConfigurationComponent*);

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  public:
  SpindizzyLiftConfigurationComponent(IComponentContainer*, SpindizzyLiftProperties*);

  /******************************\
   * Implements ResizableDialog *
  \******************************/
  bool inputResizableDialogContent(SDL_Event&);
  void renderResizableDialogContent();
  void updateResizableDialogContent(int ticks);
};

#endif
