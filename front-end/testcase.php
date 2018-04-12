    //virtual machine ip
    //two integer variable timerate and sampling_rate<?php
error_reporting(E_ALL);

/* Allow the script to hang around waiting for connections. */
set_time_limit(0);

/* Turn on implicit output flushing so we see what we're getting
 * as it comes in. */
ob_implicit_flush();

$address = 'localhost';
$port = 8000;

// Creating Server Socket
if (($sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === false) {
    echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
}

if (socket_bind($sock, $address, $port) === false) {
    echo "socket_bind() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
}

if (socket_listen($sock, 5) === false) {
    echo "socket_listen() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
}

do 
{
    if (($msgsock = socket_accept($sock)) === false) 
    {
        echo "socket_accept() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
        break;
    }
    /* Send instructions. */
    $msg = "\nWelcome to the PHP Test Server. \n" .
        "To quit, type 'quit'. To shut down the server type 'shutdown'.\n";
    socket_write($msgsock, $msg, strlen($msg));
    
    //virtual machine ip
    //two integer variable timetate and sampling_rate
    generate_IP = 160.39.137.191;
    generate_Port = 8000;
    timeRate = -1;
    sampling_rate = -1;
    
    
    //Or we can pre-set IP and port?
    
    do 
    {
        /*
        if (false === ($buf = socket_read($msgsock, 2048, PHP_NORMAL_READ))) 
        {
            echo "socket_read() failed: reason: " . socket_strerror(socket_last_error($msgsock)) . "\n";
            break 2;
        }
        */
        
        /*
        if (false === ($buf = socket_read($generate_Port, 4, PHP_NORMAL_READ))) 
        {
            echo "socket_read() failed: reason: " . socket_strerror(socket_last_error($msgsock)) . "\n";
            break 2;
        }
        if (false === ($buf = socket_read($generate_Port, 4, PHP_NORMAL_READ))) 
        {
            echo "socket_read() failed: reason: " . socket_strerror(socket_last_error($msgsock)) . "\n";
            break 2;
        }
        if (false === ($buf = socket_read($generate_Port, 4, PHP_NORMAL_READ))) 
        {
            echo "socket_read() failed: reason: " . socket_strerror(socket_last_error($msgsock)) . "\n";
            break 2;
        }
        if (false === ($buf = socket_read($generate_Port, 4, PHP_NORMAL_READ))) 
        {
            echo "socket_read() failed: reason: " . socket_strerror(socket_last_error($msgsock)) . "\n";
            break 2;
        }
        */
        
        if (($generator_sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === false) 
        {
            echo "generator_socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
        }
        if((socket_connect($generator_sock,$generate_IP, $Port)) == false)
        {
              echo "generator_socket_connect() failed.\nReason: ($result) " . socket_strerror(socket_last_error($socket)) . "\n";
        }
    
        //check if negative, if +, send to generator
        if time > 0
            socket_write($generator_socket, $timeRate, 4)
        else
            //Error
            echo "Invalid time input!";
        
        if time > 0
            socket_write($generator_socket, $timeRate, 4)
        else
            //Error
            echo "Invalid sampling rate input!";
        
        //Now wait until transmission completes...
        while(true)
        {
            if (false === ($buf = socket_read($msgsock, 2048, PHP_NORMAL_READ))) 
            {
                echo "socket_read() failed: reason: " . socket_strerror(socket_last_error($msgsock)) . "\n";
                break 2;
            }
            echo $buf
            if (time == 0)
            {
                
            }
            --time;
        }
        
        if (!$buf = trim($buf)) 
        {
            continue;
        }
        if ($buf == 'quit') 
        {
            break;
        }
        if ($buf == 'shutdown') 
        {
            socket_close($msgsock);
            break 2;
        }
        $talkback = "PHP: You said '$buf'.\n";
        socket_write($msgsock, $talkback, strlen($talkback));
        echo "$buf\n";
    } 
    while (true); 
    socket_close($msgsock);
}
while (true);

socket_close($sock);
?>
