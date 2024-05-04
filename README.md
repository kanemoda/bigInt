# BigInt

## Introduction

BigInt is a C++ library designed for performing arithmetic operations on arbitrarily large integers with precision limited only by available memory. This project aims to address the limitations of standard integer types in C++, which have fixed sizes and can quickly overflow when dealing with large numbers.

### Features:

- **Arbitrary Precision**: BigInt supports integers of any size, limited only by available memory. This enables users to perform computations with extremely large numbers that exceed the range of standard integer types.
- **Basic Arithmetic Operations**: BigInt provides methods for addition, subtraction, multiplication, and division of large integers. These operations are implemented using algorithms optimized for efficiency and precision.
- **String Conversion**: BigInt allows users to convert integers to and from string representations, making it easy to work with large numbers in human-readable formats.
- **Integration with FFTW**: The library integrates with the Fastest Fourier Transform in the West (FFTW) library to provide fast multiplication and division operations for large integers using FFT-based algorithms.

### Usage:

BigInt is designed to be simple and intuitive to use. Users can include the BigInt header file in their C++ projects and create BigInt objects to perform arithmetic operations on large integers. The library handles memory allocation and management transparently, allowing users to focus on their application logic without worrying about integer overflow or precision issues.

## Compilation Instructions

To compile the project, use the following command:

```bash
g++ *.cpp -o filename -lfftw3 -lm
```
## Requirements
In order to compile and use BigInt with FFTW integration, ensure that you have the libfftw3-dev package installed on your system. You can install it using the package manager available for your operating system. For example, on Debian-based systems, you can install it with the following command:
### For Debian/Ubuntu:
```bash
sudo apt install libfftw3-dev
```

### For Fedora:
```bash
sudo dnf install fftw-devel
```

### CentOS/RHEL:
```bash
sudo yum install fftw-devel
```

### For Arch Linux:
```bash
sudo pacman -S fftw
```

### For openSUSE:
```bash
sudo zypper install fftw-devel
```
