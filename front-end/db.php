<?php
//$servername = "35.185.3.227";
//$username = "root";
//$password = "";
//$dbname = "ase";

$servername = "127.0.0.1";
$username = "root";
$password = "lillian";
$dbname = "ase";

// Create connection
$conn = mysqli_connect($servername, $username, $password, $dbname);
global $conn;
// Check connection
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}
?>