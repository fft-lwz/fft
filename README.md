## FFTW3 库
官网：`https://www.fftw.org/` ；

Github 仓库：`https://github.com/FFTW/fftw3` ；

用户手册见 `FFTW3 manual.pdf` 。

### 安装方法
1. 从官网下载最新版本的 FFTW3 库，注意本文档基于的版本为 3.3.9 ，下载地址为 `https://www.fftw.org/fftw-3.3.9.tar.gz` ，目前 FFTW3 库最新版本为 3.3.10 ，下载完成后解压。
2. 依次执行如下指令。
```sh
# 安装目录，可以更改，但不建议以 root 权限安装在系统目录
mkdir fftw-build
cd fftw-3.3.9
# CFLAGS 是编译时的选项，开启 -g 选项以方便使用 gdb 调试
# 使用 --prefix 选项设置安装目录
# 使用 --enable-threads 和 --enable-openmp 选项以开启多线程
# 更多安装选项见 fftw 用户手册
./configure CFLAGS="-g" --prefix ~/fftw-build --enable-threads --enable-openmp
make
make install
```

### FFTW3 库测试
FFTW3 库提供了用于测试速度和正确性等的代码。它的基本使用方法如下。
```sh
cd fftw-3.3.9/tests
# 使用 -onthreads 设置线程数
# i 表示 inplace ， o 表示 outplace
# f 表示 forward ， b 表示 backward
# 数字表示数据规模（这里仅考虑一维 fft ，高维情况请参见相关文档）
./bench -onthreads=8 -s ib100000
```
更多测试方法与选项参见 `fftw-3.3.9/tests/README` 。

### FFTW3 库使用示例
见 `src/fftw_test` ，使用示例包含源代码和 `Makefile` 文件。

## FFT 简介
参见 `FFT Introduction.md` 。
