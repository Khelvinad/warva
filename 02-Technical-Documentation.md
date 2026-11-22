# Technical Documentation — WARVA

**Versi:** 0.2 · **Target:** Windows x64, OpenGL 3.3 Core, 1280 × 720, 60 FPS pada perangkat uji tim

## 1. Arsitektur

Gunakan *game loop* dengan pembaruan logika pada langkah tetap 1/60 detik dan render setelahnya. Waktu nyata yang berlalu diakumulasi agar kecepatan permainan tidak berubah menurut FPS. Modul utama:

| Modul | Tanggung jawab | Antarmuka utama |
| --- | --- | --- |
| `App` | Inisialisasi jendela/OpenGL, perpindahan menu–game–hasil, shutdown. | `run()`, `changeScene(SceneId)` |
| `Input` | Posisi mouse, klik, hold, hit test objek. | `poll()`, `consumeClick()`, `holdDuration()` |
| `World` | State pelanggan, meja, stasiun, kendaraan, slot, waktu shift. | `update(dt)`, `apply(Command)` |
| `Gameplay` | Aturan pesanan, timer, skor, reputasi, kondisi akhir. | `update(dt)`, `getResult()` |
| `Navigation` | Rute kendaraan dari gerbang ke slot/keluar. | `findRoute(start, goal)` |
| `Renderer` | Gambar sprite, depth sorting, shader pencahayaan, UI. | `render(WorldView)` |
| `Assets` | Memuat atlas sprite, font, shader, dan audio dari data tertanam. | `getTexture(id)`, `getFont(id)` |

Identitas karakter disimpan sebagai `CharacterVisualSet` berisi `characterId`, ID potret dialog, ID preview penuh, ID sprite gameplay, dan ID palet. Panel dialog atau preview mengambil aset detail; renderer dunia mengambil atlas sprite chibi dari ID tokoh yang sama. Pergantian tampilan terjadi pada batas UI/adegan, bukan dengan memperbesar sprite gameplay.

`Input` menghasilkan perintah semantik seperti `AssignParking(vehicleId, slotId)`, `StartOrder(orderId, stationId)`, `Serve(orderId, customerId)`, `Clean(tableId)`, atau `Checkout(customerId)`. `World` memeriksa prasyarat dan mengembalikan hasil berhasil/gagal beserta alasan untuk UI. Renderer hanya membaca *snapshot* state; ia tidak mengubah aturan permainan.

## 2. Model data dan status

- `Customer`: ID, kendaraan, pesanan, meja, kesabaran, timer makan, status (`Arriving`, `WaitingParking`, `Ordering`, `WaitingFood`, `Eating`, `Paying`, `Leaving`, `Gone`).
- `Vehicle`: ID, jenis (`Motorcycle`/`Car`), posisi, arah, slot, rute, status (`Gate`, `Entering`, `Parked`, `Exiting`).
- `Table`: ID, posisi, status (`CleanFree`, `Occupied`; `Dirty` ditambahkan pada WV-020/S4).
- `Station`: jenis, status (`Idle`, `Cooking`, `Ready`, `Burnt`), order aktif, timer.
- `GameSession`: sisa waktu, uang makanan, uang parkir, reputasi, generator kedatangan (interval dan bobot jenis kendaraan), fase pencahayaan, status menang/kalah.

ID harus stabil selama objek hidup. Satu kendaraan hanya boleh menguasai satu slot; slot dibebaskan setelah kendaraan benar-benar keluar. Event waktu diproses dalam urutan tetap: input → perubahan state → timer dan kedatangan → evaluasi hasil → render. Pada MVP, meja kembali `CleanFree` saat pelanggan bangkit menuju kasir; mulai WV-020, transisinya menjadi `Dirty` sampai pemain menyelesaikan aksi pembersihan.

## 3. Grafika isometrik

