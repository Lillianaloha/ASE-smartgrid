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
    <style>
        
        button {
    background-color: #4CAF50;
    color: white;
    padding: 14px 20px;
    margin: 50px;
    display: table;
    border: none;
    cursor: pointer;
    width: 30%;
}
        
        </style>

    </head>

	<body>
		<!--1、头部-->
		<div id="header">
			<div class="header-content">
				<div class="logo">
					<a href="../index.php"> <img src="../img/a.png" height="82" width="256" /> </a> 
<!--					logo picture should be changed-->
					<div class="key">
                        <p>Class Project</p>
					</div>
				</div>
				<div class="nav">
					<ul>
						<li> <a href="../index.php">Home</a> </li>
						<li class="active"> <a href="success.php">Search</a> </li>
                        <li> <a href="about.php">About Us</a> </li>
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
               <h2 align="center">
                    ---------------
               </h2>
                <h2 align="center">CSV FILE HASH</h2>
                <h2 align="center">
                    ---------------
               </h2>
                
                <table style="width:80%" border="1" align="center">
                  <tr>
                    <th height="60" >CSV FILE</th>
                    <th height="60" >HASH</th> 
                  </tr>
                  <tr>
                    <td height="60" align="center"><a href="../plot/file.csv">File 1</a></td>
                    <td height="60" align="center">Smith</td>
                  </tr>
                  <tr>
                    <td height="60" align="center">Eve</td>
                    <td height="60" align="center">Jackson</td>
                  </tr>
                  <tr>
                    <td height="60" align="center">John</td>
                    <td height="60" align="center">Doe</td>
                  </tr>
                </table>
<!--                   <div></div>-->
            </div>
                   
            <form action="table.php" method="get">

          <div class="container">
            <button type="submit">Return to check table</button>
          </div>
        </form>
		</div>

	
		<!--3、尾部-->
		<div id="footer">
			<p>
				<a href="../index.php">Home</a> |
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


	
		

