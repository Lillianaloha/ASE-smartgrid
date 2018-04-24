Required Packages (Assumed to be running in Ubuntu).
Since Qianqian is using MAC, the JAR file can be made in Ubuntu and exported to MAC?

sudo apt-get install default-jre
sudo apt-get install default-jdk
sudo apt-get install openjdk-9-jdk
sudo apt-get install maven
sudo apt-get install build-essential

Purpose
1- File Verification (Client downloads CSV file, Wants to see if same downloaded from server)
2- Exchanging AES keys for secure communication with client (if needed/enough time to implement)

client.jar (Size 16 Password) (File Location) (Action) (IP Address) (Port Number) (Location of Server PublicKey) (Location of Client PublicKey) (Location of Client PrivateKey)

server.jar (Port Number) (Action) (Location of Client PublicKey) (Location of Server PublicKey) (Location of Server PrivateKey)

How to make this work:
1- Keys need to be generated and exchanged between client and server.
2- See arguments above...

TO DO:
1- Write a method to return Hash of a file. Used when server builds CSV file
2- Write a method to generate keys from terminal

