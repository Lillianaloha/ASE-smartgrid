import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
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
import java.util.Scanner;
import java.util.stream.Stream;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
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
	 * Check if I have invalid entry 
	 * for port number by catching 
	 * Number Format exception
	 * and make sure I have valid entries (above 1024 < 
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
			else if (port > 65535)
			{
				System.out.println("Port Number too High!");
				return false;
			}
			else if (port > 0 && port < 1024)
			{
				System.out.println("Permission Denied to use Ports 1 - 1024");
				return false;
			}
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

	public static void main(String [] args) throws FileNotFoundException
	{
		// How to build the RSA Keys
		if (args.length == 0)
		{
			server signature = new server();
			signature.buildKeyPair();
			signature.printRSAKeys();
			System.exit(0);
		}

		// Hash a file, print to output
		// server.jar <file to Hash>
		else if (args.length == 1)
		{
			server hasher = new server();
			// Read all the file into byte array
			if(isValidFile(args[1]))
			{
				String tobeHashed = new String(data);
				String Hash = null;
				try 
				{
					Hash = new String(hasher.hashFile(data));
				}
				catch (NoSuchAlgorithmException e)
				{
					e.printStackTrace();
				}
				// Print the Hash
				if (Hash != null)
				{
					System.out.println(Hash);
				}
				else
				{
					die("Hash is NULL!");
				}
				System.exit(0);
			}
			else
			{
				die("Error in reading file to Hash");
			}
		}

		// Run anomaly detection
		// server.jar <file to Hash> <given percentage>
		else if (args.length == 2)
		{
			try
			{
				invalidInstance(String filePath, double errorRate);
			}
			catch(FileNotFoundException fn)
			{
				fn.printStackTrace();
			}
			System.exit(0);
		}
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

	// Generate RSA Public Keys

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
	 * Test if the input is reasonable. By reasonable, we mean if for every 
	 * two lines of data, the corresponding data from the second line is within 
	 * 'errorRate' percent difference of the first. 
	 */
	public static int invalidInstance(String filePath, double errorRate) throws FileNotFoundException{
		//error range should be any number between 0 and 1
		if(errorRate <= 0 || errorRate >= 1) 
		{
			System.out.println("wrong error rate");
		}

		Scanner scanner = new Scanner(new File(filePath));
		int[] array1 = new int[17];
		int[] array2 = new int[17];
		int counter = 0;

		//read first line
		if(scanner.hasNext()) 
		{
			array1 = Stream.of(scanner.next().split(",")).mapToInt(Integer::parseInt).toArray();
		}

		//read second line, compare, and then move to the next two lines.
		while(scanner.hasNext()){
			array2 = Stream.of(scanner.next().split(",")).mapToInt(Integer::parseInt).toArray();
			for(int i= 0; i < array1.length; i++) {
				double error = array1[i] * errorRate;
				if(array2[i] < array1[i] - error || array2[i] > array1[i] + error ) {
					counter++;
					break;
				}
			}
			array1 = array2;
		}

		scanner.close();
		return counter;
	}

	// Close Socket
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
