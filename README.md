# WARVA — Paket Perencanaan Proyek

WARVA (Warung Java) adalah game manajemen waktu desktop berlatar warung makan bernuansa Jawa. Pemain menjalankan satu shift: mengatur parkir, menyiapkan pesanan, menyajikan makanan, menerima pembayaran, dan menjaga kepuasan pelanggan.

Paket ini merupakan panduan kerja tim. Angka keseimbangan di bawah adalah **nilai awal prototipe**, bukan ketentuan dosen; ubah berdasarkan hasil uji main.

| Berkas | Kegunaan |
| --- | --- |
| `01-Game-Design-Document.md` | Identitas game, pengalaman pemain, aturan, cakupan MVP, dan fitur lanjutan. |
| `02-Technical-Documentation.md` | Arsitektur, grafika, alur data, antarmuka modul, dan pengujian teknis. |
| `03-Tech-Stack.md` | Pilihan teknologi, alasan, batasan, dan cara build. |
| `04-Sprint-Board.csv` | Backlog yang dapat diimpor ke aplikasi board. |
| `05-Panduan-Sprint-Board.md` | Aturan penggunaan board, pembagian sprint, dan definisi selesai. |
| `06-Character-Art-Direction.md` | Panduan dua gaya karakter berdasarkan referensi gambar dari pengguna. |

## Keputusan kerja

- Target: Windows PC, mouse, offline, satu pemain, tampilan 2D isometrik.
- Implementasi grafika: C++17 dan OpenGL 3.3 secara langsung; tanpa game engine.
- MVP: satu area warung dan parkir yang terlihat dalam satu adegan, satu shift yang bisa dimenangkan atau kalah, serta satu alur pelanggan lengkap.
- Karakter memiliki dua aset visual yang konsisten: potret detail untuk dialog/preview dan sprite chibi untuk permainan aktif.
- Jadwal awal: empat sprint, masing-masing dua minggu. Bila tenggat hanya enam minggu, fitur tambahan Sprint 4 dikeluarkan dari target wajib.
- Dokumen pengumpulan yang hanya berisi cover dan disclaimer AI disiapkan terpisah setelah data mata kuliah, dosen, tahun ajaran, logo, dan anggota tersedia.

## Hal yang perlu dikonfirmasi dengan dosen/tim

1. Apakah OpenGL 3.3, C++17, dan library pendukung jendela/input diperbolehkan.
2. Tenggat dan ukuran tim sebenarnya, serta nama/NIM anggota.
3. Apakah distribusi benar-benar wajib satu file `.exe`, atau satu paket offline sudah cukup. Desain saat ini menargetkan satu `.exe` dengan aset tertanam.
4. Rubrik penilaian grafika dan format bukti implementasinya.

## Mulai pengembangan

Kode sumber ada di `src/` dan panduan build ada di `BUILDING.md`. Status saat ini adalah Sprint 1 walking skeleton: jendela OpenGL 3.3, loop tetap 1/60 detik, grid isometrik, klik untuk bergerak, z-sorting, dan uji matematika isometrik. Aset karakter masih placeholder prosedural; pipeline aset tertanam menyusul pada WV-006.
