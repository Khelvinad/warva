# Game Design Document — WARVA

**Versi:** 0.2 · **Status:** rancangan awal · **Genre:** simulasi manajemen waktu · **Platform:** Windows PC offline · **Pemain:** satu orang

## 1. Konsep dan daya tarik

WARVA (Warung Java) menempatkan pemain sebagai satu staf warung yang menangani pelanggan dari kedatangan sampai pulang. Suasana warung Jawa dibangun lewat bentuk bangunan, papan menu, perabot, warna lampu, dan detail lingkungan; representasinya dibuat hangat dan mudah dibaca dari sudut isometrik. Tantangan utamanya adalah memilih tugas yang paling mendesak saat dapur, meja makan, kasir, dan parkir berebut perhatian.

**Janji pengalaman:** satu shift singkat yang mudah dipahami, makin sibuk, dan memberi keputusan nyata tentang urutan pelayanan.

## 2. Layar dan alur sesi

- Kamera tetap menampilkan ruang warung dan parkir yang berdampingan dalam satu adegan 1280 × 720. Pemain tidak perlu berganti layar saat menangani keduanya.
- Menu utama menyediakan Mulai, Cara Bermain, dan Keluar. Mading di dalam warung membuka credits berisi nama/NIM anggota setelah data diberikan.
- Satu shift berlangsung **5 menit waktu nyata**. Target awal adalah **150 koin** sebelum waktu habis. Ini angka prototipe yang disetel setelah uji main.
- Pemain menang jika pendapatan mencapai target dan reputasi masih di atas nol saat shift berakhir. Pemain kalah segera jika reputasi mencapai nol, atau saat waktu habis sebelum target tercapai.
- Layar hasil menampilkan pendapatan makanan, pendapatan parkir, pelanggan terlayani, pelanggan pergi, dan reputasi akhir; lalu pemain dapat mengulang shift.

## 3. Putaran permainan utama

1. Pelanggan datang membawa motor atau mobil dan menunggu di gerbang. Pemain memilih kendaraan lalu slot parkir yang sesuai.
2. Kendaraan bergerak menuju slot; pelanggan masuk dan memilih meja bersih yang kosong.
3. Bubble pesanan muncul. Pemain mengeklik pesanan, lalu stasiun yang diperlukan untuk mulai memasak atau meracik.
4. Saat pesanan siap, pemain mengeklik hasilnya lalu pelanggan untuk menyajikan. Makanan yang dibiarkan terlalu lama dapat gosong dan harus dibuat ulang.
5. Setelah makan, pelanggan menuju kasir. Pemain mengeklik kasir untuk menerima pembayaran.
6. Pemain memilih kendaraan pelanggan tersebut untuk membuka jalur keluar. Setelah kendaraan keluar, koin parkir ditambahkan dan slot kembali tersedia.

Setiap tindakan dipicu klik mouse. Klik dan tahan dua detik digunakan untuk membersihkan meja. Target interaksi diberi sorotan saat kursor berada di atasnya; tindakan yang belum dapat dilakukan menampilkan alasan singkat di UI.

## 4. Aturan awal untuk MVP

| Sistem | Aturan prototipe |
| --- | --- |
| Kapasitas | 3 meja, 3 slot motor, 1 slot mobil, 1 stasiun kompor, 1 stasiun minuman, 1 kasir. |
| Menu | Mi rebus, mi goreng, dan es teh. Setiap pelanggan memesan satu item. |
| Waktu masak | Mi: 8 detik; es teh: 4 detik. Mi gosong setelah 5 detik tambahan tanpa diambil. |
| Waktu makan | 20 detik setelah pesanan disajikan; pelanggan lalu menuju kasir. |
| Kesabaran | Pelanggan menunggu maksimal 30 detik untuk parkir dan 45 detik untuk pesanan. Kesabaran ditampilkan melalui indikator visual. |
| Pendapatan | Mi: 15 koin; es teh: 10 koin; parkir motor: 3 koin; parkir mobil: 5 koin. |
| Reputasi | Mulai dari 5; pelanggan pergi karena menunggu terlalu lama atau parkir penuh mengurangi 1. Maksimum 5. |
| Kedatangan | Sekitar satu pelanggan tiap 15–25 detik, dengan variasi acak terikat seed untuk pengujian. |
| Jenis kendaraan | 80% motor dan 20% mobil, dipilih generator acak ber-seed yang sama; rasio disimpan di konfigurasi. |

