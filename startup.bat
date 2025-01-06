@echo off
set IMAGE_NAME=trace32
set CONTAINER_NAME=t32

:: Vérifier si l'image existe réellement
docker inspect %IMAGE_NAME% >nul 2>&1
if errorlevel 1 (
    echo Image %IMAGE_NAME% non trouvee. Construction de l'image...
    docker build --no-cache --tag %IMAGE_NAME% --build-arg T32=Trace32_H7 .
) else (
    echo L'image %IMAGE_NAME% existe deja.
)

:: Vérifier si le conteneur existe déjà
docker ps -a --filter "name=%CONTAINER_NAME%" | findstr %CONTAINER_NAME% >nul
if errorlevel 1 (
    echo Conteneur %CONTAINER_NAME% non trouve. Lancement du conteneur...
    docker run -d --publish 127.0.0.1:3000:3000 --name %CONTAINER_NAME% %IMAGE_NAME%
) else (
    echo Le conteneur %CONTAINER_NAME% existe deja. Redemarrage du conteneur...
    docker start %CONTAINER_NAME%
)

:: Attendre 5 secondes pour que le conteneur démarre
timeout /t 5

:: Exécuter le script dans le conteneur
docker exec -it %CONTAINER_NAME% "/opt/Trace32_H7/Trace32_H7_linux.sh"

:: Attendre que l'utilisateur arrête le conteneur
echo Appuyez sur une touche pour arrêter le conteneur...
pause >nul
docker stop %CONTAINER_NAME%
echo Conteneur %CONTAINER_NAME% arrete.
