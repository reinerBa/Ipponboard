; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#include "defines.def"

#define MyAppVersion GetFileVersion('..\_build\build_output\~tmp\Ipponboard.exe')
;#define MyAppVersion GetEnv("IPPONBOARD_VERSION")
#if len(MyAppVersion) < 1
  #pragma warning "IPPONBOARD_VERSION not defined"
  #define MyAppVersion "0.0.0.0"
#endif
#define MySimpleAppVersion SimpleVersion(MyAppVersion)

#pragma message "*** Version info ***
#pragma message "Detailed version info: " + MyAppVersion
#pragma message "Simple version info:   " + MySimpleAppVersion

#define MyAppName "Ipponboard"
#define MyAppCopyright "2010-2013 Florian M�cke"
#define MyAppAuthor "Florian M�cke"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppID={{1E604DB1-15D5-43E9-9FEE-B3A5C8387449}
AppName={#MyAppName}
AppVersion={#MySimpleAppVersion}
AppVerName={#MyAppName} {#MySimpleAppVersion}
AppPublisher={#MyAppAuthor}
AppPublisherURL=http://www.ipponboard.info/
AppSupportURL=http://www.ipponboard.info/
AppUpdatesURL=http://www.ipponboard.info/
AppCopyright=Copyright (C) {#MyAppCopyright}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=no
AllowNoIcons=false
;OutputBaseFilename={#MyAppName}-{#MySimpleAppVersion}-Setup
OutputBaseFilename={#MyAppName}-{#MyAppVersion}-setup
Compression=lzma2/Ultra
SolidCompression=true
InternalCompressLevel=Ultra
MinVersion=,5.01.2600sp1
ShowLanguageDialog=yes
;WizardImageFile=compiler:wizmodernimage-IS.bmp
WizardImageFile=images\install_bg.bmp
WizardSmallImageFile=images\logo_small.bmp
VersionInfoVersion={#MyAppVersion}
VersionInfoProductName={#MyAppName}
VersionInfoProductVersion={#MyAppVersion}
VersionInfoCopyright={#MyAppCopyright}
VersionInfoCompany={#MyAppAuthor}
AlwaysUsePersonalGroup=false
AppendDefaultGroupName=false
AlwaysShowDirOnReadyPage=true
AlwaysShowGroupOnReadyPage=true
UninstallDisplayIcon={app}\Ipponboard.exe
Uninstallable=not IsPortable

[Languages]
;Name: "en"; MessagesFile: "compiler:Default.isl,compiler:MyMessages.isl"
Name: "en"; MessagesFile: "compiler:Default.isl"; LicenseFile: "License_en.rtf"
Name: "de"; MessagesFile: "compiler:Languages\German.isl"; LicenseFile: "License_de.rtf"

[CustomMessages]
de.ViewProgram=%1 �ffnen
en.ViewProgram=Open %1
;de.Survey=Meinungsumfrage
;en.Survey=Feedback/Survey
de.InstallType_Title=Installationsart w�hlen
en.InstallType_Title=Choose Installation Type
de.InstallType_SubTitle=W�hlen Sie aus, wie Sie die Software installieren m�chten.
en.InstallType_SubTitle=Select, how the software should be installed.
de.InstallType_Message=M�chten Sie mit einer normalen Installation fortfahren, oder w�rden Sie eine portable Installation (z.B. auf einem USB-Stick) bevorzugen?
en.InstallType_Message=Would you like to proceed with a normal installation or would you prefer a portable installation (e.g. install files on an usb stick)?
de.InstallType_TextNormal=Normal (mit Verkn�pfungen und Deinstallation)
en.InstallType_TextNormal=Normal (with shortcuts and uninstall)
de.InstallType_TextPortable=Portabel (keine weiteren Informationen werden gespeichert)
en.InstallType_TextPortable=Portable (no extra information will be stored)
de.InstallType_InfoTitle=Art der Installation
en.InstallType_InfoTitle=Installation type
de.InstallType_Normal=Normal
en.InstallType_Normal=Normal
de.InstallType_Portable=Portabel
en.InstallType_Portable=Portable
de.UninstallKeepSettings=M�chten Sie die Programmeinstellungen f�r eine sp�tere Installation aufheben?
en.UninstallKeepSettings=Do you want to keep your settings for a later installation?

[Tasks]
Name: "desktopicon"; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked; Check: NOT IsPortable

[Files]
Source: ..\_build\build_output\~tmp\Ipponboard.exe; DestDir: {app}; Check: "NOT IsPortable"; Flags: IgnoreVersion replacesameversion; 
Source: ..\_build\build_output\~tmp\Ipponboard.exe; DestDir: {app}; Check: IsPortable; DestName: Ipponboard-portable.exe; Flags: IgnoreVersion replacesameversion; 
;Source: ..\_build\build_output\~tmp\categories.xml; DestDir: {commonappdata}\Ipponboard; Flags: ignoreversion confirmoverwrite uninsneveruninstall; Check: NOT IsPortable 
;Source: ..\_build\build_output\~tmp\categories.xml; DestDir: {app}; Flags: ignoreversion confirmoverwrite replacesameversion; Check: IsPortable; Permissions: users-full; 
Source: ..\_build\build_output\~tmp\categories.xml; DestDir: {app}; Flags: ignoreversion confirmoverwrite replacesameversion; Permissions: users-full;
Source: ..\_build\build_output\~tmp\sounds\*.*; DestDir: {app}\sounds\; Flags: ignoreversion replacesameversion; 
Source: ..\_build\build_output\~tmp\Anleitung.pdf; DestDir: {app}; Flags: ignoreversion replacesameversion; 
Source: ..\_build\build_output\~tmp\manual.pdf; DestDir: {app}; Flags: ignoreversion replacesameversion; 
Source: ..\_build\build_output\~tmp\GamepadDemo.exe; DestDir: {app}; Flags: IgnoreVersion replacesameversion; 
Source: ..\_build\build_output\~tmp\lang\core_de.qm; DestDir: {app}\lang\; Flags: IgnoreVersion replacesameversion; 
Source: ..\_build\build_output\~tmp\lang\Ipponboard_de.qm; DestDir: {app}\lang\; Flags: IgnoreVersion replacesameversion; 
Source: ..\_build\build_output\~tmp\msvcp100.dll; DestDir: {app}; Flags: IgnoreVersion replacesameversion; 
Source: ..\_build\build_output\~tmp\msvcr100.dll; DestDir: {app}; Flags: IgnoreVersion replacesameversion; 
Source: ..\_build\build_output\~tmp\QtCore4.dll; DestDir: {app}; Flags: IgnoreVersion replacesameversion; 
Source: ..\_build\build_output\~tmp\QtGui4.dll; DestDir: {app}; Flags: IgnoreVersion replacesameversion; 

[Dirs]
;Name: {commonappdata}\Ipponboard; Permissions: users-full; Check: "NOT IsPortable" 
Name: {app}; Permissions: users-full; 
Name: {app}\lang;
Name: {app}\sounds; 

[Icons]
Name: "{group}\{#MyAppName}"; Filename: {app}\Ipponboard.exe; WorkingDir: {app}; Check: NOT IsPortable
Name: "{group}\Anleitung"; Filename: {app}\Anleitung.pdf; WorkingDir: {app}; Check: NOT IsPortable
Name: "{group}\Manual"; Filename: {app}\manual.pdf; WorkingDir: {app}; Languages: en; Check: NOT IsPortable
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: http://www.ipponboard.info/; Check: NOT IsPortable
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: {uninstallexe}; Check: NOT IsPortable
Name: "{group}\Umfrage"; Filename: "http://flo.mueckeimnetz.de/ipponboard/survey_de/"; Languages: de; Check: NOT IsPortable
Name: "{group}\Online-Survey (Feedback)"; Filename: "http://flo.mueckeimnetz.de/ipponboard/survey_en/"; Languages: en; Check: NOT IsPortable
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\Ipponboard.exe"; Tasks: desktopicon; IconFilename: {app}\Ipponboard.exe; WorkingDir: {app}; Check: NOT IsPortable

[Run]
Filename: "{app}\Ipponboard.exe"; Description: {cm:LaunchProgram,{#MyAppName}}; Flags: nowait postinstall skipifsilent unchecked; WorkingDir: {app}; Check: NOT IsPortable
Filename: "{app}\Anleitung.pdf"; Description: {cm:ViewProgram,Anleitung}; Flags: shellexec nowait postinstall skipifsilent; WorkingDir: {app}; Languages: de; Check: NOT IsPortable
Filename: "{app}\Manual.pdf"; Description: {cm:ViewProgram,manual}; Flags: shellexec nowait postinstall skipifsilent; WorkingDir: {app}; Languages: en; Check: NOT IsPortable

[Registry]
Root: HKLM; Subkey: "Software\{#MyAppName}"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"; Flags: deletekey; Check: NOT IsPortable
Root: HKLM; Subkey: "Software\{#MyAppName}"; ValueType: string; ValueName: "InstalledVersion"; ValueData: "{#MySimpleAppVersion}"; Flags: deletekey; Check: NOT IsPortable
;Root: HKCU; Subkey: "Software\{#MyAppName}"; ValueType: string; ValueName: "Language"; ValueData: "de"; Flags: uninsdeletekeyifempty; Languages: de
;Root: HKCU; Subkey: "Software\{#MyAppName}"; ValueType: string; ValueName: "Language"; ValueData: "en"; Flags: uninsdeletekeyifempty; Languages: en

[INI]
Filename: {app}\Ipponboard.ini; Section: Main; Key: Language; String: de; Languages: de; 
Filename: {app}\Ipponboard.ini; Section: Main; Key: Language; String: en; Languages: en; 
;Filename: {commonappdata}\Ipponboard\Ipponboard.ini; Section: Main; Key: Language; String: de; Check: "NOT IsPortable"; Languages: de; 
;Filename: {commonappdata}\Ipponboard\Ipponboard.ini; Section: Main; Key: Language; String: en; Check: "NOT IsPortable"; Languages: en; 

[Code]
var
  UsagePage: TInputOptionWizardPage;
  
procedure InitializeWizard;
begin
  { Create the pages }

  UsagePage := CreateInputOptionPage(wpLicense,
    ExpandConstant('{cm:InstallType_Title}'), 
    ExpandConstant('{cm:InstallType_SubTitle}'),
    ExpandConstant('{cm:InstallType_Message}'),
    True, False);
 
  UsagePage.Add(ExpandConstant('{cm:InstallType_TextNormal}'));
  UsagePage.Add(ExpandConstant('{cm:InstallType_TextPortable}'));

  case GetPreviousData('UsageMode', '') of
    'normal': UsagePage.SelectedValueIndex := 0;
    'portable': UsagePage.SelectedValueIndex := 1;
  else
    UsagePage.SelectedValueIndex := 0;
  end;
end;


function IsPortable(): Boolean;
begin
  Result := UsagePage.SelectedValueIndex = 1;
end;


function ShouldSkipPage(PageID: Integer): Boolean;
begin
  { Skip pages that shouldn't be shown }
  if (PageID = wpSelectProgramGroup) then 
  begin
    WizardForm.NoIconsCheck.Checked := IsPortable();
    Result := IsPortable();
  end else
  begin
    Result := False;
  end
end;

function UpdateReadyMemo(Space, NewLine, MemoUserInfoInfo, MemoDirInfo, MemoTypeInfo,
  MemoComponentsInfo, MemoGroupInfo, MemoTasksInfo: String): String;
var
  S: String;
begin
  { Fill the 'Ready Memo' with the normal settings and the custom settings }
  S := '';
  S := S + ExpandConstant('{cm:InstallType_InfoTitle}') + ':' + NewLine + Space;
  case UsagePage.SelectedValueIndex of
    0: S := S + ExpandConstant('{cm:InstallType_Normal}');
    1: S := S + ExpandConstant('{cm:InstallType_Portable}');
  end;
  S := S + NewLine;
  S := S + NewLine + MemoDirInfo + NewLine;
  S := S + NewLine + MemoGroupInfo + NewLine;
  S := S + NewLine + MemoTasksInfo + NewLine;
  Result := S;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  case CurUninstallStep of
    usUninstall:
      begin
        ;if (MsgBox(ExpandConstant('{cm:UninstallKeepSettings}'), mbConfirmation, mb_YesNo) = idNo) then
        ;begin
          DelTree(ExpandConstant('{commonappdata}\Ipponboard'),True,True,True);
          DelTree(ExpandConstant('{app}'),True,True,True);
        ;end;
      end;
    usPostUninstall:
      begin
        // ...insert code to perform pre-uninstall tasks here...
      end;
  end;
end;

function GetFileVersion(filename : String): String;
begin
    GetVersionNumbersString(filename, Result);
end;

