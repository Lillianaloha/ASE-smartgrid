<!DOCTYPE HTML>
<html>
<head>  
<meta charset="UTF-8">
<script>
window.onload = function () {


var dataPoints_1 = [];
//dataPoints_1 = [{x:1,y:2}];
dataPoints_1.push({x:2,y:3});
//var dataPoints_2 = [];
//var dataPoints_3 = [];
//var y = 1000;
//var limit = 100;
//var y1 = 1000;
//var y2 = 1000;
//var y3 = 1000;

//for ( var i = 0; i < limit; i++ ) {
//	y1 +=Math.round( 10 + Math.random() * (-10 -10));
//    y2 +=Math.round( 10 + Math.random() * (-10 -10))+100;
//	y3 +=Math.round( 10 + Math.random() * (-10 -10))-100;	
//	dataPoints_1.push({ y: y1 });
//	dataPoints_2.push({ y: y2 });
//	dataPoints_3.push({ y: y3 });
//}

var chart = new CanvasJS.Chart("chartContainer", {
	animationEnabled: true,
	title:{
		text: "Performance Demo"
	},
	subtitles:[{
		text: "Plotting Time vs Current or Phase Voltage"
	}],
	axisX: {
		title: 'Plotting Time'
		// valueFormatString: "DD MMM,YY"
	},
	axisY: {
		includeZero: false,
		title: 'Current or Phase Voltage'
	},
	legend:{
		cursor: "pointer",
		fontSize: 16,
		itemclick: toggleDataSeries
	},
	toolTip:{
		shared: true
	},
	data: [{
        <?php 
        
        $str = "210,209,209";
        $array = array(explode(',', $str));
        $fp = fopen('file.csv', 'a');
        foreach ($array as $fields) {
            fputcsv($fp, $fields);
        }
        $buf = "211,211,211";
        $array = array(explode(',', $buf));
        $fp = fopen('file.csv', 'a');
        foreach ($array as $fields) {
            fputcsv($fp, $fields);
        }
        $row = 1;
        if (($handle = fopen("file.csv", "r")) !== FALSE) {
          while (($data = fgetcsv($handle, ",")) !== FALSE) {
            $num = count($data);
            echo "<p> $num fields in line $row: <br /></p>\n";
            $row++;
            for ($c=1; $c <= $num; $c++) {
                dataPoints_1.push({x:$data[$c],y:$c});
//                echo $data[$c] . "<br />\n";
            }
          }
          fclose($handle);
//        }
        
        ?>
		name: "line1",
		type: "spline",
		// yValueFormatString: "#0.## °C",
		showInLegend: true,
		dataPoints: dataPoints_1
//		 [
//		 	{ x: new Date(2017,6,24), y: 31 },
//		 	{ x: new Date(2017,6,25), y: 31 },
//		 	{ x: new Date(2017,6,26), y: 29 },
//		 	{ x: new Date(2017,6,27), y: 29 },
//		 	{ x: new Date(2017,6,28), y: 31 },
//		 	{ x: new Date(2017,6,29), y: 30 },
//		 	{ x: new Date(2017,6,30), y: 29 }
//		 ]
	}
//	{
//		name: "line2",
//		type: "spline",
//		// yValueFormatString: "#0.## °C",
//		showInLegend: true,
//		dataPoints:
////		 [
////		 	{ x: new Date(2017,6,24), y: 20 },
////		 	{ x: new Date(2017,6,25), y: 20 },
////		 	{ x: new Date(2017,6,26), y: 25 },
////		 	{ x: new Date(2017,6,27), y: 25 },
////		 	{ x: new Date(2017,6,28), y: 25 },
////		 	{ x: new Date(2017,6,29), y: 25 },
////		 	{ x: new Date(2017,6,30), y: 25 }
////		 ]
//	},
//	{
//		name: "line3",
//		type: "spline",
//		// yValueFormatString: "#0.## °C",
//		showInLegend: true,
//		dataPoints: 
////		 [
////		 	{ x: new Date(2017,6,24), y: 22 },
////		 	{ x: new Date(2017,6,25), y: 19 },
////		 	{ x: new Date(2017,6,26), y: 23 },
////		 	{ x: new Date(2017,6,27), y: 24 },
////		 	{ x: new Date(2017,6,28), y: 24 },
////		 	{ x: new Date(2017,6,29), y: 23 },
////		 	{ x: new Date(2017,6,30), y: 23 }
////		 ]
//	}]
           ]
});
chart.render();

function toggleDataSeries(e){
	if (typeof(e.dataSeries.visible) === "undefined" || e.dataSeries.visible) {
		e.dataSeries.visible = false;
	}
	else{
		e.dataSeries.visible = true;
	}
	chart.render();
}

}
</script>
</head>
<body>
<div id="chartContainer" style="height: 370px; max-width: 920px; margin: 0px auto;"></div>
<script src="./canvasjs.min.js"></script>
</body>
</html>