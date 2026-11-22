# Character Art Direction — WARVA

**Versi:** 0.1 · **Referensi:** dua gambar yang dikirim pengguna pada 10 November 2026. Referensi digunakan untuk **gaya visual**, bukan identitas, kostum, pose, atau aset tokohnya.

## Prinsip utama

Setiap tokoh WARVA punya dua representasi visual yang menunjukkan karakter yang sama:

| Penggunaan | Arah gaya | Rujukan |
| --- | --- | --- |
| Dialog, ekspresi, layar pengenalan, dan preview karakter | Pixel art lebih detail, wajah dan rambut ekspresif, bentuk kepala/torso jelas; potret dapat memakai tampak samping atau 3/4. Preview penuh memakai bahasa visual yang sama sampai badan lengkap. | Gambar pertama. |
| Adegan permainan saat berjalan, memasak, duduk, melayani, atau parkir | Sprite chibi pixel art dengan kepala relatif besar, tubuh ringkas, kontur jelas, dan detail yang disederhanakan. Arah badan disesuaikan dengan dunia isometrik 3/4. | Gambar kedua. |

Potret **bukan** hasil membesarkan sprite gameplay. Keduanya dibuat terpisah, tetapi berbagi palet, gaya rambut, pakaian, aksesori, dan ciri siluet. Jangan menggunakan atau menjiplak tokoh, komposisi lembar karakter, atau aset ber-watermark dari gambar referensi.

## Karakter awal yang orisinal

- **Staf WARVA:** celemek hijau tua di atas kaus krem, kain kecil merah bata di pinggang, rambut gelap. Ini tokoh yang dikendalikan pemain; siluet celemek harus tetap terlihat pada sprite kecil.
- **Pelanggan mahasiswa:** jaket biru dan tas selempang. Cocok untuk dialog tutorial singkat; tas menjadi penanda yang tetap tampak pada versi chibi.
- **Pelanggan pekerja:** kemeja warna tanah dan tas kerja kecil.
- **Pelanggan pengendara:** jaket oranye kusam dan helm yang dibawa, bukan dipakai saat makan.

Rancangan ini adalah titik awal; tim boleh mengganti rincian budaya/kostum setelah membuat sketsa, selama dua representasi setiap tokoh tetap cocok dan tidak menyerupai karakter pada referensi.

## Ukuran dan produksi aset

| Aset | Ukuran kanvas awal | Kebutuhan minimum |
| --- | --- | --- |
| Potret dialog | 128 × 128 px | Ekspresi netral dan senang untuk staf; netral untuk satu pelanggan tutorial. Latar transparan. |
| Preview penuh | 128 × 192 px | Satu pose berdiri untuk staf pada layar pengenalan/credits karakter. Latar transparan. |
| Sprite gameplay | 48 × 64 px per frame | Idle, berjalan, dan satu aksi interaksi untuk staf; idle dan berjalan untuk pelanggan. |

Ukuran tersebut merupakan batas kanvas, bukan kewajiban mengisi semua piksel. Simpan sprite di tengah bawah frame agar titik kaki stabil. Gunakan empat arah gerak yang cocok dengan sumbu isometrik (kiri-atas, kanan-atas, kiri-bawah, kanan-bawah). Animasi berjalan awal terdiri dari empat frame per arah; idle boleh satu frame per arah. Untuk MVP, aksi memasak, melayani, dan membersihkan dapat berbagi satu animasi interaksi dua frame.

Batasi palet awal sekitar 12–16 warna utama per tokoh agar potret dan sprite tetap seragam. Gunakan outline gelap yang tidak harus hitam pekat, shading berupa blok warna yang jelas, dan tanpa blur/anti-alias. Uji aset pada skala tampil sebenarnya dengan latar sore maupun malam; jika aksesori hilang pada ukuran kecil, sederhanakan bentuknya.

## Aturan tampilan dalam game

- Potret muncul dalam panel dialog/tutorial dan saat pemain membuka informasi tokoh. Preview penuh muncul hanya pada layar pengenalan atau tampilan karakter; keduanya tidak menutupi area tugas aktif saat timer berjalan.
- Selama shift, semua karakter di dunia memakai sprite chibi. Bubble pesanan tetap berupa ikon item, sehingga pemain dapat membaca pesanan tanpa membuka dialog.
- Dialog MVP berupa kalimat pendek tanpa pilihan bercabang; shift otomatis pause saat tutorial atau preview penuh terbuka. Dialog pelanggan rutin tidak menghentikan waktu.
- Filter tekstur `NEAREST` dan skala piksel bulat menjaga tepian tajam. Hindari rotasi bebas pada sprite karakter karena merusak grid piksel; gunakan frame arah yang digambar.

## Pemeriksaan sebelum aset dinyatakan selesai

1. Letakkan potret dan sprite gameplay berdampingan tanpa nama: anggota tim dapat mengenali bahwa keduanya tokoh yang sama.
2. Periksa empat arah berjalan pada tile isometrik 64 × 32 tanpa kaki meleset dari titik pijak.
3. Periksa pose pada UI 1280 × 720 serta latar sore dan malam: wajah terbaca pada potret dan peran tokoh terbaca dari sprite kecil.
4. Pastikan semua gambar karakter dibuat sendiri atau memiliki izin penggunaan yang jelas; simpan sumber dan atribusinya.
