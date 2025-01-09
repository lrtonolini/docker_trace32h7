@echo off

set "cheminExe=.\TRACE32\bin\windows64\drivers\setup_t32_usb_driver.exe"
set "marqueurInstallation=.\TRACE32\bin\windows64\drivers\config_driver.txt"

rem Vérifier si le marqueur d'installation existe
if not exist "%marqueurInstallation%" (
    rem Le marqueur n'existe pas, donc l'installation n'a pas encore eu lieu

    rem Vérifier si le fichier exécutable existe
    if not exist "%cheminExe%" (
        echo Fichier du driver introuvable.
     ) else (
        rem Demander à l'utilisateur s'il souhaite installer les drivers
        choice /C yn /M "Do you want to install drivers ? (y/n): "
        if errorlevel 2 (
            echo Installation cancelled.
        ) else (
            echo Drivers installation in progress...
            start /wait "" "%cheminExe%"
            echo Drivers installation complete.

            rem Créer le marqueur pour indiquer que l'installation a eu lieu
            echo Drivers installation completed > "%marqueurInstallation%"
        )
    )
) else (
    echo The drivers are already installed.
    
)


start /B .\TRACE32\bin\windows64\t32marm.exe
start /B notepad README_WINDOWS.md 
exit 