# Grayscale Image Converter: Int to Double Float

This program is made by:
**Sean Kyle Dimaunahan & Gerard Christian Felipe** from S18B

This program converts grayscale images from 8-bit integers (0–255) to double-precision floats (0.0–1.0) using a C main file and an x86-64 assembly that uses scalar floating-point instructions.

## Highlights
- **Video Demo**: [Link here later]
- **Assembly implementation**: Function `imgCvtGrayIntToDouble(int *in, double *out, int n)` implemented in NASM.
- **SIMD floating-point instructions**: uses `cvtsi2sd`, `divsd`, `mulsd`, and `movsd` to convert each integer to a double and scale it.
- **Verification & timing**: `main.c` generates random pixels, verifies the output (tolerance of 0.0001), and times the conversion (30 iterations minimum).

## Build Instructions (Windows - cmd.exe)

Run these commands in a Windows shell with NASM and GCC available (Make sure that gcc and nasm are in path environment variables):

```cmd
nasm -f win64 func.asm -o func.obj
gcc -c main.c -o main.obj
gcc main.obj func.obj -o converter.exe
```

## Run

```cmd
converter
```

Alternative one-liner run

```cmd
nasm -f win64 func.asm -o func.obj && gcc -c main.c -o main.obj && gcc main.obj func.obj -o converter.exe && converter
```

## Requirements

- NASM assembler
- GCC (MinGW-w64 or similar for Windows)

## Implementation Details

### Assembly (`func.asm`)

- Function exported: `imgCvtGrayIntToDouble` with signature `void imgCvtGrayIntToDouble(int *in, double *out, int n)`.
  - Loads constants `1.0` and `255.0` from the data section and computes `1.0/255.0` into `xmm2`.
  - For each pixel: load 32-bit int, convert to double with `cvtsi2sd`, multiply by the `1/255` computed from `divsd` with `mulsd`, and store the 64-bit double with `movsd`.
  - Advances the input pointer by 4 bytes for int and output pointer by 8 bytes for double each loop iteration.

### C Main File (`main.c`)

- Allocates `int *input` and `double *output` arrays for the image sizes.
- Generates random pixels using `rand() % 256` and seeds with `time(NULL)`.
- Calls the assembly routine inside a 30-iteration (minimum) loop to compute an average conversion time.
- Compares each output element to `(double)input[i] / 255.0` with a tolerance of `0.0001`.
- Timing: uses `clock()`; average time is reported in microseconds and per-pixel time in nanoseconds.
- Sample output: the program prints the full 10×10 sample (input & output) and timing for all three test sizes.

## Tested Image Sizes

- `10×10` = 100 pixels (sample printed)
- `100×100` = 10,000 pixels
- `1000×1000` = 1,000,000 pixels

Each test reports:
- Average time per conversion (microseconds)
- Time per pixel (nanoseconds)

## Notes and Tips

- The assembly uses SSE2 scalar FP instructions; the routine is simple and straightforward to extend/optimize (e.g., vectorized conversions) if higher throughput is needed.
- On Windows, the produced executable is `converter.exe` (use that name when running from `cmd.exe`).
- If you want to change the timing sample size, modify `iterations` in `main.c` (default is 30).

## Example Output

```
Input (Integer pixels - first 10x10):
 83 157  96 161 241 190  76  46 149  45
 44  74  83   0 205  90 136 110 154 240
 17  52 108  77 186  54 164  36 249 106
120 161 221 242 138 170  65 136 191 101
 69  31  78 250 255 199  23 154 109 210
 55 192 174  53 217 182 114  49  78  92
176  50  97  37 151 247 228  98 198 243
143 195 191 243  11  26 153   2  73  99
246 137  24   2  53 187 247  75 233  48
 39  71 137 208   5 181 105 243 151  80

Output (Double float pixels - first 10x10):
0.33 0.62 0.38 0.63 0.95 0.75 0.30 0.18 0.58 0.18
0.17 0.29 0.33 0.00 0.80 0.35 0.53 0.43 0.60 0.94
0.07 0.20 0.42 0.30 0.73 0.21 0.64 0.14 0.98 0.42
0.47 0.63 0.87 0.95 0.54 0.67 0.25 0.53 0.75 0.40
0.27 0.12 0.31 0.98 1.00 0.78 0.09 0.60 0.43 0.82
0.22 0.75 0.68 0.21 0.85 0.71 0.45 0.19 0.31 0.36
0.69 0.20 0.38 0.15 0.59 0.97 0.89 0.38 0.78 0.95
0.56 0.76 0.75 0.95 0.04 0.10 0.60 0.01 0.29 0.39
0.96 0.54 0.09 0.01 0.21 0.73 0.97 0.29 0.91 0.19
0.15 0.28 0.54 0.82 0.02 0.71 0.41 0.95 0.59 0.31

Verification: PASSED - All conversions are correct!

Timing Results:
  Average time per conversion: 1234.567890 microseconds
  Time per pixel: 12345.678900 nanoseconds
```