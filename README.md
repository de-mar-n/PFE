# PFE : Voiture autonome

* DESCRIPTION:

Ce projet est divise en 2 parties:
    - Synchronisation des capteurs
    - Path-planning via des noeuds ROS

* LAUNCH:

Pour lancer le noeud ROS associe au RPLIDAR:
Depuis le workspace faire les commandes suivantes:

catkin_make
source devel/setup.sh
roslaunch rplidar_ros view_rplidar.launch

Si un probleme intervient au niveau du tty faire la commande suivante:

sudo chmod 666 /dev/ttyUSB0
