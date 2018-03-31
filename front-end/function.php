<?php include "db.php";?>
<?php
function readRows() {
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
        
    }  
}

function mysqli_field_name($result, $field_offset)
{
    $properties = mysqli_fetch_field_direct($result, $field_offset);
    return is_object($properties) ? $properties->name : false;
}
    
function showID(){
    global $conn; 
    $query = "SELECT * FROM smartgrid";
    $result = mysqli_query($conn,$query);
    if(!$result){
        die('query failed' . mysqli_error());
    } 
    while($row = mysqli_fetch_assoc($result)){
    $id = $row['TeamID'];

    echo "<option value='$id'>$id</option>";
    }
}
?>
