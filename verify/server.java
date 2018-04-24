import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Signature;
import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class server implements Runnable
{
	private final static int KEYSIZE = 2048;
	private static final String PUBLICKEYLOCATION = "./serverPublicKey.obj";
	private static final String PRIVATEKEYLOCATION = "./PrivateKey.obj";
	
	private static int port;
	private static char mode;
	
	private ServerSocket serverSocket = null;
	private Socket clientSocket = null;
	private BufferedInputStream fromClient = null;

	//server RSA Keys
	public PublicKey pubKey = null;
	private PrivateKey privKey = null;
	
	//client RSA public key
	public PublicKey clientPublicKey = null;
	private SecretKey AES = null;
	private IvParameterSpec ivspec = null;
	private byte [] signature = new byte[256];
	
	/*
	 * If an error was caught and it must die,
	 * come here
	 * */
	public static void die(String message)
	{
		System.out.println(message);
		System.exit(0);
	}
	
	/*
	 * This is to start the server class
	 * I wrote it so if I need to make it multi-threaded,
	 * most of my work is done for me
	 * */
	public server(String clientPK, String serverPK, String serverSK)
	{	
		
		/*
		 * Read all the RSA required keys
		 */
		ObjectInputStream readObject = null;
		try
		{
			readObject = new ObjectInputStream(new FileInputStream(new File(clientPK)));
			clientPublicKey = (PublicKey) readObject.readObject();

			readObject = new ObjectInputStream(new FileInputStream(new File(serverPK)));
			pubKey = (PublicKey) readObject.readObject();
			
			readObject = new ObjectInputStream(new FileInputStream(new File(serverSK)));
			privKey = (PrivateKey) readObject.readObject();
			
			readObject.close();
		}
		catch (IOException | ClassNotFoundException e)
		{
			die("Invalid File location for RSA keys");
		}	
	}
	
	public void run()
	{
		try
		{
			serverSocket = new ServerSocket(port);
			
			//Run forever until CTRL-C is pressed
			while(true)
			{
				clientSocket = serverSocket.accept();
				this.process();
			}
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}

	/*
	 * This would be put into a 
	 * Thread, but this is single-threaded
	 * */
	private void process()
	{
		try 
		{
			InputStream fromClient = clientSocket.getInputStream();
			
			switch(mode)
			{
				// decrypt
				case('d'):
				
					//Get the AES-Cipher, read 256 bytes
					fromClient.read(signature);
				
					//Decrypt the key using the Server Private RSA Key
					//Acquire the AES Key
					byte [] key = decrypt(privKey, signature);
					AES = new SecretKeySpec(key, "AES");
					
					//Get the IV Parameter
					byte [] Ivy = new byte[16]; 
					fromClient.read(Ivy);
					ivspec = new IvParameterSpec(Ivy);

					//Read the file and decrypt it with the AES Key
					byte [] answer = fromClient.readAllBytes();
					answer = AESdecrypt(answer, AES);
					
					/*
					 * Write the file to 
					 * current directory
					 * */
					FileOutputStream stream = new FileOutputStream("/home/andrew/test.txt");
					stream.write(answer);
					break;

				//verify
				case('v'):
					//Get the Signature Hash, 256-bytes
					fromClient.read(signature);
					
					//Get File
					byte [] file = fromClient.readAllBytes();
					
					//Verify it
					if(verify (file, signature, clientPublicKey))
					{
						System.out.println("signature is valid");
					}
					else
					{
						System.out.println("signature is invalid");
					}
					break;

				default:
					die("Invalid char argument!");
					break;
			}
			this.closeConnection();
			//Professor Cook said the Server should die...
			System.exit(0);
		}
		catch(BadPaddingException e)
		{
			die("You have misplace your RSA keys! Did you follow my README?");
		}
		catch (Exception e) 
		{
			die("Exception caught in Process()");
		}
	}

	/*
	 * Check if the port number is not a
	 * number. If I get a NumberFormat Exception
	 * that is how to catch this.
	 * Return true if GOOD
	 * Return false if ERROR then die
	 * */
	public static boolean isValidPortNumber(String portNumber)
	{
		try
		{
			port = Integer.parseInt(portNumber);
			return true;
		}
		catch(NumberFormatException nfe)
		{
			return false;
		}
	}
	
	/*
	 * Check if the action is size 1
	 * and it is a valid character
	 * Return true if GOOD
	 * Return false if ERROR then die
	 * */
	public static boolean isValidAction(String action)
	{
		if(action.length() != 1)
		{
			return false;
		}
		mode = action.charAt(0);
		if (mode == 'd' || mode == 'v')
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	public static void main(String [] args)
	{
		//How to build the RSA Keys
		/*
		server signature = new server();
		signature.buildKeyPair();
		signature.printRSAKeys();
		*/
		
		if(args.length != 5)
		{
			die("Invalid amount of arguments");
		}
		
		if(isValidPortNumber(args[0])==false)
		{
			die("Invalid Port Number!");
		}
		if(isValidAction(args[1]) == false)
		{
			die("Invalid action argument!");
		}
		//File Location is checked on the Constructor!
		//Pass in: Client PublicKey, Server PublicKey, Server PrivateKey Locations
		server signature = new server(args[2], args[3], args[4]);
		new Thread(signature).start();
	}
	
	/*
	 * Generate RSA Public Keys
	 * */
	public void buildKeyPair() 
	{
		KeyPairGenerator keyPairGenerator = null;
		try 
		{
			keyPairGenerator = KeyPairGenerator.getInstance("RSA");
		}
		catch (NoSuchAlgorithmException e)
		{
			e.printStackTrace();
		}
		
		keyPairGenerator.initialize(KEYSIZE);      
		KeyPair keys = keyPairGenerator.genKeyPair();
		pubKey = keys.getPublic();
		privKey = keys.getPrivate();
	}
	
	/*
	 * Print the RSA Public Keys into 
	 * Object files.
	 * The location of where it is printed is
	 * determined by the final strings
	 * */
    public void printRSAKeys()
    {
	 	ObjectOutputStream pkOUT = null;
		try
		{
			pkOUT = new ObjectOutputStream(new FileOutputStream(new File(PUBLICKEYLOCATION)));
			pkOUT.writeObject(pubKey);
			pkOUT.flush();
			
			pkOUT = new ObjectOutputStream(new FileOutputStream(new File(PRIVATEKEYLOCATION)));
			pkOUT.writeObject(privKey);
			pkOUT.flush();
			
			pkOUT.close();
		}
		catch(IOException ioe)
		{
			ioe.printStackTrace();
		}
    }
	
    /*
	 * Decrypt encrypted byte stream
	 * using the RSA Private Key
	 * return byte []
	 * */
    public static byte[] decrypt(PrivateKey privateKey, byte [] encrypted)
    		throws Exception
    {
        Cipher cipher = Cipher.getInstance("RSA/ECB/PKCS1Padding");  
        cipher.init(Cipher.DECRYPT_MODE, privateKey);    
        return cipher.doFinal(encrypted);
    }
    
    /*
  	 * Decrypt encrypted byte stream
  	 * using the AES Secret Key
  	 * return byte []
  	 * */
    public byte [] AESdecrypt(byte [] plaintext, SecretKey key) 
    		throws InvalidKeyException, NoSuchAlgorithmException, 
    		NoSuchPaddingException, IllegalBlockSizeException, 
    		BadPaddingException, InvalidAlgorithmParameterException
    {
    	Cipher de = Cipher.getInstance("AES/CBC/PKCS5Padding");
    	de.init(Cipher.DECRYPT_MODE, key, ivspec);
    	return de.doFinal(plaintext);
    }
    
    /*
  	 * Sign a byte [] 
  	 * using an RSA Private Key
  	 * and SHA-256 Hashing
  	 * */
    public static byte [] sign(byte [] plainText, PrivateKey privateKey)
    		throws Exception
    {
        Signature privateSignature = Signature.getInstance("SHA256withRSA");
        //Initialize with Private Key
        privateSignature.initSign(privateKey);
        //Place plain text into queue
        privateSignature.update(plainText);
        //Hash it and sign it with private key
        return privateSignature.sign();
    }
    
    /*
  	 * verify a byte[]
  	 * using an RSA Public Key
  	 * and decrypt it from SHA-256 Hashing
  	 * */
    
    public static boolean verify(byte [] plainText, byte [] signature, PublicKey publicKey) 
    		throws Exception 
    {
        Signature publicSignature = Signature.getInstance("SHA256withRSA");
        //Initialize with Public Key
        publicSignature.initVerify(publicKey);
        //Place plain text into queue
        publicSignature.update(plainText);
        //Decrypt signature with public key and de hash. Then compare...
        return publicSignature.verify(signature);
    }
    
    /*
  	 * Close Socket
  	 * */
    
	private void closeConnection()
	{
		try
		{
			if (fromClient!=null)
			{
				fromClient.close();
			}
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
	}
}
