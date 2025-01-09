#!/bin/bash
# Vérifier si les drivers sont déjà installés

if [ -f "./TRACE32/bin/pc_linux64/udev.conf/config_driver.txt" ]; then

    echo "The drivers are already installed."

else

    # Demander à l'utilisateur s'il veut installer les pilotes

    read -p "Do you want to install drivers? (y/n): " choice



    if [ "$choice" = "y" ]; then

        # Exécuter le script d'installation des pilotes

        ./TRACE32/bin/pc_linux64/udev.conf/setup_udevrules.sh  

        echo "Drivers are now installed." > ./TRACE32/bin/pc_linux64/udev.conf/config_driver.txt
    else
        echo "Installation cancelled."
    fi

fi
# Afficher le fichier README

gedit README_LINUX.md &

# Lancer l'application

./TRACE32/bin/pc_linux64/t32marm -c ./TRACE32/config.t32



