<?php

include 'layout/header.php';

// Mengambil id_barang dari url dengan validasi
if (isset($_GET['id_barang'])) {
    $id_barang = (int)$_GET['id_barang'];
} else {
    echo "<script>
            alert('system error');
            document.location.href = 'index.php';
          </script>";
    exit;
}

// Mengambil data barang
$barang = select("SELECT * FROM barang WHERE id_barang = $id_barang");

if (!$barang) {
    echo "<script>
            alert('Data Barang tidak ditemukan');
            document.location.href = 'index.php';
          </script>";
    exit;
}

$barang = $barang[0]; // Ambil data barang

// Check apakah tombol ditekan
if (isset($_POST['tambah'])) {
    if (create_barang($_POST) > 0) {
        echo "<script>
                alert('Data Barang Berhasil Ditambahkan');
                document.location.href = 'index.php';
            </script>";
    } else {
        echo "<script>
                alert('Data Barang Gagal Ditambahkan');
                document.location.href = 'index.php';
            </script>";
    }
}

?>


<div class="container mt-5">
    <h1>Ubah Barang</h1>
    <hr>
 
    <form action="" method="post">
        <div class="mb-3">
            <label for="nama" class="form-label">Nama Barang</label>
            <input type="text" class="form-control" id="nama" name="nama" placeholder="Nama barang..."
            required>
        </div>

        <div class="mb-3">
            <label for="jumlah" class="form-label">Jumlah</label>
            <input type="number" class="form-control" id="jumlah" name="jumlah" placeholder="Jumlah barang..."
            required>
        </div>

        <div class="mb-3">
            <label for="harga" class="form-label">Harga</label>
            <input type="number" class="form-control" id="harga" name="harga" placeholder="Harga barang..."
            required>
        </div>

        <button type="submit" name="tambah" class="btn btn-primary" style="float: right;">Tambah</button>
    </form>
</div>

<?php include 'layout/header.php'; ?>