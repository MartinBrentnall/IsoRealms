#include "SoundConfigurationDialog.h"

// TODO: Deallocate resources

SoundConfigurationDialog::SoundConfigurationDialog(IComponentContainer* componentContainer, ISoundRegistry* soundRegistry) : ResizableDialog(componentContainer, "Choose Sounds", 0.0f, 0.0f, 0.75f, 0.75f) {
  cUseSoundCommand = new UseSoundCommand(this);
  cRemoveSoundCommand = new RemoveSoundCommand(this);

  cSoundRegistry = soundRegistry;
  std::string mSoundsDirectory = System::getResource("Sounds/");
  std::vector<std::string>* mAvailableSounds = System::getFileList(mSoundsDirectory, "*.ogg");
  std::vector<ISound*> mSounds = cSoundRegistry->getSounds();
  for (unsigned int i = 0; i < mAvailableSounds->size(); i++) {
    bool mUsed = false;
    for (unsigned int j = 0; j < mSounds.size(); j++) {
      std::string mSoundName = mSounds[j]->getName();
      if (mSoundName == (*mAvailableSounds)[i]) {
        cUsingList.addItem(mSoundName);
        mUsed = true;
        break;
      }
    }
    if (!mUsed) {
      cAvailableList.addItem((*mAvailableSounds)[i]);
    }
  }
  
  EdgeRelation* mInsideDialog = new EdgeRelation(this, EdgeRelation::INSIDE);
  IComponentBoundsCalculator* mGridLayout = new ComponentEdgeLayout(mInsideDialog, mInsideDialog, mInsideDialog, mInsideDialog, NULL);
  GridLayoutComponent* mGrid = new GridLayoutComponent(2, 1, mGridLayout);
  IComponentBoundsCalculator* mLeftCell = mGrid->getCellLayout(0, 0);
  IComponentBoundsCalculator* mRightCell = mGrid->getCellLayout(1, 0);
  EdgeRelation* mInsideLeftCell = new EdgeRelation(mLeftCell, EdgeRelation::ALIGNED);
  EdgeRelation* mInsideRightCell = new EdgeRelation(mRightCell, EdgeRelation::ALIGNED);
  
  Button* mUseSoundButton = new Button(NULL, cUseSoundCommand, "Use Sound");
  IComponentBoundsCalculator* mUseButtonLayout = new ComponentEdgeLayout(NULL, NULL, mInsideRightCell, mInsideRightCell, mUseSoundButton);
  mUseSoundButton->setBoundsCalculator(mUseButtonLayout);
  
  Button* mRemoveSoundButton = new Button(NULL, cRemoveSoundCommand, "Remove Sound");
  IComponentBoundsCalculator* mRemoveButtonLayout = new ComponentEdgeLayout(NULL, NULL, mInsideLeftCell, mInsideLeftCell, mRemoveSoundButton);
  mRemoveSoundButton->setBoundsCalculator(mRemoveButtonLayout);
  
  TextLabelComponent* mAvailableLabel = new TextLabelComponent("Available:");
  IComponentBoundsCalculator* mAvailableLabelLayout = new ComponentEdgeLayout(mInsideRightCell, mInsideRightCell, NULL, NULL, mAvailableLabel);
  mAvailableLabel->setBoundsCalculator(mAvailableLabelLayout);
  
  TextLabelComponent* mUsingLabel = new TextLabelComponent("Using:");
  IComponentBoundsCalculator* mUsingLabelLayout = new ComponentEdgeLayout(mInsideLeftCell, mInsideLeftCell, NULL, NULL, mUsingLabel);
  mUsingLabel->setBoundsCalculator(mUsingLabelLayout);
  
  ScrollableContainer* mAvailableListContainer = new ScrollableContainer();
  EdgeRelation* mAdjacentAvailableLabel = new EdgeRelation(mAvailableLabel, EdgeRelation::OUTSIDE, 0.0f);
  EdgeRelation* mAdjacentUseButton = new EdgeRelation(mUseSoundButton, EdgeRelation::OUTSIDE);
  IComponentBoundsCalculator* mAvailableListLayout = new ComponentEdgeLayout(mAdjacentAvailableLabel, mInsideRightCell,
  mAdjacentUseButton, mInsideRightCell, NULL);
  mAvailableListContainer->setBoundsCalculator(mAvailableListLayout);
  mAvailableListContainer->setRootComponent(&cAvailableList);
  
  ScrollableContainer* mUsingListContainer = new ScrollableContainer();
  EdgeRelation* mAdjacentUsingLabel = new EdgeRelation(mUsingLabel, EdgeRelation::OUTSIDE, 0.0f);
  EdgeRelation* mAdjacentRemoveButton = new EdgeRelation(mRemoveSoundButton, EdgeRelation::OUTSIDE);
  IComponentBoundsCalculator* mUsingListLayout = new ComponentEdgeLayout(mAdjacentUsingLabel, mInsideLeftCell, mAdjacentRemoveButton, mInsideLeftCell, NULL);
  mUsingListContainer->setBoundsCalculator(mUsingListLayout);
  mUsingListContainer->setRootComponent(&cUsingList);
  
  addComponent(mUseSoundButton);
  addComponent(mRemoveSoundButton);
  addComponent(mAvailableLabel);
  addComponent(mUsingLabel);
  addComponent(mAvailableListContainer);
  addComponent(mUsingListContainer);
  
  setFocusedComponent(mAvailableListContainer);
}

SoundConfigurationDialog::UseSoundCommand::UseSoundCommand(SoundConfigurationDialog* parent) {
  cParent = parent;
}

void SoundConfigurationDialog::UseSoundCommand::execute() {
  std::string mFile = cParent->cAvailableList.getSelectedItem();
  SDLMixerSound* mSound = new SDLMixerSound(mFile);
  mSound->play();
  cParent->cAvailableList.removeItem(mFile);
  cParent->cUsingList.addItem(mFile);
  cParent->cSoundRegistry->addSound(mSound);
}

SoundConfigurationDialog::RemoveSoundCommand::RemoveSoundCommand(SoundConfigurationDialog* parent) {
  cParent = parent;
}

void SoundConfigurationDialog::RemoveSoundCommand::execute() {
  std::string mFile = cParent->cUsingList.getSelectedItem();
  std::vector<ISound*> mSounds = cParent->cSoundRegistry->getSounds();
  for (unsigned int i = 0; i < mSounds.size(); i++) {
    std::string mSoundName = mSounds[i]->getName();
    if (mFile == mSoundName) {
      cParent->cUsingList.removeItem(mFile);
      cParent->cAvailableList.addItem(mFile);
      cParent->cSoundRegistry->removeSound(mSounds[i]);
      return;
    }
  }
}

