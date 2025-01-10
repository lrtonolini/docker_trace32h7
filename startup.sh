#!/bin/bash

sudo $MNTWSL/Trace32_H7/TRACE32/bin/pc_linux64/udev.conf/setup_udevrules.sh

IMAGE_NAME="trace32"
CONTAINER_NAME="t32"

# Vérifier si l'image existe réellement
if ! docker inspect "$IMAGE_NAME" > /dev/null 2>&1; then
    echo "Image $IMAGE_NAME non trouvee. Construction de l'image..."
    docker build --tag "$IMAGE_NAME" --build-arg T32=Trace32_H7 .
else
    echo "L'image $IMAGE_NAME existe deja."
fi

# Vérifier si le conteneur existe déjà
if ! docker ps -a --filter "name=$CONTAINER_NAME" | grep -q "$CONTAINER_NAME"; then
    echo "Conteneur $CONTAINER_NAME non trouve. Lancement du conteneur..."
    docker run -d --device=/dev/lauterbach/trace32/1-1 --publish 127.0.0.1:3000:3000 --name "$CONTAINER_NAME" "$IMAGE_NAME"
else
    echo "Le conteneur $CONTAINER_NAME existe deja. Redemarrage du conteneur..."
    docker start "$CONTAINER_NAME"
fi

# Attendre 5 secondes pour que le conteneur démarre
sleep 5

# Exécuter le script dans le conteneur
docker exec "$CONTAINER_NAME" "/opt/Trace32_H7/TRACE32/bin/pc_linux64/t32marm"

# Attendre que l'utilisateur arrête le conteneur
echo "Appuyez sur [Entrée] pour arrêter le conteneur..."
read -r

docker stop "$CONTAINER_NAME"
echo "Conteneur $CONTAINER_NAME arrete."

