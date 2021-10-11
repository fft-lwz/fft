## 程序运行方法
```sh
./cooley-tukey [N_ARRAY]
```

## `for` 循环并行方法
```c++
# pragma omp parallel for num_threads(n) proc_bind(close/spread/master)
```

## 绑核相关选项
```sh
export OMP_NUM_THREADS=8
export OMP_PROC_BIND=true
export OMP_PLACES=cores
export GOMP_CPU_AFFINITY="0 1 2 3" # 线程与核绑定
```

## 关于大小核
`0 1 2 3` 为小核， `4 5 6 7` 为大核，小核的算力是大核的一半左右

## 关于计时
最好用 `gettimeofday` 函数计时，用 `clock` 计时会出问题