Angka dan jumlah objek di atas disimpan dalam konfigurasi permainan agar dapat diubah tanpa menyentuh logika inti. Tidak ada pembelian upgrade, multiplayer, atau level tambahan dalam MVP.

Meja kotor belum termasuk MVP: pada MVP meja kembali tersedia saat pelanggan bangkit menuju kasir. Status `Dirty` dan aksi pembersihan baru diaktifkan pada Sprint 4 (WV-020).

## 5. Prioritas tugas dan tantangan

Pemain melihat antrean tugas melalui ikon di tepi layar: kendaraan menunggu, pesanan baru, masakan siap, meja kotor, pembayaran, dan kendaraan keluar. Bila beberapa tugas muncul sekaligus, pemain bebas memilih urutan; konsekuensinya adalah waktu tunggu pelanggan.

**Insiden pertama setelah MVP:** satu meja dapat menjadi kotor setelah pelanggan pergi. Meja tidak dapat digunakan sampai pemain menahan klik selama dua detik. Insiden parkir sembarangan adalah fitur lanjutan; motor dapat menghalangi satu slot lain dan dibetulkan lewat aksi tegur yang jelas, bukan klik berulang tanpa umpan balik.

**Rush hour:** pada menit ke-3, frekuensi kedatangan meningkat selama 45 detik. Event ini baru diaktifkan setelah alur dasar stabil. Jika slot penuh, pelanggan menunggu hingga batas kesabaran lalu pergi; UI menampilkan penyebab hilangnya reputasi.

## 6. Arah visual dan audio

- Objek digambar sebagai sprite 2D isometrik. Siluet meja, kompor, kasir, kendaraan, dan pelanggan harus mudah dibedakan pada ukuran layar target.
- **Dua gaya tampilan karakter:** saat berbicara, layar pengenalan, atau preview penuh, tampilkan ilustrasi pixel art yang lebih detail dan ekspresif seperti pendekatan gambar referensi pertama. Selama bergerak dan bekerja di adegan game, gunakan sprite chibi pixel art yang sederhana dan mudah dibaca seperti pendekatan gambar referensi kedua. Yang diambil hanya bahasa visualnya; desain tokoh WARVA dibuat orisinal.
- Setiap tokoh mempertahankan ciri yang sama pada kedua gaya: palet pakaian, rambut, aksesori, dan bentuk siluet. Dialog memakai potret di panel UI; preview penuh dapat menampilkan versi badan lengkap dengan tingkat detail yang sama. Karakter dalam dunia tetap mengikuti sudut isometrik 3/4.
- Palet sore memakai warna hangat; malam memakai lingkungan lebih gelap dengan lampu warung dan jalan. Siklus sore–malam berlangsung sepanjang shift, tanpa mengurangi keterbacaan UI.
- Uap makanan, cipratan air, koin melayang, dan lampu depan kendaraan adalah efek tambahan setelah mekanik inti bekerja.
- Audio lokal mencakup umpan balik klik, pesanan siap, pembayaran, kendaraan, dan musik latar pendek yang dapat dimatikan.
- Aset dibuat sendiri atau dari sumber berlisensi sesuai aturan tugas. Semua sumber aset dicatat pada credits/dokumen aset.

Spesifikasi ukuran, animasi minimum, serta cara menghubungkan potret dan sprite dijelaskan dalam `06-Character-Art-Direction.md`.

## 7. Cakupan rilis

**MVP wajib:** menu utama, tutorial singkat, satu shift, alur pelanggan lengkap, tiga item menu, slot parkir, gerak kendaraan, pendapatan, reputasi, hasil akhir, credits, serta sprite chibi untuk staf dan pelanggan. Potret detail minimum untuk staf dan satu pelanggan digunakan pada tutorial/dialog singkat.

**Tambahan berurutan:** meja kotor → rush hour → pencahayaan sore–malam → partikel dan animasi koin → parkir sembarangan. Tambahan dipotong dari belakang bila jadwal menyempit.

## 8. Kriteria pengalaman

Pemain baru harus memahami tindakan pertama tanpa penjelasan lisan; satu sesi dapat diselesaikan dari menu utama sampai layar hasil; setiap kehilangan reputasi memiliki penyebab yang terlihat; objek tidak saling menutupi secara keliru ketika pelanggan dan kendaraan bergerak.
