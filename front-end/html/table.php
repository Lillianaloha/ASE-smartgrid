<?php include "../db.php";?>
<?php include "../function.php";?>

<!DOCTYPE html>
<html>
	<head>
		<meta charset="UTF-8">
		<title> Smartgrid | Search | Results</title>
		<link rel="stylesheet" href="../css/base.css" />
		<link rel="stylesheet" href="../css/index.css" />
		<link rel="stylesheet" href="../css/success.css" />

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
						<li class="active"> <a href="success.php">Search</a> </li>
                        <li> <a href="about.html">About Us</a> </li>
						<li> <a href="server.html">Future</a> </li>
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
						<b>Results</b>
						<i>|</i>
						<span>Get the information you want</span>
					</p>
					<p class="p2">
						path: <strong>Home</strong> > <strong>Search</strong> > <strong>Results</strong>
					</p>
				</div>
				
			</div>
            
               <div class="container">
                <h2>Information Table</h2>
                
                <a><?php 
                global $conn;
                $query2 = "SELECT * FROM smartgrid";
                $result = mysqli_query($conn, $query2);
                if(!$result) {
                    die('Query FAILED' . mysqli_error());
                }
                while($row = mysqli_fetch_assoc($result)) {
                    echo "<br>";
                    echo "Va: ".$row['Va']."-".
                    "Vb: ".$row['Vb']."； ".
                    "Vc: ".$row['Vc']."； ".
                    "Ia: ".$row['Ia']."； ".
                    "Ib: ".$row['Ib']."； ".
                    "Ib: ".$row['Ib']."<br>".
                    "Total_Power: ".$row['Total Power']."； ".
                    "Total_Fundamental_Power: ".$row['Total Fundamental Power']."； ".
                    "Phase_A_Power: ".$row['Phase A Power']."； ".
                    "Phase_B_Power: ".$row['Phase B Power']."； ".
                    "Phase_C_Power: ".$row['Phase C Power']."； ".
                    "Reactive_Power: ".$row['Reactive Power']."<br>".
                    "Phase_A_Reactive_ower: ".$row['Phase A Reactive Power']."； ".
                    "Phase_B_Reactive_Power: ".$row['Phase B Reactive Power']."； ".
                    "Phase_C_Reactive_Power: ".$row['Phase C Reactive Power']."； ".
                    "Consumed_Power: ".$row['Consumed Power']."； ".
                    "Sold_Power: ".$row['Sold Power']."<br>";       
                }
                ?></a>
            </div>
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


	
		

