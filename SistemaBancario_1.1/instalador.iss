[Setup]
AppName=SistemaBancario
AppVersion=1.0
; Usar Documents del usuario para evitar problemas de permisos
DefaultDirName={userdocs}\SistemaBancario
DefaultGroupName=SistemaBancario
OutputBaseFilename=Instalador_SistemaBancario
Compression=lzma
SolidCompression=yes
; Solicitar permisos de administrador si es necesario
PrivilegesRequired=lowest
; Permitir al usuario cambiar el directorio de instalación
DisableDirPage=no

[Files]
Source: "SFML-3.0.0\bin\sfml-graphics-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "SFML-3.0.0\bin\sfml-window-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "SFML-3.0.0\bin\sfml-system-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "citas_agendadas.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "main.exe"; DestDir: "{app}"; DestName: "SistemaBancario.exe"; Flags: ignoreversion
Source: "datos.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "backup_clientes_20250709_183846.bin"; DestDir: "{app}"; Flags: ignoreversion
Source: "cifrado_20250708_205659.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "decifrado_20250613_121652.bin"; DestDir: "{app}"; Flags: ignoreversion
Source: "AyudaTecnicaBanco.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: "SistemaBancario_Documentation.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: "hash_stored.txt"; DestDir: "{app}"; Flags: ignoreversion
[Icons]
Name: "{group}\SistemaBancario"; Filename: "{app}\SistemaBancario.exe"

[Code]
var
  SerialPage: TInputQueryWizardPage;

function CheckSerial(Serial: String): Boolean;
begin
  Result :=
    (Serial = 'Q1WX-3T8Z-PLKM-2X4V') or
    (Serial = '7AGD-KE32-YT9L-WQ1C') or
    (Serial = 'BN7M-TK5X-LZ9V-2U1P') or
    (Serial = 'JX2Z-QWER-ASDF-ZXCV') or
    (Serial = 'R8UY-MNBV-TR56-KLOP');
end;

procedure InitializeWizard;
begin
  SerialPage := CreateInputQueryPage(wpWelcome,
    'Validacion de Serial',
    'Por favor, ingresa la clave de instalacion para continuar.',
    'Ingresa una de las claves proporcionadas por el desarrollador para instalar el software.');
  SerialPage.Add('Clave de instalacion:', False);
end;

function NextButtonClick(CurPageID: Integer): Boolean;
begin
  Result := True;
  if CurPageID = SerialPage.ID then
  begin
    if not CheckSerial(SerialPage.Values[0]) then
    begin
      MsgBox('Clave incorrecta. Por favor, verifica e intenta nuevamente.', mbError, MB_OK);
      Result := False;
    end;
  end;
end;