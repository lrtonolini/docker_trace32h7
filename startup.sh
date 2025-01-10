#!/bin/bash

RULES_FILE="/etc/udev/rules.d/10-lauterbach.rules"

# Installation script
SETUP_SCRIPT="$MNTWSL/Trace32_H7/TRACE32/bin/pc_linux64/udev.conf/setup_udevrules.sh"

# Check if the file exists
if [ -f "$RULES_FILE" ]; then
    echo "The drivers are already installed. No changes needed."
else
    echo "The drivers are not installed. Proceeding with installation..."
    # Run the installation script
    bash "$SETUP_SCRIPT"
    if [ $? -eq 0 ]; then
        echo "Driver installation completed successfully."
    else
        echo "Error during driver installation."
        exit 1
    fi
fi

IMAGE_NAME="trace32"
CONTAINER_NAME="t32"

# Check if the image actually exists
if ! docker inspect "$IMAGE_NAME" > /dev/null 2>&1; then
    echo "Image $IMAGE_NAME not found. Building the image..."
    docker build --tag "$IMAGE_NAME" --build-arg T32=Trace32_H7 .
else
    echo "Image $IMAGE_NAME already exists."
fi

# Check if the container already exists
if docker ps -a --filter "name=$CONTAINER_NAME" | grep -q "$CONTAINER_NAME"; then
    echo "Container $CONTAINER_NAME already exists. Restarting the container..."
    docker start "$CONTAINER_NAME"
else
    # If the container doesn't exist, ask the user if they have hardware connected
    read -p "Do you have hardware connected? (y/n): " response

    # Check if the user has hardware
    if [[ "$response" =~ ^[Yy]$ ]]; then
        echo "You have hardware. Starting the container with device..."
        docker run -d --device=/dev/lauterbach/trace32/1-1 --publish 127.0.0.1:3000:3000 --name "$CONTAINER_NAME" "$IMAGE_NAME"
    elif [[ "$response" =~ ^[Nn]$ ]]; then
        echo "You do not have hardware. Starting the container without device..."
        docker run -d --publish 127.0.0.1:3000:3000 --name "$CONTAINER_NAME" "$IMAGE_NAME"
    else
        echo "Invalid response. Please respond with 'y' or 'n'."
    fi
fi

# Wait 5 seconds for the container to start
sleep 5

# Run the script inside the container
docker exec "$CONTAINER_NAME" "/opt/Trace32_H7/TRACE32/bin/pc_linux64/t32marm"

# Wait for the user to stop the container
echo "Press [Enter] to stop the container..."
read -r

docker stop "$CONTAINER_NAME"
echo "Container $CONTAINER_NAME stopped."
