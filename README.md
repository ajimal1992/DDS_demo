# DDS demo
## File Structure

    |-Edge
        |-fastrtps_serial-RPI
        |-Firmwares
            |-smartcity_lampposts
            |-smartcity_sensors
            |-smartcity_trafficlight
    |-UI
        |-rtps-dds-connector
        |-smartcity_server
        
### Edge
The Edge would mean the r-pi. The Firmwares folder contains all the the firmwares of the r-pi.

Compile

    ./Edge/fastrtps_serial-RPI/compile.sh
Start up Publisher

    ./Edge/fastrtps_serial-RPI/build/app publisher
Start up Subscriber

    ./Edge/fastrtps_serial-RPI/build/app subscriber
    
### UI
The UI is any node(server) which has fast-rtps and nodejs installed. 

Start up node JS server 

    node UI/smartcity_server/serverDDS.js


    
