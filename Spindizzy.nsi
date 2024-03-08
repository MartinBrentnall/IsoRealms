!include "MUI2.nsh"
!include "logiclib.nsh"
!include "FileFunc.nsh"

!define NAME "IsoRealms-Spindizzy"
!define APPFILE "Spindizzy.cmd"
!define VERSION "0.6"
!define SLUG "${NAME}-v${VERSION}"

Name "${NAME}"
OutFile "${NAME}-v${VERSION}-Setup.exe"
InstallDir "$PROGRAMFILES\${NAME}"
RequestExecutionLevel admin

!define MUI_ICON "Spindizzy.ico"
!define MUI_HEADERIMAGE
!define MUI_WELCOMEFINISHPAGE_BITMAP "Welcome.bmp"
!define MUI_HEADERIMAGE_BITMAP "Header.bmp"
!define MUI_ABORTWARNING
!define MUI_WELCOMEPAGE_TITLE "${SLUG} Setup"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "Licence.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"

Section "-hidden app"
  SectionIn RO
  SetOutPath "$INSTDIR"
  File /r "out\build\x64-Debug\Out\*.*"
  WriteRegStr HKCU "Software\${NAME}" "" $INSTDIR
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "DisplayName" "${NAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "UninstallString" "$\"$INSTDIR\Uninstall.exe$\""
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "Publisher" "Martin Brentnall"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "DisplayVersion" "${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "DisplayIcon" "$INSTDIR\Spindizzy.ico"
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
  IntFmt $0 "0x%08X" $0
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "EstimatedSize" "$0"  
SectionEnd

Section "Desktop Shortcut" DeskShort
  CreateShortCut "$DESKTOP\Spindizzy.lnk" "$INSTDIR\${APPFILE}" "" "$INSTDIR\Spindizzy.ico" 0
SectionEnd

Section "Start Menu Shortcut" SMShort
  CreateDirectory "$SMPROGRAMS\Spindizzy"
  CreateShortCut "$SMPROGRAMS\Spindizzy\Spindizzy.lnk" "$INSTDIR\${APPFILE}" "" "$INSTDIR\Spindizzy.ico" 0
SectionEnd

LangString DESC_DeskShort ${LANG_ENGLISH} "Create Shortcut on Desktop."
LangString DESC_SMShort ${LANG_ENGLISH} "Create Shortcut in Start Menu."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${DeskShort} $(DESC_DeskShort)
  !insertmacro MUI_DESCRIPTION_TEXT ${SMShort} $(DESC_SMShort)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Function un.RMDirUP
  !define RMDirUP '!insertmacro RMDirUPCall'

  !macro RMDirUPCall _PATH
    push '${_PATH}'
    Call un.RMDirUP
  !macroend

  ; $0 - current folder
  ClearErrors

  Exch $0
  ;DetailPrint "ASDF - $0\.."
  RMDir "$0\.."

  IfErrors Skip
  ${RMDirUP} "$0\.."
  Skip:

  Pop $0
FunctionEnd

Section "Uninstall"
  Delete "$DESKTOP\Spindizzy.lnk"
  Delete "$SMPROGRAMS\Spindizzy\Spindizzy.lnk"
  Delete "$INSTDIR\Uninstall.exe"
  RMDir /r "$SMPROGRAMS\Spindizzy"
  RMDir /r "$INSTDIR"
  ${RMDirUP} "$INSTDIR"
  DeleteRegKey /ifempty HKCU "Software\${NAME}"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}"
SectionEnd

