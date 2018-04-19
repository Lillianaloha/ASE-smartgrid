<?php include "../db.php";?>
<?php include "../function.php";?>

<?php 
    global $conn;
    $id = $_POST['id'];
    $query = "SELECT * FROM smartgrid ";
    $query .= "WHERE TeamID = $id ";
    $result = mysqli_query($conn, $query);
    if(!$result) {
        die('Query FAILED' . mysqli_error());
    }
    while($row = mysqli_fetch_assoc($result)) {
        
//        print_r($row);
        $user = $row['User'];
        $node = $row['NodeID'];
        
    }
    echo $id;
?>

<!DOCTYPE html>
<html>
	<head>
		<meta charset="UTF-8">
		<title> Smartgrid | Search | Results</title>
		<link rel="stylesheet" href="../css/base.css" />
		<link rel="stylesheet" href="../css/index.css" />
		<link rel="stylesheet" href="../css/success.css" />
        <style>
h2 {
    width: 100%;
    text-align:center;
    margin-top: 20px;
    margin-bottom: 20px;
}
table {
    border-collapse: collapse;
    width: 50%;
    margin-left: auto;
    margin-right: auto;
    margin-bottom: 50px;
}

th, td {
    text-align: left;
    padding: 8px;
}

tr:nth-child(even){background-color: #f2f2f2}

th {
    background-color: #4CAF50;
    color: white;
}
</style>
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

                    <table>
                    <tr>
                        <th>TeamID</th>
                        <td><?php echo $id;?></td>

                    </tr>
                    <tr>
                        <th>User</th>
                        <td><?php echo $user;?></td>
                    </tr>
                    <tr>
                        <th>NodeID</th>
                        <td><?php echo $node;?></td>
                    </tr>
                    <tr>
                        <th></th>
                        <td></td>
                    </tr>
                    <tr>
                        <th></th>
                        <td></td>
                    </tr>
                    <tr>
                        <th></th>
                        <td></td>
                    </tr>
                    <tr>
                        <th></th>
                        <td></td>
                    </tr>
                    <tr>
                        <th></th>
                        <td></td>
                    </tr>                    
                </table>
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


	
		

