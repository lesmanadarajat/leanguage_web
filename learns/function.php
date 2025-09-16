<?php
#1.
$z = $x = 5; // <-- global variabel

function my_test() {
    $y = 7; // <-- local variabel
    echo "<p>Variable x inside function : $x</p>"; //<-- x nya ga akan di nampil
    echo "<p>ini variabel y = $y</p>";
}

my_test();
echo "<p>Variable x inside function is: $x</p>";

#2.
// cara biar bisa akses variabel global ke dlm func
function Test() {
    global $z, $x;
    $z = $z + $x;
}
Test();
echo "isi dari variabel z = $z<br>";

#3.
$a = 7;
$b = 9;
function Tambah(){
    $GLOBALS['a'] = $GLOBALS['a'] + $GLOBALS['b'];
}

echo "<br>a = $a";
Tambah();
echo "<br>sekarang a adalah $a";

?>