Koordinat dunia memakai kisi 2D `(x, y)`; sprite ditempatkan pada layar dengan `screenX = originX + (x - y) × tileWidth / 2` dan `screenY = originY + (x + y) × tileHeight / 2 - elevation`. Prototipe memakai tile 64 × 32 piksel. Titik jangkar sprite berada di tengah bawah agar kaki karakter dan roda kendaraan tepat pada ubin.

Untuk urutan gambar, urutkan objek dunia berdasarkan posisi kaki pada layar (`screenY`), lalu kunci sekunder `layer` dan ID agar hasil stabil. Lantai digambar lebih dulu, lalu bayangan, objek dunia, efek cahaya, dan UI. Objek besar yang melintasi beberapa ubin dipecah jika satu titik jangkar tidak cukup menghasilkan tumpang tindih yang benar.

Transformasi kendaraan memakai matriks model 2D: translasi ke posisi dunia, rotasi menuju arah gerak, dan skala sprite. Posisi mengikuti rute secara interpolasi berdasarkan `dt`; rotasi dibatasi kecepatan sudut agar belokan terlihat halus. Rute dihitung pada graf waypoint jalan parkir dengan pencarian jalur terpendek; waypoint memasukkan gerbang, persimpangan, titik masuk slot, dan pintu keluar. Rute tidak melewati slot yang terisi atau jalur terhalang.

Pencahayaan sore–malam adalah tahap tambahan: shader mengalikan warna lingkungan berdasarkan kemajuan shift, lalu menambahkan sumber cahaya lokal sederhana pada lampu warung/jalan. UI dirender tanpa redup. Efek partikel menggunakan sprite transparan berumur terbatas dan jumlah maksimum agar FPS stabil.

## 4. Aset dan distribusi

Simpan aset sumber dalam direktori kerja pengembang, dengan manifest ID dan atribusi. Pada build rilis, atlas, font, shader, dan audio disematkan sebagai byte dalam executable; loader membacanya dari memori. Data pengguna seperti pengaturan suara atau skor terbaik, bila dibuat, disimpan di direktori data pengguna Windows saat runtime, bukan di lokasi executable. Game tetap berjalan tanpa internet.

Potret dan sprite gameplay dibuat sebagai file sumber terpisah lalu dikemas dalam atlas masing-masing. Gunakan filter tekstur `NEAREST`, koordinat UV yang selaras dengan piksel, dan skala bilangan bulat untuk panel potret agar tepian pixel art tetap tajam. Animasi sprite mengikuti atlas dengan ukuran frame tetap; dimensi dan daftar animasi tercantum dalam panduan karakter.

Build `Debug` mempertahankan log dan pemeriksaan OpenGL; build `Release` menonaktifkan log rinci. Perintah build dan dependensi dikunci di CMake. Pengemasan satu `.exe` diverifikasi pada komputer Windows lain yang tidak memiliki tool pengembang.

## 5. Penanganan kegagalan

- Shader atau aset penting gagal dimuat: tampilkan pesan yang menyebut ID aset dan hentikan startup dengan aman.
- Slot tidak cocok, terisi, atau rute tidak tersedia: jangan ubah state; tampilkan alasan singkat.
- Klik ganda atau hold terputus: satu tindakan hanya diproses satu kali ketika prasyarat terpenuhi.
- Jendela kehilangan fokus: game otomatis pause agar timer pelanggan tidak berjalan tanpa kendali.
- Ukuran jendela berubah: adegan diskalakan dengan rasio tetap; area klik memakai transformasi layar yang sama dengan render.

## 6. Pengujian dan bukti akademis

Uji otomatis logika yang tidak bergantung OpenGL: satu slot tidak diberikan ke dua kendaraan, rute tidak melewati hambatan, timer masakan tepat, reputasi tidak keluar dari rentang 0–5, dan kondisi menang/kalah. Uji manual alur penuh dari menu hingga hasil, posisi klik setelah resize, depth sorting saat dua objek berpapasan, serta transisi pencahayaan. Dokumentasikan tangkapan layar atau rekaman singkat untuk demonstrasi transformasi, z-sorting, dan shader; sertakan rumus dan potongan kode buatan tim pada laporan akhir.
