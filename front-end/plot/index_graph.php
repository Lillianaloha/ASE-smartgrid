<?php include "../db.php";?>
<!DOCTYPE html>
<html>
<head>

</head>
<body>

</body>
</html>


<html>
	<head>
		<meta charset="UTF-8">
		<title> Smartgrid | Search | Results</title>
		<link rel="stylesheet" href="../css/base.css" />
		<link rel="stylesheet" href="../css/index.css" />
		<link rel="stylesheet" href="../css/success.css" />
        <script type="text/javascript"
      src="dygraph.js"></script>
    <style>
    .page-wrapper
        {
            width:1000px;
            margin:auto;
        }
        
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
    <link rel="stylesheet" src="dygraph.css" />

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
						<li> <a href="../index.php">Home</a> </li>
						<li class="active"> <a href="success.php">Search</a> </li>
                        <li> <a href="about.php">About Us</a> </li>
						<li> <a href="server.php">Future</a> </li>
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
            <div class="page-wrapper">
    <br />
    <h2 aligh="center">Voltage Change</h2>
</div>
<div id="graphdiv"></div>
<script type="text/javascript">
    
  g = new Dygraph(

    // containing div
    document.getElementById("graphdiv"),

    // CSV or path to a CSV file.
    "file.csv",
      {labels:["id","va","vb","vc"],       
      }
  );
</script>
        
        
        
        
        
        <form action="../html/table.php" method="post">
          <div class="container">          
<!--
             <label for="ip"><b>IP</b></label>
                <input type="text" placeholder="Enter IP address" name="ip" >            
            <label for="port"><b>Port</b></label>
                <input type="text" placeholder="Enter port number" name="port" >
-->
            <button type="submit">Check Table</button>
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
