var SS = SpreadsheetApp.openById('1ahBeOdHVy9K9LGGR_TG_SUlES45CUxfq7sj14dJfcPc');

function doPost(e) {
  var ss = SpreadsheetApp.getActiveSpreadsheet();
  var sheet = ss.getSheetByName("Sheet1");

  var data = JSON.parse(e.postData.contents);
  var values = data.values.split(",");

  var nama = values[0];
  var absen = values[1];
  var kelas = values[2];
  var jurusan = values[3];
  var gender = values[4];

  var today = Utilities.formatDate(new Date(), "Asia/Jakarta", "dd/MM/yyyy");
  var timeNow = Utilities.formatDate(new Date(), "Asia/Jakarta", "HH:mm:ss");

  var lastRow = sheet.getLastRow();
  var sudahCheckIn = false;
  var rowCheckIn = -1;

  // CARI apakah siswa sudah check-in hari ini
  for (var i = 2; i <= lastRow; i++) {
    var rowDate = Utilities.formatDate(sheet.getRange(i, 1).getValue(), "Asia/Jakarta", "dd/MM/yyyy");
    var rowNama = sheet.getRange(i, 3).getValue();

    if (rowDate == today && rowNama == nama) {
      sudahCheckIn = true;
      rowCheckIn = i;
      break;
    }
  }

  // ============ LOGIKA ABSEN HARIAN ============

  // 1️⃣ BELUM CHECK-IN → BUAT BARIS BARU
  if (!sudahCheckIn) {
    sheet.appendRow([today, timeNow, nama, absen, kelas, jurusan, gender, ""]);
    return ContentService.createTextOutput("CHECKIN");
  }

  // 2️⃣ SUDAH CHECK-IN, BELUM CHECK-OUT → ISI CHECK-OUT
  var checkoutCell = sheet.getRange(rowCheckIn, 8).getValue();
  if (checkoutCell === "") {
    sheet.getRange(rowCheckIn, 8).setValue(timeNow);
    return ContentService.createTextOutput("CHECKOUT");
  }

  // 3️⃣ SUDAH CHECK-IN DAN CHECK-OUT → TOLAK ABSEN ULANG
  return ContentService.createTextOutput("ALREADY_DONE");
}
