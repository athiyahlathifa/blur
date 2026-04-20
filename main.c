#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h" 

// blur
void apply_blur(unsigned char* in, unsigned char* out, int w, int h, int c, int k_size) {
    int r = k_size / 2; 

    for (int y = 0; y < h; y++) { 
        for (int x = 0; x < w; x++) { 
            for (int ch = 0; ch < c; ch++) { 
                
                float sum = 0; 
                int count = 0; 

                for (int ky = -r; ky <= r; ky++) { 
                    for (int kx = -r; kx <= r; kx++) { 
                        
                        int py = y + ky;
                        int px = x + kx;

                       
                        if (px >= 0 && px < w && py >= 0 && py < h) { 
                           
                            sum += in[(py * w + px) * c + ch]; 
                            count++;
                        }
                    }
                }
               
                out[(y * w + x) * c + ch] = (unsigned char)(sum / count); 
            }
        }
    }
}

int main() {
    const char* nama_file_input = "foto_asli.jpg";  
    const char* nama_file_output = "foto_hasil_blur.jpg";
    
    int kernel_size = 9; 

    int w, h, c; 

    printf("Membuka gambar %s...\n", nama_file_input);
    unsigned char* img_in = stbi_load(nama_file_input, &w, &h, &c, 0);
    
    if (img_in == NULL) {
        printf("Gagal! File %s tidak ditemukan di folder ini.\n", nama_file_input);
        return 1;
    }

    unsigned char* img_out = (unsigned char*)malloc(w * h * c * sizeof(unsigned char));

    printf("Memproses efek blur...\n");
    apply_blur(img_in, img_out, w, h, c, kernel_size);

    printf("Menyimpan hasil ke %s...\n", nama_file_output);
    stbi_write_jpg(nama_file_output, w, h, c, img_out, 100);

    stbi_image_free(img_in);
    free(img_out);
    
    printf("Proses selesai.\n");
    return 0;
}