<?php include "../db.php"?>
<?php

//list($Va,$Vb,$Vc,$Ia,$Ib,$Ic,$Total_Power,$Total_Fundamental_Power,$Phase_A_Power,$Phase_B_Power,$Phase_C_Power,$Reactive_Power,$Phase_A_Reactive_ower,$Phase_B_Reactive_Power,$Phase_C_Reactive_Power,$Consumed_Power,$Sold_Power) = explode(",",$buf)
//global $abc;
//$data = "00000000210,00000000209,00000000209";
//list($user, $pass, $uid) = explode(",", $data);
//echo $user; // foo
//echo $pass;
//echo $abc;// *
global $conn;
//
    
    
//mysql_select_db("my_db", $con);
//$query = "INSERT INTO smartgrid(Va, Vb, Ia) VALUES ('$user','$pass','$uid')";
//$result = mysqli_query($conn, $query);
//if(!$result) {
//    die('Query FAILED' . mysqli_error());
//}
//echo "1 record added";
    
//$str = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17";
//list($Va,$Vb,$Vc,$Ia,$Ib,$Ic,$Total_Power,$Total_Fundamental_Power,$Phase_A_Power,$Phase_B_Power,$Phase_C_Power,$Reactive_Power,$Phase_A_Reactive_ower,$Phase_B_Reactive_Power,$Phase_C_Reactive_Power,$Consumed_Power,$Sold_Power) = explode(",",$str);
//$query = "INSERT INTO smartgrid VALUES ('$Va','$Vb','$Vc','$Ia','$Ib','$Ic','$Total_Power','$Total_Fundamental_Power','$Phase_A_Power','$Phase_B_Power','$Phase_C_Power','$Reactive_Power','$Phase_A_Reactive_ower','$Phase_B_Reactive_Power','$Phase_C_Reactive_Power','$Consumed_Power','$Sold_Power')";
//$result = mysqli_query($conn, $query);
//if(!$result) {
//    die('Query FAILED' . mysqli_error());
//}
//echo "1 record added"; 

//$string = "{00000000210,00000000209,00000000209}";
//$buf = str_replace(array('{','}'), '', $string);
//echo $buf 

//$buf = "000000210,09000000209,000000209";
//$counter = 1;
//$array = array(explode(',', strval($counter).','.$buf));
//$fp = fopen('file.csv', 'a');
//foreach ($array as $fields) {
//    fputcsv($fp, $fields);
//}

    
//$buf = "000000211,000000211,0000000211";
//$array = array(explode(',', $buf));
//$fp = fopen('file.csv', 'w');
//foreach ($array as $fields) {
//    fputcsv($fp, $fields);
////    $fields = (int)$fields;
////    echo $fields;
//}
//fclose($fp);

$file = fopen('file.csv', 'w')

//unlink('file.csv');
//$row = 1;
//if (($handle = fopen("file.csv", "r")) !== FALSE) {
//  while (($data = fgetcsv($handle, ",")) !== FALSE) {
//    $num = count($data);
//    echo "<p> $num fields in line $row: <br /></p>\n";
//    $row++;
//    for ($c=0; $c < $num; $c++) {
//        echo $data[$c] . "<br />\n";
//    }
//  }
//  fclose($handle);
//}
//unlink('file.csv'); 
?>








<!--table-->
<?php 
//global $conn;
//$query2 = "SELECT * FROM smartgrid";
//$result = mysqli_query($conn, $query2);
//if(!$result) {
//    die('Query FAILED' . mysqli_error());
//}
//while($row = mysqli_fetch_assoc($result)) {
//    echo "<br>";
//    echo "Va: ".$row['Va']."-".
//    "Vb: ".$row['Vb']."； ".
//    "Vc: ".$row['Vc']."； ".
//    "Ia: ".$row['Ia']."； ".
//    "Ib: ".$row['Ib']."； ".
//    "Ib: ".$row['Ib']."<br>".
//    "Total_Power: ".$row['Total Power']."； ".
//    "Total_Fundamental_Power: ".$row['Total Fundamental Power']."； ".
//    "Phase_A_Power: ".$row['Phase A Power']."； ".
//    "Phase_B_Power: ".$row['Phase B Power']."； ".
//    "Phase_C_Power: ".$row['Phase C Power']."； ".
//    "Reactive_Power: ".$row['Reactive Power']."<br>".
//    "Phase_A_Reactive_ower: ".$row['Phase A Reactive Power']."； ".
//    "Phase_B_Reactive_Power: ".$row['Phase B Reactive Power']."； ".
//    "Phase_C_Reactive_Power: ".$row['Phase C Reactive Power']."； ".
//    "Consumed_Power: ".$row['Consumed Power']."； ".
//    "Sold_Power: ".$row['Sold Power']."<br>";       
//}
?>

