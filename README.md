
README

----------only for host Windows with WSL2 :

please make commande :

setx MNTWSL "/mnt/c/your path/docker_trace32h7"

run .\connect_lauterbach.bat

Avant de stoper le docker, quitter Trace32 pour reprendre la main sur la sonde

Sinon, veillez faire un power cycle de la carte et de la sonde

----------Sous host linux :

export MNTWSL="/mnt/c/your path/docker_trace32h7"

run ./startup.sh