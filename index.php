<?php

include 'layout/header.php';

$data_barang = select("SELECT * FROM barang")

?>

<div class="container mt-5">
    <h1>TOKO KOMPUTER LESMANA</h1>
    <hr>

    <a href="tambah-barang.php" class="btn btn-primary mb-1">Tambah</a>

    <table class="table table-bordered table-striped mt-3">
       <thead>
        <tr>
          <th>No</th>
          <th>Nama Barang</th>
          <th>Jumlah</th>
          <th>Harga</th>
          <th>Tanggal</th>
          <th>Aksi</th>
        </tr>
       </thead>

       <tbody>
        <?php $no = 1; ?>
        <?php foreach ($data_barang as $barang) : ?>
        <tr>
          <td><?= $no++; ?></td>
          <td><?= $barang['nama']; ?></td>
          <td><?= $barang['jumlah']; ?></td>
          <td>Rp. <?= number_format($barang['harga'],2,',','.'); ?></td>
          <td><?= date('d/m/Y | H:i:s', strtotime($barang['tanggal'])); ?></td>
          <td width="15%" class="text-center">
              <a href="ubah-barang.php?id_barang<?= $barang['id_barang']; ?>" class="btn btn-success">Ubah</a>
              <a href="" class="btn btn-danger">Hapus</a>
              <a href="ubah-barang.php?id_barang<?= $barang['id_barang']; ?>" class="btn btn-warning">bebas lah</a>
          </td>
        </tr>
        <?php endforeach; ?>
       </tbody>
    </table>
</div>

<?php include 'layout/footer.php'; ?>

    