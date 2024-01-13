##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=spindizzyremake
ConfigurationName      :=Debug
WorkspacePath          :=/home/martin/spindizzyremake
ProjectPath            :=/home/martin/spindizzyremake
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Martin Brentnall
Date                   :=06/07/21
CodeLitePath           :=/home/martin/.codelite
LinkerName             :=/usr/bin/x86_64-linux-gnu-g++
SharedObjectLinkerName :=/usr/bin/x86_64-linux-gnu-g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="spindizzyremake.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -O0
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/x86_64-linux-gnu-ar rcu
CXX      := /usr/bin/x86_64-linux-gnu-g++
CC       := /usr/bin/x86_64-linux-gnu-gcc
CXXFLAGS :=  -g -Wall $(Preprocessors)
CFLAGS   :=   $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDString.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDModel.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDPanel.cpp$(ObjectSuffix) $(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(ObjectSuffix) $(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Editor_Editor_Editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Replay_Recorder_Recorder.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_HighScore_NameEntry_NameEntry.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectSaveAs_DialogProjectSaveAs.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectOpen_DialogProjectOpen.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_DialogResourcePrimitives.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_DialogResourceColoursTextures.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_DialogResource3DModels.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_Icon3DModel.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeTop.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeHorizontal.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeRight.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdge.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeProject.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconTexture.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_DialogProjectConfiguration.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_IconBoolean.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResourceType.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeDefault.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_DialogResourceElements.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Player_Player.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Pyramid_Pyramid.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_C64LiftGraphics_C64LiftGraphics.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunk.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallConstructionData.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileBlock.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileColumn.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResource.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_GeometryProcessor.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileSurfaceTemplate.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumn.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColour.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTextureIcon.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexture.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUp.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUpType.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Gyroscope_Gyroscope.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeVertical.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Zone_ZoneType.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/Modules_Spindizzy_Physics_PhysicalState.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Physics_Physics.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_DebrisGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Water_Water.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Water_WaterType.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Jewel_Jewel.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_Spindizzy_Enemy_Enemy.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSplitSurface.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSurface.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCapDialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainProperties.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainState.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeModule.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainCollisionData.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainType.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftType.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_BoundaryHandler.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Boundaries_Boundaries.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemInputs.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemBoolean.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemSlider.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeLeft.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Boundaries_AbstractBoundaryHandler.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemFileList.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemAction.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeManager.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_FrontEnd_Project_ProjectLoader.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_FrontEnd_Prompt_Prompt.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_Texture_Texture.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_HighScore_ScoreTable_ScoreTable.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_ColourScreen_ColourScreen.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Options_WrappedOptions.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_DialogResourceFonts.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_SpindizzyScriptSupport.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_HighScore_ScoreTracker_ScoreTracker.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Properties_ConditionComponent.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialogGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogEditables_DialogEditables.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Properties_PropertyModel.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_Editor_Editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Properties_DialogModel.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourceProperty.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Properties_ConditionValueEditorComponent.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Ball_Ball.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSet.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Properties_DialogCondition.cpp$(ObjectSuffix) \
	

Objects2=$(IntermediateDirectory)/IsoRealms_GUI_DynamicMenuItems.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_LookAndFeel.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Colour.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_DockedDialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModelInstance.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_RollingFloat_RollingFloat.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Enemy_EnemyType.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Properties_PropertyString.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_MenuPopup.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_GridLayoutComponent.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventPause.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_AbstractRectangularComponent.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_FloatToString.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Registry_VertexRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceScripts_DialogResourceScripts.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_MenuItem.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_FrontEnd_Throbber_Throbber.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_ComponentScriptCall.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Properties_ModelValueComponent.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Replay_Player_Player.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_Basics_Sprite_SpriteInstance.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_World_WorldSurfaces.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_FlexibleGridLayoutComponent.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_RectangularComponent.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Application.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDScreen.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentColourSelector.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_CheckBox.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Boolean.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_TabbedContainer.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/IsoRealms_GUI_Dialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconColour.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Lift_Lift.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_Integer_Integer.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_ScrollableContainer.cpp$(ObjectSuffix) 

Objects3=$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemResolution.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_SliderComponent.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrack.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyColourSaturation.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_CheckBoxWithLabel.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/IsoRealms_GUI_ComponentEdgeLayout.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyVertex.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Persistence_ParseException.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_MenuBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceVertices_DialogResourceVertices.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeInteger.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Project.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_FrontEnd_Menu_Menu.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_Container.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeFloat.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/IsoRealms_Collision_CollisionUtils.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeIcon.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Replay_ReplayLuaSupport.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternTile.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Input_AxisMapping.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Zone_Zone.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Integer.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyAsset.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunkInstance.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Options_LocalOptions.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/IsoRealms_System.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Options_Options.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_AnimatedFloat.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Spindizzy.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_TextLabelComponent.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Registry_AssetIDException.cpp$(ObjectSuffix) 

Objects4=$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeBoolean.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_IconElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_Sequence_SequenceDialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_Timer_Timer.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_Basics_HUD_HUDComponent.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms.cpp$(ObjectSuffix) $(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_GUIIcon.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_LocalAssetRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_InitException.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_HUD_HUD.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyInteger.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyAction.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyInputField.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_HighScore_ModuleHighScore.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModel.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_ComponentTable.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyColour.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Registry_InputHandlerRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Player_PlayerType.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconTexture.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_ComponentTree.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyFloat.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_FixedSizeLayout.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/IsoRealms_ResourcePropertySlider.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyScreen.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_RectangleBoundsCalculator.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_Popup.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeString.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_BasicsScriptSupport.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_AssetRegistryRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_ComponentDigitalInputMapping.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogColourSelector.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_InputSwitch_InputSwitch.cpp$(ObjectSuffix) \
	

Objects5=$(IntermediateDirectory)/Modules_Basics_ColourCycler_ColourCycler.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourceType.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Utils.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCap.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_MultipleClickDetector.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_InterruptHandler_InterruptHandler.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyColourLightness.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyColourHue.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyModel.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourceElement_ElementBounds.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/IsoRealms_Rectangle.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Common_ScreenArea.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Input_ButtonMapping.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourceInitException.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_WrappingGridComponent.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyBoolean.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_World_WorldEditor.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_IntegerField.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyInputHandler.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_Basics_HUD_HUDEditor.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_HUD_HUDComponentRelation.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumnPossibility.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_PanelComponent.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_TextFieldComponent.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Persistence_DOMNode.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Persistence_DOMNodeWriter.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Common_ScreenRay.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Common_BlockArea.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Common_ScreenLocation.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColourIcon.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyColourChannel.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Condition_ConditionElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_ModuleEditor.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_SpindizzyBlockConfigurationComponent.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Condition_Condition.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_HighScore_ModuleHighScoreScriptSupport.cpp$(ObjectSuffix) 

Objects6=$(IntermediateDirectory)/IsoRealms_Properties_ConditionValueComponent.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Collision_CollisionData.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Collision_Collision.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/IsoRealms_LuaSupport_LuaScript.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventFade.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentDefinition.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentValueProxy.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Input_KeyMapping.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Properties_PropertyCondition.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_Basics.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Registry_StringRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_ModelCycler_ModelCycler.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_SelectableComponent.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_Basics_ResourcePropertyHUDEditor.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_Theme.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_FrontEnd_FrontEndScriptSupport.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_FrontEnd_FrontEnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ScreenMode.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_Terrain.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_HighScore_HighScoreRecord.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Properties_ConditionElementIcon.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertySpindizzyWorld.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertyWorldEditor.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeBottom.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_EdgeRelation.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_ResourcePropertyTexture.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Common_BlockLocation.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Font.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_ModuleEditorScriptSupport.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogConfirmation.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Float.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Registry_FontRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Editor_ModuleEditor.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftCollisionData.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModel.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModelInstance.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_TreeNode.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_ResizableDialog.cpp$(ObjectSuffix) 

Objects7=$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentFileSelector.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Properties_Property.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_ListBox.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_Button.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_LuaSupport_LuaState.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogOKCancelUndo.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_Float_Float.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_HighScore_HighScoreTable.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_C64TerrainGraphics_C64TerrainGraphics.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Vertex.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Texture.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_String.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_Vertex_Vertex.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_IntegerToString.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Registry_FloatRegistry.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/IsoRealms_Module.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Registry_ModelRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_FrontEnd_Project_Project.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Registry_ColourRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Registry_TextureRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_Editor_EditorDialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_World_World.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallSurface.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_IsoRealmsConstants.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Collision_CollisionElement.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/IsoRealms_Assets_Registry_ScreenRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Registry_IntegerRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Registry_ActionTypeRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_IconFont.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Registry_EditableRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_WorldView_WorldView.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexturesDialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_Assets_Registry_BooleanRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_InputGroup_InputGroup.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_Replay_Replay.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_Sprite_Sprite.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_BooleanTrigger_BooleanTrigger.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_Font_Font.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_Sound_Sound.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_ProjectOptions_ProjectOptions.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_DigitalInput_DigitalInput.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_EditorOLD_DialogProperties_DialogProperties.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_String_String.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_Sequence_ActionTrack.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Modules_Basics_Sequence_Sequence.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_Debris.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_Boolean_Boolean.cpp$(ObjectSuffix) $(IntermediateDirectory)/IsoRealms_GUI_TextEditorComponent.cpp$(ObjectSuffix) 

Objects8=$(IntermediateDirectory)/Modules_Basics_Script_ScriptType.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Spindizzy_Water_WaterSurface.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_HUD_ScreenRelative.cpp$(ObjectSuffix) $(IntermediateDirectory)/Modules_Basics_Colour_Colour.cpp$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) $(Objects2) $(Objects3) $(Objects4) $(Objects5) $(Objects6) $(Objects7) $(Objects8) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	@echo $(Objects2) >> $(ObjectsFileList)
	@echo $(Objects3) >> $(ObjectsFileList)
	@echo $(Objects4) >> $(ObjectsFileList)
	@echo $(Objects5) >> $(ObjectsFileList)
	@echo $(Objects6) >> $(ObjectsFileList)
	@echo $(Objects7) >> $(ObjectsFileList)
	@echo $(Objects8) >> $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDString.cpp$(ObjectSuffix): Modules/Basics/HUD/Components/HUDString.cpp $(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDString.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/HUD/Components/HUDString.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDString.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDString.cpp$(DependSuffix): Modules/Basics/HUD/Components/HUDString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDString.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDString.cpp$(DependSuffix) -MM Modules/Basics/HUD/Components/HUDString.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDString.cpp$(PreprocessSuffix): Modules/Basics/HUD/Components/HUDString.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDString.cpp$(PreprocessSuffix) Modules/Basics/HUD/Components/HUDString.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDModel.cpp$(ObjectSuffix): Modules/Basics/HUD/Components/HUDModel.cpp $(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDModel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/HUD/Components/HUDModel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDModel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDModel.cpp$(DependSuffix): Modules/Basics/HUD/Components/HUDModel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDModel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDModel.cpp$(DependSuffix) -MM Modules/Basics/HUD/Components/HUDModel.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDModel.cpp$(PreprocessSuffix): Modules/Basics/HUD/Components/HUDModel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDModel.cpp$(PreprocessSuffix) Modules/Basics/HUD/Components/HUDModel.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDPanel.cpp$(ObjectSuffix): Modules/Basics/HUD/Components/HUDPanel.cpp $(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDPanel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/HUD/Components/HUDPanel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDPanel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDPanel.cpp$(DependSuffix): Modules/Basics/HUD/Components/HUDPanel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDPanel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDPanel.cpp$(DependSuffix) -MM Modules/Basics/HUD/Components/HUDPanel.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDPanel.cpp$(PreprocessSuffix): Modules/Basics/HUD/Components/HUDPanel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDPanel.cpp$(PreprocessSuffix) Modules/Basics/HUD/Components/HUDPanel.cpp

$(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(ObjectSuffix): build/CMakeFiles/3.16.3/CompilerIdC/CMakeCCompilerId.c $(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/martin/spindizzyremake/build/CMakeFiles/3.16.3/CompilerIdC/CMakeCCompilerId.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(DependSuffix): build/CMakeFiles/3.16.3/CompilerIdC/CMakeCCompilerId.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(ObjectSuffix) -MF$(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(DependSuffix) -MM build/CMakeFiles/3.16.3/CompilerIdC/CMakeCCompilerId.c

$(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(PreprocessSuffix): build/CMakeFiles/3.16.3/CompilerIdC/CMakeCCompilerId.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(PreprocessSuffix) build/CMakeFiles/3.16.3/CompilerIdC/CMakeCCompilerId.c

$(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(ObjectSuffix): build/CMakeFiles/3.16.3/CompilerIdCXX/CMakeCXXCompilerId.cpp $(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/build/CMakeFiles/3.16.3/CompilerIdCXX/CMakeCXXCompilerId.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(DependSuffix): build/CMakeFiles/3.16.3/CompilerIdCXX/CMakeCXXCompilerId.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(DependSuffix) -MM build/CMakeFiles/3.16.3/CompilerIdCXX/CMakeCXXCompilerId.cpp

$(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(PreprocessSuffix): build/CMakeFiles/3.16.3/CompilerIdCXX/CMakeCXXCompilerId.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/build_CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(PreprocessSuffix) build/CMakeFiles/3.16.3/CompilerIdCXX/CMakeCXXCompilerId.cpp

$(IntermediateDirectory)/Modules_Editor_Editor_Editor.cpp$(ObjectSuffix): Modules/Editor/Editor/Editor.cpp $(IntermediateDirectory)/Modules_Editor_Editor_Editor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Editor/Editor/Editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Editor_Editor_Editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Editor_Editor_Editor.cpp$(DependSuffix): Modules/Editor/Editor/Editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Editor_Editor_Editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Editor_Editor_Editor.cpp$(DependSuffix) -MM Modules/Editor/Editor/Editor.cpp

$(IntermediateDirectory)/Modules_Editor_Editor_Editor.cpp$(PreprocessSuffix): Modules/Editor/Editor/Editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Editor_Editor_Editor.cpp$(PreprocessSuffix) Modules/Editor/Editor/Editor.cpp

$(IntermediateDirectory)/Modules_Replay_Recorder_Recorder.cpp$(ObjectSuffix): Modules/Replay/Recorder/Recorder.cpp $(IntermediateDirectory)/Modules_Replay_Recorder_Recorder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Replay/Recorder/Recorder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Replay_Recorder_Recorder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Replay_Recorder_Recorder.cpp$(DependSuffix): Modules/Replay/Recorder/Recorder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Replay_Recorder_Recorder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Replay_Recorder_Recorder.cpp$(DependSuffix) -MM Modules/Replay/Recorder/Recorder.cpp

$(IntermediateDirectory)/Modules_Replay_Recorder_Recorder.cpp$(PreprocessSuffix): Modules/Replay/Recorder/Recorder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Replay_Recorder_Recorder.cpp$(PreprocessSuffix) Modules/Replay/Recorder/Recorder.cpp

$(IntermediateDirectory)/Modules_HighScore_NameEntry_NameEntry.cpp$(ObjectSuffix): Modules/HighScore/NameEntry/NameEntry.cpp $(IntermediateDirectory)/Modules_HighScore_NameEntry_NameEntry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/HighScore/NameEntry/NameEntry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_HighScore_NameEntry_NameEntry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_HighScore_NameEntry_NameEntry.cpp$(DependSuffix): Modules/HighScore/NameEntry/NameEntry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_HighScore_NameEntry_NameEntry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_HighScore_NameEntry_NameEntry.cpp$(DependSuffix) -MM Modules/HighScore/NameEntry/NameEntry.cpp

$(IntermediateDirectory)/Modules_HighScore_NameEntry_NameEntry.cpp$(PreprocessSuffix): Modules/HighScore/NameEntry/NameEntry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_HighScore_NameEntry_NameEntry.cpp$(PreprocessSuffix) Modules/HighScore/NameEntry/NameEntry.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectSaveAs_DialogProjectSaveAs.cpp$(ObjectSuffix): Modules/EditorOLD/DialogProjectSaveAs/DialogProjectSaveAs.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectSaveAs_DialogProjectSaveAs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogProjectSaveAs/DialogProjectSaveAs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectSaveAs_DialogProjectSaveAs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectSaveAs_DialogProjectSaveAs.cpp$(DependSuffix): Modules/EditorOLD/DialogProjectSaveAs/DialogProjectSaveAs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectSaveAs_DialogProjectSaveAs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectSaveAs_DialogProjectSaveAs.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogProjectSaveAs/DialogProjectSaveAs.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectSaveAs_DialogProjectSaveAs.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogProjectSaveAs/DialogProjectSaveAs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectSaveAs_DialogProjectSaveAs.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogProjectSaveAs/DialogProjectSaveAs.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectOpen_DialogProjectOpen.cpp$(ObjectSuffix): Modules/EditorOLD/DialogProjectOpen/DialogProjectOpen.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectOpen_DialogProjectOpen.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogProjectOpen/DialogProjectOpen.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectOpen_DialogProjectOpen.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectOpen_DialogProjectOpen.cpp$(DependSuffix): Modules/EditorOLD/DialogProjectOpen/DialogProjectOpen.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectOpen_DialogProjectOpen.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectOpen_DialogProjectOpen.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogProjectOpen/DialogProjectOpen.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectOpen_DialogProjectOpen.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogProjectOpen/DialogProjectOpen.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectOpen_DialogProjectOpen.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogProjectOpen/DialogProjectOpen.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_DialogResourcePrimitives.cpp$(ObjectSuffix): Modules/EditorOLD/DialogResourcePrimitives/DialogResourcePrimitives.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_DialogResourcePrimitives.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogResourcePrimitives/DialogResourcePrimitives.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_DialogResourcePrimitives.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_DialogResourcePrimitives.cpp$(DependSuffix): Modules/EditorOLD/DialogResourcePrimitives/DialogResourcePrimitives.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_DialogResourcePrimitives.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_DialogResourcePrimitives.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogResourcePrimitives/DialogResourcePrimitives.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_DialogResourcePrimitives.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogResourcePrimitives/DialogResourcePrimitives.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_DialogResourcePrimitives.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogResourcePrimitives/DialogResourcePrimitives.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_DialogResourceColoursTextures.cpp$(ObjectSuffix): Modules/EditorOLD/DialogResourceColoursTextures/DialogResourceColoursTextures.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_DialogResourceColoursTextures.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogResourceColoursTextures/DialogResourceColoursTextures.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_DialogResourceColoursTextures.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_DialogResourceColoursTextures.cpp$(DependSuffix): Modules/EditorOLD/DialogResourceColoursTextures/DialogResourceColoursTextures.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_DialogResourceColoursTextures.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_DialogResourceColoursTextures.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogResourceColoursTextures/DialogResourceColoursTextures.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_DialogResourceColoursTextures.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogResourceColoursTextures/DialogResourceColoursTextures.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_DialogResourceColoursTextures.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogResourceColoursTextures/DialogResourceColoursTextures.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_DialogResource3DModels.cpp$(ObjectSuffix): Modules/EditorOLD/DialogResource3DModels/DialogResource3DModels.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_DialogResource3DModels.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogResource3DModels/DialogResource3DModels.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_DialogResource3DModels.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_DialogResource3DModels.cpp$(DependSuffix): Modules/EditorOLD/DialogResource3DModels/DialogResource3DModels.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_DialogResource3DModels.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_DialogResource3DModels.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogResource3DModels/DialogResource3DModels.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_DialogResource3DModels.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogResource3DModels/DialogResource3DModels.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_DialogResource3DModels.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogResource3DModels/DialogResource3DModels.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_Icon3DModel.cpp$(ObjectSuffix): Modules/EditorOLD/DialogResource3DModels/Icon3DModel.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_Icon3DModel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogResource3DModels/Icon3DModel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_Icon3DModel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_Icon3DModel.cpp$(DependSuffix): Modules/EditorOLD/DialogResource3DModels/Icon3DModel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_Icon3DModel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_Icon3DModel.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogResource3DModels/Icon3DModel.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_Icon3DModel.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogResource3DModels/Icon3DModel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogResource3DModels_Icon3DModel.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogResource3DModels/Icon3DModel.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeTop.cpp$(ObjectSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeTop.cpp $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeTop.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/ScreenEdge/ScreenEdgeTop.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeTop.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeTop.cpp$(DependSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeTop.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeTop.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeTop.cpp$(DependSuffix) -MM Modules/EditorOLD/ScreenEdge/ScreenEdgeTop.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeTop.cpp$(PreprocessSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeTop.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeTop.cpp$(PreprocessSuffix) Modules/EditorOLD/ScreenEdge/ScreenEdgeTop.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeHorizontal.cpp$(ObjectSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeHorizontal.cpp $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeHorizontal.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/ScreenEdge/ScreenEdgeHorizontal.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeHorizontal.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeHorizontal.cpp$(DependSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeHorizontal.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeHorizontal.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeHorizontal.cpp$(DependSuffix) -MM Modules/EditorOLD/ScreenEdge/ScreenEdgeHorizontal.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeHorizontal.cpp$(PreprocessSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeHorizontal.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeHorizontal.cpp$(PreprocessSuffix) Modules/EditorOLD/ScreenEdge/ScreenEdgeHorizontal.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeRight.cpp$(ObjectSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeRight.cpp $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeRight.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/ScreenEdge/ScreenEdgeRight.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeRight.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeRight.cpp$(DependSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeRight.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeRight.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeRight.cpp$(DependSuffix) -MM Modules/EditorOLD/ScreenEdge/ScreenEdgeRight.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeRight.cpp$(PreprocessSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeRight.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeRight.cpp$(PreprocessSuffix) Modules/EditorOLD/ScreenEdge/ScreenEdgeRight.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdge.cpp$(ObjectSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdge.cpp $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdge.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/ScreenEdge/ScreenEdge.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdge.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdge.cpp$(DependSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdge.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdge.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdge.cpp$(DependSuffix) -MM Modules/EditorOLD/ScreenEdge/ScreenEdge.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdge.cpp$(PreprocessSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdge.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdge.cpp$(PreprocessSuffix) Modules/EditorOLD/ScreenEdge/ScreenEdge.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeProject.cpp$(ObjectSuffix): Modules/EditorOLD/DialogProjectConfiguration/TreeNodeProject.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeProject.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogProjectConfiguration/TreeNodeProject.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeProject.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeProject.cpp$(DependSuffix): Modules/EditorOLD/DialogProjectConfiguration/TreeNodeProject.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeProject.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeProject.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogProjectConfiguration/TreeNodeProject.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeProject.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogProjectConfiguration/TreeNodeProject.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeProject.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogProjectConfiguration/TreeNodeProject.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconTexture.cpp$(ObjectSuffix): Modules/EditorOLD/DialogResourceColoursTextures/IconTexture.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconTexture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogResourceColoursTextures/IconTexture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconTexture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconTexture.cpp$(DependSuffix): Modules/EditorOLD/DialogResourceColoursTextures/IconTexture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconTexture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconTexture.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogResourceColoursTextures/IconTexture.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconTexture.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogResourceColoursTextures/IconTexture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconTexture.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogResourceColoursTextures/IconTexture.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_DialogProjectConfiguration.cpp$(ObjectSuffix): Modules/EditorOLD/DialogProjectConfiguration/DialogProjectConfiguration.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_DialogProjectConfiguration.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogProjectConfiguration/DialogProjectConfiguration.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_DialogProjectConfiguration.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_DialogProjectConfiguration.cpp$(DependSuffix): Modules/EditorOLD/DialogProjectConfiguration/DialogProjectConfiguration.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_DialogProjectConfiguration.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_DialogProjectConfiguration.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogProjectConfiguration/DialogProjectConfiguration.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_DialogProjectConfiguration.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogProjectConfiguration/DialogProjectConfiguration.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_DialogProjectConfiguration.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogProjectConfiguration/DialogProjectConfiguration.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_IconBoolean.cpp$(ObjectSuffix): Modules/EditorOLD/DialogResourcePrimitives/IconBoolean.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_IconBoolean.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogResourcePrimitives/IconBoolean.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_IconBoolean.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_IconBoolean.cpp$(DependSuffix): Modules/EditorOLD/DialogResourcePrimitives/IconBoolean.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_IconBoolean.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_IconBoolean.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogResourcePrimitives/IconBoolean.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_IconBoolean.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogResourcePrimitives/IconBoolean.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourcePrimitives_IconBoolean.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogResourcePrimitives/IconBoolean.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResourceType.cpp$(ObjectSuffix): Modules/EditorOLD/DialogProjectConfiguration/TreeNodeResourceType.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResourceType.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogProjectConfiguration/TreeNodeResourceType.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResourceType.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResourceType.cpp$(DependSuffix): Modules/EditorOLD/DialogProjectConfiguration/TreeNodeResourceType.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResourceType.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResourceType.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogProjectConfiguration/TreeNodeResourceType.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResourceType.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogProjectConfiguration/TreeNodeResourceType.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResourceType.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogProjectConfiguration/TreeNodeResourceType.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeDefault.cpp$(ObjectSuffix): Modules/EditorOLD/DialogProjectConfiguration/TreeNodeDefault.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeDefault.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogProjectConfiguration/TreeNodeDefault.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeDefault.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeDefault.cpp$(DependSuffix): Modules/EditorOLD/DialogProjectConfiguration/TreeNodeDefault.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeDefault.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeDefault.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogProjectConfiguration/TreeNodeDefault.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeDefault.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogProjectConfiguration/TreeNodeDefault.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeDefault.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogProjectConfiguration/TreeNodeDefault.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_DialogResourceElements.cpp$(ObjectSuffix): Modules/EditorOLD/DialogResourceElements/DialogResourceElements.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_DialogResourceElements.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogResourceElements/DialogResourceElements.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_DialogResourceElements.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_DialogResourceElements.cpp$(DependSuffix): Modules/EditorOLD/DialogResourceElements/DialogResourceElements.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_DialogResourceElements.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_DialogResourceElements.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogResourceElements/DialogResourceElements.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_DialogResourceElements.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogResourceElements/DialogResourceElements.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_DialogResourceElements.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogResourceElements/DialogResourceElements.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Player_Player.cpp$(ObjectSuffix): Modules/Spindizzy/Player/Player.cpp $(IntermediateDirectory)/Modules_Spindizzy_Player_Player.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Player/Player.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Player_Player.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Player_Player.cpp$(DependSuffix): Modules/Spindizzy/Player/Player.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Player_Player.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Player_Player.cpp$(DependSuffix) -MM Modules/Spindizzy/Player/Player.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Player_Player.cpp$(PreprocessSuffix): Modules/Spindizzy/Player/Player.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Player_Player.cpp$(PreprocessSuffix) Modules/Spindizzy/Player/Player.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Pyramid_Pyramid.cpp$(ObjectSuffix): Modules/Spindizzy/Pyramid/Pyramid.cpp $(IntermediateDirectory)/Modules_Spindizzy_Pyramid_Pyramid.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Pyramid/Pyramid.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Pyramid_Pyramid.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Pyramid_Pyramid.cpp$(DependSuffix): Modules/Spindizzy/Pyramid/Pyramid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Pyramid_Pyramid.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Pyramid_Pyramid.cpp$(DependSuffix) -MM Modules/Spindizzy/Pyramid/Pyramid.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Pyramid_Pyramid.cpp$(PreprocessSuffix): Modules/Spindizzy/Pyramid/Pyramid.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Pyramid_Pyramid.cpp$(PreprocessSuffix) Modules/Spindizzy/Pyramid/Pyramid.cpp

$(IntermediateDirectory)/Modules_Spindizzy_C64LiftGraphics_C64LiftGraphics.cpp$(ObjectSuffix): Modules/Spindizzy/C64LiftGraphics/C64LiftGraphics.cpp $(IntermediateDirectory)/Modules_Spindizzy_C64LiftGraphics_C64LiftGraphics.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/C64LiftGraphics/C64LiftGraphics.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_C64LiftGraphics_C64LiftGraphics.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_C64LiftGraphics_C64LiftGraphics.cpp$(DependSuffix): Modules/Spindizzy/C64LiftGraphics/C64LiftGraphics.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_C64LiftGraphics_C64LiftGraphics.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_C64LiftGraphics_C64LiftGraphics.cpp$(DependSuffix) -MM Modules/Spindizzy/C64LiftGraphics/C64LiftGraphics.cpp

$(IntermediateDirectory)/Modules_Spindizzy_C64LiftGraphics_C64LiftGraphics.cpp$(PreprocessSuffix): Modules/Spindizzy/C64LiftGraphics/C64LiftGraphics.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_C64LiftGraphics_C64LiftGraphics.cpp$(PreprocessSuffix) Modules/Spindizzy/C64LiftGraphics/C64LiftGraphics.cpp

$(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunk.cpp$(ObjectSuffix): Modules/Spindizzy/DebrisChunk/DebrisChunk.cpp $(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunk.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/DebrisChunk/DebrisChunk.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunk.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunk.cpp$(DependSuffix): Modules/Spindizzy/DebrisChunk/DebrisChunk.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunk.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunk.cpp$(DependSuffix) -MM Modules/Spindizzy/DebrisChunk/DebrisChunk.cpp

$(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunk.cpp$(PreprocessSuffix): Modules/Spindizzy/DebrisChunk/DebrisChunk.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunk.cpp$(PreprocessSuffix) Modules/Spindizzy/DebrisChunk/DebrisChunk.cpp

$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallConstructionData.cpp$(ObjectSuffix): Modules/Spindizzy/GeometryProcessor/WallConstructionData.cpp $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallConstructionData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/GeometryProcessor/WallConstructionData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallConstructionData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallConstructionData.cpp$(DependSuffix): Modules/Spindizzy/GeometryProcessor/WallConstructionData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallConstructionData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallConstructionData.cpp$(DependSuffix) -MM Modules/Spindizzy/GeometryProcessor/WallConstructionData.cpp

$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallConstructionData.cpp$(PreprocessSuffix): Modules/Spindizzy/GeometryProcessor/WallConstructionData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallConstructionData.cpp$(PreprocessSuffix) Modules/Spindizzy/GeometryProcessor/WallConstructionData.cpp

$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileBlock.cpp$(ObjectSuffix): Modules/Spindizzy/GeometryProcessor/TileBlock.cpp $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileBlock.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/GeometryProcessor/TileBlock.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileBlock.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileBlock.cpp$(DependSuffix): Modules/Spindizzy/GeometryProcessor/TileBlock.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileBlock.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileBlock.cpp$(DependSuffix) -MM Modules/Spindizzy/GeometryProcessor/TileBlock.cpp

$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileBlock.cpp$(PreprocessSuffix): Modules/Spindizzy/GeometryProcessor/TileBlock.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileBlock.cpp$(PreprocessSuffix) Modules/Spindizzy/GeometryProcessor/TileBlock.cpp

$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileColumn.cpp$(ObjectSuffix): Modules/Spindizzy/GeometryProcessor/TileColumn.cpp $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileColumn.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/GeometryProcessor/TileColumn.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileColumn.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileColumn.cpp$(DependSuffix): Modules/Spindizzy/GeometryProcessor/TileColumn.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileColumn.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileColumn.cpp$(DependSuffix) -MM Modules/Spindizzy/GeometryProcessor/TileColumn.cpp

$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileColumn.cpp$(PreprocessSuffix): Modules/Spindizzy/GeometryProcessor/TileColumn.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileColumn.cpp$(PreprocessSuffix) Modules/Spindizzy/GeometryProcessor/TileColumn.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResource.cpp$(ObjectSuffix): Modules/EditorOLD/DialogProjectConfiguration/TreeNodeResource.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogProjectConfiguration/TreeNodeResource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResource.cpp$(DependSuffix): Modules/EditorOLD/DialogProjectConfiguration/TreeNodeResource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResource.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogProjectConfiguration/TreeNodeResource.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResource.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogProjectConfiguration/TreeNodeResource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeResource.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogProjectConfiguration/TreeNodeResource.cpp

$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_GeometryProcessor.cpp$(ObjectSuffix): Modules/Spindizzy/GeometryProcessor/GeometryProcessor.cpp $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_GeometryProcessor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/GeometryProcessor/GeometryProcessor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_GeometryProcessor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_GeometryProcessor.cpp$(DependSuffix): Modules/Spindizzy/GeometryProcessor/GeometryProcessor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_GeometryProcessor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_GeometryProcessor.cpp$(DependSuffix) -MM Modules/Spindizzy/GeometryProcessor/GeometryProcessor.cpp

$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_GeometryProcessor.cpp$(PreprocessSuffix): Modules/Spindizzy/GeometryProcessor/GeometryProcessor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_GeometryProcessor.cpp$(PreprocessSuffix) Modules/Spindizzy/GeometryProcessor/GeometryProcessor.cpp

$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileSurfaceTemplate.cpp$(ObjectSuffix): Modules/Spindizzy/GeometryProcessor/TileSurfaceTemplate.cpp $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileSurfaceTemplate.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/GeometryProcessor/TileSurfaceTemplate.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileSurfaceTemplate.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileSurfaceTemplate.cpp$(DependSuffix): Modules/Spindizzy/GeometryProcessor/TileSurfaceTemplate.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileSurfaceTemplate.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileSurfaceTemplate.cpp$(DependSuffix) -MM Modules/Spindizzy/GeometryProcessor/TileSurfaceTemplate.cpp

$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileSurfaceTemplate.cpp$(PreprocessSuffix): Modules/Spindizzy/GeometryProcessor/TileSurfaceTemplate.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_TileSurfaceTemplate.cpp$(PreprocessSuffix) Modules/Spindizzy/GeometryProcessor/TileSurfaceTemplate.cpp

$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumn.cpp$(ObjectSuffix): Modules/Spindizzy/GeometryProcessor/WallColumn.cpp $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumn.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/GeometryProcessor/WallColumn.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumn.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumn.cpp$(DependSuffix): Modules/Spindizzy/GeometryProcessor/WallColumn.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumn.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumn.cpp$(DependSuffix) -MM Modules/Spindizzy/GeometryProcessor/WallColumn.cpp

$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumn.cpp$(PreprocessSuffix): Modules/Spindizzy/GeometryProcessor/WallColumn.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumn.cpp$(PreprocessSuffix) Modules/Spindizzy/GeometryProcessor/WallColumn.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColour.cpp$(ObjectSuffix): Modules/Spindizzy/ThemeSet/ThemeColour.cpp $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColour.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/ThemeSet/ThemeColour.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColour.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColour.cpp$(DependSuffix): Modules/Spindizzy/ThemeSet/ThemeColour.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColour.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColour.cpp$(DependSuffix) -MM Modules/Spindizzy/ThemeSet/ThemeColour.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColour.cpp$(PreprocessSuffix): Modules/Spindizzy/ThemeSet/ThemeColour.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColour.cpp$(PreprocessSuffix) Modules/Spindizzy/ThemeSet/ThemeColour.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTextureIcon.cpp$(ObjectSuffix): Modules/Spindizzy/ThemeSet/ThemeTextureIcon.cpp $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTextureIcon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/ThemeSet/ThemeTextureIcon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTextureIcon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTextureIcon.cpp$(DependSuffix): Modules/Spindizzy/ThemeSet/ThemeTextureIcon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTextureIcon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTextureIcon.cpp$(DependSuffix) -MM Modules/Spindizzy/ThemeSet/ThemeTextureIcon.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTextureIcon.cpp$(PreprocessSuffix): Modules/Spindizzy/ThemeSet/ThemeTextureIcon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTextureIcon.cpp$(PreprocessSuffix) Modules/Spindizzy/ThemeSet/ThemeTextureIcon.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexture.cpp$(ObjectSuffix): Modules/Spindizzy/ThemeSet/ThemeTexture.cpp $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/ThemeSet/ThemeTexture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexture.cpp$(DependSuffix): Modules/Spindizzy/ThemeSet/ThemeTexture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexture.cpp$(DependSuffix) -MM Modules/Spindizzy/ThemeSet/ThemeTexture.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexture.cpp$(PreprocessSuffix): Modules/Spindizzy/ThemeSet/ThemeTexture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexture.cpp$(PreprocessSuffix) Modules/Spindizzy/ThemeSet/ThemeTexture.cpp

$(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUp.cpp$(ObjectSuffix): Modules/Spindizzy/PickUp/PickUp.cpp $(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/PickUp/PickUp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUp.cpp$(DependSuffix): Modules/Spindizzy/PickUp/PickUp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUp.cpp$(DependSuffix) -MM Modules/Spindizzy/PickUp/PickUp.cpp

$(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUp.cpp$(PreprocessSuffix): Modules/Spindizzy/PickUp/PickUp.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUp.cpp$(PreprocessSuffix) Modules/Spindizzy/PickUp/PickUp.cpp

$(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUpType.cpp$(ObjectSuffix): Modules/Spindizzy/PickUp/PickUpType.cpp $(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUpType.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/PickUp/PickUpType.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUpType.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUpType.cpp$(DependSuffix): Modules/Spindizzy/PickUp/PickUpType.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUpType.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUpType.cpp$(DependSuffix) -MM Modules/Spindizzy/PickUp/PickUpType.cpp

$(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUpType.cpp$(PreprocessSuffix): Modules/Spindizzy/PickUp/PickUpType.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_PickUp_PickUpType.cpp$(PreprocessSuffix) Modules/Spindizzy/PickUp/PickUpType.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Gyroscope_Gyroscope.cpp$(ObjectSuffix): Modules/Spindizzy/Gyroscope/Gyroscope.cpp $(IntermediateDirectory)/Modules_Spindizzy_Gyroscope_Gyroscope.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Gyroscope/Gyroscope.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Gyroscope_Gyroscope.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Gyroscope_Gyroscope.cpp$(DependSuffix): Modules/Spindizzy/Gyroscope/Gyroscope.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Gyroscope_Gyroscope.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Gyroscope_Gyroscope.cpp$(DependSuffix) -MM Modules/Spindizzy/Gyroscope/Gyroscope.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Gyroscope_Gyroscope.cpp$(PreprocessSuffix): Modules/Spindizzy/Gyroscope/Gyroscope.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Gyroscope_Gyroscope.cpp$(PreprocessSuffix) Modules/Spindizzy/Gyroscope/Gyroscope.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeVertical.cpp$(ObjectSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeVertical.cpp $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeVertical.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/ScreenEdge/ScreenEdgeVertical.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeVertical.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeVertical.cpp$(DependSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeVertical.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeVertical.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeVertical.cpp$(DependSuffix) -MM Modules/EditorOLD/ScreenEdge/ScreenEdgeVertical.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeVertical.cpp$(PreprocessSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeVertical.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeVertical.cpp$(PreprocessSuffix) Modules/EditorOLD/ScreenEdge/ScreenEdgeVertical.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Zone_ZoneType.cpp$(ObjectSuffix): Modules/Spindizzy/Zone/ZoneType.cpp $(IntermediateDirectory)/Modules_Spindizzy_Zone_ZoneType.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Zone/ZoneType.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Zone_ZoneType.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Zone_ZoneType.cpp$(DependSuffix): Modules/Spindizzy/Zone/ZoneType.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Zone_ZoneType.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Zone_ZoneType.cpp$(DependSuffix) -MM Modules/Spindizzy/Zone/ZoneType.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Zone_ZoneType.cpp$(PreprocessSuffix): Modules/Spindizzy/Zone/ZoneType.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Zone_ZoneType.cpp$(PreprocessSuffix) Modules/Spindizzy/Zone/ZoneType.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Physics_PhysicalState.cpp$(ObjectSuffix): Modules/Spindizzy/Physics/PhysicalState.cpp $(IntermediateDirectory)/Modules_Spindizzy_Physics_PhysicalState.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Physics/PhysicalState.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Physics_PhysicalState.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Physics_PhysicalState.cpp$(DependSuffix): Modules/Spindizzy/Physics/PhysicalState.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Physics_PhysicalState.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Physics_PhysicalState.cpp$(DependSuffix) -MM Modules/Spindizzy/Physics/PhysicalState.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Physics_PhysicalState.cpp$(PreprocessSuffix): Modules/Spindizzy/Physics/PhysicalState.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Physics_PhysicalState.cpp$(PreprocessSuffix) Modules/Spindizzy/Physics/PhysicalState.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Physics_Physics.cpp$(ObjectSuffix): Modules/Spindizzy/Physics/Physics.cpp $(IntermediateDirectory)/Modules_Spindizzy_Physics_Physics.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Physics/Physics.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Physics_Physics.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Physics_Physics.cpp$(DependSuffix): Modules/Spindizzy/Physics/Physics.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Physics_Physics.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Physics_Physics.cpp$(DependSuffix) -MM Modules/Spindizzy/Physics/Physics.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Physics_Physics.cpp$(PreprocessSuffix): Modules/Spindizzy/Physics/Physics.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Physics_Physics.cpp$(PreprocessSuffix) Modules/Spindizzy/Physics/Physics.cpp

$(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_DebrisGenerator.cpp$(ObjectSuffix): Modules/Spindizzy/DebrisGenerator/DebrisGenerator.cpp $(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_DebrisGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/DebrisGenerator/DebrisGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_DebrisGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_DebrisGenerator.cpp$(DependSuffix): Modules/Spindizzy/DebrisGenerator/DebrisGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_DebrisGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_DebrisGenerator.cpp$(DependSuffix) -MM Modules/Spindizzy/DebrisGenerator/DebrisGenerator.cpp

$(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_DebrisGenerator.cpp$(PreprocessSuffix): Modules/Spindizzy/DebrisGenerator/DebrisGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_DebrisGenerator.cpp$(PreprocessSuffix) Modules/Spindizzy/DebrisGenerator/DebrisGenerator.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Water_Water.cpp$(ObjectSuffix): Modules/Spindizzy/Water/Water.cpp $(IntermediateDirectory)/Modules_Spindizzy_Water_Water.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Water/Water.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Water_Water.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Water_Water.cpp$(DependSuffix): Modules/Spindizzy/Water/Water.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Water_Water.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Water_Water.cpp$(DependSuffix) -MM Modules/Spindizzy/Water/Water.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Water_Water.cpp$(PreprocessSuffix): Modules/Spindizzy/Water/Water.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Water_Water.cpp$(PreprocessSuffix) Modules/Spindizzy/Water/Water.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Water_WaterType.cpp$(ObjectSuffix): Modules/Spindizzy/Water/WaterType.cpp $(IntermediateDirectory)/Modules_Spindizzy_Water_WaterType.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Water/WaterType.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Water_WaterType.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Water_WaterType.cpp$(DependSuffix): Modules/Spindizzy/Water/WaterType.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Water_WaterType.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Water_WaterType.cpp$(DependSuffix) -MM Modules/Spindizzy/Water/WaterType.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Water_WaterType.cpp$(PreprocessSuffix): Modules/Spindizzy/Water/WaterType.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Water_WaterType.cpp$(PreprocessSuffix) Modules/Spindizzy/Water/WaterType.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Jewel_Jewel.cpp$(ObjectSuffix): Modules/Spindizzy/Jewel/Jewel.cpp $(IntermediateDirectory)/Modules_Spindizzy_Jewel_Jewel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Jewel/Jewel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Jewel_Jewel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Jewel_Jewel.cpp$(DependSuffix): Modules/Spindizzy/Jewel/Jewel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Jewel_Jewel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Jewel_Jewel.cpp$(DependSuffix) -MM Modules/Spindizzy/Jewel/Jewel.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Jewel_Jewel.cpp$(PreprocessSuffix): Modules/Spindizzy/Jewel/Jewel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Jewel_Jewel.cpp$(PreprocessSuffix) Modules/Spindizzy/Jewel/Jewel.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Enemy_Enemy.cpp$(ObjectSuffix): Modules/Spindizzy/Enemy/Enemy.cpp $(IntermediateDirectory)/Modules_Spindizzy_Enemy_Enemy.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Enemy/Enemy.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Enemy_Enemy.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Enemy_Enemy.cpp$(DependSuffix): Modules/Spindizzy/Enemy/Enemy.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Enemy_Enemy.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Enemy_Enemy.cpp$(DependSuffix) -MM Modules/Spindizzy/Enemy/Enemy.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Enemy_Enemy.cpp$(PreprocessSuffix): Modules/Spindizzy/Enemy/Enemy.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Enemy_Enemy.cpp$(PreprocessSuffix) Modules/Spindizzy/Enemy/Enemy.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSplitSurface.cpp$(ObjectSuffix): Modules/Spindizzy/Terrain/TileSplitSurface.cpp $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSplitSurface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Terrain/TileSplitSurface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSplitSurface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSplitSurface.cpp$(DependSuffix): Modules/Spindizzy/Terrain/TileSplitSurface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSplitSurface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSplitSurface.cpp$(DependSuffix) -MM Modules/Spindizzy/Terrain/TileSplitSurface.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSplitSurface.cpp$(PreprocessSuffix): Modules/Spindizzy/Terrain/TileSplitSurface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSplitSurface.cpp$(PreprocessSuffix) Modules/Spindizzy/Terrain/TileSplitSurface.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSurface.cpp$(ObjectSuffix): Modules/Spindizzy/Terrain/TileSurface.cpp $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSurface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Terrain/TileSurface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSurface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSurface.cpp$(DependSuffix): Modules/Spindizzy/Terrain/TileSurface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSurface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSurface.cpp$(DependSuffix) -MM Modules/Spindizzy/Terrain/TileSurface.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSurface.cpp$(PreprocessSuffix): Modules/Spindizzy/Terrain/TileSurface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TileSurface.cpp$(PreprocessSuffix) Modules/Spindizzy/Terrain/TileSurface.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCapDialog.cpp$(ObjectSuffix): Modules/Spindizzy/Terrain/WallPatternCapDialog.cpp $(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCapDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Terrain/WallPatternCapDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCapDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCapDialog.cpp$(DependSuffix): Modules/Spindizzy/Terrain/WallPatternCapDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCapDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCapDialog.cpp$(DependSuffix) -MM Modules/Spindizzy/Terrain/WallPatternCapDialog.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCapDialog.cpp$(PreprocessSuffix): Modules/Spindizzy/Terrain/WallPatternCapDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCapDialog.cpp$(PreprocessSuffix) Modules/Spindizzy/Terrain/WallPatternCapDialog.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainProperties.cpp$(ObjectSuffix): Modules/Spindizzy/Terrain/TerrainProperties.cpp $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainProperties.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Terrain/TerrainProperties.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainProperties.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainProperties.cpp$(DependSuffix): Modules/Spindizzy/Terrain/TerrainProperties.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainProperties.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainProperties.cpp$(DependSuffix) -MM Modules/Spindizzy/Terrain/TerrainProperties.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainProperties.cpp$(PreprocessSuffix): Modules/Spindizzy/Terrain/TerrainProperties.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainProperties.cpp$(PreprocessSuffix) Modules/Spindizzy/Terrain/TerrainProperties.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainState.cpp$(ObjectSuffix): Modules/Spindizzy/Terrain/TerrainState.cpp $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainState.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Terrain/TerrainState.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainState.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainState.cpp$(DependSuffix): Modules/Spindizzy/Terrain/TerrainState.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainState.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainState.cpp$(DependSuffix) -MM Modules/Spindizzy/Terrain/TerrainState.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainState.cpp$(PreprocessSuffix): Modules/Spindizzy/Terrain/TerrainState.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainState.cpp$(PreprocessSuffix) Modules/Spindizzy/Terrain/TerrainState.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeModule.cpp$(ObjectSuffix): Modules/EditorOLD/DialogProjectConfiguration/TreeNodeModule.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeModule.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogProjectConfiguration/TreeNodeModule.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeModule.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeModule.cpp$(DependSuffix): Modules/EditorOLD/DialogProjectConfiguration/TreeNodeModule.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeModule.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeModule.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogProjectConfiguration/TreeNodeModule.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeModule.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogProjectConfiguration/TreeNodeModule.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogProjectConfiguration_TreeNodeModule.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogProjectConfiguration/TreeNodeModule.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainCollisionData.cpp$(ObjectSuffix): Modules/Spindizzy/Terrain/TerrainCollisionData.cpp $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainCollisionData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Terrain/TerrainCollisionData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainCollisionData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainCollisionData.cpp$(DependSuffix): Modules/Spindizzy/Terrain/TerrainCollisionData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainCollisionData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainCollisionData.cpp$(DependSuffix) -MM Modules/Spindizzy/Terrain/TerrainCollisionData.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainCollisionData.cpp$(PreprocessSuffix): Modules/Spindizzy/Terrain/TerrainCollisionData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainCollisionData.cpp$(PreprocessSuffix) Modules/Spindizzy/Terrain/TerrainCollisionData.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainType.cpp$(ObjectSuffix): Modules/Spindizzy/Terrain/TerrainType.cpp $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainType.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Terrain/TerrainType.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainType.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainType.cpp$(DependSuffix): Modules/Spindizzy/Terrain/TerrainType.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainType.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainType.cpp$(DependSuffix) -MM Modules/Spindizzy/Terrain/TerrainType.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainType.cpp$(PreprocessSuffix): Modules/Spindizzy/Terrain/TerrainType.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_TerrainType.cpp$(PreprocessSuffix) Modules/Spindizzy/Terrain/TerrainType.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftType.cpp$(ObjectSuffix): Modules/Spindizzy/Lift/LiftType.cpp $(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftType.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Lift/LiftType.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftType.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftType.cpp$(DependSuffix): Modules/Spindizzy/Lift/LiftType.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftType.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftType.cpp$(DependSuffix) -MM Modules/Spindizzy/Lift/LiftType.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftType.cpp$(PreprocessSuffix): Modules/Spindizzy/Lift/LiftType.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftType.cpp$(PreprocessSuffix) Modules/Spindizzy/Lift/LiftType.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_BoundaryHandler.cpp$(ObjectSuffix): Modules/Spindizzy/Boundaries/BoundaryHandler.cpp $(IntermediateDirectory)/Modules_Spindizzy_Boundaries_BoundaryHandler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Boundaries/BoundaryHandler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_BoundaryHandler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_BoundaryHandler.cpp$(DependSuffix): Modules/Spindizzy/Boundaries/BoundaryHandler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_BoundaryHandler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_BoundaryHandler.cpp$(DependSuffix) -MM Modules/Spindizzy/Boundaries/BoundaryHandler.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_BoundaryHandler.cpp$(PreprocessSuffix): Modules/Spindizzy/Boundaries/BoundaryHandler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Boundaries_BoundaryHandler.cpp$(PreprocessSuffix) Modules/Spindizzy/Boundaries/BoundaryHandler.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_Boundaries.cpp$(ObjectSuffix): Modules/Spindizzy/Boundaries/Boundaries.cpp $(IntermediateDirectory)/Modules_Spindizzy_Boundaries_Boundaries.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Boundaries/Boundaries.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_Boundaries.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_Boundaries.cpp$(DependSuffix): Modules/Spindizzy/Boundaries/Boundaries.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_Boundaries.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_Boundaries.cpp$(DependSuffix) -MM Modules/Spindizzy/Boundaries/Boundaries.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_Boundaries.cpp$(PreprocessSuffix): Modules/Spindizzy/Boundaries/Boundaries.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Boundaries_Boundaries.cpp$(PreprocessSuffix) Modules/Spindizzy/Boundaries/Boundaries.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemInputs.cpp$(ObjectSuffix): Modules/FrontEnd/Menu/MenuItemInputs.cpp $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemInputs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/FrontEnd/Menu/MenuItemInputs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemInputs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemInputs.cpp$(DependSuffix): Modules/FrontEnd/Menu/MenuItemInputs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemInputs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemInputs.cpp$(DependSuffix) -MM Modules/FrontEnd/Menu/MenuItemInputs.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemInputs.cpp$(PreprocessSuffix): Modules/FrontEnd/Menu/MenuItemInputs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemInputs.cpp$(PreprocessSuffix) Modules/FrontEnd/Menu/MenuItemInputs.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemBoolean.cpp$(ObjectSuffix): Modules/FrontEnd/Menu/MenuItemBoolean.cpp $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemBoolean.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/FrontEnd/Menu/MenuItemBoolean.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemBoolean.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemBoolean.cpp$(DependSuffix): Modules/FrontEnd/Menu/MenuItemBoolean.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemBoolean.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemBoolean.cpp$(DependSuffix) -MM Modules/FrontEnd/Menu/MenuItemBoolean.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemBoolean.cpp$(PreprocessSuffix): Modules/FrontEnd/Menu/MenuItemBoolean.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemBoolean.cpp$(PreprocessSuffix) Modules/FrontEnd/Menu/MenuItemBoolean.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemSlider.cpp$(ObjectSuffix): Modules/FrontEnd/Menu/MenuItemSlider.cpp $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemSlider.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/FrontEnd/Menu/MenuItemSlider.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemSlider.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemSlider.cpp$(DependSuffix): Modules/FrontEnd/Menu/MenuItemSlider.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemSlider.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemSlider.cpp$(DependSuffix) -MM Modules/FrontEnd/Menu/MenuItemSlider.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemSlider.cpp$(PreprocessSuffix): Modules/FrontEnd/Menu/MenuItemSlider.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemSlider.cpp$(PreprocessSuffix) Modules/FrontEnd/Menu/MenuItemSlider.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeLeft.cpp$(ObjectSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeLeft.cpp $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeLeft.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/ScreenEdge/ScreenEdgeLeft.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeLeft.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeLeft.cpp$(DependSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeLeft.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeLeft.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeLeft.cpp$(DependSuffix) -MM Modules/EditorOLD/ScreenEdge/ScreenEdgeLeft.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeLeft.cpp$(PreprocessSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeLeft.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeLeft.cpp$(PreprocessSuffix) Modules/EditorOLD/ScreenEdge/ScreenEdgeLeft.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_AbstractBoundaryHandler.cpp$(ObjectSuffix): Modules/Spindizzy/Boundaries/AbstractBoundaryHandler.cpp $(IntermediateDirectory)/Modules_Spindizzy_Boundaries_AbstractBoundaryHandler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Boundaries/AbstractBoundaryHandler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_AbstractBoundaryHandler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_AbstractBoundaryHandler.cpp$(DependSuffix): Modules/Spindizzy/Boundaries/AbstractBoundaryHandler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_AbstractBoundaryHandler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_AbstractBoundaryHandler.cpp$(DependSuffix) -MM Modules/Spindizzy/Boundaries/AbstractBoundaryHandler.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Boundaries_AbstractBoundaryHandler.cpp$(PreprocessSuffix): Modules/Spindizzy/Boundaries/AbstractBoundaryHandler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Boundaries_AbstractBoundaryHandler.cpp$(PreprocessSuffix) Modules/Spindizzy/Boundaries/AbstractBoundaryHandler.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemFileList.cpp$(ObjectSuffix): Modules/FrontEnd/Menu/MenuItemFileList.cpp $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemFileList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/FrontEnd/Menu/MenuItemFileList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemFileList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemFileList.cpp$(DependSuffix): Modules/FrontEnd/Menu/MenuItemFileList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemFileList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemFileList.cpp$(DependSuffix) -MM Modules/FrontEnd/Menu/MenuItemFileList.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemFileList.cpp$(PreprocessSuffix): Modules/FrontEnd/Menu/MenuItemFileList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemFileList.cpp$(PreprocessSuffix) Modules/FrontEnd/Menu/MenuItemFileList.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemAction.cpp$(ObjectSuffix): Modules/FrontEnd/Menu/MenuItemAction.cpp $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemAction.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/FrontEnd/Menu/MenuItemAction.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemAction.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemAction.cpp$(DependSuffix): Modules/FrontEnd/Menu/MenuItemAction.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemAction.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemAction.cpp$(DependSuffix) -MM Modules/FrontEnd/Menu/MenuItemAction.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemAction.cpp$(PreprocessSuffix): Modules/FrontEnd/Menu/MenuItemAction.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemAction.cpp$(PreprocessSuffix) Modules/FrontEnd/Menu/MenuItemAction.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeManager.cpp$(ObjectSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeManager.cpp $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/ScreenEdge/ScreenEdgeManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeManager.cpp$(DependSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeManager.cpp$(DependSuffix) -MM Modules/EditorOLD/ScreenEdge/ScreenEdgeManager.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeManager.cpp$(PreprocessSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeManager.cpp$(PreprocessSuffix) Modules/EditorOLD/ScreenEdge/ScreenEdgeManager.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Project_ProjectLoader.cpp$(ObjectSuffix): Modules/FrontEnd/Project/ProjectLoader.cpp $(IntermediateDirectory)/Modules_FrontEnd_Project_ProjectLoader.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/FrontEnd/Project/ProjectLoader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_FrontEnd_Project_ProjectLoader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_FrontEnd_Project_ProjectLoader.cpp$(DependSuffix): Modules/FrontEnd/Project/ProjectLoader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_FrontEnd_Project_ProjectLoader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_FrontEnd_Project_ProjectLoader.cpp$(DependSuffix) -MM Modules/FrontEnd/Project/ProjectLoader.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Project_ProjectLoader.cpp$(PreprocessSuffix): Modules/FrontEnd/Project/ProjectLoader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_FrontEnd_Project_ProjectLoader.cpp$(PreprocessSuffix) Modules/FrontEnd/Project/ProjectLoader.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Prompt_Prompt.cpp$(ObjectSuffix): Modules/FrontEnd/Prompt/Prompt.cpp $(IntermediateDirectory)/Modules_FrontEnd_Prompt_Prompt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/FrontEnd/Prompt/Prompt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_FrontEnd_Prompt_Prompt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_FrontEnd_Prompt_Prompt.cpp$(DependSuffix): Modules/FrontEnd/Prompt/Prompt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_FrontEnd_Prompt_Prompt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_FrontEnd_Prompt_Prompt.cpp$(DependSuffix) -MM Modules/FrontEnd/Prompt/Prompt.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Prompt_Prompt.cpp$(PreprocessSuffix): Modules/FrontEnd/Prompt/Prompt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_FrontEnd_Prompt_Prompt.cpp$(PreprocessSuffix) Modules/FrontEnd/Prompt/Prompt.cpp

$(IntermediateDirectory)/Modules_Basics_Texture_Texture.cpp$(ObjectSuffix): Modules/Basics/Texture/Texture.cpp $(IntermediateDirectory)/Modules_Basics_Texture_Texture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Texture/Texture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Texture_Texture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Texture_Texture.cpp$(DependSuffix): Modules/Basics/Texture/Texture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Texture_Texture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Texture_Texture.cpp$(DependSuffix) -MM Modules/Basics/Texture/Texture.cpp

$(IntermediateDirectory)/Modules_Basics_Texture_Texture.cpp$(PreprocessSuffix): Modules/Basics/Texture/Texture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Texture_Texture.cpp$(PreprocessSuffix) Modules/Basics/Texture/Texture.cpp

$(IntermediateDirectory)/Modules_HighScore_ScoreTable_ScoreTable.cpp$(ObjectSuffix): Modules/HighScore/ScoreTable/ScoreTable.cpp $(IntermediateDirectory)/Modules_HighScore_ScoreTable_ScoreTable.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/HighScore/ScoreTable/ScoreTable.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_HighScore_ScoreTable_ScoreTable.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_HighScore_ScoreTable_ScoreTable.cpp$(DependSuffix): Modules/HighScore/ScoreTable/ScoreTable.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_HighScore_ScoreTable_ScoreTable.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_HighScore_ScoreTable_ScoreTable.cpp$(DependSuffix) -MM Modules/HighScore/ScoreTable/ScoreTable.cpp

$(IntermediateDirectory)/Modules_HighScore_ScoreTable_ScoreTable.cpp$(PreprocessSuffix): Modules/HighScore/ScoreTable/ScoreTable.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_HighScore_ScoreTable_ScoreTable.cpp$(PreprocessSuffix) Modules/HighScore/ScoreTable/ScoreTable.cpp

$(IntermediateDirectory)/Modules_Basics_ColourScreen_ColourScreen.cpp$(ObjectSuffix): Modules/Basics/ColourScreen/ColourScreen.cpp $(IntermediateDirectory)/Modules_Basics_ColourScreen_ColourScreen.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/ColourScreen/ColourScreen.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_ColourScreen_ColourScreen.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_ColourScreen_ColourScreen.cpp$(DependSuffix): Modules/Basics/ColourScreen/ColourScreen.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_ColourScreen_ColourScreen.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_ColourScreen_ColourScreen.cpp$(DependSuffix) -MM Modules/Basics/ColourScreen/ColourScreen.cpp

$(IntermediateDirectory)/Modules_Basics_ColourScreen_ColourScreen.cpp$(PreprocessSuffix): Modules/Basics/ColourScreen/ColourScreen.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_ColourScreen_ColourScreen.cpp$(PreprocessSuffix) Modules/Basics/ColourScreen/ColourScreen.cpp

$(IntermediateDirectory)/IsoRealms_Options_WrappedOptions.cpp$(ObjectSuffix): IsoRealms/Options/WrappedOptions.cpp $(IntermediateDirectory)/IsoRealms_Options_WrappedOptions.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Options/WrappedOptions.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Options_WrappedOptions.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Options_WrappedOptions.cpp$(DependSuffix): IsoRealms/Options/WrappedOptions.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Options_WrappedOptions.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Options_WrappedOptions.cpp$(DependSuffix) -MM IsoRealms/Options/WrappedOptions.cpp

$(IntermediateDirectory)/IsoRealms_Options_WrappedOptions.cpp$(PreprocessSuffix): IsoRealms/Options/WrappedOptions.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Options_WrappedOptions.cpp$(PreprocessSuffix) IsoRealms/Options/WrappedOptions.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_DialogResourceFonts.cpp$(ObjectSuffix): Modules/EditorOLD/DialogResourceFonts/DialogResourceFonts.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_DialogResourceFonts.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogResourceFonts/DialogResourceFonts.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_DialogResourceFonts.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_DialogResourceFonts.cpp$(DependSuffix): Modules/EditorOLD/DialogResourceFonts/DialogResourceFonts.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_DialogResourceFonts.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_DialogResourceFonts.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogResourceFonts/DialogResourceFonts.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_DialogResourceFonts.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogResourceFonts/DialogResourceFonts.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_DialogResourceFonts.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogResourceFonts/DialogResourceFonts.cpp

$(IntermediateDirectory)/Modules_Spindizzy_SpindizzyScriptSupport.cpp$(ObjectSuffix): Modules/Spindizzy/SpindizzyScriptSupport.cpp $(IntermediateDirectory)/Modules_Spindizzy_SpindizzyScriptSupport.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/SpindizzyScriptSupport.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_SpindizzyScriptSupport.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_SpindizzyScriptSupport.cpp$(DependSuffix): Modules/Spindizzy/SpindizzyScriptSupport.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_SpindizzyScriptSupport.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_SpindizzyScriptSupport.cpp$(DependSuffix) -MM Modules/Spindizzy/SpindizzyScriptSupport.cpp

$(IntermediateDirectory)/Modules_Spindizzy_SpindizzyScriptSupport.cpp$(PreprocessSuffix): Modules/Spindizzy/SpindizzyScriptSupport.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_SpindizzyScriptSupport.cpp$(PreprocessSuffix) Modules/Spindizzy/SpindizzyScriptSupport.cpp

$(IntermediateDirectory)/Modules_HighScore_ScoreTracker_ScoreTracker.cpp$(ObjectSuffix): Modules/HighScore/ScoreTracker/ScoreTracker.cpp $(IntermediateDirectory)/Modules_HighScore_ScoreTracker_ScoreTracker.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/HighScore/ScoreTracker/ScoreTracker.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_HighScore_ScoreTracker_ScoreTracker.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_HighScore_ScoreTracker_ScoreTracker.cpp$(DependSuffix): Modules/HighScore/ScoreTracker/ScoreTracker.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_HighScore_ScoreTracker_ScoreTracker.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_HighScore_ScoreTracker_ScoreTracker.cpp$(DependSuffix) -MM Modules/HighScore/ScoreTracker/ScoreTracker.cpp

$(IntermediateDirectory)/Modules_HighScore_ScoreTracker_ScoreTracker.cpp$(PreprocessSuffix): Modules/HighScore/ScoreTracker/ScoreTracker.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_HighScore_ScoreTracker_ScoreTracker.cpp$(PreprocessSuffix) Modules/HighScore/ScoreTracker/ScoreTracker.cpp

$(IntermediateDirectory)/IsoRealms_Properties_ConditionComponent.cpp$(ObjectSuffix): IsoRealms/Properties/ConditionComponent.cpp $(IntermediateDirectory)/IsoRealms_Properties_ConditionComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Properties/ConditionComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Properties_ConditionComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Properties_ConditionComponent.cpp$(DependSuffix): IsoRealms/Properties/ConditionComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Properties_ConditionComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Properties_ConditionComponent.cpp$(DependSuffix) -MM IsoRealms/Properties/ConditionComponent.cpp

$(IntermediateDirectory)/IsoRealms_Properties_ConditionComponent.cpp$(PreprocessSuffix): IsoRealms/Properties/ConditionComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Properties_ConditionComponent.cpp$(PreprocessSuffix) IsoRealms/Properties/ConditionComponent.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialogGenerator.cpp$(ObjectSuffix): Modules/Spindizzy/ThemeSet/ThemeSelectionDialogGenerator.cpp $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialogGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/ThemeSet/ThemeSelectionDialogGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialogGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialogGenerator.cpp$(DependSuffix): Modules/Spindizzy/ThemeSet/ThemeSelectionDialogGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialogGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialogGenerator.cpp$(DependSuffix) -MM Modules/Spindizzy/ThemeSet/ThemeSelectionDialogGenerator.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialogGenerator.cpp$(PreprocessSuffix): Modules/Spindizzy/ThemeSet/ThemeSelectionDialogGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialogGenerator.cpp$(PreprocessSuffix) Modules/Spindizzy/ThemeSet/ThemeSelectionDialogGenerator.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogEditables_DialogEditables.cpp$(ObjectSuffix): Modules/EditorOLD/DialogEditables/DialogEditables.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogEditables_DialogEditables.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogEditables/DialogEditables.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogEditables_DialogEditables.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogEditables_DialogEditables.cpp$(DependSuffix): Modules/EditorOLD/DialogEditables/DialogEditables.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogEditables_DialogEditables.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogEditables_DialogEditables.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogEditables/DialogEditables.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogEditables_DialogEditables.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogEditables/DialogEditables.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogEditables_DialogEditables.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogEditables/DialogEditables.cpp

$(IntermediateDirectory)/IsoRealms_Properties_PropertyModel.cpp$(ObjectSuffix): IsoRealms/Properties/PropertyModel.cpp $(IntermediateDirectory)/IsoRealms_Properties_PropertyModel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Properties/PropertyModel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Properties_PropertyModel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Properties_PropertyModel.cpp$(DependSuffix): IsoRealms/Properties/PropertyModel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Properties_PropertyModel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Properties_PropertyModel.cpp$(DependSuffix) -MM IsoRealms/Properties/PropertyModel.cpp

$(IntermediateDirectory)/IsoRealms_Properties_PropertyModel.cpp$(PreprocessSuffix): IsoRealms/Properties/PropertyModel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Properties_PropertyModel.cpp$(PreprocessSuffix) IsoRealms/Properties/PropertyModel.cpp

$(IntermediateDirectory)/Modules_EditorOLD_Editor_Editor.cpp$(ObjectSuffix): Modules/EditorOLD/Editor/Editor.cpp $(IntermediateDirectory)/Modules_EditorOLD_Editor_Editor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/Editor/Editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_Editor_Editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_Editor_Editor.cpp$(DependSuffix): Modules/EditorOLD/Editor/Editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_Editor_Editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_Editor_Editor.cpp$(DependSuffix) -MM Modules/EditorOLD/Editor/Editor.cpp

$(IntermediateDirectory)/Modules_EditorOLD_Editor_Editor.cpp$(PreprocessSuffix): Modules/EditorOLD/Editor/Editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_Editor_Editor.cpp$(PreprocessSuffix) Modules/EditorOLD/Editor/Editor.cpp

$(IntermediateDirectory)/IsoRealms_Properties_DialogModel.cpp$(ObjectSuffix): IsoRealms/Properties/DialogModel.cpp $(IntermediateDirectory)/IsoRealms_Properties_DialogModel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Properties/DialogModel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Properties_DialogModel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Properties_DialogModel.cpp$(DependSuffix): IsoRealms/Properties/DialogModel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Properties_DialogModel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Properties_DialogModel.cpp$(DependSuffix) -MM IsoRealms/Properties/DialogModel.cpp

$(IntermediateDirectory)/IsoRealms_Properties_DialogModel.cpp$(PreprocessSuffix): IsoRealms/Properties/DialogModel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Properties_DialogModel.cpp$(PreprocessSuffix) IsoRealms/Properties/DialogModel.cpp

$(IntermediateDirectory)/IsoRealms_ResourceProperty.cpp$(ObjectSuffix): IsoRealms/ResourceProperty.cpp $(IntermediateDirectory)/IsoRealms_ResourceProperty.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourceProperty.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourceProperty.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourceProperty.cpp$(DependSuffix): IsoRealms/ResourceProperty.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourceProperty.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourceProperty.cpp$(DependSuffix) -MM IsoRealms/ResourceProperty.cpp

$(IntermediateDirectory)/IsoRealms_ResourceProperty.cpp$(PreprocessSuffix): IsoRealms/ResourceProperty.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourceProperty.cpp$(PreprocessSuffix) IsoRealms/ResourceProperty.cpp

$(IntermediateDirectory)/IsoRealms_Properties_ConditionValueEditorComponent.cpp$(ObjectSuffix): IsoRealms/Properties/ConditionValueEditorComponent.cpp $(IntermediateDirectory)/IsoRealms_Properties_ConditionValueEditorComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Properties/ConditionValueEditorComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Properties_ConditionValueEditorComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Properties_ConditionValueEditorComponent.cpp$(DependSuffix): IsoRealms/Properties/ConditionValueEditorComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Properties_ConditionValueEditorComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Properties_ConditionValueEditorComponent.cpp$(DependSuffix) -MM IsoRealms/Properties/ConditionValueEditorComponent.cpp

$(IntermediateDirectory)/IsoRealms_Properties_ConditionValueEditorComponent.cpp$(PreprocessSuffix): IsoRealms/Properties/ConditionValueEditorComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Properties_ConditionValueEditorComponent.cpp$(PreprocessSuffix) IsoRealms/Properties/ConditionValueEditorComponent.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Ball_Ball.cpp$(ObjectSuffix): Modules/Spindizzy/Ball/Ball.cpp $(IntermediateDirectory)/Modules_Spindizzy_Ball_Ball.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Ball/Ball.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Ball_Ball.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Ball_Ball.cpp$(DependSuffix): Modules/Spindizzy/Ball/Ball.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Ball_Ball.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Ball_Ball.cpp$(DependSuffix) -MM Modules/Spindizzy/Ball/Ball.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Ball_Ball.cpp$(PreprocessSuffix): Modules/Spindizzy/Ball/Ball.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Ball_Ball.cpp$(PreprocessSuffix) Modules/Spindizzy/Ball/Ball.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSet.cpp$(ObjectSuffix): Modules/Spindizzy/ThemeSet/ThemeSet.cpp $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/ThemeSet/ThemeSet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSet.cpp$(DependSuffix): Modules/Spindizzy/ThemeSet/ThemeSet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSet.cpp$(DependSuffix) -MM Modules/Spindizzy/ThemeSet/ThemeSet.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSet.cpp$(PreprocessSuffix): Modules/Spindizzy/ThemeSet/ThemeSet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSet.cpp$(PreprocessSuffix) Modules/Spindizzy/ThemeSet/ThemeSet.cpp

$(IntermediateDirectory)/IsoRealms_Properties_DialogCondition.cpp$(ObjectSuffix): IsoRealms/Properties/DialogCondition.cpp $(IntermediateDirectory)/IsoRealms_Properties_DialogCondition.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Properties/DialogCondition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Properties_DialogCondition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Properties_DialogCondition.cpp$(DependSuffix): IsoRealms/Properties/DialogCondition.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Properties_DialogCondition.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Properties_DialogCondition.cpp$(DependSuffix) -MM IsoRealms/Properties/DialogCondition.cpp

$(IntermediateDirectory)/IsoRealms_Properties_DialogCondition.cpp$(PreprocessSuffix): IsoRealms/Properties/DialogCondition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Properties_DialogCondition.cpp$(PreprocessSuffix) IsoRealms/Properties/DialogCondition.cpp

$(IntermediateDirectory)/IsoRealms_GUI_DynamicMenuItems.cpp$(ObjectSuffix): IsoRealms/GUI/DynamicMenuItems.cpp $(IntermediateDirectory)/IsoRealms_GUI_DynamicMenuItems.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/DynamicMenuItems.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_DynamicMenuItems.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_DynamicMenuItems.cpp$(DependSuffix): IsoRealms/GUI/DynamicMenuItems.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_DynamicMenuItems.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_DynamicMenuItems.cpp$(DependSuffix) -MM IsoRealms/GUI/DynamicMenuItems.cpp

$(IntermediateDirectory)/IsoRealms_GUI_DynamicMenuItems.cpp$(PreprocessSuffix): IsoRealms/GUI/DynamicMenuItems.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_DynamicMenuItems.cpp$(PreprocessSuffix) IsoRealms/GUI/DynamicMenuItems.cpp

$(IntermediateDirectory)/IsoRealms_GUI_LookAndFeel.cpp$(ObjectSuffix): IsoRealms/GUI/LookAndFeel.cpp $(IntermediateDirectory)/IsoRealms_GUI_LookAndFeel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/LookAndFeel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_LookAndFeel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_LookAndFeel.cpp$(DependSuffix): IsoRealms/GUI/LookAndFeel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_LookAndFeel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_LookAndFeel.cpp$(DependSuffix) -MM IsoRealms/GUI/LookAndFeel.cpp

$(IntermediateDirectory)/IsoRealms_GUI_LookAndFeel.cpp$(PreprocessSuffix): IsoRealms/GUI/LookAndFeel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_LookAndFeel.cpp$(PreprocessSuffix) IsoRealms/GUI/LookAndFeel.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Colour.cpp$(ObjectSuffix): IsoRealms/Assets/Fixed/Colour.cpp $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Colour.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Fixed/Colour.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Colour.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Colour.cpp$(DependSuffix): IsoRealms/Assets/Fixed/Colour.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Colour.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Colour.cpp$(DependSuffix) -MM IsoRealms/Assets/Fixed/Colour.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Colour.cpp$(PreprocessSuffix): IsoRealms/Assets/Fixed/Colour.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Colour.cpp$(PreprocessSuffix) IsoRealms/Assets/Fixed/Colour.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_DockedDialog.cpp$(ObjectSuffix): Modules/EditorOLD/ScreenEdge/DockedDialog.cpp $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_DockedDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/ScreenEdge/DockedDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_DockedDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_DockedDialog.cpp$(DependSuffix): Modules/EditorOLD/ScreenEdge/DockedDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_DockedDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_DockedDialog.cpp$(DependSuffix) -MM Modules/EditorOLD/ScreenEdge/DockedDialog.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_DockedDialog.cpp$(PreprocessSuffix): Modules/EditorOLD/ScreenEdge/DockedDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_DockedDialog.cpp$(PreprocessSuffix) Modules/EditorOLD/ScreenEdge/DockedDialog.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModelInstance.cpp$(ObjectSuffix): IsoRealms/GUI/ValueComponents/ComponentResourceModelInstance.cpp $(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModelInstance.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/ValueComponents/ComponentResourceModelInstance.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModelInstance.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModelInstance.cpp$(DependSuffix): IsoRealms/GUI/ValueComponents/ComponentResourceModelInstance.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModelInstance.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModelInstance.cpp$(DependSuffix) -MM IsoRealms/GUI/ValueComponents/ComponentResourceModelInstance.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModelInstance.cpp$(PreprocessSuffix): IsoRealms/GUI/ValueComponents/ComponentResourceModelInstance.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModelInstance.cpp$(PreprocessSuffix) IsoRealms/GUI/ValueComponents/ComponentResourceModelInstance.cpp

$(IntermediateDirectory)/Modules_Basics_RollingFloat_RollingFloat.cpp$(ObjectSuffix): Modules/Basics/RollingFloat/RollingFloat.cpp $(IntermediateDirectory)/Modules_Basics_RollingFloat_RollingFloat.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/RollingFloat/RollingFloat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_RollingFloat_RollingFloat.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_RollingFloat_RollingFloat.cpp$(DependSuffix): Modules/Basics/RollingFloat/RollingFloat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_RollingFloat_RollingFloat.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_RollingFloat_RollingFloat.cpp$(DependSuffix) -MM Modules/Basics/RollingFloat/RollingFloat.cpp

$(IntermediateDirectory)/Modules_Basics_RollingFloat_RollingFloat.cpp$(PreprocessSuffix): Modules/Basics/RollingFloat/RollingFloat.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_RollingFloat_RollingFloat.cpp$(PreprocessSuffix) Modules/Basics/RollingFloat/RollingFloat.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Enemy_EnemyType.cpp$(ObjectSuffix): Modules/Spindizzy/Enemy/EnemyType.cpp $(IntermediateDirectory)/Modules_Spindizzy_Enemy_EnemyType.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Enemy/EnemyType.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Enemy_EnemyType.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Enemy_EnemyType.cpp$(DependSuffix): Modules/Spindizzy/Enemy/EnemyType.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Enemy_EnemyType.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Enemy_EnemyType.cpp$(DependSuffix) -MM Modules/Spindizzy/Enemy/EnemyType.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Enemy_EnemyType.cpp$(PreprocessSuffix): Modules/Spindizzy/Enemy/EnemyType.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Enemy_EnemyType.cpp$(PreprocessSuffix) Modules/Spindizzy/Enemy/EnemyType.cpp

$(IntermediateDirectory)/IsoRealms_Properties_PropertyString.cpp$(ObjectSuffix): IsoRealms/Properties/PropertyString.cpp $(IntermediateDirectory)/IsoRealms_Properties_PropertyString.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Properties/PropertyString.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Properties_PropertyString.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Properties_PropertyString.cpp$(DependSuffix): IsoRealms/Properties/PropertyString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Properties_PropertyString.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Properties_PropertyString.cpp$(DependSuffix) -MM IsoRealms/Properties/PropertyString.cpp

$(IntermediateDirectory)/IsoRealms_Properties_PropertyString.cpp$(PreprocessSuffix): IsoRealms/Properties/PropertyString.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Properties_PropertyString.cpp$(PreprocessSuffix) IsoRealms/Properties/PropertyString.cpp

$(IntermediateDirectory)/IsoRealms_GUI_MenuPopup.cpp$(ObjectSuffix): IsoRealms/GUI/MenuPopup.cpp $(IntermediateDirectory)/IsoRealms_GUI_MenuPopup.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/MenuPopup.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_MenuPopup.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_MenuPopup.cpp$(DependSuffix): IsoRealms/GUI/MenuPopup.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_MenuPopup.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_MenuPopup.cpp$(DependSuffix) -MM IsoRealms/GUI/MenuPopup.cpp

$(IntermediateDirectory)/IsoRealms_GUI_MenuPopup.cpp$(PreprocessSuffix): IsoRealms/GUI/MenuPopup.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_MenuPopup.cpp$(PreprocessSuffix) IsoRealms/GUI/MenuPopup.cpp

$(IntermediateDirectory)/IsoRealms_GUI_GridLayoutComponent.cpp$(ObjectSuffix): IsoRealms/GUI/GridLayoutComponent.cpp $(IntermediateDirectory)/IsoRealms_GUI_GridLayoutComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/GridLayoutComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_GridLayoutComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_GridLayoutComponent.cpp$(DependSuffix): IsoRealms/GUI/GridLayoutComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_GridLayoutComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_GridLayoutComponent.cpp$(DependSuffix) -MM IsoRealms/GUI/GridLayoutComponent.cpp

$(IntermediateDirectory)/IsoRealms_GUI_GridLayoutComponent.cpp$(PreprocessSuffix): IsoRealms/GUI/GridLayoutComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_GridLayoutComponent.cpp$(PreprocessSuffix) IsoRealms/GUI/GridLayoutComponent.cpp

$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventPause.cpp$(ObjectSuffix): Modules/Basics/Sequence/ColourTrackEventPause.cpp $(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventPause.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Sequence/ColourTrackEventPause.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventPause.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventPause.cpp$(DependSuffix): Modules/Basics/Sequence/ColourTrackEventPause.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventPause.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventPause.cpp$(DependSuffix) -MM Modules/Basics/Sequence/ColourTrackEventPause.cpp

$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventPause.cpp$(PreprocessSuffix): Modules/Basics/Sequence/ColourTrackEventPause.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventPause.cpp$(PreprocessSuffix) Modules/Basics/Sequence/ColourTrackEventPause.cpp

$(IntermediateDirectory)/IsoRealms_GUI_AbstractRectangularComponent.cpp$(ObjectSuffix): IsoRealms/GUI/AbstractRectangularComponent.cpp $(IntermediateDirectory)/IsoRealms_GUI_AbstractRectangularComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/AbstractRectangularComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_AbstractRectangularComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_AbstractRectangularComponent.cpp$(DependSuffix): IsoRealms/GUI/AbstractRectangularComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_AbstractRectangularComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_AbstractRectangularComponent.cpp$(DependSuffix) -MM IsoRealms/GUI/AbstractRectangularComponent.cpp

$(IntermediateDirectory)/IsoRealms_GUI_AbstractRectangularComponent.cpp$(PreprocessSuffix): IsoRealms/GUI/AbstractRectangularComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_AbstractRectangularComponent.cpp$(PreprocessSuffix) IsoRealms/GUI/AbstractRectangularComponent.cpp

$(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_FloatToString.cpp$(ObjectSuffix): IsoRealms/Assets/TypeConverted/FloatToString.cpp $(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_FloatToString.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/TypeConverted/FloatToString.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_FloatToString.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_FloatToString.cpp$(DependSuffix): IsoRealms/Assets/TypeConverted/FloatToString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_FloatToString.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_FloatToString.cpp$(DependSuffix) -MM IsoRealms/Assets/TypeConverted/FloatToString.cpp

$(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_FloatToString.cpp$(PreprocessSuffix): IsoRealms/Assets/TypeConverted/FloatToString.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_FloatToString.cpp$(PreprocessSuffix) IsoRealms/Assets/TypeConverted/FloatToString.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_VertexRegistry.cpp$(ObjectSuffix): IsoRealms/Assets/Registry/VertexRegistry.cpp $(IntermediateDirectory)/IsoRealms_Assets_Registry_VertexRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Registry/VertexRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Registry_VertexRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Registry_VertexRegistry.cpp$(DependSuffix): IsoRealms/Assets/Registry/VertexRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Registry_VertexRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Registry_VertexRegistry.cpp$(DependSuffix) -MM IsoRealms/Assets/Registry/VertexRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_VertexRegistry.cpp$(PreprocessSuffix): IsoRealms/Assets/Registry/VertexRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Registry_VertexRegistry.cpp$(PreprocessSuffix) IsoRealms/Assets/Registry/VertexRegistry.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceScripts_DialogResourceScripts.cpp$(ObjectSuffix): Modules/EditorOLD/DialogResourceScripts/DialogResourceScripts.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceScripts_DialogResourceScripts.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogResourceScripts/DialogResourceScripts.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceScripts_DialogResourceScripts.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceScripts_DialogResourceScripts.cpp$(DependSuffix): Modules/EditorOLD/DialogResourceScripts/DialogResourceScripts.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceScripts_DialogResourceScripts.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceScripts_DialogResourceScripts.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogResourceScripts/DialogResourceScripts.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceScripts_DialogResourceScripts.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogResourceScripts/DialogResourceScripts.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceScripts_DialogResourceScripts.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogResourceScripts/DialogResourceScripts.cpp

$(IntermediateDirectory)/IsoRealms_GUI_MenuItem.cpp$(ObjectSuffix): IsoRealms/GUI/MenuItem.cpp $(IntermediateDirectory)/IsoRealms_GUI_MenuItem.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/MenuItem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_MenuItem.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_MenuItem.cpp$(DependSuffix): IsoRealms/GUI/MenuItem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_MenuItem.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_MenuItem.cpp$(DependSuffix) -MM IsoRealms/GUI/MenuItem.cpp

$(IntermediateDirectory)/IsoRealms_GUI_MenuItem.cpp$(PreprocessSuffix): IsoRealms/GUI/MenuItem.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_MenuItem.cpp$(PreprocessSuffix) IsoRealms/GUI/MenuItem.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Throbber_Throbber.cpp$(ObjectSuffix): Modules/FrontEnd/Throbber/Throbber.cpp $(IntermediateDirectory)/Modules_FrontEnd_Throbber_Throbber.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/FrontEnd/Throbber/Throbber.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_FrontEnd_Throbber_Throbber.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_FrontEnd_Throbber_Throbber.cpp$(DependSuffix): Modules/FrontEnd/Throbber/Throbber.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_FrontEnd_Throbber_Throbber.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_FrontEnd_Throbber_Throbber.cpp$(DependSuffix) -MM Modules/FrontEnd/Throbber/Throbber.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Throbber_Throbber.cpp$(PreprocessSuffix): Modules/FrontEnd/Throbber/Throbber.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_FrontEnd_Throbber_Throbber.cpp$(PreprocessSuffix) Modules/FrontEnd/Throbber/Throbber.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ComponentScriptCall.cpp$(ObjectSuffix): IsoRealms/GUI/ComponentScriptCall.cpp $(IntermediateDirectory)/IsoRealms_GUI_ComponentScriptCall.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/ComponentScriptCall.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_ComponentScriptCall.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_ComponentScriptCall.cpp$(DependSuffix): IsoRealms/GUI/ComponentScriptCall.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_ComponentScriptCall.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_ComponentScriptCall.cpp$(DependSuffix) -MM IsoRealms/GUI/ComponentScriptCall.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ComponentScriptCall.cpp$(PreprocessSuffix): IsoRealms/GUI/ComponentScriptCall.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_ComponentScriptCall.cpp$(PreprocessSuffix) IsoRealms/GUI/ComponentScriptCall.cpp

$(IntermediateDirectory)/IsoRealms_Properties_ModelValueComponent.cpp$(ObjectSuffix): IsoRealms/Properties/ModelValueComponent.cpp $(IntermediateDirectory)/IsoRealms_Properties_ModelValueComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Properties/ModelValueComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Properties_ModelValueComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Properties_ModelValueComponent.cpp$(DependSuffix): IsoRealms/Properties/ModelValueComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Properties_ModelValueComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Properties_ModelValueComponent.cpp$(DependSuffix) -MM IsoRealms/Properties/ModelValueComponent.cpp

$(IntermediateDirectory)/IsoRealms_Properties_ModelValueComponent.cpp$(PreprocessSuffix): IsoRealms/Properties/ModelValueComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Properties_ModelValueComponent.cpp$(PreprocessSuffix) IsoRealms/Properties/ModelValueComponent.cpp

$(IntermediateDirectory)/Modules_Replay_Player_Player.cpp$(ObjectSuffix): Modules/Replay/Player/Player.cpp $(IntermediateDirectory)/Modules_Replay_Player_Player.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Replay/Player/Player.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Replay_Player_Player.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Replay_Player_Player.cpp$(DependSuffix): Modules/Replay/Player/Player.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Replay_Player_Player.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Replay_Player_Player.cpp$(DependSuffix) -MM Modules/Replay/Player/Player.cpp

$(IntermediateDirectory)/Modules_Replay_Player_Player.cpp$(PreprocessSuffix): Modules/Replay/Player/Player.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Replay_Player_Player.cpp$(PreprocessSuffix) Modules/Replay/Player/Player.cpp

$(IntermediateDirectory)/Modules_Basics_Sprite_SpriteInstance.cpp$(ObjectSuffix): Modules/Basics/Sprite/SpriteInstance.cpp $(IntermediateDirectory)/Modules_Basics_Sprite_SpriteInstance.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Sprite/SpriteInstance.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Sprite_SpriteInstance.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Sprite_SpriteInstance.cpp$(DependSuffix): Modules/Basics/Sprite/SpriteInstance.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Sprite_SpriteInstance.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Sprite_SpriteInstance.cpp$(DependSuffix) -MM Modules/Basics/Sprite/SpriteInstance.cpp

$(IntermediateDirectory)/Modules_Basics_Sprite_SpriteInstance.cpp$(PreprocessSuffix): Modules/Basics/Sprite/SpriteInstance.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Sprite_SpriteInstance.cpp$(PreprocessSuffix) Modules/Basics/Sprite/SpriteInstance.cpp

$(IntermediateDirectory)/Modules_Spindizzy_World_WorldSurfaces.cpp$(ObjectSuffix): Modules/Spindizzy/World/WorldSurfaces.cpp $(IntermediateDirectory)/Modules_Spindizzy_World_WorldSurfaces.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/World/WorldSurfaces.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_World_WorldSurfaces.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_World_WorldSurfaces.cpp$(DependSuffix): Modules/Spindizzy/World/WorldSurfaces.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_World_WorldSurfaces.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_World_WorldSurfaces.cpp$(DependSuffix) -MM Modules/Spindizzy/World/WorldSurfaces.cpp

$(IntermediateDirectory)/Modules_Spindizzy_World_WorldSurfaces.cpp$(PreprocessSuffix): Modules/Spindizzy/World/WorldSurfaces.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_World_WorldSurfaces.cpp$(PreprocessSuffix) Modules/Spindizzy/World/WorldSurfaces.cpp

$(IntermediateDirectory)/IsoRealms_GUI_FlexibleGridLayoutComponent.cpp$(ObjectSuffix): IsoRealms/GUI/FlexibleGridLayoutComponent.cpp $(IntermediateDirectory)/IsoRealms_GUI_FlexibleGridLayoutComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/FlexibleGridLayoutComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_FlexibleGridLayoutComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_FlexibleGridLayoutComponent.cpp$(DependSuffix): IsoRealms/GUI/FlexibleGridLayoutComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_FlexibleGridLayoutComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_FlexibleGridLayoutComponent.cpp$(DependSuffix) -MM IsoRealms/GUI/FlexibleGridLayoutComponent.cpp

$(IntermediateDirectory)/IsoRealms_GUI_FlexibleGridLayoutComponent.cpp$(PreprocessSuffix): IsoRealms/GUI/FlexibleGridLayoutComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_FlexibleGridLayoutComponent.cpp$(PreprocessSuffix) IsoRealms/GUI/FlexibleGridLayoutComponent.cpp

$(IntermediateDirectory)/IsoRealms_GUI_RectangularComponent.cpp$(ObjectSuffix): IsoRealms/GUI/RectangularComponent.cpp $(IntermediateDirectory)/IsoRealms_GUI_RectangularComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/RectangularComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_RectangularComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_RectangularComponent.cpp$(DependSuffix): IsoRealms/GUI/RectangularComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_RectangularComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_RectangularComponent.cpp$(DependSuffix) -MM IsoRealms/GUI/RectangularComponent.cpp

$(IntermediateDirectory)/IsoRealms_GUI_RectangularComponent.cpp$(PreprocessSuffix): IsoRealms/GUI/RectangularComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_RectangularComponent.cpp$(PreprocessSuffix) IsoRealms/GUI/RectangularComponent.cpp

$(IntermediateDirectory)/IsoRealms_Application.cpp$(ObjectSuffix): IsoRealms/Application.cpp $(IntermediateDirectory)/IsoRealms_Application.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Application.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Application.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Application.cpp$(DependSuffix): IsoRealms/Application.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Application.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Application.cpp$(DependSuffix) -MM IsoRealms/Application.cpp

$(IntermediateDirectory)/IsoRealms_Application.cpp$(PreprocessSuffix): IsoRealms/Application.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Application.cpp$(PreprocessSuffix) IsoRealms/Application.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDScreen.cpp$(ObjectSuffix): Modules/Basics/HUD/Components/HUDScreen.cpp $(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDScreen.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/HUD/Components/HUDScreen.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDScreen.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDScreen.cpp$(DependSuffix): Modules/Basics/HUD/Components/HUDScreen.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDScreen.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDScreen.cpp$(DependSuffix) -MM Modules/Basics/HUD/Components/HUDScreen.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDScreen.cpp$(PreprocessSuffix): Modules/Basics/HUD/Components/HUDScreen.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_HUD_Components_HUDScreen.cpp$(PreprocessSuffix) Modules/Basics/HUD/Components/HUDScreen.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentColourSelector.cpp$(ObjectSuffix): IsoRealms/GUI/Dialogs/ComponentColourSelector.cpp $(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentColourSelector.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/Dialogs/ComponentColourSelector.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentColourSelector.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentColourSelector.cpp$(DependSuffix): IsoRealms/GUI/Dialogs/ComponentColourSelector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentColourSelector.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentColourSelector.cpp$(DependSuffix) -MM IsoRealms/GUI/Dialogs/ComponentColourSelector.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentColourSelector.cpp$(PreprocessSuffix): IsoRealms/GUI/Dialogs/ComponentColourSelector.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentColourSelector.cpp$(PreprocessSuffix) IsoRealms/GUI/Dialogs/ComponentColourSelector.cpp

$(IntermediateDirectory)/IsoRealms_GUI_CheckBox.cpp$(ObjectSuffix): IsoRealms/GUI/CheckBox.cpp $(IntermediateDirectory)/IsoRealms_GUI_CheckBox.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/CheckBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_CheckBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_CheckBox.cpp$(DependSuffix): IsoRealms/GUI/CheckBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_CheckBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_CheckBox.cpp$(DependSuffix) -MM IsoRealms/GUI/CheckBox.cpp

$(IntermediateDirectory)/IsoRealms_GUI_CheckBox.cpp$(PreprocessSuffix): IsoRealms/GUI/CheckBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_CheckBox.cpp$(PreprocessSuffix) IsoRealms/GUI/CheckBox.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Boolean.cpp$(ObjectSuffix): IsoRealms/Assets/Fixed/Boolean.cpp $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Boolean.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Fixed/Boolean.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Boolean.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Boolean.cpp$(DependSuffix): IsoRealms/Assets/Fixed/Boolean.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Boolean.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Boolean.cpp$(DependSuffix) -MM IsoRealms/Assets/Fixed/Boolean.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Boolean.cpp$(PreprocessSuffix): IsoRealms/Assets/Fixed/Boolean.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Boolean.cpp$(PreprocessSuffix) IsoRealms/Assets/Fixed/Boolean.cpp

$(IntermediateDirectory)/IsoRealms_GUI_TabbedContainer.cpp$(ObjectSuffix): IsoRealms/GUI/TabbedContainer.cpp $(IntermediateDirectory)/IsoRealms_GUI_TabbedContainer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/TabbedContainer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_TabbedContainer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_TabbedContainer.cpp$(DependSuffix): IsoRealms/GUI/TabbedContainer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_TabbedContainer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_TabbedContainer.cpp$(DependSuffix) -MM IsoRealms/GUI/TabbedContainer.cpp

$(IntermediateDirectory)/IsoRealms_GUI_TabbedContainer.cpp$(PreprocessSuffix): IsoRealms/GUI/TabbedContainer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_TabbedContainer.cpp$(PreprocessSuffix) IsoRealms/GUI/TabbedContainer.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Dialog.cpp$(ObjectSuffix): IsoRealms/GUI/Dialog.cpp $(IntermediateDirectory)/IsoRealms_GUI_Dialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/Dialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_Dialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_Dialog.cpp$(DependSuffix): IsoRealms/GUI/Dialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_Dialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_Dialog.cpp$(DependSuffix) -MM IsoRealms/GUI/Dialog.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Dialog.cpp$(PreprocessSuffix): IsoRealms/GUI/Dialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_Dialog.cpp$(PreprocessSuffix) IsoRealms/GUI/Dialog.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconColour.cpp$(ObjectSuffix): Modules/EditorOLD/DialogResourceColoursTextures/IconColour.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconColour.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogResourceColoursTextures/IconColour.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconColour.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconColour.cpp$(DependSuffix): Modules/EditorOLD/DialogResourceColoursTextures/IconColour.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconColour.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconColour.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogResourceColoursTextures/IconColour.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconColour.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogResourceColoursTextures/IconColour.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceColoursTextures_IconColour.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogResourceColoursTextures/IconColour.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Lift_Lift.cpp$(ObjectSuffix): Modules/Spindizzy/Lift/Lift.cpp $(IntermediateDirectory)/Modules_Spindizzy_Lift_Lift.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Lift/Lift.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Lift_Lift.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Lift_Lift.cpp$(DependSuffix): Modules/Spindizzy/Lift/Lift.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Lift_Lift.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Lift_Lift.cpp$(DependSuffix) -MM Modules/Spindizzy/Lift/Lift.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Lift_Lift.cpp$(PreprocessSuffix): Modules/Spindizzy/Lift/Lift.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Lift_Lift.cpp$(PreprocessSuffix) Modules/Spindizzy/Lift/Lift.cpp

$(IntermediateDirectory)/Modules_Basics_Integer_Integer.cpp$(ObjectSuffix): Modules/Basics/Integer/Integer.cpp $(IntermediateDirectory)/Modules_Basics_Integer_Integer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Integer/Integer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Integer_Integer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Integer_Integer.cpp$(DependSuffix): Modules/Basics/Integer/Integer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Integer_Integer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Integer_Integer.cpp$(DependSuffix) -MM Modules/Basics/Integer/Integer.cpp

$(IntermediateDirectory)/Modules_Basics_Integer_Integer.cpp$(PreprocessSuffix): Modules/Basics/Integer/Integer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Integer_Integer.cpp$(PreprocessSuffix) Modules/Basics/Integer/Integer.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ScrollableContainer.cpp$(ObjectSuffix): IsoRealms/GUI/ScrollableContainer.cpp $(IntermediateDirectory)/IsoRealms_GUI_ScrollableContainer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/ScrollableContainer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_ScrollableContainer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_ScrollableContainer.cpp$(DependSuffix): IsoRealms/GUI/ScrollableContainer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_ScrollableContainer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_ScrollableContainer.cpp$(DependSuffix) -MM IsoRealms/GUI/ScrollableContainer.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ScrollableContainer.cpp$(PreprocessSuffix): IsoRealms/GUI/ScrollableContainer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_ScrollableContainer.cpp$(PreprocessSuffix) IsoRealms/GUI/ScrollableContainer.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemResolution.cpp$(ObjectSuffix): Modules/FrontEnd/Menu/MenuItemResolution.cpp $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemResolution.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/FrontEnd/Menu/MenuItemResolution.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemResolution.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemResolution.cpp$(DependSuffix): Modules/FrontEnd/Menu/MenuItemResolution.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemResolution.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemResolution.cpp$(DependSuffix) -MM Modules/FrontEnd/Menu/MenuItemResolution.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemResolution.cpp$(PreprocessSuffix): Modules/FrontEnd/Menu/MenuItemResolution.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_FrontEnd_Menu_MenuItemResolution.cpp$(PreprocessSuffix) Modules/FrontEnd/Menu/MenuItemResolution.cpp

$(IntermediateDirectory)/IsoRealms_GUI_SliderComponent.cpp$(ObjectSuffix): IsoRealms/GUI/SliderComponent.cpp $(IntermediateDirectory)/IsoRealms_GUI_SliderComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/SliderComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_SliderComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_SliderComponent.cpp$(DependSuffix): IsoRealms/GUI/SliderComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_SliderComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_SliderComponent.cpp$(DependSuffix) -MM IsoRealms/GUI/SliderComponent.cpp

$(IntermediateDirectory)/IsoRealms_GUI_SliderComponent.cpp$(PreprocessSuffix): IsoRealms/GUI/SliderComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_SliderComponent.cpp$(PreprocessSuffix) IsoRealms/GUI/SliderComponent.cpp

$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrack.cpp$(ObjectSuffix): Modules/Basics/Sequence/ColourTrack.cpp $(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrack.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Sequence/ColourTrack.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrack.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrack.cpp$(DependSuffix): Modules/Basics/Sequence/ColourTrack.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrack.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrack.cpp$(DependSuffix) -MM Modules/Basics/Sequence/ColourTrack.cpp

$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrack.cpp$(PreprocessSuffix): Modules/Basics/Sequence/ColourTrack.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrack.cpp$(PreprocessSuffix) Modules/Basics/Sequence/ColourTrack.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourSaturation.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyColourSaturation.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyColourSaturation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyColourSaturation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourSaturation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourSaturation.cpp$(DependSuffix): IsoRealms/ResourcePropertyColourSaturation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourSaturation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourSaturation.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyColourSaturation.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourSaturation.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyColourSaturation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyColourSaturation.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyColourSaturation.cpp

$(IntermediateDirectory)/IsoRealms_GUI_CheckBoxWithLabel.cpp$(ObjectSuffix): IsoRealms/GUI/CheckBoxWithLabel.cpp $(IntermediateDirectory)/IsoRealms_GUI_CheckBoxWithLabel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/CheckBoxWithLabel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_CheckBoxWithLabel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_CheckBoxWithLabel.cpp$(DependSuffix): IsoRealms/GUI/CheckBoxWithLabel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_CheckBoxWithLabel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_CheckBoxWithLabel.cpp$(DependSuffix) -MM IsoRealms/GUI/CheckBoxWithLabel.cpp

$(IntermediateDirectory)/IsoRealms_GUI_CheckBoxWithLabel.cpp$(PreprocessSuffix): IsoRealms/GUI/CheckBoxWithLabel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_CheckBoxWithLabel.cpp$(PreprocessSuffix) IsoRealms/GUI/CheckBoxWithLabel.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ComponentEdgeLayout.cpp$(ObjectSuffix): IsoRealms/GUI/ComponentEdgeLayout.cpp $(IntermediateDirectory)/IsoRealms_GUI_ComponentEdgeLayout.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/ComponentEdgeLayout.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_ComponentEdgeLayout.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_ComponentEdgeLayout.cpp$(DependSuffix): IsoRealms/GUI/ComponentEdgeLayout.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_ComponentEdgeLayout.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_ComponentEdgeLayout.cpp$(DependSuffix) -MM IsoRealms/GUI/ComponentEdgeLayout.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ComponentEdgeLayout.cpp$(PreprocessSuffix): IsoRealms/GUI/ComponentEdgeLayout.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_ComponentEdgeLayout.cpp$(PreprocessSuffix) IsoRealms/GUI/ComponentEdgeLayout.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyVertex.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyVertex.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyVertex.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyVertex.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyVertex.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyVertex.cpp$(DependSuffix): IsoRealms/ResourcePropertyVertex.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyVertex.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyVertex.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyVertex.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyVertex.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyVertex.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyVertex.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyVertex.cpp

$(IntermediateDirectory)/IsoRealms_Persistence_ParseException.cpp$(ObjectSuffix): IsoRealms/Persistence/ParseException.cpp $(IntermediateDirectory)/IsoRealms_Persistence_ParseException.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Persistence/ParseException.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Persistence_ParseException.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Persistence_ParseException.cpp$(DependSuffix): IsoRealms/Persistence/ParseException.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Persistence_ParseException.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Persistence_ParseException.cpp$(DependSuffix) -MM IsoRealms/Persistence/ParseException.cpp

$(IntermediateDirectory)/IsoRealms_Persistence_ParseException.cpp$(PreprocessSuffix): IsoRealms/Persistence/ParseException.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Persistence_ParseException.cpp$(PreprocessSuffix) IsoRealms/Persistence/ParseException.cpp

$(IntermediateDirectory)/IsoRealms_GUI_MenuBar.cpp$(ObjectSuffix): IsoRealms/GUI/MenuBar.cpp $(IntermediateDirectory)/IsoRealms_GUI_MenuBar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/MenuBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_MenuBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_MenuBar.cpp$(DependSuffix): IsoRealms/GUI/MenuBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_MenuBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_MenuBar.cpp$(DependSuffix) -MM IsoRealms/GUI/MenuBar.cpp

$(IntermediateDirectory)/IsoRealms_GUI_MenuBar.cpp$(PreprocessSuffix): IsoRealms/GUI/MenuBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_MenuBar.cpp$(PreprocessSuffix) IsoRealms/GUI/MenuBar.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceVertices_DialogResourceVertices.cpp$(ObjectSuffix): Modules/EditorOLD/DialogResourceVertices/DialogResourceVertices.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceVertices_DialogResourceVertices.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogResourceVertices/DialogResourceVertices.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceVertices_DialogResourceVertices.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceVertices_DialogResourceVertices.cpp$(DependSuffix): Modules/EditorOLD/DialogResourceVertices/DialogResourceVertices.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceVertices_DialogResourceVertices.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceVertices_DialogResourceVertices.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogResourceVertices/DialogResourceVertices.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceVertices_DialogResourceVertices.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogResourceVertices/DialogResourceVertices.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceVertices_DialogResourceVertices.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogResourceVertices/DialogResourceVertices.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeInteger.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyNativeInteger.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeInteger.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyNativeInteger.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeInteger.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeInteger.cpp$(DependSuffix): IsoRealms/ResourcePropertyNativeInteger.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeInteger.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeInteger.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyNativeInteger.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeInteger.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyNativeInteger.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeInteger.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyNativeInteger.cpp

$(IntermediateDirectory)/IsoRealms_Project.cpp$(ObjectSuffix): IsoRealms/Project.cpp $(IntermediateDirectory)/IsoRealms_Project.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Project.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Project.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Project.cpp$(DependSuffix): IsoRealms/Project.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Project.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Project.cpp$(DependSuffix) -MM IsoRealms/Project.cpp

$(IntermediateDirectory)/IsoRealms_Project.cpp$(PreprocessSuffix): IsoRealms/Project.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Project.cpp$(PreprocessSuffix) IsoRealms/Project.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Menu_Menu.cpp$(ObjectSuffix): Modules/FrontEnd/Menu/Menu.cpp $(IntermediateDirectory)/Modules_FrontEnd_Menu_Menu.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/FrontEnd/Menu/Menu.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_FrontEnd_Menu_Menu.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_FrontEnd_Menu_Menu.cpp$(DependSuffix): Modules/FrontEnd/Menu/Menu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_FrontEnd_Menu_Menu.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_FrontEnd_Menu_Menu.cpp$(DependSuffix) -MM Modules/FrontEnd/Menu/Menu.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Menu_Menu.cpp$(PreprocessSuffix): Modules/FrontEnd/Menu/Menu.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_FrontEnd_Menu_Menu.cpp$(PreprocessSuffix) Modules/FrontEnd/Menu/Menu.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Container.cpp$(ObjectSuffix): IsoRealms/GUI/Container.cpp $(IntermediateDirectory)/IsoRealms_GUI_Container.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/Container.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_Container.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_Container.cpp$(DependSuffix): IsoRealms/GUI/Container.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_Container.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_Container.cpp$(DependSuffix) -MM IsoRealms/GUI/Container.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Container.cpp$(PreprocessSuffix): IsoRealms/GUI/Container.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_Container.cpp$(PreprocessSuffix) IsoRealms/GUI/Container.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeFloat.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyNativeFloat.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeFloat.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyNativeFloat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeFloat.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeFloat.cpp$(DependSuffix): IsoRealms/ResourcePropertyNativeFloat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeFloat.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeFloat.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyNativeFloat.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeFloat.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyNativeFloat.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeFloat.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyNativeFloat.cpp

$(IntermediateDirectory)/IsoRealms_Collision_CollisionUtils.cpp$(ObjectSuffix): IsoRealms/Collision/CollisionUtils.cpp $(IntermediateDirectory)/IsoRealms_Collision_CollisionUtils.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Collision/CollisionUtils.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Collision_CollisionUtils.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Collision_CollisionUtils.cpp$(DependSuffix): IsoRealms/Collision/CollisionUtils.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Collision_CollisionUtils.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Collision_CollisionUtils.cpp$(DependSuffix) -MM IsoRealms/Collision/CollisionUtils.cpp

$(IntermediateDirectory)/IsoRealms_Collision_CollisionUtils.cpp$(PreprocessSuffix): IsoRealms/Collision/CollisionUtils.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Collision_CollisionUtils.cpp$(PreprocessSuffix) IsoRealms/Collision/CollisionUtils.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeIcon.cpp$(ObjectSuffix): Modules/Spindizzy/ThemeSet/ThemeIcon.cpp $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeIcon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/ThemeSet/ThemeIcon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeIcon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeIcon.cpp$(DependSuffix): Modules/Spindizzy/ThemeSet/ThemeIcon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeIcon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeIcon.cpp$(DependSuffix) -MM Modules/Spindizzy/ThemeSet/ThemeIcon.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeIcon.cpp$(PreprocessSuffix): Modules/Spindizzy/ThemeSet/ThemeIcon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeIcon.cpp$(PreprocessSuffix) Modules/Spindizzy/ThemeSet/ThemeIcon.cpp

$(IntermediateDirectory)/Modules_Replay_ReplayLuaSupport.cpp$(ObjectSuffix): Modules/Replay/ReplayLuaSupport.cpp $(IntermediateDirectory)/Modules_Replay_ReplayLuaSupport.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Replay/ReplayLuaSupport.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Replay_ReplayLuaSupport.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Replay_ReplayLuaSupport.cpp$(DependSuffix): Modules/Replay/ReplayLuaSupport.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Replay_ReplayLuaSupport.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Replay_ReplayLuaSupport.cpp$(DependSuffix) -MM Modules/Replay/ReplayLuaSupport.cpp

$(IntermediateDirectory)/Modules_Replay_ReplayLuaSupport.cpp$(PreprocessSuffix): Modules/Replay/ReplayLuaSupport.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Replay_ReplayLuaSupport.cpp$(PreprocessSuffix) Modules/Replay/ReplayLuaSupport.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternTile.cpp$(ObjectSuffix): Modules/Spindizzy/Terrain/WallPatternTile.cpp $(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternTile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Terrain/WallPatternTile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternTile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternTile.cpp$(DependSuffix): Modules/Spindizzy/Terrain/WallPatternTile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternTile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternTile.cpp$(DependSuffix) -MM Modules/Spindizzy/Terrain/WallPatternTile.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternTile.cpp$(PreprocessSuffix): Modules/Spindizzy/Terrain/WallPatternTile.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternTile.cpp$(PreprocessSuffix) Modules/Spindizzy/Terrain/WallPatternTile.cpp

$(IntermediateDirectory)/IsoRealms_Input_AxisMapping.cpp$(ObjectSuffix): IsoRealms/Input/AxisMapping.cpp $(IntermediateDirectory)/IsoRealms_Input_AxisMapping.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Input/AxisMapping.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Input_AxisMapping.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Input_AxisMapping.cpp$(DependSuffix): IsoRealms/Input/AxisMapping.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Input_AxisMapping.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Input_AxisMapping.cpp$(DependSuffix) -MM IsoRealms/Input/AxisMapping.cpp

$(IntermediateDirectory)/IsoRealms_Input_AxisMapping.cpp$(PreprocessSuffix): IsoRealms/Input/AxisMapping.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Input_AxisMapping.cpp$(PreprocessSuffix) IsoRealms/Input/AxisMapping.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Zone_Zone.cpp$(ObjectSuffix): Modules/Spindizzy/Zone/Zone.cpp $(IntermediateDirectory)/Modules_Spindizzy_Zone_Zone.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Zone/Zone.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Zone_Zone.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Zone_Zone.cpp$(DependSuffix): Modules/Spindizzy/Zone/Zone.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Zone_Zone.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Zone_Zone.cpp$(DependSuffix) -MM Modules/Spindizzy/Zone/Zone.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Zone_Zone.cpp$(PreprocessSuffix): Modules/Spindizzy/Zone/Zone.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Zone_Zone.cpp$(PreprocessSuffix) Modules/Spindizzy/Zone/Zone.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Integer.cpp$(ObjectSuffix): IsoRealms/Assets/Fixed/Integer.cpp $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Integer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Fixed/Integer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Integer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Integer.cpp$(DependSuffix): IsoRealms/Assets/Fixed/Integer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Integer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Integer.cpp$(DependSuffix) -MM IsoRealms/Assets/Fixed/Integer.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Integer.cpp$(PreprocessSuffix): IsoRealms/Assets/Fixed/Integer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Integer.cpp$(PreprocessSuffix) IsoRealms/Assets/Fixed/Integer.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyAsset.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyAsset.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyAsset.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyAsset.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyAsset.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyAsset.cpp$(DependSuffix): IsoRealms/ResourcePropertyAsset.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyAsset.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyAsset.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyAsset.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyAsset.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyAsset.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyAsset.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyAsset.cpp

$(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunkInstance.cpp$(ObjectSuffix): Modules/Spindizzy/DebrisChunk/DebrisChunkInstance.cpp $(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunkInstance.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/DebrisChunk/DebrisChunkInstance.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunkInstance.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunkInstance.cpp$(DependSuffix): Modules/Spindizzy/DebrisChunk/DebrisChunkInstance.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunkInstance.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunkInstance.cpp$(DependSuffix) -MM Modules/Spindizzy/DebrisChunk/DebrisChunkInstance.cpp

$(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunkInstance.cpp$(PreprocessSuffix): Modules/Spindizzy/DebrisChunk/DebrisChunkInstance.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_DebrisChunk_DebrisChunkInstance.cpp$(PreprocessSuffix) Modules/Spindizzy/DebrisChunk/DebrisChunkInstance.cpp

$(IntermediateDirectory)/IsoRealms_Options_LocalOptions.cpp$(ObjectSuffix): IsoRealms/Options/LocalOptions.cpp $(IntermediateDirectory)/IsoRealms_Options_LocalOptions.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Options/LocalOptions.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Options_LocalOptions.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Options_LocalOptions.cpp$(DependSuffix): IsoRealms/Options/LocalOptions.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Options_LocalOptions.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Options_LocalOptions.cpp$(DependSuffix) -MM IsoRealms/Options/LocalOptions.cpp

$(IntermediateDirectory)/IsoRealms_Options_LocalOptions.cpp$(PreprocessSuffix): IsoRealms/Options/LocalOptions.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Options_LocalOptions.cpp$(PreprocessSuffix) IsoRealms/Options/LocalOptions.cpp

$(IntermediateDirectory)/IsoRealms_System.cpp$(ObjectSuffix): IsoRealms/System.cpp $(IntermediateDirectory)/IsoRealms_System.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/System.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_System.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_System.cpp$(DependSuffix): IsoRealms/System.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_System.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_System.cpp$(DependSuffix) -MM IsoRealms/System.cpp

$(IntermediateDirectory)/IsoRealms_System.cpp$(PreprocessSuffix): IsoRealms/System.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_System.cpp$(PreprocessSuffix) IsoRealms/System.cpp

$(IntermediateDirectory)/IsoRealms_Options_Options.cpp$(ObjectSuffix): IsoRealms/Options/Options.cpp $(IntermediateDirectory)/IsoRealms_Options_Options.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Options/Options.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Options_Options.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Options_Options.cpp$(DependSuffix): IsoRealms/Options/Options.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Options_Options.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Options_Options.cpp$(DependSuffix) -MM IsoRealms/Options/Options.cpp

$(IntermediateDirectory)/IsoRealms_Options_Options.cpp$(PreprocessSuffix): IsoRealms/Options/Options.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Options_Options.cpp$(PreprocessSuffix) IsoRealms/Options/Options.cpp

$(IntermediateDirectory)/IsoRealms_AnimatedFloat.cpp$(ObjectSuffix): IsoRealms/AnimatedFloat.cpp $(IntermediateDirectory)/IsoRealms_AnimatedFloat.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/AnimatedFloat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_AnimatedFloat.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_AnimatedFloat.cpp$(DependSuffix): IsoRealms/AnimatedFloat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_AnimatedFloat.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_AnimatedFloat.cpp$(DependSuffix) -MM IsoRealms/AnimatedFloat.cpp

$(IntermediateDirectory)/IsoRealms_AnimatedFloat.cpp$(PreprocessSuffix): IsoRealms/AnimatedFloat.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_AnimatedFloat.cpp$(PreprocessSuffix) IsoRealms/AnimatedFloat.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Spindizzy.cpp$(ObjectSuffix): Modules/Spindizzy/Spindizzy.cpp $(IntermediateDirectory)/Modules_Spindizzy_Spindizzy.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Spindizzy.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Spindizzy.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Spindizzy.cpp$(DependSuffix): Modules/Spindizzy/Spindizzy.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Spindizzy.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Spindizzy.cpp$(DependSuffix) -MM Modules/Spindizzy/Spindizzy.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Spindizzy.cpp$(PreprocessSuffix): Modules/Spindizzy/Spindizzy.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Spindizzy.cpp$(PreprocessSuffix) Modules/Spindizzy/Spindizzy.cpp

$(IntermediateDirectory)/IsoRealms_GUI_TextLabelComponent.cpp$(ObjectSuffix): IsoRealms/GUI/TextLabelComponent.cpp $(IntermediateDirectory)/IsoRealms_GUI_TextLabelComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/TextLabelComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_TextLabelComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_TextLabelComponent.cpp$(DependSuffix): IsoRealms/GUI/TextLabelComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_TextLabelComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_TextLabelComponent.cpp$(DependSuffix) -MM IsoRealms/GUI/TextLabelComponent.cpp

$(IntermediateDirectory)/IsoRealms_GUI_TextLabelComponent.cpp$(PreprocessSuffix): IsoRealms/GUI/TextLabelComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_TextLabelComponent.cpp$(PreprocessSuffix) IsoRealms/GUI/TextLabelComponent.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_AssetIDException.cpp$(ObjectSuffix): IsoRealms/Assets/Registry/AssetIDException.cpp $(IntermediateDirectory)/IsoRealms_Assets_Registry_AssetIDException.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Registry/AssetIDException.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Registry_AssetIDException.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Registry_AssetIDException.cpp$(DependSuffix): IsoRealms/Assets/Registry/AssetIDException.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Registry_AssetIDException.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Registry_AssetIDException.cpp$(DependSuffix) -MM IsoRealms/Assets/Registry/AssetIDException.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_AssetIDException.cpp$(PreprocessSuffix): IsoRealms/Assets/Registry/AssetIDException.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Registry_AssetIDException.cpp$(PreprocessSuffix) IsoRealms/Assets/Registry/AssetIDException.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeBoolean.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyNativeBoolean.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeBoolean.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyNativeBoolean.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeBoolean.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeBoolean.cpp$(DependSuffix): IsoRealms/ResourcePropertyNativeBoolean.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeBoolean.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeBoolean.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyNativeBoolean.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeBoolean.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyNativeBoolean.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeBoolean.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyNativeBoolean.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_IconElement.cpp$(ObjectSuffix): Modules/EditorOLD/DialogResourceElements/IconElement.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_IconElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogResourceElements/IconElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_IconElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_IconElement.cpp$(DependSuffix): Modules/EditorOLD/DialogResourceElements/IconElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_IconElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_IconElement.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogResourceElements/IconElement.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_IconElement.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogResourceElements/IconElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceElements_IconElement.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogResourceElements/IconElement.cpp

$(IntermediateDirectory)/Modules_Basics_Sequence_SequenceDialog.cpp$(ObjectSuffix): Modules/Basics/Sequence/SequenceDialog.cpp $(IntermediateDirectory)/Modules_Basics_Sequence_SequenceDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Sequence/SequenceDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Sequence_SequenceDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Sequence_SequenceDialog.cpp$(DependSuffix): Modules/Basics/Sequence/SequenceDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Sequence_SequenceDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Sequence_SequenceDialog.cpp$(DependSuffix) -MM Modules/Basics/Sequence/SequenceDialog.cpp

$(IntermediateDirectory)/Modules_Basics_Sequence_SequenceDialog.cpp$(PreprocessSuffix): Modules/Basics/Sequence/SequenceDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Sequence_SequenceDialog.cpp$(PreprocessSuffix) Modules/Basics/Sequence/SequenceDialog.cpp

$(IntermediateDirectory)/Modules_Basics_Timer_Timer.cpp$(ObjectSuffix): Modules/Basics/Timer/Timer.cpp $(IntermediateDirectory)/Modules_Basics_Timer_Timer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Timer/Timer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Timer_Timer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Timer_Timer.cpp$(DependSuffix): Modules/Basics/Timer/Timer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Timer_Timer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Timer_Timer.cpp$(DependSuffix) -MM Modules/Basics/Timer/Timer.cpp

$(IntermediateDirectory)/Modules_Basics_Timer_Timer.cpp$(PreprocessSuffix): Modules/Basics/Timer/Timer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Timer_Timer.cpp$(PreprocessSuffix) Modules/Basics/Timer/Timer.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_HUDComponent.cpp$(ObjectSuffix): Modules/Basics/HUD/HUDComponent.cpp $(IntermediateDirectory)/Modules_Basics_HUD_HUDComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/HUD/HUDComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_HUD_HUDComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_HUD_HUDComponent.cpp$(DependSuffix): Modules/Basics/HUD/HUDComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_HUD_HUDComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_HUD_HUDComponent.cpp$(DependSuffix) -MM Modules/Basics/HUD/HUDComponent.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_HUDComponent.cpp$(PreprocessSuffix): Modules/Basics/HUD/HUDComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_HUD_HUDComponent.cpp$(PreprocessSuffix) Modules/Basics/HUD/HUDComponent.cpp

$(IntermediateDirectory)/IsoRealms.cpp$(ObjectSuffix): IsoRealms.cpp $(IntermediateDirectory)/IsoRealms.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms.cpp$(DependSuffix): IsoRealms.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms.cpp$(DependSuffix) -MM IsoRealms.cpp

$(IntermediateDirectory)/IsoRealms.cpp$(PreprocessSuffix): IsoRealms.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms.cpp$(PreprocessSuffix) IsoRealms.cpp

$(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(ObjectSuffix): CMakeFiles/3.16.3/CompilerIdC/CMakeCCompilerId.c $(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/martin/spindizzyremake/CMakeFiles/3.16.3/CompilerIdC/CMakeCCompilerId.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(DependSuffix): CMakeFiles/3.16.3/CompilerIdC/CMakeCCompilerId.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(DependSuffix) -MM CMakeFiles/3.16.3/CompilerIdC/CMakeCCompilerId.c

$(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(PreprocessSuffix): CMakeFiles/3.16.3/CompilerIdC/CMakeCCompilerId.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdC_CMakeCCompilerId.c$(PreprocessSuffix) CMakeFiles/3.16.3/CompilerIdC/CMakeCCompilerId.c

$(IntermediateDirectory)/IsoRealms_GUI_GUIIcon.cpp$(ObjectSuffix): IsoRealms/GUI/GUIIcon.cpp $(IntermediateDirectory)/IsoRealms_GUI_GUIIcon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/GUIIcon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_GUIIcon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_GUIIcon.cpp$(DependSuffix): IsoRealms/GUI/GUIIcon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_GUIIcon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_GUIIcon.cpp$(DependSuffix) -MM IsoRealms/GUI/GUIIcon.cpp

$(IntermediateDirectory)/IsoRealms_GUI_GUIIcon.cpp$(PreprocessSuffix): IsoRealms/GUI/GUIIcon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_GUIIcon.cpp$(PreprocessSuffix) IsoRealms/GUI/GUIIcon.cpp

$(IntermediateDirectory)/IsoRealms_LocalAssetRegistry.cpp$(ObjectSuffix): IsoRealms/LocalAssetRegistry.cpp $(IntermediateDirectory)/IsoRealms_LocalAssetRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/LocalAssetRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_LocalAssetRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_LocalAssetRegistry.cpp$(DependSuffix): IsoRealms/LocalAssetRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_LocalAssetRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_LocalAssetRegistry.cpp$(DependSuffix) -MM IsoRealms/LocalAssetRegistry.cpp

$(IntermediateDirectory)/IsoRealms_LocalAssetRegistry.cpp$(PreprocessSuffix): IsoRealms/LocalAssetRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_LocalAssetRegistry.cpp$(PreprocessSuffix) IsoRealms/LocalAssetRegistry.cpp

$(IntermediateDirectory)/IsoRealms_InitException.cpp$(ObjectSuffix): IsoRealms/InitException.cpp $(IntermediateDirectory)/IsoRealms_InitException.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/InitException.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_InitException.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_InitException.cpp$(DependSuffix): IsoRealms/InitException.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_InitException.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_InitException.cpp$(DependSuffix) -MM IsoRealms/InitException.cpp

$(IntermediateDirectory)/IsoRealms_InitException.cpp$(PreprocessSuffix): IsoRealms/InitException.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_InitException.cpp$(PreprocessSuffix) IsoRealms/InitException.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_HUD.cpp$(ObjectSuffix): Modules/Basics/HUD/HUD.cpp $(IntermediateDirectory)/Modules_Basics_HUD_HUD.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/HUD/HUD.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_HUD_HUD.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_HUD_HUD.cpp$(DependSuffix): Modules/Basics/HUD/HUD.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_HUD_HUD.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_HUD_HUD.cpp$(DependSuffix) -MM Modules/Basics/HUD/HUD.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_HUD.cpp$(PreprocessSuffix): Modules/Basics/HUD/HUD.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_HUD_HUD.cpp$(PreprocessSuffix) Modules/Basics/HUD/HUD.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyInteger.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyInteger.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyInteger.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyInteger.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyInteger.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyInteger.cpp$(DependSuffix): IsoRealms/ResourcePropertyInteger.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyInteger.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyInteger.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyInteger.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyInteger.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyInteger.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyInteger.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyInteger.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyAction.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyAction.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyAction.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyAction.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyAction.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyAction.cpp$(DependSuffix): IsoRealms/ResourcePropertyAction.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyAction.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyAction.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyAction.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyAction.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyAction.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyAction.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyAction.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyInputField.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyInputField.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyInputField.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyInputField.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyInputField.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyInputField.cpp$(DependSuffix): IsoRealms/ResourcePropertyInputField.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyInputField.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyInputField.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyInputField.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyInputField.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyInputField.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyInputField.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyInputField.cpp

$(IntermediateDirectory)/Modules_HighScore_ModuleHighScore.cpp$(ObjectSuffix): Modules/HighScore/ModuleHighScore.cpp $(IntermediateDirectory)/Modules_HighScore_ModuleHighScore.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/HighScore/ModuleHighScore.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_HighScore_ModuleHighScore.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_HighScore_ModuleHighScore.cpp$(DependSuffix): Modules/HighScore/ModuleHighScore.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_HighScore_ModuleHighScore.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_HighScore_ModuleHighScore.cpp$(DependSuffix) -MM Modules/HighScore/ModuleHighScore.cpp

$(IntermediateDirectory)/Modules_HighScore_ModuleHighScore.cpp$(PreprocessSuffix): Modules/HighScore/ModuleHighScore.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_HighScore_ModuleHighScore.cpp$(PreprocessSuffix) Modules/HighScore/ModuleHighScore.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModel.cpp$(ObjectSuffix): IsoRealms/GUI/Icons/ComponentIconModel.cpp $(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/Icons/ComponentIconModel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModel.cpp$(DependSuffix): IsoRealms/GUI/Icons/ComponentIconModel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModel.cpp$(DependSuffix) -MM IsoRealms/GUI/Icons/ComponentIconModel.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModel.cpp$(PreprocessSuffix): IsoRealms/GUI/Icons/ComponentIconModel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModel.cpp$(PreprocessSuffix) IsoRealms/GUI/Icons/ComponentIconModel.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ComponentTable.cpp$(ObjectSuffix): IsoRealms/GUI/ComponentTable.cpp $(IntermediateDirectory)/IsoRealms_GUI_ComponentTable.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/ComponentTable.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_ComponentTable.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_ComponentTable.cpp$(DependSuffix): IsoRealms/GUI/ComponentTable.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_ComponentTable.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_ComponentTable.cpp$(DependSuffix) -MM IsoRealms/GUI/ComponentTable.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ComponentTable.cpp$(PreprocessSuffix): IsoRealms/GUI/ComponentTable.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_ComponentTable.cpp$(PreprocessSuffix) IsoRealms/GUI/ComponentTable.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyColour.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyColour.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyColour.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyColour.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyColour.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyColour.cpp$(DependSuffix): IsoRealms/ResourcePropertyColour.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyColour.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyColour.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyColour.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyColour.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyColour.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyColour.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyColour.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_InputHandlerRegistry.cpp$(ObjectSuffix): IsoRealms/Assets/Registry/InputHandlerRegistry.cpp $(IntermediateDirectory)/IsoRealms_Assets_Registry_InputHandlerRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Registry/InputHandlerRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Registry_InputHandlerRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Registry_InputHandlerRegistry.cpp$(DependSuffix): IsoRealms/Assets/Registry/InputHandlerRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Registry_InputHandlerRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Registry_InputHandlerRegistry.cpp$(DependSuffix) -MM IsoRealms/Assets/Registry/InputHandlerRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_InputHandlerRegistry.cpp$(PreprocessSuffix): IsoRealms/Assets/Registry/InputHandlerRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Registry_InputHandlerRegistry.cpp$(PreprocessSuffix) IsoRealms/Assets/Registry/InputHandlerRegistry.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Player_PlayerType.cpp$(ObjectSuffix): Modules/Spindizzy/Player/PlayerType.cpp $(IntermediateDirectory)/Modules_Spindizzy_Player_PlayerType.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Player/PlayerType.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Player_PlayerType.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Player_PlayerType.cpp$(DependSuffix): Modules/Spindizzy/Player/PlayerType.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Player_PlayerType.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Player_PlayerType.cpp$(DependSuffix) -MM Modules/Spindizzy/Player/PlayerType.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Player_PlayerType.cpp$(PreprocessSuffix): Modules/Spindizzy/Player/PlayerType.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Player_PlayerType.cpp$(PreprocessSuffix) Modules/Spindizzy/Player/PlayerType.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconTexture.cpp$(ObjectSuffix): IsoRealms/GUI/Icons/ComponentIconTexture.cpp $(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconTexture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/Icons/ComponentIconTexture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconTexture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconTexture.cpp$(DependSuffix): IsoRealms/GUI/Icons/ComponentIconTexture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconTexture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconTexture.cpp$(DependSuffix) -MM IsoRealms/GUI/Icons/ComponentIconTexture.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconTexture.cpp$(PreprocessSuffix): IsoRealms/GUI/Icons/ComponentIconTexture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconTexture.cpp$(PreprocessSuffix) IsoRealms/GUI/Icons/ComponentIconTexture.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ComponentTree.cpp$(ObjectSuffix): IsoRealms/GUI/ComponentTree.cpp $(IntermediateDirectory)/IsoRealms_GUI_ComponentTree.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/ComponentTree.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_ComponentTree.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_ComponentTree.cpp$(DependSuffix): IsoRealms/GUI/ComponentTree.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_ComponentTree.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_ComponentTree.cpp$(DependSuffix) -MM IsoRealms/GUI/ComponentTree.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ComponentTree.cpp$(PreprocessSuffix): IsoRealms/GUI/ComponentTree.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_ComponentTree.cpp$(PreprocessSuffix) IsoRealms/GUI/ComponentTree.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyFloat.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyFloat.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyFloat.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyFloat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyFloat.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyFloat.cpp$(DependSuffix): IsoRealms/ResourcePropertyFloat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyFloat.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyFloat.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyFloat.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyFloat.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyFloat.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyFloat.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyFloat.cpp

$(IntermediateDirectory)/IsoRealms_GUI_FixedSizeLayout.cpp$(ObjectSuffix): IsoRealms/GUI/FixedSizeLayout.cpp $(IntermediateDirectory)/IsoRealms_GUI_FixedSizeLayout.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/FixedSizeLayout.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_FixedSizeLayout.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_FixedSizeLayout.cpp$(DependSuffix): IsoRealms/GUI/FixedSizeLayout.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_FixedSizeLayout.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_FixedSizeLayout.cpp$(DependSuffix) -MM IsoRealms/GUI/FixedSizeLayout.cpp

$(IntermediateDirectory)/IsoRealms_GUI_FixedSizeLayout.cpp$(PreprocessSuffix): IsoRealms/GUI/FixedSizeLayout.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_FixedSizeLayout.cpp$(PreprocessSuffix) IsoRealms/GUI/FixedSizeLayout.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertySlider.cpp$(ObjectSuffix): IsoRealms/ResourcePropertySlider.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertySlider.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertySlider.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertySlider.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertySlider.cpp$(DependSuffix): IsoRealms/ResourcePropertySlider.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertySlider.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertySlider.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertySlider.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertySlider.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertySlider.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertySlider.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertySlider.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyScreen.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyScreen.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyScreen.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyScreen.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyScreen.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyScreen.cpp$(DependSuffix): IsoRealms/ResourcePropertyScreen.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyScreen.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyScreen.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyScreen.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyScreen.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyScreen.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyScreen.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyScreen.cpp

$(IntermediateDirectory)/IsoRealms_GUI_RectangleBoundsCalculator.cpp$(ObjectSuffix): IsoRealms/GUI/RectangleBoundsCalculator.cpp $(IntermediateDirectory)/IsoRealms_GUI_RectangleBoundsCalculator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/RectangleBoundsCalculator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_RectangleBoundsCalculator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_RectangleBoundsCalculator.cpp$(DependSuffix): IsoRealms/GUI/RectangleBoundsCalculator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_RectangleBoundsCalculator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_RectangleBoundsCalculator.cpp$(DependSuffix) -MM IsoRealms/GUI/RectangleBoundsCalculator.cpp

$(IntermediateDirectory)/IsoRealms_GUI_RectangleBoundsCalculator.cpp$(PreprocessSuffix): IsoRealms/GUI/RectangleBoundsCalculator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_RectangleBoundsCalculator.cpp$(PreprocessSuffix) IsoRealms/GUI/RectangleBoundsCalculator.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Popup.cpp$(ObjectSuffix): IsoRealms/GUI/Popup.cpp $(IntermediateDirectory)/IsoRealms_GUI_Popup.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/Popup.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_Popup.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_Popup.cpp$(DependSuffix): IsoRealms/GUI/Popup.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_Popup.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_Popup.cpp$(DependSuffix) -MM IsoRealms/GUI/Popup.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Popup.cpp$(PreprocessSuffix): IsoRealms/GUI/Popup.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_Popup.cpp$(PreprocessSuffix) IsoRealms/GUI/Popup.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeString.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyNativeString.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeString.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyNativeString.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeString.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeString.cpp$(DependSuffix): IsoRealms/ResourcePropertyNativeString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeString.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeString.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyNativeString.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeString.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyNativeString.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyNativeString.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyNativeString.cpp

$(IntermediateDirectory)/Modules_Basics_BasicsScriptSupport.cpp$(ObjectSuffix): Modules/Basics/BasicsScriptSupport.cpp $(IntermediateDirectory)/Modules_Basics_BasicsScriptSupport.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/BasicsScriptSupport.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_BasicsScriptSupport.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_BasicsScriptSupport.cpp$(DependSuffix): Modules/Basics/BasicsScriptSupport.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_BasicsScriptSupport.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_BasicsScriptSupport.cpp$(DependSuffix) -MM Modules/Basics/BasicsScriptSupport.cpp

$(IntermediateDirectory)/Modules_Basics_BasicsScriptSupport.cpp$(PreprocessSuffix): Modules/Basics/BasicsScriptSupport.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_BasicsScriptSupport.cpp$(PreprocessSuffix) Modules/Basics/BasicsScriptSupport.cpp

$(IntermediateDirectory)/IsoRealms_AssetRegistryRegistry.cpp$(ObjectSuffix): IsoRealms/AssetRegistryRegistry.cpp $(IntermediateDirectory)/IsoRealms_AssetRegistryRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/AssetRegistryRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_AssetRegistryRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_AssetRegistryRegistry.cpp$(DependSuffix): IsoRealms/AssetRegistryRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_AssetRegistryRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_AssetRegistryRegistry.cpp$(DependSuffix) -MM IsoRealms/AssetRegistryRegistry.cpp

$(IntermediateDirectory)/IsoRealms_AssetRegistryRegistry.cpp$(PreprocessSuffix): IsoRealms/AssetRegistryRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_AssetRegistryRegistry.cpp$(PreprocessSuffix) IsoRealms/AssetRegistryRegistry.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ComponentDigitalInputMapping.cpp$(ObjectSuffix): IsoRealms/GUI/ComponentDigitalInputMapping.cpp $(IntermediateDirectory)/IsoRealms_GUI_ComponentDigitalInputMapping.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/ComponentDigitalInputMapping.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_ComponentDigitalInputMapping.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_ComponentDigitalInputMapping.cpp$(DependSuffix): IsoRealms/GUI/ComponentDigitalInputMapping.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_ComponentDigitalInputMapping.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_ComponentDigitalInputMapping.cpp$(DependSuffix) -MM IsoRealms/GUI/ComponentDigitalInputMapping.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ComponentDigitalInputMapping.cpp$(PreprocessSuffix): IsoRealms/GUI/ComponentDigitalInputMapping.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_ComponentDigitalInputMapping.cpp$(PreprocessSuffix) IsoRealms/GUI/ComponentDigitalInputMapping.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogColourSelector.cpp$(ObjectSuffix): IsoRealms/GUI/Dialogs/DialogColourSelector.cpp $(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogColourSelector.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/Dialogs/DialogColourSelector.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogColourSelector.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogColourSelector.cpp$(DependSuffix): IsoRealms/GUI/Dialogs/DialogColourSelector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogColourSelector.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogColourSelector.cpp$(DependSuffix) -MM IsoRealms/GUI/Dialogs/DialogColourSelector.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogColourSelector.cpp$(PreprocessSuffix): IsoRealms/GUI/Dialogs/DialogColourSelector.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogColourSelector.cpp$(PreprocessSuffix) IsoRealms/GUI/Dialogs/DialogColourSelector.cpp

$(IntermediateDirectory)/Modules_Basics_InputSwitch_InputSwitch.cpp$(ObjectSuffix): Modules/Basics/InputSwitch/InputSwitch.cpp $(IntermediateDirectory)/Modules_Basics_InputSwitch_InputSwitch.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/InputSwitch/InputSwitch.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_InputSwitch_InputSwitch.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_InputSwitch_InputSwitch.cpp$(DependSuffix): Modules/Basics/InputSwitch/InputSwitch.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_InputSwitch_InputSwitch.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_InputSwitch_InputSwitch.cpp$(DependSuffix) -MM Modules/Basics/InputSwitch/InputSwitch.cpp

$(IntermediateDirectory)/Modules_Basics_InputSwitch_InputSwitch.cpp$(PreprocessSuffix): Modules/Basics/InputSwitch/InputSwitch.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_InputSwitch_InputSwitch.cpp$(PreprocessSuffix) Modules/Basics/InputSwitch/InputSwitch.cpp

$(IntermediateDirectory)/Modules_Basics_ColourCycler_ColourCycler.cpp$(ObjectSuffix): Modules/Basics/ColourCycler/ColourCycler.cpp $(IntermediateDirectory)/Modules_Basics_ColourCycler_ColourCycler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/ColourCycler/ColourCycler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_ColourCycler_ColourCycler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_ColourCycler_ColourCycler.cpp$(DependSuffix): Modules/Basics/ColourCycler/ColourCycler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_ColourCycler_ColourCycler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_ColourCycler_ColourCycler.cpp$(DependSuffix) -MM Modules/Basics/ColourCycler/ColourCycler.cpp

$(IntermediateDirectory)/Modules_Basics_ColourCycler_ColourCycler.cpp$(PreprocessSuffix): Modules/Basics/ColourCycler/ColourCycler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_ColourCycler_ColourCycler.cpp$(PreprocessSuffix) Modules/Basics/ColourCycler/ColourCycler.cpp

$(IntermediateDirectory)/IsoRealms_ResourceType.cpp$(ObjectSuffix): IsoRealms/ResourceType.cpp $(IntermediateDirectory)/IsoRealms_ResourceType.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourceType.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourceType.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourceType.cpp$(DependSuffix): IsoRealms/ResourceType.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourceType.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourceType.cpp$(DependSuffix) -MM IsoRealms/ResourceType.cpp

$(IntermediateDirectory)/IsoRealms_ResourceType.cpp$(PreprocessSuffix): IsoRealms/ResourceType.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourceType.cpp$(PreprocessSuffix) IsoRealms/ResourceType.cpp

$(IntermediateDirectory)/IsoRealms_Utils.cpp$(ObjectSuffix): IsoRealms/Utils.cpp $(IntermediateDirectory)/IsoRealms_Utils.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Utils.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Utils.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Utils.cpp$(DependSuffix): IsoRealms/Utils.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Utils.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Utils.cpp$(DependSuffix) -MM IsoRealms/Utils.cpp

$(IntermediateDirectory)/IsoRealms_Utils.cpp$(PreprocessSuffix): IsoRealms/Utils.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Utils.cpp$(PreprocessSuffix) IsoRealms/Utils.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCap.cpp$(ObjectSuffix): Modules/Spindizzy/Terrain/WallPatternCap.cpp $(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Terrain/WallPatternCap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCap.cpp$(DependSuffix): Modules/Spindizzy/Terrain/WallPatternCap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCap.cpp$(DependSuffix) -MM Modules/Spindizzy/Terrain/WallPatternCap.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCap.cpp$(PreprocessSuffix): Modules/Spindizzy/Terrain/WallPatternCap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallPatternCap.cpp$(PreprocessSuffix) Modules/Spindizzy/Terrain/WallPatternCap.cpp

$(IntermediateDirectory)/IsoRealms_MultipleClickDetector.cpp$(ObjectSuffix): IsoRealms/MultipleClickDetector.cpp $(IntermediateDirectory)/IsoRealms_MultipleClickDetector.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/MultipleClickDetector.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_MultipleClickDetector.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_MultipleClickDetector.cpp$(DependSuffix): IsoRealms/MultipleClickDetector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_MultipleClickDetector.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_MultipleClickDetector.cpp$(DependSuffix) -MM IsoRealms/MultipleClickDetector.cpp

$(IntermediateDirectory)/IsoRealms_MultipleClickDetector.cpp$(PreprocessSuffix): IsoRealms/MultipleClickDetector.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_MultipleClickDetector.cpp$(PreprocessSuffix) IsoRealms/MultipleClickDetector.cpp

$(IntermediateDirectory)/Modules_Basics_InterruptHandler_InterruptHandler.cpp$(ObjectSuffix): Modules/Basics/InterruptHandler/InterruptHandler.cpp $(IntermediateDirectory)/Modules_Basics_InterruptHandler_InterruptHandler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/InterruptHandler/InterruptHandler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_InterruptHandler_InterruptHandler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_InterruptHandler_InterruptHandler.cpp$(DependSuffix): Modules/Basics/InterruptHandler/InterruptHandler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_InterruptHandler_InterruptHandler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_InterruptHandler_InterruptHandler.cpp$(DependSuffix) -MM Modules/Basics/InterruptHandler/InterruptHandler.cpp

$(IntermediateDirectory)/Modules_Basics_InterruptHandler_InterruptHandler.cpp$(PreprocessSuffix): Modules/Basics/InterruptHandler/InterruptHandler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_InterruptHandler_InterruptHandler.cpp$(PreprocessSuffix) Modules/Basics/InterruptHandler/InterruptHandler.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourLightness.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyColourLightness.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyColourLightness.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyColourLightness.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourLightness.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourLightness.cpp$(DependSuffix): IsoRealms/ResourcePropertyColourLightness.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourLightness.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourLightness.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyColourLightness.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourLightness.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyColourLightness.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyColourLightness.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyColourLightness.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourHue.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyColourHue.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyColourHue.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyColourHue.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourHue.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourHue.cpp$(DependSuffix): IsoRealms/ResourcePropertyColourHue.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourHue.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourHue.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyColourHue.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourHue.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyColourHue.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyColourHue.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyColourHue.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyModel.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyModel.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyModel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyModel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyModel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyModel.cpp$(DependSuffix): IsoRealms/ResourcePropertyModel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyModel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyModel.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyModel.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyModel.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyModel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyModel.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyModel.cpp

$(IntermediateDirectory)/IsoRealms_ResourceElement_ElementBounds.cpp$(ObjectSuffix): IsoRealms/ResourceElement/ElementBounds.cpp $(IntermediateDirectory)/IsoRealms_ResourceElement_ElementBounds.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourceElement/ElementBounds.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourceElement_ElementBounds.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourceElement_ElementBounds.cpp$(DependSuffix): IsoRealms/ResourceElement/ElementBounds.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourceElement_ElementBounds.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourceElement_ElementBounds.cpp$(DependSuffix) -MM IsoRealms/ResourceElement/ElementBounds.cpp

$(IntermediateDirectory)/IsoRealms_ResourceElement_ElementBounds.cpp$(PreprocessSuffix): IsoRealms/ResourceElement/ElementBounds.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourceElement_ElementBounds.cpp$(PreprocessSuffix) IsoRealms/ResourceElement/ElementBounds.cpp

$(IntermediateDirectory)/IsoRealms_Rectangle.cpp$(ObjectSuffix): IsoRealms/Rectangle.cpp $(IntermediateDirectory)/IsoRealms_Rectangle.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Rectangle.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Rectangle.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Rectangle.cpp$(DependSuffix): IsoRealms/Rectangle.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Rectangle.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Rectangle.cpp$(DependSuffix) -MM IsoRealms/Rectangle.cpp

$(IntermediateDirectory)/IsoRealms_Rectangle.cpp$(PreprocessSuffix): IsoRealms/Rectangle.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Rectangle.cpp$(PreprocessSuffix) IsoRealms/Rectangle.cpp

$(IntermediateDirectory)/IsoRealms_Common_ScreenArea.cpp$(ObjectSuffix): IsoRealms/Common/ScreenArea.cpp $(IntermediateDirectory)/IsoRealms_Common_ScreenArea.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Common/ScreenArea.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Common_ScreenArea.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Common_ScreenArea.cpp$(DependSuffix): IsoRealms/Common/ScreenArea.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Common_ScreenArea.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Common_ScreenArea.cpp$(DependSuffix) -MM IsoRealms/Common/ScreenArea.cpp

$(IntermediateDirectory)/IsoRealms_Common_ScreenArea.cpp$(PreprocessSuffix): IsoRealms/Common/ScreenArea.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Common_ScreenArea.cpp$(PreprocessSuffix) IsoRealms/Common/ScreenArea.cpp

$(IntermediateDirectory)/IsoRealms_Input_ButtonMapping.cpp$(ObjectSuffix): IsoRealms/Input/ButtonMapping.cpp $(IntermediateDirectory)/IsoRealms_Input_ButtonMapping.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Input/ButtonMapping.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Input_ButtonMapping.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Input_ButtonMapping.cpp$(DependSuffix): IsoRealms/Input/ButtonMapping.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Input_ButtonMapping.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Input_ButtonMapping.cpp$(DependSuffix) -MM IsoRealms/Input/ButtonMapping.cpp

$(IntermediateDirectory)/IsoRealms_Input_ButtonMapping.cpp$(PreprocessSuffix): IsoRealms/Input/ButtonMapping.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Input_ButtonMapping.cpp$(PreprocessSuffix) IsoRealms/Input/ButtonMapping.cpp

$(IntermediateDirectory)/IsoRealms_ResourceInitException.cpp$(ObjectSuffix): IsoRealms/ResourceInitException.cpp $(IntermediateDirectory)/IsoRealms_ResourceInitException.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourceInitException.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourceInitException.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourceInitException.cpp$(DependSuffix): IsoRealms/ResourceInitException.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourceInitException.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourceInitException.cpp$(DependSuffix) -MM IsoRealms/ResourceInitException.cpp

$(IntermediateDirectory)/IsoRealms_ResourceInitException.cpp$(PreprocessSuffix): IsoRealms/ResourceInitException.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourceInitException.cpp$(PreprocessSuffix) IsoRealms/ResourceInitException.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialog.cpp$(ObjectSuffix): Modules/Spindizzy/ThemeSet/ThemeSelectionDialog.cpp $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/ThemeSet/ThemeSelectionDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialog.cpp$(DependSuffix): Modules/Spindizzy/ThemeSet/ThemeSelectionDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialog.cpp$(DependSuffix) -MM Modules/Spindizzy/ThemeSet/ThemeSelectionDialog.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialog.cpp$(PreprocessSuffix): Modules/Spindizzy/ThemeSet/ThemeSelectionDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeSelectionDialog.cpp$(PreprocessSuffix) Modules/Spindizzy/ThemeSet/ThemeSelectionDialog.cpp

$(IntermediateDirectory)/IsoRealms_GUI_WrappingGridComponent.cpp$(ObjectSuffix): IsoRealms/GUI/WrappingGridComponent.cpp $(IntermediateDirectory)/IsoRealms_GUI_WrappingGridComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/WrappingGridComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_WrappingGridComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_WrappingGridComponent.cpp$(DependSuffix): IsoRealms/GUI/WrappingGridComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_WrappingGridComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_WrappingGridComponent.cpp$(DependSuffix) -MM IsoRealms/GUI/WrappingGridComponent.cpp

$(IntermediateDirectory)/IsoRealms_GUI_WrappingGridComponent.cpp$(PreprocessSuffix): IsoRealms/GUI/WrappingGridComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_WrappingGridComponent.cpp$(PreprocessSuffix) IsoRealms/GUI/WrappingGridComponent.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyBoolean.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyBoolean.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyBoolean.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyBoolean.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyBoolean.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyBoolean.cpp$(DependSuffix): IsoRealms/ResourcePropertyBoolean.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyBoolean.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyBoolean.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyBoolean.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyBoolean.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyBoolean.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyBoolean.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyBoolean.cpp

$(IntermediateDirectory)/Modules_Spindizzy_World_WorldEditor.cpp$(ObjectSuffix): Modules/Spindizzy/World/WorldEditor.cpp $(IntermediateDirectory)/Modules_Spindizzy_World_WorldEditor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/World/WorldEditor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_World_WorldEditor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_World_WorldEditor.cpp$(DependSuffix): Modules/Spindizzy/World/WorldEditor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_World_WorldEditor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_World_WorldEditor.cpp$(DependSuffix) -MM Modules/Spindizzy/World/WorldEditor.cpp

$(IntermediateDirectory)/Modules_Spindizzy_World_WorldEditor.cpp$(PreprocessSuffix): Modules/Spindizzy/World/WorldEditor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_World_WorldEditor.cpp$(PreprocessSuffix) Modules/Spindizzy/World/WorldEditor.cpp

$(IntermediateDirectory)/IsoRealms_GUI_IntegerField.cpp$(ObjectSuffix): IsoRealms/GUI/IntegerField.cpp $(IntermediateDirectory)/IsoRealms_GUI_IntegerField.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/IntegerField.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_IntegerField.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_IntegerField.cpp$(DependSuffix): IsoRealms/GUI/IntegerField.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_IntegerField.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_IntegerField.cpp$(DependSuffix) -MM IsoRealms/GUI/IntegerField.cpp

$(IntermediateDirectory)/IsoRealms_GUI_IntegerField.cpp$(PreprocessSuffix): IsoRealms/GUI/IntegerField.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_IntegerField.cpp$(PreprocessSuffix) IsoRealms/GUI/IntegerField.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyInputHandler.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyInputHandler.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyInputHandler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyInputHandler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyInputHandler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyInputHandler.cpp$(DependSuffix): IsoRealms/ResourcePropertyInputHandler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyInputHandler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyInputHandler.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyInputHandler.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyInputHandler.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyInputHandler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyInputHandler.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyInputHandler.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_HUDEditor.cpp$(ObjectSuffix): Modules/Basics/HUD/HUDEditor.cpp $(IntermediateDirectory)/Modules_Basics_HUD_HUDEditor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/HUD/HUDEditor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_HUD_HUDEditor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_HUD_HUDEditor.cpp$(DependSuffix): Modules/Basics/HUD/HUDEditor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_HUD_HUDEditor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_HUD_HUDEditor.cpp$(DependSuffix) -MM Modules/Basics/HUD/HUDEditor.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_HUDEditor.cpp$(PreprocessSuffix): Modules/Basics/HUD/HUDEditor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_HUD_HUDEditor.cpp$(PreprocessSuffix) Modules/Basics/HUD/HUDEditor.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_HUDComponentRelation.cpp$(ObjectSuffix): Modules/Basics/HUD/HUDComponentRelation.cpp $(IntermediateDirectory)/Modules_Basics_HUD_HUDComponentRelation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/HUD/HUDComponentRelation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_HUD_HUDComponentRelation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_HUD_HUDComponentRelation.cpp$(DependSuffix): Modules/Basics/HUD/HUDComponentRelation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_HUD_HUDComponentRelation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_HUD_HUDComponentRelation.cpp$(DependSuffix) -MM Modules/Basics/HUD/HUDComponentRelation.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_HUDComponentRelation.cpp$(PreprocessSuffix): Modules/Basics/HUD/HUDComponentRelation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_HUD_HUDComponentRelation.cpp$(PreprocessSuffix) Modules/Basics/HUD/HUDComponentRelation.cpp

$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumnPossibility.cpp$(ObjectSuffix): Modules/Spindizzy/GeometryProcessor/WallColumnPossibility.cpp $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumnPossibility.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/GeometryProcessor/WallColumnPossibility.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumnPossibility.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumnPossibility.cpp$(DependSuffix): Modules/Spindizzy/GeometryProcessor/WallColumnPossibility.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumnPossibility.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumnPossibility.cpp$(DependSuffix) -MM Modules/Spindizzy/GeometryProcessor/WallColumnPossibility.cpp

$(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumnPossibility.cpp$(PreprocessSuffix): Modules/Spindizzy/GeometryProcessor/WallColumnPossibility.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_GeometryProcessor_WallColumnPossibility.cpp$(PreprocessSuffix) Modules/Spindizzy/GeometryProcessor/WallColumnPossibility.cpp

$(IntermediateDirectory)/IsoRealms_GUI_PanelComponent.cpp$(ObjectSuffix): IsoRealms/GUI/PanelComponent.cpp $(IntermediateDirectory)/IsoRealms_GUI_PanelComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/PanelComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_PanelComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_PanelComponent.cpp$(DependSuffix): IsoRealms/GUI/PanelComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_PanelComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_PanelComponent.cpp$(DependSuffix) -MM IsoRealms/GUI/PanelComponent.cpp

$(IntermediateDirectory)/IsoRealms_GUI_PanelComponent.cpp$(PreprocessSuffix): IsoRealms/GUI/PanelComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_PanelComponent.cpp$(PreprocessSuffix) IsoRealms/GUI/PanelComponent.cpp

$(IntermediateDirectory)/IsoRealms_GUI_TextFieldComponent.cpp$(ObjectSuffix): IsoRealms/GUI/TextFieldComponent.cpp $(IntermediateDirectory)/IsoRealms_GUI_TextFieldComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/TextFieldComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_TextFieldComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_TextFieldComponent.cpp$(DependSuffix): IsoRealms/GUI/TextFieldComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_TextFieldComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_TextFieldComponent.cpp$(DependSuffix) -MM IsoRealms/GUI/TextFieldComponent.cpp

$(IntermediateDirectory)/IsoRealms_GUI_TextFieldComponent.cpp$(PreprocessSuffix): IsoRealms/GUI/TextFieldComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_TextFieldComponent.cpp$(PreprocessSuffix) IsoRealms/GUI/TextFieldComponent.cpp

$(IntermediateDirectory)/IsoRealms_Persistence_DOMNode.cpp$(ObjectSuffix): IsoRealms/Persistence/DOMNode.cpp $(IntermediateDirectory)/IsoRealms_Persistence_DOMNode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Persistence/DOMNode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Persistence_DOMNode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Persistence_DOMNode.cpp$(DependSuffix): IsoRealms/Persistence/DOMNode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Persistence_DOMNode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Persistence_DOMNode.cpp$(DependSuffix) -MM IsoRealms/Persistence/DOMNode.cpp

$(IntermediateDirectory)/IsoRealms_Persistence_DOMNode.cpp$(PreprocessSuffix): IsoRealms/Persistence/DOMNode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Persistence_DOMNode.cpp$(PreprocessSuffix) IsoRealms/Persistence/DOMNode.cpp

$(IntermediateDirectory)/IsoRealms_Persistence_DOMNodeWriter.cpp$(ObjectSuffix): IsoRealms/Persistence/DOMNodeWriter.cpp $(IntermediateDirectory)/IsoRealms_Persistence_DOMNodeWriter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Persistence/DOMNodeWriter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Persistence_DOMNodeWriter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Persistence_DOMNodeWriter.cpp$(DependSuffix): IsoRealms/Persistence/DOMNodeWriter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Persistence_DOMNodeWriter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Persistence_DOMNodeWriter.cpp$(DependSuffix) -MM IsoRealms/Persistence/DOMNodeWriter.cpp

$(IntermediateDirectory)/IsoRealms_Persistence_DOMNodeWriter.cpp$(PreprocessSuffix): IsoRealms/Persistence/DOMNodeWriter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Persistence_DOMNodeWriter.cpp$(PreprocessSuffix) IsoRealms/Persistence/DOMNodeWriter.cpp

$(IntermediateDirectory)/IsoRealms_Common_ScreenRay.cpp$(ObjectSuffix): IsoRealms/Common/ScreenRay.cpp $(IntermediateDirectory)/IsoRealms_Common_ScreenRay.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Common/ScreenRay.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Common_ScreenRay.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Common_ScreenRay.cpp$(DependSuffix): IsoRealms/Common/ScreenRay.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Common_ScreenRay.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Common_ScreenRay.cpp$(DependSuffix) -MM IsoRealms/Common/ScreenRay.cpp

$(IntermediateDirectory)/IsoRealms_Common_ScreenRay.cpp$(PreprocessSuffix): IsoRealms/Common/ScreenRay.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Common_ScreenRay.cpp$(PreprocessSuffix) IsoRealms/Common/ScreenRay.cpp

$(IntermediateDirectory)/IsoRealms_Common_BlockArea.cpp$(ObjectSuffix): IsoRealms/Common/BlockArea.cpp $(IntermediateDirectory)/IsoRealms_Common_BlockArea.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Common/BlockArea.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Common_BlockArea.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Common_BlockArea.cpp$(DependSuffix): IsoRealms/Common/BlockArea.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Common_BlockArea.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Common_BlockArea.cpp$(DependSuffix) -MM IsoRealms/Common/BlockArea.cpp

$(IntermediateDirectory)/IsoRealms_Common_BlockArea.cpp$(PreprocessSuffix): IsoRealms/Common/BlockArea.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Common_BlockArea.cpp$(PreprocessSuffix) IsoRealms/Common/BlockArea.cpp

$(IntermediateDirectory)/IsoRealms_Common_ScreenLocation.cpp$(ObjectSuffix): IsoRealms/Common/ScreenLocation.cpp $(IntermediateDirectory)/IsoRealms_Common_ScreenLocation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Common/ScreenLocation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Common_ScreenLocation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Common_ScreenLocation.cpp$(DependSuffix): IsoRealms/Common/ScreenLocation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Common_ScreenLocation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Common_ScreenLocation.cpp$(DependSuffix) -MM IsoRealms/Common/ScreenLocation.cpp

$(IntermediateDirectory)/IsoRealms_Common_ScreenLocation.cpp$(PreprocessSuffix): IsoRealms/Common/ScreenLocation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Common_ScreenLocation.cpp$(PreprocessSuffix) IsoRealms/Common/ScreenLocation.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColourIcon.cpp$(ObjectSuffix): Modules/Spindizzy/ThemeSet/ThemeColourIcon.cpp $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColourIcon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/ThemeSet/ThemeColourIcon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColourIcon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColourIcon.cpp$(DependSuffix): Modules/Spindizzy/ThemeSet/ThemeColourIcon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColourIcon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColourIcon.cpp$(DependSuffix) -MM Modules/Spindizzy/ThemeSet/ThemeColourIcon.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColourIcon.cpp$(PreprocessSuffix): Modules/Spindizzy/ThemeSet/ThemeColourIcon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeColourIcon.cpp$(PreprocessSuffix) Modules/Spindizzy/ThemeSet/ThemeColourIcon.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourChannel.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyColourChannel.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyColourChannel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyColourChannel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourChannel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourChannel.cpp$(DependSuffix): IsoRealms/ResourcePropertyColourChannel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourChannel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourChannel.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyColourChannel.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyColourChannel.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyColourChannel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyColourChannel.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyColourChannel.cpp

$(IntermediateDirectory)/IsoRealms_Condition_ConditionElement.cpp$(ObjectSuffix): IsoRealms/Condition/ConditionElement.cpp $(IntermediateDirectory)/IsoRealms_Condition_ConditionElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Condition/ConditionElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Condition_ConditionElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Condition_ConditionElement.cpp$(DependSuffix): IsoRealms/Condition/ConditionElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Condition_ConditionElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Condition_ConditionElement.cpp$(DependSuffix) -MM IsoRealms/Condition/ConditionElement.cpp

$(IntermediateDirectory)/IsoRealms_Condition_ConditionElement.cpp$(PreprocessSuffix): IsoRealms/Condition/ConditionElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Condition_ConditionElement.cpp$(PreprocessSuffix) IsoRealms/Condition/ConditionElement.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ModuleEditor.cpp$(ObjectSuffix): Modules/EditorOLD/ModuleEditor.cpp $(IntermediateDirectory)/Modules_EditorOLD_ModuleEditor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/ModuleEditor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_ModuleEditor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_ModuleEditor.cpp$(DependSuffix): Modules/EditorOLD/ModuleEditor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_ModuleEditor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_ModuleEditor.cpp$(DependSuffix) -MM Modules/EditorOLD/ModuleEditor.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ModuleEditor.cpp$(PreprocessSuffix): Modules/EditorOLD/ModuleEditor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_ModuleEditor.cpp$(PreprocessSuffix) Modules/EditorOLD/ModuleEditor.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_SpindizzyBlockConfigurationComponent.cpp$(ObjectSuffix): Modules/Spindizzy/Terrain/SpindizzyBlockConfigurationComponent.cpp $(IntermediateDirectory)/Modules_Spindizzy_Terrain_SpindizzyBlockConfigurationComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Terrain/SpindizzyBlockConfigurationComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Terrain_SpindizzyBlockConfigurationComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Terrain_SpindizzyBlockConfigurationComponent.cpp$(DependSuffix): Modules/Spindizzy/Terrain/SpindizzyBlockConfigurationComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Terrain_SpindizzyBlockConfigurationComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Terrain_SpindizzyBlockConfigurationComponent.cpp$(DependSuffix) -MM Modules/Spindizzy/Terrain/SpindizzyBlockConfigurationComponent.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_SpindizzyBlockConfigurationComponent.cpp$(PreprocessSuffix): Modules/Spindizzy/Terrain/SpindizzyBlockConfigurationComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_SpindizzyBlockConfigurationComponent.cpp$(PreprocessSuffix) Modules/Spindizzy/Terrain/SpindizzyBlockConfigurationComponent.cpp

$(IntermediateDirectory)/IsoRealms_Condition_Condition.cpp$(ObjectSuffix): IsoRealms/Condition/Condition.cpp $(IntermediateDirectory)/IsoRealms_Condition_Condition.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Condition/Condition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Condition_Condition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Condition_Condition.cpp$(DependSuffix): IsoRealms/Condition/Condition.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Condition_Condition.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Condition_Condition.cpp$(DependSuffix) -MM IsoRealms/Condition/Condition.cpp

$(IntermediateDirectory)/IsoRealms_Condition_Condition.cpp$(PreprocessSuffix): IsoRealms/Condition/Condition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Condition_Condition.cpp$(PreprocessSuffix) IsoRealms/Condition/Condition.cpp

$(IntermediateDirectory)/Modules_HighScore_ModuleHighScoreScriptSupport.cpp$(ObjectSuffix): Modules/HighScore/ModuleHighScoreScriptSupport.cpp $(IntermediateDirectory)/Modules_HighScore_ModuleHighScoreScriptSupport.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/HighScore/ModuleHighScoreScriptSupport.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_HighScore_ModuleHighScoreScriptSupport.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_HighScore_ModuleHighScoreScriptSupport.cpp$(DependSuffix): Modules/HighScore/ModuleHighScoreScriptSupport.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_HighScore_ModuleHighScoreScriptSupport.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_HighScore_ModuleHighScoreScriptSupport.cpp$(DependSuffix) -MM Modules/HighScore/ModuleHighScoreScriptSupport.cpp

$(IntermediateDirectory)/Modules_HighScore_ModuleHighScoreScriptSupport.cpp$(PreprocessSuffix): Modules/HighScore/ModuleHighScoreScriptSupport.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_HighScore_ModuleHighScoreScriptSupport.cpp$(PreprocessSuffix) Modules/HighScore/ModuleHighScoreScriptSupport.cpp

$(IntermediateDirectory)/IsoRealms_Properties_ConditionValueComponent.cpp$(ObjectSuffix): IsoRealms/Properties/ConditionValueComponent.cpp $(IntermediateDirectory)/IsoRealms_Properties_ConditionValueComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Properties/ConditionValueComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Properties_ConditionValueComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Properties_ConditionValueComponent.cpp$(DependSuffix): IsoRealms/Properties/ConditionValueComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Properties_ConditionValueComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Properties_ConditionValueComponent.cpp$(DependSuffix) -MM IsoRealms/Properties/ConditionValueComponent.cpp

$(IntermediateDirectory)/IsoRealms_Properties_ConditionValueComponent.cpp$(PreprocessSuffix): IsoRealms/Properties/ConditionValueComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Properties_ConditionValueComponent.cpp$(PreprocessSuffix) IsoRealms/Properties/ConditionValueComponent.cpp

$(IntermediateDirectory)/IsoRealms_Collision_CollisionData.cpp$(ObjectSuffix): IsoRealms/Collision/CollisionData.cpp $(IntermediateDirectory)/IsoRealms_Collision_CollisionData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Collision/CollisionData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Collision_CollisionData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Collision_CollisionData.cpp$(DependSuffix): IsoRealms/Collision/CollisionData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Collision_CollisionData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Collision_CollisionData.cpp$(DependSuffix) -MM IsoRealms/Collision/CollisionData.cpp

$(IntermediateDirectory)/IsoRealms_Collision_CollisionData.cpp$(PreprocessSuffix): IsoRealms/Collision/CollisionData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Collision_CollisionData.cpp$(PreprocessSuffix) IsoRealms/Collision/CollisionData.cpp

$(IntermediateDirectory)/IsoRealms_Collision_Collision.cpp$(ObjectSuffix): IsoRealms/Collision/Collision.cpp $(IntermediateDirectory)/IsoRealms_Collision_Collision.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Collision/Collision.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Collision_Collision.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Collision_Collision.cpp$(DependSuffix): IsoRealms/Collision/Collision.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Collision_Collision.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Collision_Collision.cpp$(DependSuffix) -MM IsoRealms/Collision/Collision.cpp

$(IntermediateDirectory)/IsoRealms_Collision_Collision.cpp$(PreprocessSuffix): IsoRealms/Collision/Collision.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Collision_Collision.cpp$(PreprocessSuffix) IsoRealms/Collision/Collision.cpp

$(IntermediateDirectory)/IsoRealms_LuaSupport_LuaScript.cpp$(ObjectSuffix): IsoRealms/LuaSupport/LuaScript.cpp $(IntermediateDirectory)/IsoRealms_LuaSupport_LuaScript.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/LuaSupport/LuaScript.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_LuaSupport_LuaScript.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_LuaSupport_LuaScript.cpp$(DependSuffix): IsoRealms/LuaSupport/LuaScript.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_LuaSupport_LuaScript.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_LuaSupport_LuaScript.cpp$(DependSuffix) -MM IsoRealms/LuaSupport/LuaScript.cpp

$(IntermediateDirectory)/IsoRealms_LuaSupport_LuaScript.cpp$(PreprocessSuffix): IsoRealms/LuaSupport/LuaScript.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_LuaSupport_LuaScript.cpp$(PreprocessSuffix) IsoRealms/LuaSupport/LuaScript.cpp

$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventFade.cpp$(ObjectSuffix): Modules/Basics/Sequence/ColourTrackEventFade.cpp $(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventFade.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Sequence/ColourTrackEventFade.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventFade.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventFade.cpp$(DependSuffix): Modules/Basics/Sequence/ColourTrackEventFade.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventFade.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventFade.cpp$(DependSuffix) -MM Modules/Basics/Sequence/ColourTrackEventFade.cpp

$(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventFade.cpp$(PreprocessSuffix): Modules/Basics/Sequence/ColourTrackEventFade.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Sequence_ColourTrackEventFade.cpp$(PreprocessSuffix) Modules/Basics/Sequence/ColourTrackEventFade.cpp

$(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentDefinition.cpp$(ObjectSuffix): IsoRealms/LuaSupport/ArgumentDefinition.cpp $(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentDefinition.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/LuaSupport/ArgumentDefinition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentDefinition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentDefinition.cpp$(DependSuffix): IsoRealms/LuaSupport/ArgumentDefinition.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentDefinition.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentDefinition.cpp$(DependSuffix) -MM IsoRealms/LuaSupport/ArgumentDefinition.cpp

$(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentDefinition.cpp$(PreprocessSuffix): IsoRealms/LuaSupport/ArgumentDefinition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentDefinition.cpp$(PreprocessSuffix) IsoRealms/LuaSupport/ArgumentDefinition.cpp

$(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentValueProxy.cpp$(ObjectSuffix): IsoRealms/LuaSupport/ArgumentValueProxy.cpp $(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentValueProxy.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/LuaSupport/ArgumentValueProxy.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentValueProxy.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentValueProxy.cpp$(DependSuffix): IsoRealms/LuaSupport/ArgumentValueProxy.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentValueProxy.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentValueProxy.cpp$(DependSuffix) -MM IsoRealms/LuaSupport/ArgumentValueProxy.cpp

$(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentValueProxy.cpp$(PreprocessSuffix): IsoRealms/LuaSupport/ArgumentValueProxy.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_LuaSupport_ArgumentValueProxy.cpp$(PreprocessSuffix) IsoRealms/LuaSupport/ArgumentValueProxy.cpp

$(IntermediateDirectory)/IsoRealms_Input_KeyMapping.cpp$(ObjectSuffix): IsoRealms/Input/KeyMapping.cpp $(IntermediateDirectory)/IsoRealms_Input_KeyMapping.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Input/KeyMapping.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Input_KeyMapping.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Input_KeyMapping.cpp$(DependSuffix): IsoRealms/Input/KeyMapping.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Input_KeyMapping.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Input_KeyMapping.cpp$(DependSuffix) -MM IsoRealms/Input/KeyMapping.cpp

$(IntermediateDirectory)/IsoRealms_Input_KeyMapping.cpp$(PreprocessSuffix): IsoRealms/Input/KeyMapping.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Input_KeyMapping.cpp$(PreprocessSuffix) IsoRealms/Input/KeyMapping.cpp

$(IntermediateDirectory)/IsoRealms_Properties_PropertyCondition.cpp$(ObjectSuffix): IsoRealms/Properties/PropertyCondition.cpp $(IntermediateDirectory)/IsoRealms_Properties_PropertyCondition.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Properties/PropertyCondition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Properties_PropertyCondition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Properties_PropertyCondition.cpp$(DependSuffix): IsoRealms/Properties/PropertyCondition.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Properties_PropertyCondition.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Properties_PropertyCondition.cpp$(DependSuffix) -MM IsoRealms/Properties/PropertyCondition.cpp

$(IntermediateDirectory)/IsoRealms_Properties_PropertyCondition.cpp$(PreprocessSuffix): IsoRealms/Properties/PropertyCondition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Properties_PropertyCondition.cpp$(PreprocessSuffix) IsoRealms/Properties/PropertyCondition.cpp

$(IntermediateDirectory)/Modules_Basics_Basics.cpp$(ObjectSuffix): Modules/Basics/Basics.cpp $(IntermediateDirectory)/Modules_Basics_Basics.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Basics.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Basics.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Basics.cpp$(DependSuffix): Modules/Basics/Basics.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Basics.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Basics.cpp$(DependSuffix) -MM Modules/Basics/Basics.cpp

$(IntermediateDirectory)/Modules_Basics_Basics.cpp$(PreprocessSuffix): Modules/Basics/Basics.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Basics.cpp$(PreprocessSuffix) Modules/Basics/Basics.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_StringRegistry.cpp$(ObjectSuffix): IsoRealms/Assets/Registry/StringRegistry.cpp $(IntermediateDirectory)/IsoRealms_Assets_Registry_StringRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Registry/StringRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Registry_StringRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Registry_StringRegistry.cpp$(DependSuffix): IsoRealms/Assets/Registry/StringRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Registry_StringRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Registry_StringRegistry.cpp$(DependSuffix) -MM IsoRealms/Assets/Registry/StringRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_StringRegistry.cpp$(PreprocessSuffix): IsoRealms/Assets/Registry/StringRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Registry_StringRegistry.cpp$(PreprocessSuffix) IsoRealms/Assets/Registry/StringRegistry.cpp

$(IntermediateDirectory)/Modules_Basics_ModelCycler_ModelCycler.cpp$(ObjectSuffix): Modules/Basics/ModelCycler/ModelCycler.cpp $(IntermediateDirectory)/Modules_Basics_ModelCycler_ModelCycler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/ModelCycler/ModelCycler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_ModelCycler_ModelCycler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_ModelCycler_ModelCycler.cpp$(DependSuffix): Modules/Basics/ModelCycler/ModelCycler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_ModelCycler_ModelCycler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_ModelCycler_ModelCycler.cpp$(DependSuffix) -MM Modules/Basics/ModelCycler/ModelCycler.cpp

$(IntermediateDirectory)/Modules_Basics_ModelCycler_ModelCycler.cpp$(PreprocessSuffix): Modules/Basics/ModelCycler/ModelCycler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_ModelCycler_ModelCycler.cpp$(PreprocessSuffix) Modules/Basics/ModelCycler/ModelCycler.cpp

$(IntermediateDirectory)/IsoRealms_GUI_SelectableComponent.cpp$(ObjectSuffix): IsoRealms/GUI/SelectableComponent.cpp $(IntermediateDirectory)/IsoRealms_GUI_SelectableComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/SelectableComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_SelectableComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_SelectableComponent.cpp$(DependSuffix): IsoRealms/GUI/SelectableComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_SelectableComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_SelectableComponent.cpp$(DependSuffix) -MM IsoRealms/GUI/SelectableComponent.cpp

$(IntermediateDirectory)/IsoRealms_GUI_SelectableComponent.cpp$(PreprocessSuffix): IsoRealms/GUI/SelectableComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_SelectableComponent.cpp$(PreprocessSuffix) IsoRealms/GUI/SelectableComponent.cpp

$(IntermediateDirectory)/Modules_Basics_ResourcePropertyHUDEditor.cpp$(ObjectSuffix): Modules/Basics/ResourcePropertyHUDEditor.cpp $(IntermediateDirectory)/Modules_Basics_ResourcePropertyHUDEditor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/ResourcePropertyHUDEditor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_ResourcePropertyHUDEditor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_ResourcePropertyHUDEditor.cpp$(DependSuffix): Modules/Basics/ResourcePropertyHUDEditor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_ResourcePropertyHUDEditor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_ResourcePropertyHUDEditor.cpp$(DependSuffix) -MM Modules/Basics/ResourcePropertyHUDEditor.cpp

$(IntermediateDirectory)/Modules_Basics_ResourcePropertyHUDEditor.cpp$(PreprocessSuffix): Modules/Basics/ResourcePropertyHUDEditor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_ResourcePropertyHUDEditor.cpp$(PreprocessSuffix) Modules/Basics/ResourcePropertyHUDEditor.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_Theme.cpp$(ObjectSuffix): Modules/Spindizzy/ThemeSet/Theme.cpp $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_Theme.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/ThemeSet/Theme.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_Theme.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_Theme.cpp$(DependSuffix): Modules/Spindizzy/ThemeSet/Theme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_Theme.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_Theme.cpp$(DependSuffix) -MM Modules/Spindizzy/ThemeSet/Theme.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_Theme.cpp$(PreprocessSuffix): Modules/Spindizzy/ThemeSet/Theme.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_Theme.cpp$(PreprocessSuffix) Modules/Spindizzy/ThemeSet/Theme.cpp

$(IntermediateDirectory)/Modules_FrontEnd_FrontEndScriptSupport.cpp$(ObjectSuffix): Modules/FrontEnd/FrontEndScriptSupport.cpp $(IntermediateDirectory)/Modules_FrontEnd_FrontEndScriptSupport.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/FrontEnd/FrontEndScriptSupport.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_FrontEnd_FrontEndScriptSupport.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_FrontEnd_FrontEndScriptSupport.cpp$(DependSuffix): Modules/FrontEnd/FrontEndScriptSupport.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_FrontEnd_FrontEndScriptSupport.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_FrontEnd_FrontEndScriptSupport.cpp$(DependSuffix) -MM Modules/FrontEnd/FrontEndScriptSupport.cpp

$(IntermediateDirectory)/Modules_FrontEnd_FrontEndScriptSupport.cpp$(PreprocessSuffix): Modules/FrontEnd/FrontEndScriptSupport.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_FrontEnd_FrontEndScriptSupport.cpp$(PreprocessSuffix) Modules/FrontEnd/FrontEndScriptSupport.cpp

$(IntermediateDirectory)/Modules_FrontEnd_FrontEnd.cpp$(ObjectSuffix): Modules/FrontEnd/FrontEnd.cpp $(IntermediateDirectory)/Modules_FrontEnd_FrontEnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/FrontEnd/FrontEnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_FrontEnd_FrontEnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_FrontEnd_FrontEnd.cpp$(DependSuffix): Modules/FrontEnd/FrontEnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_FrontEnd_FrontEnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_FrontEnd_FrontEnd.cpp$(DependSuffix) -MM Modules/FrontEnd/FrontEnd.cpp

$(IntermediateDirectory)/Modules_FrontEnd_FrontEnd.cpp$(PreprocessSuffix): Modules/FrontEnd/FrontEnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_FrontEnd_FrontEnd.cpp$(PreprocessSuffix) Modules/FrontEnd/FrontEnd.cpp

$(IntermediateDirectory)/IsoRealms_ScreenMode.cpp$(ObjectSuffix): IsoRealms/ScreenMode.cpp $(IntermediateDirectory)/IsoRealms_ScreenMode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ScreenMode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ScreenMode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ScreenMode.cpp$(DependSuffix): IsoRealms/ScreenMode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ScreenMode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ScreenMode.cpp$(DependSuffix) -MM IsoRealms/ScreenMode.cpp

$(IntermediateDirectory)/IsoRealms_ScreenMode.cpp$(PreprocessSuffix): IsoRealms/ScreenMode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ScreenMode.cpp$(PreprocessSuffix) IsoRealms/ScreenMode.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_Terrain.cpp$(ObjectSuffix): Modules/Spindizzy/Terrain/Terrain.cpp $(IntermediateDirectory)/Modules_Spindizzy_Terrain_Terrain.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Terrain/Terrain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Terrain_Terrain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Terrain_Terrain.cpp$(DependSuffix): Modules/Spindizzy/Terrain/Terrain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Terrain_Terrain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Terrain_Terrain.cpp$(DependSuffix) -MM Modules/Spindizzy/Terrain/Terrain.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_Terrain.cpp$(PreprocessSuffix): Modules/Spindizzy/Terrain/Terrain.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_Terrain.cpp$(PreprocessSuffix) Modules/Spindizzy/Terrain/Terrain.cpp

$(IntermediateDirectory)/Modules_HighScore_HighScoreRecord.cpp$(ObjectSuffix): Modules/HighScore/HighScoreRecord.cpp $(IntermediateDirectory)/Modules_HighScore_HighScoreRecord.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/HighScore/HighScoreRecord.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_HighScore_HighScoreRecord.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_HighScore_HighScoreRecord.cpp$(DependSuffix): Modules/HighScore/HighScoreRecord.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_HighScore_HighScoreRecord.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_HighScore_HighScoreRecord.cpp$(DependSuffix) -MM Modules/HighScore/HighScoreRecord.cpp

$(IntermediateDirectory)/Modules_HighScore_HighScoreRecord.cpp$(PreprocessSuffix): Modules/HighScore/HighScoreRecord.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_HighScore_HighScoreRecord.cpp$(PreprocessSuffix) Modules/HighScore/HighScoreRecord.cpp

$(IntermediateDirectory)/IsoRealms_Properties_ConditionElementIcon.cpp$(ObjectSuffix): IsoRealms/Properties/ConditionElementIcon.cpp $(IntermediateDirectory)/IsoRealms_Properties_ConditionElementIcon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Properties/ConditionElementIcon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Properties_ConditionElementIcon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Properties_ConditionElementIcon.cpp$(DependSuffix): IsoRealms/Properties/ConditionElementIcon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Properties_ConditionElementIcon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Properties_ConditionElementIcon.cpp$(DependSuffix) -MM IsoRealms/Properties/ConditionElementIcon.cpp

$(IntermediateDirectory)/IsoRealms_Properties_ConditionElementIcon.cpp$(PreprocessSuffix): IsoRealms/Properties/ConditionElementIcon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Properties_ConditionElementIcon.cpp$(PreprocessSuffix) IsoRealms/Properties/ConditionElementIcon.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertySpindizzyWorld.cpp$(ObjectSuffix): Modules/Spindizzy/ResourcePropertySpindizzyWorld.cpp $(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertySpindizzyWorld.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/ResourcePropertySpindizzyWorld.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertySpindizzyWorld.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertySpindizzyWorld.cpp$(DependSuffix): Modules/Spindizzy/ResourcePropertySpindizzyWorld.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertySpindizzyWorld.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertySpindizzyWorld.cpp$(DependSuffix) -MM Modules/Spindizzy/ResourcePropertySpindizzyWorld.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertySpindizzyWorld.cpp$(PreprocessSuffix): Modules/Spindizzy/ResourcePropertySpindizzyWorld.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertySpindizzyWorld.cpp$(PreprocessSuffix) Modules/Spindizzy/ResourcePropertySpindizzyWorld.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertyWorldEditor.cpp$(ObjectSuffix): Modules/Spindizzy/ResourcePropertyWorldEditor.cpp $(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertyWorldEditor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/ResourcePropertyWorldEditor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertyWorldEditor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertyWorldEditor.cpp$(DependSuffix): Modules/Spindizzy/ResourcePropertyWorldEditor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertyWorldEditor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertyWorldEditor.cpp$(DependSuffix) -MM Modules/Spindizzy/ResourcePropertyWorldEditor.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertyWorldEditor.cpp$(PreprocessSuffix): Modules/Spindizzy/ResourcePropertyWorldEditor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_ResourcePropertyWorldEditor.cpp$(PreprocessSuffix) Modules/Spindizzy/ResourcePropertyWorldEditor.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeBottom.cpp$(ObjectSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeBottom.cpp $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeBottom.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/ScreenEdge/ScreenEdgeBottom.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeBottom.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeBottom.cpp$(DependSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeBottom.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeBottom.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeBottom.cpp$(DependSuffix) -MM Modules/EditorOLD/ScreenEdge/ScreenEdgeBottom.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeBottom.cpp$(PreprocessSuffix): Modules/EditorOLD/ScreenEdge/ScreenEdgeBottom.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_ScreenEdge_ScreenEdgeBottom.cpp$(PreprocessSuffix) Modules/EditorOLD/ScreenEdge/ScreenEdgeBottom.cpp

$(IntermediateDirectory)/IsoRealms_GUI_EdgeRelation.cpp$(ObjectSuffix): IsoRealms/GUI/EdgeRelation.cpp $(IntermediateDirectory)/IsoRealms_GUI_EdgeRelation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/EdgeRelation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_EdgeRelation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_EdgeRelation.cpp$(DependSuffix): IsoRealms/GUI/EdgeRelation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_EdgeRelation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_EdgeRelation.cpp$(DependSuffix) -MM IsoRealms/GUI/EdgeRelation.cpp

$(IntermediateDirectory)/IsoRealms_GUI_EdgeRelation.cpp$(PreprocessSuffix): IsoRealms/GUI/EdgeRelation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_EdgeRelation.cpp$(PreprocessSuffix) IsoRealms/GUI/EdgeRelation.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyTexture.cpp$(ObjectSuffix): IsoRealms/ResourcePropertyTexture.cpp $(IntermediateDirectory)/IsoRealms_ResourcePropertyTexture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/ResourcePropertyTexture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_ResourcePropertyTexture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_ResourcePropertyTexture.cpp$(DependSuffix): IsoRealms/ResourcePropertyTexture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_ResourcePropertyTexture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_ResourcePropertyTexture.cpp$(DependSuffix) -MM IsoRealms/ResourcePropertyTexture.cpp

$(IntermediateDirectory)/IsoRealms_ResourcePropertyTexture.cpp$(PreprocessSuffix): IsoRealms/ResourcePropertyTexture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_ResourcePropertyTexture.cpp$(PreprocessSuffix) IsoRealms/ResourcePropertyTexture.cpp

$(IntermediateDirectory)/IsoRealms_Common_BlockLocation.cpp$(ObjectSuffix): IsoRealms/Common/BlockLocation.cpp $(IntermediateDirectory)/IsoRealms_Common_BlockLocation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Common/BlockLocation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Common_BlockLocation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Common_BlockLocation.cpp$(DependSuffix): IsoRealms/Common/BlockLocation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Common_BlockLocation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Common_BlockLocation.cpp$(DependSuffix) -MM IsoRealms/Common/BlockLocation.cpp

$(IntermediateDirectory)/IsoRealms_Common_BlockLocation.cpp$(PreprocessSuffix): IsoRealms/Common/BlockLocation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Common_BlockLocation.cpp$(PreprocessSuffix) IsoRealms/Common/BlockLocation.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Font.cpp$(ObjectSuffix): IsoRealms/Assets/Fixed/Font.cpp $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Font.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Fixed/Font.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Font.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Font.cpp$(DependSuffix): IsoRealms/Assets/Fixed/Font.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Font.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Font.cpp$(DependSuffix) -MM IsoRealms/Assets/Fixed/Font.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Font.cpp$(PreprocessSuffix): IsoRealms/Assets/Fixed/Font.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Font.cpp$(PreprocessSuffix) IsoRealms/Assets/Fixed/Font.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ModuleEditorScriptSupport.cpp$(ObjectSuffix): Modules/EditorOLD/ModuleEditorScriptSupport.cpp $(IntermediateDirectory)/Modules_EditorOLD_ModuleEditorScriptSupport.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/ModuleEditorScriptSupport.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_ModuleEditorScriptSupport.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_ModuleEditorScriptSupport.cpp$(DependSuffix): Modules/EditorOLD/ModuleEditorScriptSupport.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_ModuleEditorScriptSupport.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_ModuleEditorScriptSupport.cpp$(DependSuffix) -MM Modules/EditorOLD/ModuleEditorScriptSupport.cpp

$(IntermediateDirectory)/Modules_EditorOLD_ModuleEditorScriptSupport.cpp$(PreprocessSuffix): Modules/EditorOLD/ModuleEditorScriptSupport.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_ModuleEditorScriptSupport.cpp$(PreprocessSuffix) Modules/EditorOLD/ModuleEditorScriptSupport.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogConfirmation.cpp$(ObjectSuffix): Modules/EditorOLD/DialogConfirmation.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogConfirmation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogConfirmation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogConfirmation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogConfirmation.cpp$(DependSuffix): Modules/EditorOLD/DialogConfirmation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogConfirmation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogConfirmation.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogConfirmation.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogConfirmation.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogConfirmation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogConfirmation.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogConfirmation.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Float.cpp$(ObjectSuffix): IsoRealms/Assets/Fixed/Float.cpp $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Float.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Fixed/Float.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Float.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Float.cpp$(DependSuffix): IsoRealms/Assets/Fixed/Float.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Float.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Float.cpp$(DependSuffix) -MM IsoRealms/Assets/Fixed/Float.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Float.cpp$(PreprocessSuffix): IsoRealms/Assets/Fixed/Float.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Float.cpp$(PreprocessSuffix) IsoRealms/Assets/Fixed/Float.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_FontRegistry.cpp$(ObjectSuffix): IsoRealms/Assets/Registry/FontRegistry.cpp $(IntermediateDirectory)/IsoRealms_Assets_Registry_FontRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Registry/FontRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Registry_FontRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Registry_FontRegistry.cpp$(DependSuffix): IsoRealms/Assets/Registry/FontRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Registry_FontRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Registry_FontRegistry.cpp$(DependSuffix) -MM IsoRealms/Assets/Registry/FontRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_FontRegistry.cpp$(PreprocessSuffix): IsoRealms/Assets/Registry/FontRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Registry_FontRegistry.cpp$(PreprocessSuffix) IsoRealms/Assets/Registry/FontRegistry.cpp

$(IntermediateDirectory)/Modules_Editor_ModuleEditor.cpp$(ObjectSuffix): Modules/Editor/ModuleEditor.cpp $(IntermediateDirectory)/Modules_Editor_ModuleEditor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Editor/ModuleEditor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Editor_ModuleEditor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Editor_ModuleEditor.cpp$(DependSuffix): Modules/Editor/ModuleEditor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Editor_ModuleEditor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Editor_ModuleEditor.cpp$(DependSuffix) -MM Modules/Editor/ModuleEditor.cpp

$(IntermediateDirectory)/Modules_Editor_ModuleEditor.cpp$(PreprocessSuffix): Modules/Editor/ModuleEditor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Editor_ModuleEditor.cpp$(PreprocessSuffix) Modules/Editor/ModuleEditor.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftCollisionData.cpp$(ObjectSuffix): Modules/Spindizzy/Lift/LiftCollisionData.cpp $(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftCollisionData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Lift/LiftCollisionData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftCollisionData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftCollisionData.cpp$(DependSuffix): Modules/Spindizzy/Lift/LiftCollisionData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftCollisionData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftCollisionData.cpp$(DependSuffix) -MM Modules/Spindizzy/Lift/LiftCollisionData.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftCollisionData.cpp$(PreprocessSuffix): Modules/Spindizzy/Lift/LiftCollisionData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Lift_LiftCollisionData.cpp$(PreprocessSuffix) Modules/Spindizzy/Lift/LiftCollisionData.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModel.cpp$(ObjectSuffix): IsoRealms/GUI/ValueComponents/ComponentResourceModel.cpp $(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/ValueComponents/ComponentResourceModel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModel.cpp$(DependSuffix): IsoRealms/GUI/ValueComponents/ComponentResourceModel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModel.cpp$(DependSuffix) -MM IsoRealms/GUI/ValueComponents/ComponentResourceModel.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModel.cpp$(PreprocessSuffix): IsoRealms/GUI/ValueComponents/ComponentResourceModel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_ValueComponents_ComponentResourceModel.cpp$(PreprocessSuffix) IsoRealms/GUI/ValueComponents/ComponentResourceModel.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModelInstance.cpp$(ObjectSuffix): IsoRealms/GUI/Icons/ComponentIconModelInstance.cpp $(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModelInstance.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/Icons/ComponentIconModelInstance.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModelInstance.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModelInstance.cpp$(DependSuffix): IsoRealms/GUI/Icons/ComponentIconModelInstance.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModelInstance.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModelInstance.cpp$(DependSuffix) -MM IsoRealms/GUI/Icons/ComponentIconModelInstance.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModelInstance.cpp$(PreprocessSuffix): IsoRealms/GUI/Icons/ComponentIconModelInstance.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_Icons_ComponentIconModelInstance.cpp$(PreprocessSuffix) IsoRealms/GUI/Icons/ComponentIconModelInstance.cpp

$(IntermediateDirectory)/IsoRealms_GUI_TreeNode.cpp$(ObjectSuffix): IsoRealms/GUI/TreeNode.cpp $(IntermediateDirectory)/IsoRealms_GUI_TreeNode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/TreeNode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_TreeNode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_TreeNode.cpp$(DependSuffix): IsoRealms/GUI/TreeNode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_TreeNode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_TreeNode.cpp$(DependSuffix) -MM IsoRealms/GUI/TreeNode.cpp

$(IntermediateDirectory)/IsoRealms_GUI_TreeNode.cpp$(PreprocessSuffix): IsoRealms/GUI/TreeNode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_TreeNode.cpp$(PreprocessSuffix) IsoRealms/GUI/TreeNode.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ResizableDialog.cpp$(ObjectSuffix): IsoRealms/GUI/ResizableDialog.cpp $(IntermediateDirectory)/IsoRealms_GUI_ResizableDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/ResizableDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_ResizableDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_ResizableDialog.cpp$(DependSuffix): IsoRealms/GUI/ResizableDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_ResizableDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_ResizableDialog.cpp$(DependSuffix) -MM IsoRealms/GUI/ResizableDialog.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ResizableDialog.cpp$(PreprocessSuffix): IsoRealms/GUI/ResizableDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_ResizableDialog.cpp$(PreprocessSuffix) IsoRealms/GUI/ResizableDialog.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentFileSelector.cpp$(ObjectSuffix): IsoRealms/GUI/Dialogs/ComponentFileSelector.cpp $(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentFileSelector.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/Dialogs/ComponentFileSelector.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentFileSelector.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentFileSelector.cpp$(DependSuffix): IsoRealms/GUI/Dialogs/ComponentFileSelector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentFileSelector.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentFileSelector.cpp$(DependSuffix) -MM IsoRealms/GUI/Dialogs/ComponentFileSelector.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentFileSelector.cpp$(PreprocessSuffix): IsoRealms/GUI/Dialogs/ComponentFileSelector.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_Dialogs_ComponentFileSelector.cpp$(PreprocessSuffix) IsoRealms/GUI/Dialogs/ComponentFileSelector.cpp

$(IntermediateDirectory)/IsoRealms_Properties_Property.cpp$(ObjectSuffix): IsoRealms/Properties/Property.cpp $(IntermediateDirectory)/IsoRealms_Properties_Property.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Properties/Property.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Properties_Property.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Properties_Property.cpp$(DependSuffix): IsoRealms/Properties/Property.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Properties_Property.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Properties_Property.cpp$(DependSuffix) -MM IsoRealms/Properties/Property.cpp

$(IntermediateDirectory)/IsoRealms_Properties_Property.cpp$(PreprocessSuffix): IsoRealms/Properties/Property.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Properties_Property.cpp$(PreprocessSuffix) IsoRealms/Properties/Property.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ListBox.cpp$(ObjectSuffix): IsoRealms/GUI/ListBox.cpp $(IntermediateDirectory)/IsoRealms_GUI_ListBox.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/ListBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_ListBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_ListBox.cpp$(DependSuffix): IsoRealms/GUI/ListBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_ListBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_ListBox.cpp$(DependSuffix) -MM IsoRealms/GUI/ListBox.cpp

$(IntermediateDirectory)/IsoRealms_GUI_ListBox.cpp$(PreprocessSuffix): IsoRealms/GUI/ListBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_ListBox.cpp$(PreprocessSuffix) IsoRealms/GUI/ListBox.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Button.cpp$(ObjectSuffix): IsoRealms/GUI/Button.cpp $(IntermediateDirectory)/IsoRealms_GUI_Button.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/Button.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_Button.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_Button.cpp$(DependSuffix): IsoRealms/GUI/Button.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_Button.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_Button.cpp$(DependSuffix) -MM IsoRealms/GUI/Button.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Button.cpp$(PreprocessSuffix): IsoRealms/GUI/Button.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_Button.cpp$(PreprocessSuffix) IsoRealms/GUI/Button.cpp

$(IntermediateDirectory)/IsoRealms_LuaSupport_LuaState.cpp$(ObjectSuffix): IsoRealms/LuaSupport/LuaState.cpp $(IntermediateDirectory)/IsoRealms_LuaSupport_LuaState.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/LuaSupport/LuaState.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_LuaSupport_LuaState.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_LuaSupport_LuaState.cpp$(DependSuffix): IsoRealms/LuaSupport/LuaState.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_LuaSupport_LuaState.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_LuaSupport_LuaState.cpp$(DependSuffix) -MM IsoRealms/LuaSupport/LuaState.cpp

$(IntermediateDirectory)/IsoRealms_LuaSupport_LuaState.cpp$(PreprocessSuffix): IsoRealms/LuaSupport/LuaState.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_LuaSupport_LuaState.cpp$(PreprocessSuffix) IsoRealms/LuaSupport/LuaState.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogOKCancelUndo.cpp$(ObjectSuffix): IsoRealms/GUI/Dialogs/DialogOKCancelUndo.cpp $(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogOKCancelUndo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/Dialogs/DialogOKCancelUndo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogOKCancelUndo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogOKCancelUndo.cpp$(DependSuffix): IsoRealms/GUI/Dialogs/DialogOKCancelUndo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogOKCancelUndo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogOKCancelUndo.cpp$(DependSuffix) -MM IsoRealms/GUI/Dialogs/DialogOKCancelUndo.cpp

$(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogOKCancelUndo.cpp$(PreprocessSuffix): IsoRealms/GUI/Dialogs/DialogOKCancelUndo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_Dialogs_DialogOKCancelUndo.cpp$(PreprocessSuffix) IsoRealms/GUI/Dialogs/DialogOKCancelUndo.cpp

$(IntermediateDirectory)/Modules_Basics_Float_Float.cpp$(ObjectSuffix): Modules/Basics/Float/Float.cpp $(IntermediateDirectory)/Modules_Basics_Float_Float.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Float/Float.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Float_Float.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Float_Float.cpp$(DependSuffix): Modules/Basics/Float/Float.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Float_Float.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Float_Float.cpp$(DependSuffix) -MM Modules/Basics/Float/Float.cpp

$(IntermediateDirectory)/Modules_Basics_Float_Float.cpp$(PreprocessSuffix): Modules/Basics/Float/Float.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Float_Float.cpp$(PreprocessSuffix) Modules/Basics/Float/Float.cpp

$(IntermediateDirectory)/Modules_HighScore_HighScoreTable.cpp$(ObjectSuffix): Modules/HighScore/HighScoreTable.cpp $(IntermediateDirectory)/Modules_HighScore_HighScoreTable.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/HighScore/HighScoreTable.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_HighScore_HighScoreTable.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_HighScore_HighScoreTable.cpp$(DependSuffix): Modules/HighScore/HighScoreTable.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_HighScore_HighScoreTable.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_HighScore_HighScoreTable.cpp$(DependSuffix) -MM Modules/HighScore/HighScoreTable.cpp

$(IntermediateDirectory)/Modules_HighScore_HighScoreTable.cpp$(PreprocessSuffix): Modules/HighScore/HighScoreTable.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_HighScore_HighScoreTable.cpp$(PreprocessSuffix) Modules/HighScore/HighScoreTable.cpp

$(IntermediateDirectory)/Modules_Spindizzy_C64TerrainGraphics_C64TerrainGraphics.cpp$(ObjectSuffix): Modules/Spindizzy/C64TerrainGraphics/C64TerrainGraphics.cpp $(IntermediateDirectory)/Modules_Spindizzy_C64TerrainGraphics_C64TerrainGraphics.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/C64TerrainGraphics/C64TerrainGraphics.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_C64TerrainGraphics_C64TerrainGraphics.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_C64TerrainGraphics_C64TerrainGraphics.cpp$(DependSuffix): Modules/Spindizzy/C64TerrainGraphics/C64TerrainGraphics.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_C64TerrainGraphics_C64TerrainGraphics.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_C64TerrainGraphics_C64TerrainGraphics.cpp$(DependSuffix) -MM Modules/Spindizzy/C64TerrainGraphics/C64TerrainGraphics.cpp

$(IntermediateDirectory)/Modules_Spindizzy_C64TerrainGraphics_C64TerrainGraphics.cpp$(PreprocessSuffix): Modules/Spindizzy/C64TerrainGraphics/C64TerrainGraphics.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_C64TerrainGraphics_C64TerrainGraphics.cpp$(PreprocessSuffix) Modules/Spindizzy/C64TerrainGraphics/C64TerrainGraphics.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Vertex.cpp$(ObjectSuffix): IsoRealms/Assets/Fixed/Vertex.cpp $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Vertex.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Fixed/Vertex.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Vertex.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Vertex.cpp$(DependSuffix): IsoRealms/Assets/Fixed/Vertex.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Vertex.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Vertex.cpp$(DependSuffix) -MM IsoRealms/Assets/Fixed/Vertex.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Vertex.cpp$(PreprocessSuffix): IsoRealms/Assets/Fixed/Vertex.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Vertex.cpp$(PreprocessSuffix) IsoRealms/Assets/Fixed/Vertex.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Texture.cpp$(ObjectSuffix): IsoRealms/Assets/Fixed/Texture.cpp $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Texture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Fixed/Texture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Texture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Texture.cpp$(DependSuffix): IsoRealms/Assets/Fixed/Texture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Texture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Texture.cpp$(DependSuffix) -MM IsoRealms/Assets/Fixed/Texture.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_Texture.cpp$(PreprocessSuffix): IsoRealms/Assets/Fixed/Texture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_Texture.cpp$(PreprocessSuffix) IsoRealms/Assets/Fixed/Texture.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_String.cpp$(ObjectSuffix): IsoRealms/Assets/Fixed/String.cpp $(IntermediateDirectory)/IsoRealms_Assets_Fixed_String.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Fixed/String.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Fixed_String.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Fixed_String.cpp$(DependSuffix): IsoRealms/Assets/Fixed/String.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Fixed_String.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Fixed_String.cpp$(DependSuffix) -MM IsoRealms/Assets/Fixed/String.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Fixed_String.cpp$(PreprocessSuffix): IsoRealms/Assets/Fixed/String.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Fixed_String.cpp$(PreprocessSuffix) IsoRealms/Assets/Fixed/String.cpp

$(IntermediateDirectory)/Modules_Basics_Vertex_Vertex.cpp$(ObjectSuffix): Modules/Basics/Vertex/Vertex.cpp $(IntermediateDirectory)/Modules_Basics_Vertex_Vertex.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Vertex/Vertex.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Vertex_Vertex.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Vertex_Vertex.cpp$(DependSuffix): Modules/Basics/Vertex/Vertex.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Vertex_Vertex.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Vertex_Vertex.cpp$(DependSuffix) -MM Modules/Basics/Vertex/Vertex.cpp

$(IntermediateDirectory)/Modules_Basics_Vertex_Vertex.cpp$(PreprocessSuffix): Modules/Basics/Vertex/Vertex.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Vertex_Vertex.cpp$(PreprocessSuffix) Modules/Basics/Vertex/Vertex.cpp

$(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_IntegerToString.cpp$(ObjectSuffix): IsoRealms/Assets/TypeConverted/IntegerToString.cpp $(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_IntegerToString.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/TypeConverted/IntegerToString.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_IntegerToString.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_IntegerToString.cpp$(DependSuffix): IsoRealms/Assets/TypeConverted/IntegerToString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_IntegerToString.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_IntegerToString.cpp$(DependSuffix) -MM IsoRealms/Assets/TypeConverted/IntegerToString.cpp

$(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_IntegerToString.cpp$(PreprocessSuffix): IsoRealms/Assets/TypeConverted/IntegerToString.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_TypeConverted_IntegerToString.cpp$(PreprocessSuffix) IsoRealms/Assets/TypeConverted/IntegerToString.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_FloatRegistry.cpp$(ObjectSuffix): IsoRealms/Assets/Registry/FloatRegistry.cpp $(IntermediateDirectory)/IsoRealms_Assets_Registry_FloatRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Registry/FloatRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Registry_FloatRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Registry_FloatRegistry.cpp$(DependSuffix): IsoRealms/Assets/Registry/FloatRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Registry_FloatRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Registry_FloatRegistry.cpp$(DependSuffix) -MM IsoRealms/Assets/Registry/FloatRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_FloatRegistry.cpp$(PreprocessSuffix): IsoRealms/Assets/Registry/FloatRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Registry_FloatRegistry.cpp$(PreprocessSuffix) IsoRealms/Assets/Registry/FloatRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Module.cpp$(ObjectSuffix): IsoRealms/Module.cpp $(IntermediateDirectory)/IsoRealms_Module.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Module.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Module.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Module.cpp$(DependSuffix): IsoRealms/Module.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Module.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Module.cpp$(DependSuffix) -MM IsoRealms/Module.cpp

$(IntermediateDirectory)/IsoRealms_Module.cpp$(PreprocessSuffix): IsoRealms/Module.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Module.cpp$(PreprocessSuffix) IsoRealms/Module.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_ModelRegistry.cpp$(ObjectSuffix): IsoRealms/Assets/Registry/ModelRegistry.cpp $(IntermediateDirectory)/IsoRealms_Assets_Registry_ModelRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Registry/ModelRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Registry_ModelRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Registry_ModelRegistry.cpp$(DependSuffix): IsoRealms/Assets/Registry/ModelRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Registry_ModelRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Registry_ModelRegistry.cpp$(DependSuffix) -MM IsoRealms/Assets/Registry/ModelRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_ModelRegistry.cpp$(PreprocessSuffix): IsoRealms/Assets/Registry/ModelRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Registry_ModelRegistry.cpp$(PreprocessSuffix) IsoRealms/Assets/Registry/ModelRegistry.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Project_Project.cpp$(ObjectSuffix): Modules/FrontEnd/Project/Project.cpp $(IntermediateDirectory)/Modules_FrontEnd_Project_Project.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/FrontEnd/Project/Project.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_FrontEnd_Project_Project.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_FrontEnd_Project_Project.cpp$(DependSuffix): Modules/FrontEnd/Project/Project.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_FrontEnd_Project_Project.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_FrontEnd_Project_Project.cpp$(DependSuffix) -MM Modules/FrontEnd/Project/Project.cpp

$(IntermediateDirectory)/Modules_FrontEnd_Project_Project.cpp$(PreprocessSuffix): Modules/FrontEnd/Project/Project.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_FrontEnd_Project_Project.cpp$(PreprocessSuffix) Modules/FrontEnd/Project/Project.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_ColourRegistry.cpp$(ObjectSuffix): IsoRealms/Assets/Registry/ColourRegistry.cpp $(IntermediateDirectory)/IsoRealms_Assets_Registry_ColourRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Registry/ColourRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Registry_ColourRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Registry_ColourRegistry.cpp$(DependSuffix): IsoRealms/Assets/Registry/ColourRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Registry_ColourRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Registry_ColourRegistry.cpp$(DependSuffix) -MM IsoRealms/Assets/Registry/ColourRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_ColourRegistry.cpp$(PreprocessSuffix): IsoRealms/Assets/Registry/ColourRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Registry_ColourRegistry.cpp$(PreprocessSuffix) IsoRealms/Assets/Registry/ColourRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_TextureRegistry.cpp$(ObjectSuffix): IsoRealms/Assets/Registry/TextureRegistry.cpp $(IntermediateDirectory)/IsoRealms_Assets_Registry_TextureRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Registry/TextureRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Registry_TextureRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Registry_TextureRegistry.cpp$(DependSuffix): IsoRealms/Assets/Registry/TextureRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Registry_TextureRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Registry_TextureRegistry.cpp$(DependSuffix) -MM IsoRealms/Assets/Registry/TextureRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_TextureRegistry.cpp$(PreprocessSuffix): IsoRealms/Assets/Registry/TextureRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Registry_TextureRegistry.cpp$(PreprocessSuffix) IsoRealms/Assets/Registry/TextureRegistry.cpp

$(IntermediateDirectory)/Modules_EditorOLD_Editor_EditorDialog.cpp$(ObjectSuffix): Modules/EditorOLD/Editor/EditorDialog.cpp $(IntermediateDirectory)/Modules_EditorOLD_Editor_EditorDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/Editor/EditorDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_Editor_EditorDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_Editor_EditorDialog.cpp$(DependSuffix): Modules/EditorOLD/Editor/EditorDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_Editor_EditorDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_Editor_EditorDialog.cpp$(DependSuffix) -MM Modules/EditorOLD/Editor/EditorDialog.cpp

$(IntermediateDirectory)/Modules_EditorOLD_Editor_EditorDialog.cpp$(PreprocessSuffix): Modules/EditorOLD/Editor/EditorDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_Editor_EditorDialog.cpp$(PreprocessSuffix) Modules/EditorOLD/Editor/EditorDialog.cpp

$(IntermediateDirectory)/Modules_Spindizzy_World_World.cpp$(ObjectSuffix): Modules/Spindizzy/World/World.cpp $(IntermediateDirectory)/Modules_Spindizzy_World_World.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/World/World.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_World_World.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_World_World.cpp$(DependSuffix): Modules/Spindizzy/World/World.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_World_World.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_World_World.cpp$(DependSuffix) -MM Modules/Spindizzy/World/World.cpp

$(IntermediateDirectory)/Modules_Spindizzy_World_World.cpp$(PreprocessSuffix): Modules/Spindizzy/World/World.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_World_World.cpp$(PreprocessSuffix) Modules/Spindizzy/World/World.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallSurface.cpp$(ObjectSuffix): Modules/Spindizzy/Terrain/WallSurface.cpp $(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallSurface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Terrain/WallSurface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallSurface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallSurface.cpp$(DependSuffix): Modules/Spindizzy/Terrain/WallSurface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallSurface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallSurface.cpp$(DependSuffix) -MM Modules/Spindizzy/Terrain/WallSurface.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallSurface.cpp$(PreprocessSuffix): Modules/Spindizzy/Terrain/WallSurface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Terrain_WallSurface.cpp$(PreprocessSuffix) Modules/Spindizzy/Terrain/WallSurface.cpp

$(IntermediateDirectory)/IsoRealms_IsoRealmsConstants.cpp$(ObjectSuffix): IsoRealms/IsoRealmsConstants.cpp $(IntermediateDirectory)/IsoRealms_IsoRealmsConstants.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/IsoRealmsConstants.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_IsoRealmsConstants.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_IsoRealmsConstants.cpp$(DependSuffix): IsoRealms/IsoRealmsConstants.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_IsoRealmsConstants.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_IsoRealmsConstants.cpp$(DependSuffix) -MM IsoRealms/IsoRealmsConstants.cpp

$(IntermediateDirectory)/IsoRealms_IsoRealmsConstants.cpp$(PreprocessSuffix): IsoRealms/IsoRealmsConstants.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_IsoRealmsConstants.cpp$(PreprocessSuffix) IsoRealms/IsoRealmsConstants.cpp

$(IntermediateDirectory)/IsoRealms_Collision_CollisionElement.cpp$(ObjectSuffix): IsoRealms/Collision/CollisionElement.cpp $(IntermediateDirectory)/IsoRealms_Collision_CollisionElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Collision/CollisionElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Collision_CollisionElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Collision_CollisionElement.cpp$(DependSuffix): IsoRealms/Collision/CollisionElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Collision_CollisionElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Collision_CollisionElement.cpp$(DependSuffix) -MM IsoRealms/Collision/CollisionElement.cpp

$(IntermediateDirectory)/IsoRealms_Collision_CollisionElement.cpp$(PreprocessSuffix): IsoRealms/Collision/CollisionElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Collision_CollisionElement.cpp$(PreprocessSuffix) IsoRealms/Collision/CollisionElement.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_ScreenRegistry.cpp$(ObjectSuffix): IsoRealms/Assets/Registry/ScreenRegistry.cpp $(IntermediateDirectory)/IsoRealms_Assets_Registry_ScreenRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Registry/ScreenRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Registry_ScreenRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Registry_ScreenRegistry.cpp$(DependSuffix): IsoRealms/Assets/Registry/ScreenRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Registry_ScreenRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Registry_ScreenRegistry.cpp$(DependSuffix) -MM IsoRealms/Assets/Registry/ScreenRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_ScreenRegistry.cpp$(PreprocessSuffix): IsoRealms/Assets/Registry/ScreenRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Registry_ScreenRegistry.cpp$(PreprocessSuffix) IsoRealms/Assets/Registry/ScreenRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_IntegerRegistry.cpp$(ObjectSuffix): IsoRealms/Assets/Registry/IntegerRegistry.cpp $(IntermediateDirectory)/IsoRealms_Assets_Registry_IntegerRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Registry/IntegerRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Registry_IntegerRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Registry_IntegerRegistry.cpp$(DependSuffix): IsoRealms/Assets/Registry/IntegerRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Registry_IntegerRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Registry_IntegerRegistry.cpp$(DependSuffix) -MM IsoRealms/Assets/Registry/IntegerRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_IntegerRegistry.cpp$(PreprocessSuffix): IsoRealms/Assets/Registry/IntegerRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Registry_IntegerRegistry.cpp$(PreprocessSuffix) IsoRealms/Assets/Registry/IntegerRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_ActionTypeRegistry.cpp$(ObjectSuffix): IsoRealms/Assets/Registry/ActionTypeRegistry.cpp $(IntermediateDirectory)/IsoRealms_Assets_Registry_ActionTypeRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Registry/ActionTypeRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Registry_ActionTypeRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Registry_ActionTypeRegistry.cpp$(DependSuffix): IsoRealms/Assets/Registry/ActionTypeRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Registry_ActionTypeRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Registry_ActionTypeRegistry.cpp$(DependSuffix) -MM IsoRealms/Assets/Registry/ActionTypeRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_ActionTypeRegistry.cpp$(PreprocessSuffix): IsoRealms/Assets/Registry/ActionTypeRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Registry_ActionTypeRegistry.cpp$(PreprocessSuffix) IsoRealms/Assets/Registry/ActionTypeRegistry.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_IconFont.cpp$(ObjectSuffix): Modules/EditorOLD/DialogResourceFonts/IconFont.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_IconFont.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogResourceFonts/IconFont.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_IconFont.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_IconFont.cpp$(DependSuffix): Modules/EditorOLD/DialogResourceFonts/IconFont.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_IconFont.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_IconFont.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogResourceFonts/IconFont.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_IconFont.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogResourceFonts/IconFont.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogResourceFonts_IconFont.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogResourceFonts/IconFont.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_EditableRegistry.cpp$(ObjectSuffix): IsoRealms/Assets/Registry/EditableRegistry.cpp $(IntermediateDirectory)/IsoRealms_Assets_Registry_EditableRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Registry/EditableRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Registry_EditableRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Registry_EditableRegistry.cpp$(DependSuffix): IsoRealms/Assets/Registry/EditableRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Registry_EditableRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Registry_EditableRegistry.cpp$(DependSuffix) -MM IsoRealms/Assets/Registry/EditableRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_EditableRegistry.cpp$(PreprocessSuffix): IsoRealms/Assets/Registry/EditableRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Registry_EditableRegistry.cpp$(PreprocessSuffix) IsoRealms/Assets/Registry/EditableRegistry.cpp

$(IntermediateDirectory)/Modules_Spindizzy_WorldView_WorldView.cpp$(ObjectSuffix): Modules/Spindizzy/WorldView/WorldView.cpp $(IntermediateDirectory)/Modules_Spindizzy_WorldView_WorldView.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/WorldView/WorldView.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_WorldView_WorldView.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_WorldView_WorldView.cpp$(DependSuffix): Modules/Spindizzy/WorldView/WorldView.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_WorldView_WorldView.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_WorldView_WorldView.cpp$(DependSuffix) -MM Modules/Spindizzy/WorldView/WorldView.cpp

$(IntermediateDirectory)/Modules_Spindizzy_WorldView_WorldView.cpp$(PreprocessSuffix): Modules/Spindizzy/WorldView/WorldView.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_WorldView_WorldView.cpp$(PreprocessSuffix) Modules/Spindizzy/WorldView/WorldView.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexturesDialog.cpp$(ObjectSuffix): Modules/Spindizzy/ThemeSet/ThemeTexturesDialog.cpp $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexturesDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/ThemeSet/ThemeTexturesDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexturesDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexturesDialog.cpp$(DependSuffix): Modules/Spindizzy/ThemeSet/ThemeTexturesDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexturesDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexturesDialog.cpp$(DependSuffix) -MM Modules/Spindizzy/ThemeSet/ThemeTexturesDialog.cpp

$(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexturesDialog.cpp$(PreprocessSuffix): Modules/Spindizzy/ThemeSet/ThemeTexturesDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_ThemeSet_ThemeTexturesDialog.cpp$(PreprocessSuffix) Modules/Spindizzy/ThemeSet/ThemeTexturesDialog.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_BooleanRegistry.cpp$(ObjectSuffix): IsoRealms/Assets/Registry/BooleanRegistry.cpp $(IntermediateDirectory)/IsoRealms_Assets_Registry_BooleanRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/Assets/Registry/BooleanRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_Assets_Registry_BooleanRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_Assets_Registry_BooleanRegistry.cpp$(DependSuffix): IsoRealms/Assets/Registry/BooleanRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_Assets_Registry_BooleanRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_Assets_Registry_BooleanRegistry.cpp$(DependSuffix) -MM IsoRealms/Assets/Registry/BooleanRegistry.cpp

$(IntermediateDirectory)/IsoRealms_Assets_Registry_BooleanRegistry.cpp$(PreprocessSuffix): IsoRealms/Assets/Registry/BooleanRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_Assets_Registry_BooleanRegistry.cpp$(PreprocessSuffix) IsoRealms/Assets/Registry/BooleanRegistry.cpp

$(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(ObjectSuffix): CMakeFiles/3.16.3/CompilerIdCXX/CMakeCXXCompilerId.cpp $(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/CMakeFiles/3.16.3/CompilerIdCXX/CMakeCXXCompilerId.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(DependSuffix): CMakeFiles/3.16.3/CompilerIdCXX/CMakeCXXCompilerId.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(DependSuffix) -MM CMakeFiles/3.16.3/CompilerIdCXX/CMakeCXXCompilerId.cpp

$(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(PreprocessSuffix): CMakeFiles/3.16.3/CompilerIdCXX/CMakeCXXCompilerId.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CMakeFiles_3.16.3_CompilerIdCXX_CMakeCXXCompilerId.cpp$(PreprocessSuffix) CMakeFiles/3.16.3/CompilerIdCXX/CMakeCXXCompilerId.cpp

$(IntermediateDirectory)/Modules_Basics_InputGroup_InputGroup.cpp$(ObjectSuffix): Modules/Basics/InputGroup/InputGroup.cpp $(IntermediateDirectory)/Modules_Basics_InputGroup_InputGroup.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/InputGroup/InputGroup.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_InputGroup_InputGroup.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_InputGroup_InputGroup.cpp$(DependSuffix): Modules/Basics/InputGroup/InputGroup.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_InputGroup_InputGroup.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_InputGroup_InputGroup.cpp$(DependSuffix) -MM Modules/Basics/InputGroup/InputGroup.cpp

$(IntermediateDirectory)/Modules_Basics_InputGroup_InputGroup.cpp$(PreprocessSuffix): Modules/Basics/InputGroup/InputGroup.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_InputGroup_InputGroup.cpp$(PreprocessSuffix) Modules/Basics/InputGroup/InputGroup.cpp

$(IntermediateDirectory)/Modules_Replay_Replay.cpp$(ObjectSuffix): Modules/Replay/Replay.cpp $(IntermediateDirectory)/Modules_Replay_Replay.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Replay/Replay.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Replay_Replay.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Replay_Replay.cpp$(DependSuffix): Modules/Replay/Replay.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Replay_Replay.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Replay_Replay.cpp$(DependSuffix) -MM Modules/Replay/Replay.cpp

$(IntermediateDirectory)/Modules_Replay_Replay.cpp$(PreprocessSuffix): Modules/Replay/Replay.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Replay_Replay.cpp$(PreprocessSuffix) Modules/Replay/Replay.cpp

$(IntermediateDirectory)/Modules_Basics_Sprite_Sprite.cpp$(ObjectSuffix): Modules/Basics/Sprite/Sprite.cpp $(IntermediateDirectory)/Modules_Basics_Sprite_Sprite.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Sprite/Sprite.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Sprite_Sprite.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Sprite_Sprite.cpp$(DependSuffix): Modules/Basics/Sprite/Sprite.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Sprite_Sprite.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Sprite_Sprite.cpp$(DependSuffix) -MM Modules/Basics/Sprite/Sprite.cpp

$(IntermediateDirectory)/Modules_Basics_Sprite_Sprite.cpp$(PreprocessSuffix): Modules/Basics/Sprite/Sprite.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Sprite_Sprite.cpp$(PreprocessSuffix) Modules/Basics/Sprite/Sprite.cpp

$(IntermediateDirectory)/Modules_Basics_BooleanTrigger_BooleanTrigger.cpp$(ObjectSuffix): Modules/Basics/BooleanTrigger/BooleanTrigger.cpp $(IntermediateDirectory)/Modules_Basics_BooleanTrigger_BooleanTrigger.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/BooleanTrigger/BooleanTrigger.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_BooleanTrigger_BooleanTrigger.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_BooleanTrigger_BooleanTrigger.cpp$(DependSuffix): Modules/Basics/BooleanTrigger/BooleanTrigger.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_BooleanTrigger_BooleanTrigger.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_BooleanTrigger_BooleanTrigger.cpp$(DependSuffix) -MM Modules/Basics/BooleanTrigger/BooleanTrigger.cpp

$(IntermediateDirectory)/Modules_Basics_BooleanTrigger_BooleanTrigger.cpp$(PreprocessSuffix): Modules/Basics/BooleanTrigger/BooleanTrigger.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_BooleanTrigger_BooleanTrigger.cpp$(PreprocessSuffix) Modules/Basics/BooleanTrigger/BooleanTrigger.cpp

$(IntermediateDirectory)/Modules_Basics_Font_Font.cpp$(ObjectSuffix): Modules/Basics/Font/Font.cpp $(IntermediateDirectory)/Modules_Basics_Font_Font.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Font/Font.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Font_Font.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Font_Font.cpp$(DependSuffix): Modules/Basics/Font/Font.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Font_Font.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Font_Font.cpp$(DependSuffix) -MM Modules/Basics/Font/Font.cpp

$(IntermediateDirectory)/Modules_Basics_Font_Font.cpp$(PreprocessSuffix): Modules/Basics/Font/Font.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Font_Font.cpp$(PreprocessSuffix) Modules/Basics/Font/Font.cpp

$(IntermediateDirectory)/Modules_Basics_Sound_Sound.cpp$(ObjectSuffix): Modules/Basics/Sound/Sound.cpp $(IntermediateDirectory)/Modules_Basics_Sound_Sound.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Sound/Sound.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Sound_Sound.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Sound_Sound.cpp$(DependSuffix): Modules/Basics/Sound/Sound.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Sound_Sound.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Sound_Sound.cpp$(DependSuffix) -MM Modules/Basics/Sound/Sound.cpp

$(IntermediateDirectory)/Modules_Basics_Sound_Sound.cpp$(PreprocessSuffix): Modules/Basics/Sound/Sound.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Sound_Sound.cpp$(PreprocessSuffix) Modules/Basics/Sound/Sound.cpp

$(IntermediateDirectory)/Modules_Basics_ProjectOptions_ProjectOptions.cpp$(ObjectSuffix): Modules/Basics/ProjectOptions/ProjectOptions.cpp $(IntermediateDirectory)/Modules_Basics_ProjectOptions_ProjectOptions.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/ProjectOptions/ProjectOptions.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_ProjectOptions_ProjectOptions.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_ProjectOptions_ProjectOptions.cpp$(DependSuffix): Modules/Basics/ProjectOptions/ProjectOptions.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_ProjectOptions_ProjectOptions.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_ProjectOptions_ProjectOptions.cpp$(DependSuffix) -MM Modules/Basics/ProjectOptions/ProjectOptions.cpp

$(IntermediateDirectory)/Modules_Basics_ProjectOptions_ProjectOptions.cpp$(PreprocessSuffix): Modules/Basics/ProjectOptions/ProjectOptions.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_ProjectOptions_ProjectOptions.cpp$(PreprocessSuffix) Modules/Basics/ProjectOptions/ProjectOptions.cpp

$(IntermediateDirectory)/Modules_Basics_DigitalInput_DigitalInput.cpp$(ObjectSuffix): Modules/Basics/DigitalInput/DigitalInput.cpp $(IntermediateDirectory)/Modules_Basics_DigitalInput_DigitalInput.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/DigitalInput/DigitalInput.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_DigitalInput_DigitalInput.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_DigitalInput_DigitalInput.cpp$(DependSuffix): Modules/Basics/DigitalInput/DigitalInput.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_DigitalInput_DigitalInput.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_DigitalInput_DigitalInput.cpp$(DependSuffix) -MM Modules/Basics/DigitalInput/DigitalInput.cpp

$(IntermediateDirectory)/Modules_Basics_DigitalInput_DigitalInput.cpp$(PreprocessSuffix): Modules/Basics/DigitalInput/DigitalInput.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_DigitalInput_DigitalInput.cpp$(PreprocessSuffix) Modules/Basics/DigitalInput/DigitalInput.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProperties_DialogProperties.cpp$(ObjectSuffix): Modules/EditorOLD/DialogProperties/DialogProperties.cpp $(IntermediateDirectory)/Modules_EditorOLD_DialogProperties_DialogProperties.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/EditorOLD/DialogProperties/DialogProperties.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_EditorOLD_DialogProperties_DialogProperties.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_EditorOLD_DialogProperties_DialogProperties.cpp$(DependSuffix): Modules/EditorOLD/DialogProperties/DialogProperties.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_EditorOLD_DialogProperties_DialogProperties.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_EditorOLD_DialogProperties_DialogProperties.cpp$(DependSuffix) -MM Modules/EditorOLD/DialogProperties/DialogProperties.cpp

$(IntermediateDirectory)/Modules_EditorOLD_DialogProperties_DialogProperties.cpp$(PreprocessSuffix): Modules/EditorOLD/DialogProperties/DialogProperties.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_EditorOLD_DialogProperties_DialogProperties.cpp$(PreprocessSuffix) Modules/EditorOLD/DialogProperties/DialogProperties.cpp

$(IntermediateDirectory)/Modules_Basics_String_String.cpp$(ObjectSuffix): Modules/Basics/String/String.cpp $(IntermediateDirectory)/Modules_Basics_String_String.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/String/String.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_String_String.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_String_String.cpp$(DependSuffix): Modules/Basics/String/String.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_String_String.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_String_String.cpp$(DependSuffix) -MM Modules/Basics/String/String.cpp

$(IntermediateDirectory)/Modules_Basics_String_String.cpp$(PreprocessSuffix): Modules/Basics/String/String.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_String_String.cpp$(PreprocessSuffix) Modules/Basics/String/String.cpp

$(IntermediateDirectory)/Modules_Basics_Sequence_ActionTrack.cpp$(ObjectSuffix): Modules/Basics/Sequence/ActionTrack.cpp $(IntermediateDirectory)/Modules_Basics_Sequence_ActionTrack.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Sequence/ActionTrack.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Sequence_ActionTrack.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Sequence_ActionTrack.cpp$(DependSuffix): Modules/Basics/Sequence/ActionTrack.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Sequence_ActionTrack.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Sequence_ActionTrack.cpp$(DependSuffix) -MM Modules/Basics/Sequence/ActionTrack.cpp

$(IntermediateDirectory)/Modules_Basics_Sequence_ActionTrack.cpp$(PreprocessSuffix): Modules/Basics/Sequence/ActionTrack.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Sequence_ActionTrack.cpp$(PreprocessSuffix) Modules/Basics/Sequence/ActionTrack.cpp

$(IntermediateDirectory)/Modules_Basics_Sequence_Sequence.cpp$(ObjectSuffix): Modules/Basics/Sequence/Sequence.cpp $(IntermediateDirectory)/Modules_Basics_Sequence_Sequence.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Sequence/Sequence.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Sequence_Sequence.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Sequence_Sequence.cpp$(DependSuffix): Modules/Basics/Sequence/Sequence.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Sequence_Sequence.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Sequence_Sequence.cpp$(DependSuffix) -MM Modules/Basics/Sequence/Sequence.cpp

$(IntermediateDirectory)/Modules_Basics_Sequence_Sequence.cpp$(PreprocessSuffix): Modules/Basics/Sequence/Sequence.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Sequence_Sequence.cpp$(PreprocessSuffix) Modules/Basics/Sequence/Sequence.cpp

$(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_Debris.cpp$(ObjectSuffix): Modules/Spindizzy/DebrisGenerator/Debris.cpp $(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_Debris.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/DebrisGenerator/Debris.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_Debris.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_Debris.cpp$(DependSuffix): Modules/Spindizzy/DebrisGenerator/Debris.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_Debris.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_Debris.cpp$(DependSuffix) -MM Modules/Spindizzy/DebrisGenerator/Debris.cpp

$(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_Debris.cpp$(PreprocessSuffix): Modules/Spindizzy/DebrisGenerator/Debris.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_DebrisGenerator_Debris.cpp$(PreprocessSuffix) Modules/Spindizzy/DebrisGenerator/Debris.cpp

$(IntermediateDirectory)/Modules_Basics_Boolean_Boolean.cpp$(ObjectSuffix): Modules/Basics/Boolean/Boolean.cpp $(IntermediateDirectory)/Modules_Basics_Boolean_Boolean.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Boolean/Boolean.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Boolean_Boolean.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Boolean_Boolean.cpp$(DependSuffix): Modules/Basics/Boolean/Boolean.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Boolean_Boolean.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Boolean_Boolean.cpp$(DependSuffix) -MM Modules/Basics/Boolean/Boolean.cpp

$(IntermediateDirectory)/Modules_Basics_Boolean_Boolean.cpp$(PreprocessSuffix): Modules/Basics/Boolean/Boolean.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Boolean_Boolean.cpp$(PreprocessSuffix) Modules/Basics/Boolean/Boolean.cpp

$(IntermediateDirectory)/IsoRealms_GUI_TextEditorComponent.cpp$(ObjectSuffix): IsoRealms/GUI/TextEditorComponent.cpp $(IntermediateDirectory)/IsoRealms_GUI_TextEditorComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/IsoRealms/GUI/TextEditorComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IsoRealms_GUI_TextEditorComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IsoRealms_GUI_TextEditorComponent.cpp$(DependSuffix): IsoRealms/GUI/TextEditorComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IsoRealms_GUI_TextEditorComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IsoRealms_GUI_TextEditorComponent.cpp$(DependSuffix) -MM IsoRealms/GUI/TextEditorComponent.cpp

$(IntermediateDirectory)/IsoRealms_GUI_TextEditorComponent.cpp$(PreprocessSuffix): IsoRealms/GUI/TextEditorComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IsoRealms_GUI_TextEditorComponent.cpp$(PreprocessSuffix) IsoRealms/GUI/TextEditorComponent.cpp

$(IntermediateDirectory)/Modules_Basics_Script_ScriptType.cpp$(ObjectSuffix): Modules/Basics/Script/ScriptType.cpp $(IntermediateDirectory)/Modules_Basics_Script_ScriptType.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Script/ScriptType.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Script_ScriptType.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Script_ScriptType.cpp$(DependSuffix): Modules/Basics/Script/ScriptType.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Script_ScriptType.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Script_ScriptType.cpp$(DependSuffix) -MM Modules/Basics/Script/ScriptType.cpp

$(IntermediateDirectory)/Modules_Basics_Script_ScriptType.cpp$(PreprocessSuffix): Modules/Basics/Script/ScriptType.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Script_ScriptType.cpp$(PreprocessSuffix) Modules/Basics/Script/ScriptType.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Water_WaterSurface.cpp$(ObjectSuffix): Modules/Spindizzy/Water/WaterSurface.cpp $(IntermediateDirectory)/Modules_Spindizzy_Water_WaterSurface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Spindizzy/Water/WaterSurface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Spindizzy_Water_WaterSurface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Spindizzy_Water_WaterSurface.cpp$(DependSuffix): Modules/Spindizzy/Water/WaterSurface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Spindizzy_Water_WaterSurface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Spindizzy_Water_WaterSurface.cpp$(DependSuffix) -MM Modules/Spindizzy/Water/WaterSurface.cpp

$(IntermediateDirectory)/Modules_Spindizzy_Water_WaterSurface.cpp$(PreprocessSuffix): Modules/Spindizzy/Water/WaterSurface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Spindizzy_Water_WaterSurface.cpp$(PreprocessSuffix) Modules/Spindizzy/Water/WaterSurface.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_ScreenRelative.cpp$(ObjectSuffix): Modules/Basics/HUD/ScreenRelative.cpp $(IntermediateDirectory)/Modules_Basics_HUD_ScreenRelative.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/HUD/ScreenRelative.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_HUD_ScreenRelative.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_HUD_ScreenRelative.cpp$(DependSuffix): Modules/Basics/HUD/ScreenRelative.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_HUD_ScreenRelative.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_HUD_ScreenRelative.cpp$(DependSuffix) -MM Modules/Basics/HUD/ScreenRelative.cpp

$(IntermediateDirectory)/Modules_Basics_HUD_ScreenRelative.cpp$(PreprocessSuffix): Modules/Basics/HUD/ScreenRelative.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_HUD_ScreenRelative.cpp$(PreprocessSuffix) Modules/Basics/HUD/ScreenRelative.cpp

$(IntermediateDirectory)/Modules_Basics_Colour_Colour.cpp$(ObjectSuffix): Modules/Basics/Colour/Colour.cpp $(IntermediateDirectory)/Modules_Basics_Colour_Colour.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/martin/spindizzyremake/Modules/Basics/Colour/Colour.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Modules_Basics_Colour_Colour.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Modules_Basics_Colour_Colour.cpp$(DependSuffix): Modules/Basics/Colour/Colour.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Modules_Basics_Colour_Colour.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Modules_Basics_Colour_Colour.cpp$(DependSuffix) -MM Modules/Basics/Colour/Colour.cpp

$(IntermediateDirectory)/Modules_Basics_Colour_Colour.cpp$(PreprocessSuffix): Modules/Basics/Colour/Colour.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Modules_Basics_Colour_Colour.cpp$(PreprocessSuffix) Modules/Basics/Colour/Colour.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


