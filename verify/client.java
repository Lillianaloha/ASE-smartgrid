//Name: Andrew Quijano
//UNI: afq2101

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.charset.StandardCharsets;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.SecureRandom;
import java.security.Signature;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.KeySpec;

import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.PBEKeySpec;
import javax.crypto.spec.SecretKeySpec;

public class client
{
	//Actual variables for the class
	private static char ACTION;
	private static int port;
	private Socket clientSocket = null;
	private static byte [] data = null;
	private static final int KEYSIZE = 2048;
	private static final String PUBLICKEYLOCATION = "./clientPublicKey.obj";
	private static final String PRIVATEKEYLOCATION = "./clientPrivateKey.obj";
	
	//client RSA Keys
	public PublicKey pubKey = null;
	private PrivateKey privKey = null;

	//server RSA Public Key
	public PublicKey serverPublicKey = null;
	
	//AES-Key
	private SecretKey AES = null;
	
	private OutputStream toServer = null;
	
	//AES
	SecureRandom srand = new SecureRandom();
	byte [] salt = new byte [8];
	byte [] iv = new byte[16];
	IvParameterSpec ivspec = null;
	
	/*
	 * Should a method need to die
	 * come here
	 */
	public static void die(String message)
	{
		System.out.println(message);
		System.exit(0);
	}
	
	/*
	 * Method Purpose:
	 * Check if a String contains only the following:
	 * 1- lowercase letters
	 * 2- uppercase letters
	 * 3- integers
	 * 
	 * IF THERE IS JUST ONE CHARACTER THAT DOESN'T MATCH
	 * RETURN FALSE
	 * 
	 * ALSO MAKE SURE PASSWORD IS NOT LONGER THAN 16 chars!
	 */
	public static boolean isValidPassword(String passwd)
	{
		//Is it a 16 character password
		if (passwd.length() != 16)
		{
			return false;
		}
		char testChar;
		for(int i = 0; i < passwd.length();i++)
		{
			testChar = passwd.charAt(i);
			//It is NOT a digit/Letter
			if(!Character.isLetterOrDigit(testChar))
			{
				//System.out.println("Invalid at: " + testChar + "\n");
				return false;
			}
		}
		return true;
	}
	
	/*
	 * Check if the action is size 1
	 * and it is a valid character
	 * Return true if GOOD
	 * Return false if ERROR then die
	 */
	
