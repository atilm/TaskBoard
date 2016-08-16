; Installer creation script for the
; "nullsoft scriptable install system"
; http://nsis.sourceforge.net/

!define VERSION "0.01.000"

;--------------------------------

; The name of the installer
Name "Task Board ${VERSION}"

; The file to write
OutFile "Install TaskBoard ${VERSION}.exe"

; The default installation directory
InstallDir $PROGRAMFILES\TaskBoard

; Request application privileges for Windows Vista
RequestExecutionLevel admin

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\TaskBoard" "Install_Dir"

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "Task Board"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "TaskBoard\release\TaskBoard.exe"
  File "C:\Qt\5.5\mingw492_32\bin\libgcc_s_dw2-1.dll"
  File "C:\Qt\5.5\mingw492_32\bin\libstdc++-6.dll"
  File "C:\Qt\5.5\mingw492_32\bin\libwinpthread-1.dll"
  File "C:\Qt\5.5\mingw492_32\bin\Qt5Core.dll"
  File "C:\Qt\5.5\mingw492_32\bin\Qt5Gui.dll"
  File "C:\Qt\5.5\mingw492_32\bin\Qt5PrintSupport.dll"
  File "C:\Qt\5.5\mingw492_32\bin\Qt5Sql.dll"
  File "C:\Qt\5.5\mingw492_32\bin\Qt5Widgets.dll"
  
  CreateDirectory "$INSTDIR\platforms\"
  SetOutPath "$INSTDIR\platforms\"
  File "C:\Qt\5.5\mingw492_32\plugins\platforms\"
  
SectionEnd

Section "Create Uninstaller"
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\TaskBoard "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TaskBoard" "DisplayName" "TaskBoard_v${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TaskBoard" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TaskBoard" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TaskBoard" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Task Board"
  CreateShortcut "$SMPROGRAMS\Task Board\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortcut "$SMPROGRAMS\Task Board\Task Board.lnk" "$INSTDIR\TaskBoard.exe" "" "$INSTDIR\TaskBoard.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TaskBoard"
  DeleteRegKey HKLM SOFTWARE\TaskBoard

  ; Remove files and uninstaller
  Delete "$INSTDIR\*.*"
  Delete "$INSTDIR\platforms\*.*"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Task Board\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\Task Board"
  RMDir "$INSTDIR\platforms"
  RMDir "$INSTDIR"
  Delete "$APPDATA\TaskBoard\tasks.db"
  RMDir "$APPDATA\TaskBoard"

SectionEnd
