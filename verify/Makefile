.PHONY:
default: client server

client: client.class
	echo Main-Class: client > MANIFEST.MF
	jar -cvmf MANIFEST.MF client.jar client.class

server: server.class
	echo Main-Class: server > MANIFEST.MF
	jar -cvmf MANIFEST.MF server.jar server.class 
	
client.class: client.java
	javac client.java

server.class: server.java 
	javac server.java

.PHONY:
clean:
	rm -rf *.class
	rm -rf *~
	rm -rf server
	rm -rf client
	rm -rf MANIFEST.MF
	rm -rf client.jar
	rm -rf server.jar
.PHONY:
all:	clean default

testAclient: all
	java -jar client.jar aaaaabbbbbcccccd ./cat.jpg a 192.168.223.131 11273 ./PublicKey/serverPublicKey.obj ./PublicKey/clientPublicKey.obj ./PrivateKey/clientPrivateKey.obj

testBclient: all
	java -jar client.jar aaaaabbbbbcccccd ./cat.jpg b 192.168.223.131 11273 ./PublicKey/serverPublicKey.obj ./PublicKey/clientPublicKey.obj ./PrivateKey/clientPrivateKey.obj

testCclient: all
	java -jar client.jar aaaaabbbbbcccccd ./cat.jpg c 192.168.223.131 11273 ./PublicKey/serverPublicKey.obj ./PublicKey/clientPublicKey.obj ./PrivateKey/clientPrivateKey.obj

testDServer: all
	java -jar server.jar 11273 d ./PublicKey/clientPublicKey.obj ./PublicKey/serverPublicKey.obj ./PrivateKey/serverPrivateKey.obj

testVServer: all	
	java -jar server.jar 11273 v ./PublicKey/clientPublicKey.obj ./PublicKey/serverPublicKey.obj ./PrivateKey/serverPrivateKey.obj
