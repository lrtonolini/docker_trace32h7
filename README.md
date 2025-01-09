README 

<<<<<<< HEAD
----------sous host Windows avec WSL2 : 
=======
----------only for host Windows with WSL2 : 
>>>>>>> 725ba98f9e8f8cf728c80e37071d9cbd9b58ecd7



please make commande : 

<<<<<<< HEAD
setx MNTWSL "/mnt/c/<your path>/docker_trace32h7"
=======
setx WSLENV "/mnt/c/your path/docker_trace32h7"
>>>>>>> 725ba98f9e8f8cf728c80e37071d9cbd9b58ecd7

run .\connect_lauterbach.bat

Avant de stoper le docker, quitter Trace32 pour reprendre la main sur la sonde 

Sinon, veillez faire un power cycle de la carte et de la sonde 


----------Sous host linux : 

export MNTWSL="/mnt/c/your path/docker_trace32h7"

run ./startup.sh
