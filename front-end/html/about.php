<?php include "../db.php";?>
<?php include "../function.php";?>

<!DOCTYPE html>
<html>
	<head>
		<meta charset="UTF-8">
		<title> Smartgrid | Search </title>
		<link rel="stylesheet" href="../css/base.css" />
		<link rel="stylesheet" href="../css/index.css" />
		<link rel="stylesheet" href="../css/about.css" />
	</head>

	<body>
		<!--1、头部-->
		<div id="header">
			<div class="header-content">
				<div class="logo">
					<a href="../index.html"> <img src="../img/a.png" height="82" width="256" /> </a> 
<!--					logo picture should be changed-->
					<div class="key">
                        <p>Class Project</p>
					</div>
				</div>
				<div class="nav">
					<ul>
						<li> <a href="../index.html">Home</a> </li>
						<li> <a href="success.php">Search</a> </li>
                        <li class="active"> <a href="about.php">About Us</a> </li>
						<li> <a href="future.php">Future</a> </li>
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
						<b>About</b>
						<i>|</i>
						<span>Learn more about us</span>
					</p>
					<p class="p2">
						path: <strong>Home</strong> > <strong>About</strong>
					</p>
				</div>
				
			</div>
			
            <!--内容-->
			<div class="content">
				<div class="section1">
					<h3>Principle goals</h3>
					<p>
					
					1. Generate data on gateway (Offline scenario) <br>
					2. Generating synthetic “smart meter” data from each node (Online Scenario) <br>
					3. Assign a table or database for each registered group/team. Database on the server <br>
                    4. Storage and Retrieval of data from database located at the gateway <br>

						
					</p>
				</div>
            </div>
		</div>
			
		<!--3、尾部-->
		<div id="footer">
			<p>
				<a href="../index.html">Home</a> |
				<a href="success.php">Search</a> |
				<a href="about.php">About Us</a> |
				<a href="future.php">Future</a> |
			</p>
			<p>
				E-mail: qz2302@columbia.edu
			</p>
		</div>
	</body>
</html>


	
		

