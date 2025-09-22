<?php
$x = null;
var_dump(5);
echo "<br>";
var_dump("Asep");
echo "<br>";
var_dump(3.14);
echo "<br>";
var_dump([1,2,45,7]);
echo "<br>";
var_dump(true);
echo "<br>";
var_dump($x);


// change value of variable
$y = "<br><h4>john wick</h4>";

echo $y;
$y = 1;
echo $y;

// ubah type data tpi ga ngubah value nya
echo "<br>";
$z = "17";
var_dump($z);

echo "<br>";

$z = (int) $z;
var_dump($z);

?>