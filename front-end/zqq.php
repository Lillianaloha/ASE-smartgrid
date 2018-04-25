<?php

//header('Location: plot/index_graph.html') 
    
//    
//$time_rate = $_GET['time'];
//$sampling_rate = "000".$_GET['rate'];
//echo $time_rate;
//echo $sampling_rate;
    
//    $a = "10";
//$c = intval($a);
//if($c == 10){echo $c;}
//

//$myfile = fopen("portnumber.txt", "r");
//$txt = fread($myfile,filesize("portnumber.txt"));
//$port = intval($txt);
//echo $port;
//$content = str_replace($txt,strval($port+1),$txt);
//$myfile2 = fopen("portnumber.txt", "w");
//fwrite($myfile2,$content);




echo exec('/users/zqq/exe Yupeng');
//echo strval(123).'.csv';



 list($Va,$Vb,$Vc,$Ia,$Ib,$Ic,$Total_Power,$Total_Fundamental_Power,$Phase_A_Power,$Phase_B_Power,$Phase_C_Power,$Reactive_Power,$Phase_A_Reactive_Power,$Phase_B_Reactive_Power,$Phase_C_Reactive_Power,$Consumed_Power,$Sold_Power) = explode(",",$str);
        $query = "INSERT INTO smartgrid2(Va,Vb,Vc,Ia,Ib,Ic,Total Power,Total Fundamental Power,Phase A Power,Phase B Power,Phase C Power,Reactive Power,Phase A Reactive Power,Phase B Reactive Power,Phase C Reactive Power,Consumed Power,Sold Power) VALUES ('$Va','$Vb','$Vc','$Ia','$Ib','$Ic','$Total_Power','$Total_Fundamental_Power','$Phase_A_Power','$Phase_B_Power','$Phase_C_Power','$Reactive_Power','$Phase_A_Reactive_Power','$Phase_B_Reactive_Power','$Phase_C_Reactive_Power','$Consumed_Power','$Sold_Power')";
        $result = mysqli_query($conn, $query);
?>