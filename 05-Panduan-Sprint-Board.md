# Panduan Sprint Board — WARVA

`04-Sprint-Board.csv` adalah backlog awal yang dapat dibuka di spreadsheet atau diimpor ke aplikasi board. Setiap baris merupakan pekerjaan yang menghasilkan sesuatu yang dapat diperiksa, bukan sekadar aktivitas umum.

## Kolom dan alur kerja

- `ID`: pengenal tetap untuk diskusi dan commit.
- `Sprint`: S1–S4; `Backlog` untuk pekerjaan yang belum masuk jadwal wajib.
- `Status`: `To Do` → `In Progress` → `Review` → `Done`; gunakan `Blocked` beserta alasan di catatan bila terhambat.
- `Owner`: `Unassigned` sampai anggota tim diputuskan; satu orang bertanggung jawab utama per kartu, anggota lain boleh membantu.
- `Estimate`: perkiraan hari kerja orang, bukan kalender.
- `Depends On`: ID tugas yang harus selesai lebih dahulu.
- `Acceptance Criteria`: kondisi yang dapat diperiksa sebelum kartu ditutup.

## Jadwal relatif

| Sprint | Minggu | Hasil yang harus terlihat |
| --- | --- | --- |
| S1 | 1–2 | Prototipe OpenGL dengan adegan isometrik, input, sprite tertanam, style sheet karakter, dan sprite chibi awal. |
| S2 | 3–4 | Alur pesanan warung, menu dan petunjuk bermain, serta potret dan panel dialog. |
| S3 | 5–6 | Parkir, satu shift lengkap, hasil menang/kalah: **MVP dapat dimainkan**. |
| S4 | 7–8 | Insiden meja, rush hour, pencahayaan, polish, credits, dan rilis offline. |

Jika tersedia hanya enam minggu, S1–S3 menjadi target utama; tugas S4 dipilih hanya setelah MVP lulus uji. Jangan memulai fitur Backlog sebelum seluruh bug yang memutus alur utama selesai.

Beban person-day saat ini: S1 20, S2 19, S3 15, S4 16 (total 70). Dengan tim dua orang dan kapasitas 10 hari kerja per orang per sprint, batas tiap sprint adalah 20 person-day. S1 berada tepat pada batas, jadi WV-030 baru dimulai setelah WV-029 disetujui; S3 menyisakan lima hari untuk perbaikan integrasi MVP. Angka ini mengasumsikan tim dua orang; bila ukuran tim berbeda, sesuaikan pembagian kartu sebelum sprint dimulai dan perbarui estimasi berdasarkan kecepatan aktual.

## Definisi selesai

Sebuah kartu selesai ketika fitur berjalan pada build `Release`, skenario pada `Acceptance Criteria` berhasil, tidak ada error OpenGL yang diketahui, dan perubahan telah ditinjau anggota lain bila tim lebih dari satu orang. Untuk kartu desain, selesai berarti keputusan tertulis konsisten dengan GDD dan dokumentasi teknis.

Untuk aset karakter, `Done` juga berarti potret dan sprite gameplay dapat dikenali sebagai tokoh yang sama, jelas pada ukuran tampil sebenarnya, dan tidak menyalin karakter pada gambar referensi.

Pada akhir setiap sprint, tim menunjukkan build yang dapat dijalankan dan mencatat temuan serta perubahan angka keseimbangan. Catat keputusan scope baru di GDD, lalu perbarui board agar keduanya tetap sejalan.
