import java.io.BufferedInputStream;
import java.io.File;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.MessageDigest;
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
	private static byte [] data;
	private static final String PUBLICKEYLOCATION = "./PublicKey/serverPublicKey.obj";
	private static final String PRIVATEKEYLOCATION = "./PrivateKey/serverPrivateKey.obj";

	private static int port;
	private static char mode;
	
	private ServerSocket serverSocket = null;
	private Socket clientSocket = null;
	private BufferedInputStream fromClient = null;

	//server RSA Keys
	public static PublicKey pubKey = null;
	private static PrivateKey privKey = null;

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
	 * This is to start the server class
	 * I wrote it so if I need to make it multi-threaded,
	 * most of my work is done for me
	 * */
	public server(String clientPK, String serverPK, String serverSK)
	{	

		// Read all the RSA required keys
		ObjectInputStream readObject = null;
		try
		{
			if (mode != 's')
			{
				readObject = new ObjectInputStream(new FileInputStream(new File(clientPK)));
				clientPublicKey = (PublicKey) readObject.readObject();

				readObject = new ObjectInputStream(new FileInputStream(new File(serverPK)));
				pubKey = (PublicKey) readObject.readObject();

				readObject = new ObjectInputStream(new FileInputStream(new File(serverSK)));
				privKey = (PrivateKey) readObject.readObject();

				readObject.close();
			}
			else
			{
                                System.out.println("Read objects");
				readObject = new ObjectInputStream(new FileInputStream(new File(PUBLICKEYLOCATION)));
				pubKey = (PublicKey) readObject.readObject();

				readObject = new ObjectInputStream(new FileInputStream(new File(PRIVATEKEYLOCATION)));
				privKey = (PrivateKey) readObject.readObject();

				readObject.close();
			}
		}
		catch (IOException | ClassNotFoundException e)
		{
			die("Invalid File location for RSA keys");
		}	
                System.out.println("done with keys");
	}

	public void run()
	{
		try
		{
                        System.out.println("new server socket made");
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
                System.out.println("Start!");
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
				
			// ASE PLUG IN
			case('s'):

                                System.out.println("Case S");
				
				// Get Stream to print signature/error message
                                /*
                                ObjectInputStream getSize = new ObjectInputStream(fromClient);
                                int size = getSize.readInt();
                                getSize.close();
				*/
                                
				OutputStream toClient = clientSocket.getOutputStream();
    				BufferedReader br = new BufferedReader(new InputStreamReader(fromClient));
	
				/*
				byte [] fileSearch = new byte [size];
                                fromClient.read(fileSearch);
				String fileName = new String(fileSearch, "UTF-8");
				*/
				String fileName = br.readLine();
				//br.close();
	
				//fileName = "./" + fileName;
				System.out.println("Search for file with this path: " + fileName);
				
				// Look for file, byte [] data is filled
				if(isValidFile(fileName))
				{	
                                        System.out.println("Got a valid file!");

					// Get Signature and send it
					byte [] signature = sign(data, privKey);			
					toClient.write(signature);
					toClient.flush();
					
					// Send Server Public Key...
					ObjectOutputStream pubkeyOUT = new ObjectOutputStream(clientSocket.getOutputStream());
					pubkeyOUT.writeObject(pubKey);
					pubkeyOUT.flush();
					
					toClient.close();
					pubkeyOUT.close();
				}
				//File not Found send error...
				else
				{
                                        System.out.println("Did not get a valid file!");
					// I can do this by sending NOT 256 bytes
					byte error = 0x0;
					toClient.write(error);
					toClient.flush();
				}
			
                                br.close();
				toClient.close();
				break;
			default:
				die("Invalid char argument!");
				break;
			}
			this.closeConnection();
			System.exit(0);
		}
		catch(BadPaddingException e)
		{
			die("You have misplace your RSA keys! Did you follow my README?");
		}
		catch (Exception e) 
		{
                        e.printStackTrace();
			//die("Exception caught in Process()");
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
		if (mode == 'd' || mode == 'v' || mode == 's')
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

                /*
                 * For future reference...
                 * DO NOT have multiple instances 
                 * of starting a thread in if/else.
                 *
                 * It is not like C where I can just run a thread
                 * You can use conditions to load the thread. NOT RUN IT!
                 */
                Thread t = null;

		// How to build the RSA Keys
		if (args.length == 0)
		{
			server.buildKeyPair();
			server.printRSAKeys();
			System.exit(0);
		}

		// Hash a file, print to output
		// server.jar <file to Hash>
		else if (args.length == 1)
		{
			// Read all the file into byte array
			if(isValidFile(args[0]))
			{			
				String Hash = null;
				try 
				{
					Hash = new String(server.hashFile(data));
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

		// Run Method to Test Anomalies in Dataset by increments
		// server.jar <data set> <given percentage>
		else if (args.length == 2)
		{		        
			try 
			{
				invalidInstance(args[0], args[1]);
			} 
			catch (Exception e) 
			{
				die("Invalid File or invalid percentage input");
			}
                        System.exit(0);
		}
		
		// 1- Get connection from Client...
		// 2- Get the file the client is looking for
		// 3- Sign it 
		// 4- Send the Server Public Key as well
		
		// WILL READ KEYS FROM FOLDERS ATTACH WITH GIT
		// java -jar server.jar <arg> <port>
                else if (args.length == 3)
		{
			// Is argument 's'?
			if(isValidAction(args[0]) == false)
			{
				die("Invalid action argument!");
			}
			
			// Valid Port to Listen to
			if(isValidPortNumber(args[1])==false)
			{
				die("Invalid Port Number!");
			}
			// Read Files from Default Location
			
                        // I also don't need Client Public Key...
			//System.out.println("Starting Thread");
                        t = new Thread(new server(null, null, null));
                        //System.out.println("It ended?");
		}
                else
                {
		
                    if(args.length != 5)
                    {
                        die("Invalid amount of arguments: " + args.length);
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
                    server verify = new server(args[2], args[3], args[4]);
                    t = new Thread(verify);
                }
                t.start();
	}

	// Generate RSA Public Keys

	public static void buildKeyPair() 
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
	public static void printRSAKeys()
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

	/*
	 * Test if the input is reasonable. By reasonable, we mean if for every 
	 * two lines of data, the corresponding data from the second line is within 
	 * 'errorRate' percent difference of the first. 
	 */
	public static int invalidInstance(String filePath, String errorInput) 
			throws Exception
	{
		double errorRate = Double.parseDouble(errorInput);
		
		//error range should be any number between 0 and 1
		if(errorRate <= 0 || errorRate >= 1)
		{
			System.out.println("Invalid Error Rate!");
		}
		
		Scanner scanner = new Scanner(new File(filePath));
		int [] array1 = new int[17];
		int [] array2 = new int[17];
		int counter = 0;
		int size = 0;
		
		// Skip Header Header
		scanner.nextLine();
		
		//read first line
		if(scanner.hasNext()) 
		{
			array1 = Stream.of(scanner.next().split(",")).mapToInt(Integer::parseInt).toArray();
			++size;
		}

		//read second line, compare, and then move to the next two lines.
		while(scanner.hasNext())
		{
			array2 = Stream.of(scanner.next().split(",")).mapToInt(Integer::parseInt).toArray();
			for(int i= 0; i < array1.length; i++) 
			{
				double error = array1[i] * errorRate;
				if(array2[i] < array1[i] - error || array2[i] > array1[i] + error )
				{
					counter++;
					break;
				}
			}
			++size;
			array1 = array2;
		}
		
		System.out.println("Given the file: " + filePath + " There are " + counter + " out of " + size + " entries " 
				+ " where the data increments over " + errorRate);
		scanner.close();
		return counter;
	}
	
	// Build Hash
	public static byte [] hashFile(byte [] original) 
			throws NoSuchAlgorithmException
	{
		MessageDigest digest = MessageDigest.getInstance("SHA-256");
		return digest.digest(original);
	}
}
