 ### Airskin with Rpi
 
 * Connect to the Rpi via ssh (Raspdberry has a static IP and must be connected to the robots network:
 
    ```ssh bdr@192.168.10.161 -p12345```
 * Set the ROS_MASTER_URI and ROS_IP:
 
    ```export ROS_MASTER_URI=http://192.168.10.44:11311```
    
    ```export ROS_IP=192.168.10.161```
    
  * Start the node
  
    ```roslaunch tuw_airskin_rpi airskin_node.launch```
    
  Dont mind the error message in the very beginning if it only occurs once.