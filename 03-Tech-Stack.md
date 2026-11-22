# Tech Stack Decision — WARVA

**Keputusan:** gunakan C++17 dan OpenGL langsung. Keputusan ini mengikuti asumsi bahwa dosen mengizinkan C++ dan mengharuskan implementasi grafika melalui OpenGL atau library grafika, bukan game engine. Konfirmasi aturan resmi sebelum menambah dependensi.

| Komponen | Pilihan | Alasan dan batas penggunaan |
| --- | --- | --- |
| Bahasa | C++17 | Kendali langsung atas OpenGL dan struktur data game; dukungan tool Windows matang. |
| API grafika | OpenGL 3.3 Core | Shader, tekstur, transformasi, dan pengurutan gambar diimplementasikan oleh tim. |
| Jendela & input | GLFW | Hanya membuat jendela, konteks OpenGL, dan menerima input; tidak menyediakan logika game. |
| Pemuat fungsi OpenGL | GLAD | Memanggil fungsi OpenGL modern secara portabel. |
| Matematika | GLM | Vektor dan matriks untuk proyeksi serta transformasi. |
| Gambar | stb_image | Membaca PNG aset saat pengembangan atau dari memori saat rilis. |
| Font | stb_truetype | Membuat atlas teks UI dari font berlisensi yang disertakan. |
| Audio | miniaudio | Efek dan musik lokal dengan dependensi kecil; dapat ditunda sampai inti game selesai. |
| Build | CMake + MSVC | Build `Debug`/`Release` yang dapat diulang di Windows x64. |
| Versi kode | Git | Riwayat perubahan, branch fitur, dan review sederhana. |
| Pembuatan aset | Krita/LibreSprite atau alat gambar setara | Potret detail dan sprite chibi dibuat terpisah sebagai PNG, lalu dikemas dalam atlas; sumber dan lisensi dicatat. |

## Keputusan implementasi

- Renderer memakai sprite batch sederhana berbasis quad bertekstur, shader vertex/fragment, dan blending alpha. UI digambar pada pass terpisah.
- Tekstur karakter memakai `GL_NEAREST` tanpa penghalusan agar kedua gaya pixel art tetap tajam. Potret dan sprite gameplay memakai atlas terpisah dengan ID karakter yang sama.
- Peta disusun sebagai data tile dan objek, bukan dibuat melalui game engine. Kode membaca manifest aset dan konfigurasi gameplay dari data proyek.
- Navigasi kendaraan memakai graf waypoint yang dibuat tim. Ini cukup untuk area parkir kecil dan tetap menunjukkan algoritme pencarian rute.
- Aset rilis disematkan ke `.exe`. Hindari ketergantungan pada server, launcher, atau instalasi engine.
- Dependensi disimpan dengan versi tetap dan lisensi masing-masing diperiksa sebelum distribusi. Versi persisnya dicatat di repository saat proyek kode dimulai.

## Alternatif yang dipertimbangkan

| Alternatif | Hasil pertimbangan |
| --- | --- |
| Godot/Unity | Mempercepat produksi, tetapi dapat menyembunyikan pekerjaan grafika yang menjadi fokus mata kuliah; tidak dipilih untuk rencana awal. |
| Java + LWJGL | Layak bila Java diwajibkan, tetapi pilihan bahasa tim saat ini adalah C++. |
| SDL/SFML sebagai renderer 2D | Memudahkan sprite, namun mengurangi ruang demonstrasi implementasi OpenGL langsung. |

## Risiko dan mitigasi

- **Waktu setup OpenGL:** Sprint 1 wajib menghasilkan jendela, shader, sprite, dan input klik sebelum konten game diperluas.
- **Satu executable:** uji penyematan aset sejak awal dengan satu sprite dan satu font; jangan menunggu akhir proyek.
- **Kinerja saat ramai:** batasi partikel, gabungkan gambar sprite yang memakai atlas sama, dan ukur frame time pada rush hour.
- **Keterbacaan isometrik:** uji sketsa pada ukuran layar target sebelum menggambar seluruh aset.

## Lingkungan pengembangan minimum

Windows 10/11 x64, Visual Studio Build Tools dengan C++, CMake, Git, dan driver yang mendukung OpenGL 3.3. Spesifikasi perangkat uji aktual dicatat saat tim mulai mengembangkan agar target 60 FPS dapat dibuktikan.
