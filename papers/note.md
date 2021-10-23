#### fftw-paper

##### 摘要

本文介绍的是FFTW，用于计算一维或多维DFT的便携C包，它很快。

三个思想导致了高效：

* 转换器的计算使用一个**执行器**，它所用的代码经过了高度优化，具有可组合性。
* 运行时，**计划器**（planner）根据当前机器的架构来组合代码块。
* 代码块由“Caml Light dialect of ML”中的一个**生成器**自动生成，具有冗长、优化和不可读的特点，但我们可以通过对生成器的修改来实现更改所生成的代码块。

##### 引言

FFTW用起来很简单，用户仅仅通过计划器和执行器与FFTW交互；编译后不使用生成器。FFTW支持并行（用Cilk编写的执行器）

执行器使用[**库利-图基**（Cooley-Tukey）**算法**](#1)，递归循环节在代码块中处理。

算法大致过程：

```c++
fftw_plan plan;
int n = 1024;
COMPLEX A[n], B[n];

/* plan the computation */
plan = fftw_create_plan(n);

/* execute the plan */
fftw(plan, A);

/* the plan can be reused for other inputs
of size n */
fftw(plan, B);
```

大多数优化算法都针对这些参数进行了调整，而相比之下，FFTW的计划器能够运行时优化：使用dp算法找到一个足够好的解决方案（以字节码的形式表示，且这个解释过程开销可忽略不计）。

生成器制造代码块，用于计算给定尺寸的变换。内部则是实现符号算术中的Cooley-Turky算法，结果经简化转为C。简化的过程适用各种优化方式，可以手动进行。这种生成方式具有可实现更高基数、可轻松使用优化的优点。

##### 表现结果

展现了一些基准测试的结果。

下表给出了基准中包含的FFT实现的完整列表：

![image-20211004093221219](C:\Users\Kini\AppData\Roaming\Typora\typora-user-images\image-20211004093221219.png)

下图展示了167MHz UltraSPARC-I上的结果：

![image-20211004093524576](C:\Users\Kini\AppData\Roaming\Typora\typora-user-images\image-20211004093524576.png)

在RS/6000上，FFTW的效率始终不低于ESSL：

![image-20211004093636916](C:\Users\Kini\AppData\Roaming\Typora\typora-user-images\image-20211004093636916.png)

![image-20211004152435020](C:\Users\Kini\AppData\Roaming\Typora\typora-user-images\image-20211004152435020.png)

基准测试工作方式：

* 使用每个FFT子程序多次计算DFT，测量耗时并除以迭代次数以获取单次耗时。
* 由于DFT的迭代过程的发散性，每次FFT之后的初始化工作的耗时应当被挖去。
* 每次迭代中通过逆变换来跟随变换，以避免重新初始化输入数组。（然而FFTW不想测量这个缩放带来的比例因子，因为在很多场景下，这种比例因子很容易被乘法因子吸收）

##### 生成器

使用Caml Light编写（因为该语言易于表达符号操作，且易于执行优化）。

输入：

* 整数n，用以计算大小为n的傅里叶变换C代码片段；

* 选项1：决定计算前向还是后项变换
* 选项2：可选，将输入乘以一组旋转因子

过程：

对C的AST的一个子集进行操作，首先生成原始程序的AST，然后对AST进行局部优化，最后解析AST以生成C代码。其中生成AST支持复数的算数表示，并会自动转换成只用实数的AST。

下面是N为素数的Caml程序的例子：

![image-20211004160201472](C:\Users\Kini\AppData\Roaming\Typora\typora-user-images\image-20211004160201472.png)

**生成阶段**：

生成阶段为代码块创建一个粗略的AST，其中包含很多冗余代码。生成器使用Cooley-Tukey算法，总共约60行代码。顺带一提，可以用额外20行代码为质数算法生成AST。

生成器照搬Cooley-Tukey算法，由一个递归函数genfft组成，接受三个参数：变换的大小N，一个输入函数input和一个输出函数output。

输入函数n输入的返回值是第n个输入的实部和虚部的AST，以符号形式表示。

输出函数应用两个参数k和x并返回一个AST，用于将表达式x存储至第k个输出变量。

**$N_1$和$N_2$的选择直觉来讲，应当选取$N_1$是$N$的最小素因子，但事实证明$N_1$尽可能接近$\sqrt{N}$时，能得到更高效的代码块，其原因是减少代码块算术运算数以及减少访存。**

**优化阶段**：

优化器将原始AST优化为更高效的AST，由一组本地应用到AST所有节点的规则组成。

具体的例子如下：

![image-20211004163351288](C:\Users\Kini\AppData\Roaming\Typora\typora-user-images\image-20211004163351288.png)

上图是优化器实际实现的片段，运用一系列逻辑处理两个实数相乘。

![image-20211004163638265](C:\Users\Kini\AppData\Roaming\Typora\typora-user-images\image-20211004163638265.png)

浮点常量是存储在内存中的某处的，因此上图左侧需要加载0.5和-0.5两格常数，而右侧只需要加载常数0.5一次。这条规则带来了10%~15%的加速。

![image-20211004163646643](C:\Users\Kini\AppData\Roaming\Typora\typora-user-images\image-20211004163646643.png)

上图中的代码比较，直觉表明右侧使用临时变量预先计算并存储c+d会提高效率，但是在某些情况下，这种消除公共子表达式的优化方法会产生更慢的代码。原因尚未完全探明，可能是因为C编译器在显式声明临时变量时会不必要地浪费寄存器。

##### 结语

介绍了FFTW的设计和性能。

FFTW已经拥有三个并行版本，使用了Cilk、Pisix线程和MPI。

FFTW支持多维实复变换。

具有计划缓存机制，将生成的计划缓存以减少计划时间。

#### <span id="1">库利-图基（Cooley-Tukey）算法</span>

$N=N_1N_2$，常用的是基2-FFT，同样可适用于混合基FFT以及分裂基FFT。

与其他的DFT算法具有可联合性。

抽取方法：时域抽取和频域抽取