#include <stdio.h>
#include <stdlib.h>

// Ini wajib ditulis sebelum memanggil stb_image [cite: 228]
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h" 

// --- FUNGSI UTAMA UNTUK BLUR GAMBAR ---
void apply_blur(unsigned char* in, unsigned char* out, int w, int h, int c, int k_size) {
    int r = k_size / 2; // Menentukan seberapa luas bingkai kernel [cite: 228]

    for (int y = 0; y < h; y++) { // Loop baris gambar [cite: 229]
        for (int x = 0; x < w; x++) { // Loop kolom gambar [cite: 229]
            for (int ch = 0; ch < c; ch++) { // Loop warna (Red, Green, Blue) [cite: 229]
                
                float sum = 0; // Tempat menampung jumlah warna [cite: 229]
                int count = 0; // Menghitung berapa piksel yang diambil [cite: 230]

                // Jendela Kernel bergeser di sekitar piksel inti
                for (int ky = -r; ky <= r; ky++) { 
                    for (int kx = -r; kx <= r; kx++) { 
                        
                        int py = y + ky;
                        int px = x + kx;

                        // Pastikan bingkai tidak keluar dari tepi gambar
                        if (px >= 0 && px < w && py >= 0 && py < h) { 
                            // Tambahkan warna dari piksel tetangga
                            sum += in[(py * w + px) * c + ch]; 
                            count++;
                        }
                    }
                }
                // Simpan warna rata-rata (blur) ke memori output [cite: 232]
                out[(y * w + x) * c + ch] = (unsigned char)(sum / count); 
            }
        }
    }
}

int main() {
    // 1. DEKLARASI NAMA FILE MANUAL
    const char* nama_file_input = "foto_asli.jpg";  // Pastikan nama filenya sama!
    const char* nama_file_output = "foto_hasil_blur.jpg";
    
    // Semakin besar angka kernel (harus ganjil: 3, 5, 7, 15), gambarnya makin blur!
    int kernel_size = 9; 

    int w, h, c; // Width (Lebar), Height (Tinggi), Channels (Warna)

    // 2. BACA GAMBAR KE MEMORI
    printf("Sedang membuka gambar %s...\n", nama_file_input);
    unsigned char* img_in = stbi_load(nama_file_input, &w, &h, &c, 0);
    
    if (img_in == NULL) {
        printf("Gagal! File %s tidak ditemukan di folder ini.\n", nama_file_input);
        return 1;
    }

    // 3. SIAPKAN MEMORI KOSONG UNTUK HASIL BLUR
    unsigned char* img_out = (unsigned char*)malloc(w * h * c * sizeof(unsigned char));

    // 4. JALANKAN PROSES BLUR
    printf("Memproses efek blur (Tunggu sebentar)...\n");
    apply_blur(img_in, img_out, w, h, c, kernel_size);

    // 5. SIMPAN HASILNYA JADI FILE JPG BARU
    printf("Menyimpan hasil ke %s...\n", nama_file_output);
    stbi_write_jpg(nama_file_output, w, h, c, img_out, 100);

    // 6. BERSIHKAN MEMORI
    stbi_image_free(img_in);
    free(img_out);
    
    printf("Mantap! Proses selesai.\n");
    return 0;
}