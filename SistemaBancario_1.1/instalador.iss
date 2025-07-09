[Setup]
AppName=SistemaBancario
AppVersion=1.0
DefaultDirName={pf}\SistemaBancario
DefaultGroupName=SistemaBancario
OutputBaseFilename=Instalador_SistemaBancario
Compression=lzma
SolidCompression=yes

[Files]
Source: "main.exe"; DestDir: "{app}"; DestName: "SistemaBancario.exe"; Flags: ignoreversion
Source: "datos.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "backup_clientes_20250601_135712.bak"; DestDir: "{app}"; Flags: ignoreversion
Source: "backup_clientes_20250601_135712_cifrado.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "backup_clientes_20250601_135712_descifrado.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "respaldo_clientes.bin"; DestDir: "{app}"; Flags: ignoreversion
Source: "AyudaTecnicaBanco.chm"; DestDir: "{app}"; Flags: ignoreversion
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
    'Validaci�n de Serial',
    'Por favor, ingresa la clave de instalaci�n para continuar.',
    'Ingresa una de las claves proporcionadas por el desarrollador para instalar el software.');
  SerialPage.Add('Clave de instalaci�n:', False);
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