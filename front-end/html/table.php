<?php include "../db.php";?>
<?php include "../function.php";?>

<?php 
                global $conn;
                $query2 = "SELECT * FROM smartgrid LIMIT 10";
                $result = mysqli_query($conn, $query2);
                if(!$result) {
                    die('Query FAILED' . mysqli_error());
                }
                $td1 = '';
                $td2 = '';
                $td3 = '';
                $td4 = '';
                $td5 = '';
                $td6 = '';
                $td7 = '';
                $td8 = '';
                $td9 = '';
                $td10 = '';
                $td11 = '';
                $td12 = '';
                $td13 = '';
                $td14 = '';
                $td15 = '';
                $td16 = '';
                $td17 = '';
                $td18 = '';
                while($row = mysqli_fetch_assoc($result)) {
                    $td1 .= "<td>".$row['Id']."</td>";
                    $td2 .= "<td>".$row['Va']."</td>";
                    $td3 .= "<td>".$row['Vb']."</td>";
                    $td4 .= "<td>".$row['Vc']."</td>";
                    $td5 .= "<td>".$row['Ia']."</td>";
                    $td6 .= "<td>".$row['Ib']."</td>";
                    $td7 .= "<td>".$row['Ic']."</td>";
                    $td8 .= "<td>".$row['Total Power']."</td>";
                    $td9 .= "<td>".$row['Total Fundamental Power']."</td>";
                    $td10 .= "<td>".$row['Phase A Power']."</td>";
                    $td11 .= "<td>".$row['Phase B Power']."</td>";
                    $td12 .= "<td>".$row['Phase C Power']."</td>";
                    $td13 .= "<td>".$row['Reactive Power']."</td>";
                    $td14 .= "<td>".$row['Phase A Reactive Power']."</td>";
                    $td15 .= "<td>".$row['Phase B Reactive Power']."</td>";
                    $td16 .= "<td>".$row['Phase C Reactive Power']."</td>";
                    $td17 .= "<td>".$row['Consumed Power']."</td>";
                    $td18 .= "<td>".$row['Sold Power']."</td>";
                }
//                    echo "<br>";
//                    echo "Va: ".$row['Va']."-".
//                    "Vb: ".$row['Vb']."； ".
//                    "Vc: ".$row['Vc']."； ".
//                    "Ia: ".$row['Ia']."； ".
//                    "Ib: ".$row['Ib']."； ".
//                    "Ic: ".$row['Ic']."<br>".
//                    "Total_Power: ".$row['Total Power']."； ".
//                    "Total_Fundamental_Power: ".$row['Total Fundamental Power']."； ".
//                    "Phase_A_Power: ".$row['Phase A Power']."； ".
//                    "Phase_B_Power: ".$row['Phase B Power']."； ".
//                    "Phase_C_Power: ".$row['Phase C Power']."； ".
//                    "Reactive_Power: ".$row['Reactive Power']."<br>".
//                    "Phase_A_Reactive_ower: ".$row['Phase A Reactive Power']."； ".
//                    "Phase_B_Reactive_Power: ".$row['Phase B Reactive Power']."； ".
//                    "Phase_C_Reactive_Power: ".$row['Phase C Reactive Power']."； ".
//                    "Consumed_Power: ".$row['Consumed Power']."； ".
//                    "Sold_Power: ".$row['Sold Power']."<br>";       
//                }
?>



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
                <h2 align="center">Information Table</h2>
                
                <table border="1" align="center">
                <thead>
                    <tr align="center">
                        <th>Id</th>
                        <?php echo $td1; ?>
                    </tr>
                    <tr align="center">
                        <th>Va</th>
                        <?php echo $td2; ?>
                    </tr>
                    <tr align="center">
                        <th>Vb</th>
                        <?php echo $td3; ?>
                    </tr>
                    <tr align="center">
                        <th>Vc</th>
                        <?php echo $td4; ?>
                    </tr>
                    <tr align="center">
                        <th>Ia</th>
                        <?php echo $td5; ?>
                    </tr>
                    <tr align="center">
                        <th>Ib</th>
                        <?php echo $td6; ?>
                    </tr>
                    <tr align="center">
                        <th>Ic</th>
                        <?php echo $td7; ?>
                    </tr>
                    <tr align="center">
                        <th>Total Power</th>
                        <?php echo $td8; ?>
                    </tr>
                    <tr align="center">
                        <th>Total Fundamental Power</th>
                        <?php echo $td9; ?>
                    </tr>
                    <tr align="center">
                        <th>Phase A Powerr</th>
                        <?php echo $td10; ?>
                    </tr>
                    <tr align="center">
                        <th>Phase B Power</th>
                        <?php echo $td11; ?>
                    </tr>
                    <tr align="center">
                        <th>Phase C Power</th>
                        <?php echo $td12; ?>
                    </tr>
                    <tr align="center">
                        <th>Reactive Power</th>
                        <?php echo $td13; ?>
                    </tr>
                    <tr align="center">
                        <th>Phase A Reactive Power</th>
                        <?php echo $td14; ?>
                    </tr>
                    <tr align="center">
                        <th>Phase B Reactive Power</th>
                        <?php echo $td15; ?>
                    </tr>
                    <tr align="center">
                        <th>Phase C Reactive Power</th>
                        <?php echo $td16; ?>
                    </tr>
                    <tr align="center">
                        <th>Consumed Power</th>
                        <?php echo $td17; ?>
                    </tr>
                    <tr align="center">
                        <th>Sold Power</th>
                        <?php echo $td18; ?>
                    </tr>                               
                </thead>
                   </table>
<!--                   <div></div>-->
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


	
		

