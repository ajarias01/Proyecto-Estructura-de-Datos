[Setup]
AppName=SistemaBancario
AppVersion=1.0
DefaultDirName={pf}\SistemaBancario
DefaultGroupName=SistemaBancario
OutputBaseFilename=Instalador_SistemaBancario
Compression=lzma
SolidCompression=yes

[Files]
Source: "C:\Users\MEGAPC\Pictures\Deberes\Proyecto-Estructura-de-Datos\main.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\SistemaBancario"; Filename: "{app}\SistemaBancario.exe"

[Code]
var
  SerialPage: TInputQueryWizardPage;

function CheckSerial(Serial: String): Boolean;
begin
  Result :=
    (Serial = 'SU6G-2EYA-UPC8-0LM6') or
    (Serial = 'PWQN-KI6E-WLQL-R0D1') or
    (Serial = 'TF2F-X70R-ECQZ-KOVC') or
    (Serial = 'YU2W-40YP-KKMG-0PT1') or
    (Serial = '75DE-SRWT-CAGS-L4C7');
end;

procedure InitializeWizard;
begin
  SerialPage := CreateInputQueryPage(wpWelcome, 'Validación de Serial', 
    'Por favor, ingresa la clave de instalación para continuar.', 
    'Ingresa una de las claves proporcionadas por el desarrollador para instalar el software.');

  SerialPage.Add('Clave de instalación:', False);
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