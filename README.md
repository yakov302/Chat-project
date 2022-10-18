# Chat-project
Chat application that supports multiple users, rooms and private chats simultaneously.  TCP &amp; UDP network protocols, sha256 password encryption and Multithreading.

Requirements: linux (ubuntu) virtualization (GUI) enable.
  
1. Run the server:   
    a. set server ip & port:

          go to file - Chat-project/server/resources/server_ip_and_port.txt 
          Write in first line - server ip.
          Write in second line -server port.   
    b. bild & run server:

          go to directory - Chat-project/server/src
          run command - make check.
         
2. Run the client:   
    a. set server ip & port: 

          go to file - Chat-project/client/resources/server_ip_and_port.txt 
          Write in first line - server ip.
          Write in second line -server port. 
            
    b. bild & run client:
          go to directory - Chat-project/client/src
          run command - make check.