	public static boolean isValidAction(String action)
	{
		if(action.length() != 1)
		{
			return false;
		}
		ACTION = action.charAt(0);
		if (ACTION == 'a' || ACTION == 'b' || ACTION == 'c')
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	/*
	 * Check if the file I am sending is valid
	 * if it is valid
	 * 1- I will create a byte [] called data which 
	 * has the complete byte stream of the file I intend to send
	 * 2- If it does NOT exist, kill the program as I was told.
	 */
	public static boolean isValidFile(String fileLocation)
	{
		try
		{
			File information = new File(fileLocation);
			FileInputStream readFile = new FileInputStream(new File(fileLocation));

			//Check if it is too large?
			data = new byte[(int) information.length()];
			
			// Get Byte Stream of File
			int size = readFile.read(data, 0, data.length);
			if (size != data.length)
			{
				die("File wasn't read entirely...");
			}
			
			readFile.close();
			return true;
		}
		catch(IOException e)
		{
			return false;
		}
	}
	
	/*
	 * Check if I have a valid IP
	 * check for
	 * 1- Valid octet count
	 * 2- valid numeric ranges for IP Address
	 */
	public static boolean isValidIP(String IP)
	{
		//Check if there are 4 octets!
		String octets [] = IP.split("\\.");

		if (octets.length != 4)
		{
			//System.out.println("Invalid Number of dots");
			return false;
		}
		
		/*
		 * Check if the values for all octets 
		 * is [0, 255] 
		 */
		int [] octetValue = new int[4];
		try 
		{
			for (int i = 0; i < 4; i++)
			{
				octetValue [i] = Integer.parseInt(octets[i]);
				if (octetValue[i] < 0 || octetValue[i] > 255)
				{
					return false;
				}
			}
		}
		catch(NumberFormatException nfe)
		{
			return false;
		}
		return true;
	}
	
	/*
	 * Check if I have
	 * invalid entry for port number
	 * by catching numberformat exception
	 */
	public static boolean isValidPortNumber(String portNum)
	{
		try
		{
			port = Integer.parseInt(portNum);
			if (port <= 0)
			{
				System.out.println("Illegal Port Number argument.");
				return false;
			}
			else if (port > 0 && port < 1024)
			{
				System.out.println("Permission Denied to use Ports 1 - 1024");
			}
			return true;
		}
		catch(NumberFormatException nfe)
		{
			return false;
		}
	}
    
	public static void main (String [] args)
	{
		// Build RSA Keys
		if (args.length == 0)
		{
			 client networkClient = new client();
			 networkClient.buildKeyPair();
			 networkClient.printRSAKeys();
			 System.exit(0);
		}
		else if (args.length != 8)
		{
			die("Invalid amount of arguments");
		}
		
		//Error checking methods invoked
		if(isValidPassword(args[0])==false)
		{
			die("Invalid Password!");
		}
		if(isValidFile(args[1])==false)
		{
			die("I/O exception caught, File does not exist!");
		}
		if(isValidAction(args[2])==false)
		{
			die("Invalid Action!");
		}
		if(isValidIP(args[3])==false)
		{
			die("Invalid IP Address!");
		}
		if(isValidPortNumber(args[4]) == false)
		{
			die("Invalid Port Number!");
		}
		//Send: password, IP, serverPublicKey, Client Public Key, Client PrivateKey
		client networkClient = new client(args[0],args[3], args[5], args[6], args[7]);
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
     * Generate AES Secret Key
     * AES class variable becomes not null after this
     * */
    public void buildAES(String password) 
    		throws NoSuchAlgorithmException, InvalidKeySpecException
    {
    	srand.nextBytes(salt);
    	srand.nextBytes(iv);
    	ivspec = new IvParameterSpec(iv);
    
    	//Allows me to pick my password, salt, iterations, and keysize
    	KeySpec passwd = new PBEKeySpec(password.toCharArray(), salt, 1000, 128);
    	
    	//Generate AES
    	SecretKeyFactory factory =
    		    SecretKeyFactory.getInstance("PBKDF2WithHmacSHA256");
    	SecretKey tmp = factory.generateSecret(passwd);
    	AES = new SecretKeySpec(tmp.getEncoded(), "AES");
    }
 
    public static byte [] encrypt(PublicKey publicKey, byte [] plaintext) 
    		throws Exception
    {
    	Cipher cipher = Cipher.getInstance("RSA/ECB/PKCS1Padding");
    	cipher.init(Cipher.ENCRYPT_MODE, publicKey);
    	return cipher.doFinal(plaintext);
    }
    
    public static byte [] encryptKey(PublicKey publicKey, SecretKey sk) 
    		throws Exception
    {
    	Cipher cipher = Cipher.getInstance("RSA/ECB/PKCS1Padding");
    	cipher.init(Cipher.ENCRYPT_MODE, publicKey);
    	return cipher.doFinal(sk.getEncoded());
    }
    
    public byte [] AESencrypt(byte [] plaintext, SecretKey key) 
    		throws Exception
    {
    	Cipher en = Cipher.getInstance("AES/CBC/PKCS5Padding");
    	en.init(Cipher.ENCRYPT_MODE, key, ivspec);
    	return en.doFinal(plaintext);
    }
	  
    public static byte [] sign(byte [] plainText, PrivateKey privateKey)
    		throws Exception
    {
        Signature privateSignature = Signature.getInstance("SHA256withRSA");
        //Initialize with Private Key
        privateSignature.initSign(privateKey);
        //Place plain text into queue
        privateSignature.update(plainText);
        //Hash it and sign it with private key
        byte [] signed = privateSignature.sign();
        return signed;
    }
    
    public client()
    {
    	
    }
    
	public client(String password, String IP, String serverPK, String clientPK, String clientSK)
	{
		//Final check, Find the Public Key!
		ObjectInputStream readObject = null;
		Object input = null;
		try
		{
			//Read all the RSA Keys
			readObject = new ObjectInputStream(new FileInputStream(new File(serverPK)));
			input = readObject.readObject();
			serverPublicKey = (PublicKey) input;
			
			readObject = new ObjectInputStream(new FileInputStream(new File(clientPK)));
			input = readObject.readObject();
			pubKey = (PublicKey) input;
			
			readObject = new ObjectInputStream(new FileInputStream(new File(clientSK)));
			input = readObject.readObject();
			privKey = (PrivateKey) input;
			
			readObject.close();
	
			//Establish Connection
			clientSocket = new Socket(IP, port);
			toServer = clientSocket.getOutputStream();
			
			//Data is read and ready to go!	
			if(ACTION=='a')
			{
				// Get the Instance for AES-128 bit
				this.buildAES(password);
				//encrypt the file
				byte [] encrypted = this.AESencrypt(data, AES);
			
				//Convert AES to byte stream to it can be encrypted
				byte [] encryptedAES = encryptKey(serverPublicKey, AES);
				
				//Send AES-Key
				toServer.write(encryptedAES);
				toServer.flush();
				
				//Send IV-Parameter
				toServer.write(iv);
				toServer.flush();
				
				//Send File
				toServer.write(encrypted);
				toServer.flush();
			}
			else if (ACTION == 'b')
			{
				//Send Signed File
				byte [] signature = sign(data, privKey);			
				toServer.write(signature);
				toServer.flush();
				
				//Send plain text file
				toServer.write(data);
				toServer.flush();
			}
			else if(ACTION == 'c')
			{
				//Sign File
				byte [] signature = sign(data, privKey);
				
				//Change the byte of file!
				if (data[0] != 0x00)
				{
					System.out.format("Original: %x\n", data[0]);
					data[0] = (byte) 0x00;
					System.out.format("Modified: %x\n", data[0]);
				}
				else
				{
					System.out.format("Original: %x\n", data[0]);
					data[0] = (byte) 0xff; 
					System.out.format("Modified: %x\n", data[0]);
				}
				
				//Send Signed File
				toServer.write(signature);
				toServer.flush();

				//Send plaintext
				toServer.write(data);
				toServer.flush();
			}
			this.closeConnection();
		}
		catch (UnknownHostException socket)
		{
			die("Unknown Host...in constructor");
		}
		catch (IOException socket)
		{
			die("RSA Files not Found/No connection to Server");
		}
		catch (NoSuchAlgorithmException e)
		{
			die("No such Algorithm in constructor");
		}
		catch(BadPaddingException e)
		{
			die("You have misplace your RSA keys! Did you follow my README?");
		}
		catch (Exception e)
		{
			die("Most likely one of those Crypto stuff malfunctioned");
		}
	}
	
	/*
	 * This method is to close the connection
	 * */
	private void closeConnection()
	{
		try
		{
			toServer.close();
			clientSocket.close();
		}
		catch (IOException e)
		{
			die("Failed to close connection");
		}
	}
	
	// Build Hash
	public static String hashFile(String originalString) throws NoSuchAlgorithmException
	{
		MessageDigest digest = MessageDigest.getInstance("SHA-256");
		byte [] encodedhash = digest.digest(originalString.getBytes(StandardCharsets.UTF_8));
		
		StringBuffer hexString = new StringBuffer();
	    for (int i = 0; i < encodedhash.length; i++) 
	    {
	    	String hex = Integer.toHexString(0xff & encodedhash[i]);
	    	if(hex.length() == 1) 
	    	{
	    		hexString.append('0');
	    	}
	    	hexString.append(hex);
	    }
	    return hexString.toString();
	}
}