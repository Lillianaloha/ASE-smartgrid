<?php include "../db.php";?>
<?php include "../function.php";?>

<!DOCTYPE html>
<html>
	<head>
		<meta charset="UTF-8">
		<title> Smartgrid | Search </title>
		<link rel="stylesheet" href="../css/base.css" />
		<link rel="stylesheet" href="../css/index.css" />
		<link rel="stylesheet" href="../css/success.css" />
	</head>
	
	<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {font-family: Arial, Helvetica, sans-serif;}
form {border: 3px solid #f1f1f1;}

input[type=text], input[type=password] {
    width: 100%;
    padding: 12px 20px;
    margin: 8px 0;
    display: inline-block;
    border: 1px solid #ccc;
    box-sizing: border-box;
}

button {
    background-color: #4CAF50;
    color: white;
    padding: 14px 20px;
    margin: 8px 0;
    border: none;
    cursor: pointer;
    width: 100%;
}

button:hover {
    opacity: 0.8;
}

.cancelbtn {
    width: auto;
    padding: 10px 18px;
    background-color: #f44336;
}

.imgcontainer {
    text-align: center;
    margin: 24px 0 12px 0;
}

img.avatar {
    width: 40%;
    border-radius: 50%;
}

.container {
    padding: 16px;
    width: 50%;
    margin-left: auto;
    margin-right: auto;
}

</style>
</head>

	<body>
		<!--1、头部-->
		<div id="header">
			<div class="header-content">
				<div class="logo">
					<a href="index.html"> <img src="../img/a.png" height="82" width="256" /> </a> 
<!--					logo picture should be changed-->
					<div class="key">
                        <p>Class Project</p>
					</div>
				</div>
				<div class="nav">
					<ul>
						<li> <a href="../index.html">Home</a> </li>
						<li class="active"> <a href="success.php">Search</a> </li>
                        <li> <a href="about.html">About Us</a> </li>
						<li> <a href="future.html">Future</a> </li>
					</ul>
				</div>
			</div>
		</div>
		
		<!--2、主体内容-->

        <div id="main">
			<!--位置-->
			<div class="location">
				<div class="section1">
					<p class="p1">
						<b>Search</b>
						<i>|</i>
						<span>Get the information you want</span>
					</p>
					<p class="p2">
						path: <strong>Home</strong> > <strong>Search</strong>
					</p>
				</div>
				
			</div>
<!--		<form action="../testcase.php" method="get">	-->
        <form action="../plot/graph.php" method="get">
          <div class="imgcontainer">
            <img src="../img/gateway.png" height="70" width="90">
            <h1>Enter</h1>
          </div>

          <div class="container">
            <label for="time"><b>Time</b></label>
                <input type="text" placeholder="Enter amount of time" name="time" >
            <label for="rate"><b>Rate</b></label>
                <input type="text" placeholder="Enter sampling rate" name="rate" >            
             <label for="ip"><b>IP</b></label>
                <input type="text" placeholder="Enter IP address" name="ip" >            
            <label for="port"><b>Port</b></label>
                <input type="text" placeholder="Enter port number" name="port" >
            <button type="submit">Search</button>
          </div>
        </form>
		</div>

		<!--3、尾部-->
		<div id="footer">
			<p>
				<a href="../index.html">Home</a> |
				<a href="success.php">Search</a> |
				<a href="about.html">About Us</a> |
				<a href="future.html">Future</a> |
			</p>
			<p>
				E-mail: qz2302@columbia.edu
			</p>
		</div>
	</body>
</html>


	
		

