#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

// External assembly function from func.asm
extern void imgCvtGrayIntToDouble(int *in, double *out, int n);

// Function to generate random pixel values
void generate_random_pixels(int *pixels, int n) {
    for (int i = 0; i < n; i++) {
        pixels[i] = rand() % 256;  // Random values from 0 to 255 (RGB 8-bit)
    }
}

// Function to print a portion of the image
void print_sample(int *input, double *output, int height, int width, int sample_size) {
    int rows = (height < sample_size) ? height : sample_size;
    int cols = (width < sample_size) ? width : sample_size;
    
    printf("\nInput (Integer pixels - first %dx%d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%3d ", input[i * width + j]);
        }
        printf("\n");
    }
    
    printf("\nOutput (Double float pixels - first %dx%d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%.2f ", output[i * width + j]);
        }
        printf("\n");
    }
}

// Function to verify correctness
int verify_conversion(int *input, double *output, int n) {
    const double tolerance = 0.0001; // Margin of Error
    for (int i = 0; i < n; i++) {
        double expected = (double)input[i] / 255.0; // Standard math vs assembly code
        double diff = (output[i] - expected);
        if (diff < 0) diff = -diff;
        if (diff > tolerance) {
            printf("ERROR at index %d: input=%d, expected=%.10f, got=%.10f\n", 
                   i, input[i], expected, output[i]);
            return 0;
        }
    }
    return 1;
}

void run_test(int height, int width) {
    int n = height * width;
    printf("\n========================================\n");
    printf("Testing %dx%d image (%d pixels)\n", height, width, n);
    printf("========================================\n");
    
    int *input = (int *)malloc(n * sizeof(int));
    double *output = (double *)malloc(n * sizeof(double));
    
    if (!input || !output) {
        printf("Memory allocation failed!\n");
        free(input);
        free(output);
        return;
    }
    
    // Generate random pixels
    generate_random_pixels(input, n);
    
    // Note#4 You must run at least 30 times to get the average execution time.
    int iterations = 30; // Number of iterations for timing
    int64_t start_ticks = clock();
    for (int i = 0; i < iterations; i++) {
        imgCvtGrayIntToDouble(input, output, n); // func.asm main function
    }
    int64_t end_ticks = clock();
    double avg_time = ((double)(end_ticks - start_ticks) / CLOCKS_PER_SEC / iterations) * 1000000.0; // Convert to microseconds
    
    // Print sample output for 10x10 only
    if (height == 10 && width == 10) {
        print_sample(input, output, height, width, 10);
    }
    
    // Verify correctness
    printf("\nVerification: ");
    if (verify_conversion(input, output, n)) {
        printf("PASSED - All conversions are correct!\n");
    } else {
        printf("FAILED - Some conversions are incorrect!\n");
    }
    
    printf("\nTiming Results:\n");
    if (height == 10 && width == 10) {
        printf("  Average time per conversion: %.300f microseconds\n", avg_time);
        printf("  Time per pixel: %.300f nanoseconds\n", (avg_time * 1000.0) / n);
    } else {
        printf("  Average time per conversion: %.20f microseconds\n", avg_time);
        printf("  Time per pixel: %.20f nanoseconds\n", (avg_time * 1000.0) / n);
    }
    
    free(input);
    free(output);
}

int main() {
    printf("Grayscale Image Converter: Int to Double Float\n");
    printf("Using x86-64 Assembly with Scalar SIMD Instructions\n");
    printf("=================================================\n");
    
    // Seed random number generator
    srand((unsigned int)time(NULL));
    
    /* Note#3 Time the asm function only for input image size width*height= {10*10, 100*100, and  1000*1000} */ 
    run_test(10, 10);
    run_test(100, 100);
    run_test(1000, 1000);
    
    printf("\n=================================================\n");
    printf("All tests completed!\n");
    
    return 0;
}
