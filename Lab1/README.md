<!--
 * @Author: Firefly
 * @Date: 2020-03-30 16:07:27
 * @Descripttion: 
 * @LastEditTime: 2020-03-30 22:06:07
 -->


## lab1

运行如下命令编译工程
```make clean```

```make```


生成 sudoku_solve 可知执行文件

最后运行：

```./sudoku_solve```

输入执行文件， 即可输出答案和最后输出时间！！！


测试报告在： lab1-test-report中！！！

<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>

### note

- 实验一暂定每人写一份代码，（如果有新的思路，可能有不同的实现）， 放到你自己的文件夹下

- 当然可以在组员的代码基础上改进，直接提交pull request， 由代码文件夹创建者merge
- merge文件夹是最终合并的代码也就是提交使用的代码， 可以直接使用速度最快的组员的代码， 也可以组合出一份最优的！

- 如果有多位组员写了代码， makefile 编译生成到自己的文件夹下（共用一个makefile，方便队友）， merge生成的exe到lab1目录，方便助教评测！

- 评测的样例可以放到公共的文件夹 sample 下！




## 数据对比评测 - from 吴嘉豪

测试环境: win 10 平台 x86 Ubuntu-18.04 wsl1

```
Architecture:        x86_64
CPU op-mode(s):      32-bit, 64-bit
Byte Order:          Little Endian
CPU(s):              4
On-line CPU(s) list: 0-3
Thread(s) per core:  2
Core(s) per socket:  2
Socket(s):           1
Vendor ID:           GenuineIntel
CPU family:          6
Model:               142
Model name:          Intel(R) Core(TM) i5-7200U CPU @ 2.50GHz
Stepping:            9
CPU MHz:             2701.000
CPU max MHz:         2701.0000
BogoMIPS:            5402.00
Virtualization:      VT-x
Hypervisor vendor:   Windows Subsystem for Linux
Virtualization type: container
Flags:               fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm pni pclmulqdq dtes64 monitor ds_cpl vmx est tm2 ssse3 fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave osxsave avx f16c rdrand lahf_lm abm 3dnowprefetch fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid mpx rdseed adx smap clflushopt intel_pt ibrs ibpb stibp ssbd
```

下表中, 数据量为数独问题的数量, 线程数为线程池中开启的线程数, 平均时间为以上述条件多次测试取平均值的执行时间(不包括数据IO)

| 序号 | 数据量 | 线程数 | 平均时间 |
| ---- | ------ | ------ | -------- |
| 1    | 1      | 1      | 0.00147  |
| 2    | 1      | 2      | 0.00169  |
| 3    | 1      | 3      | 0.00207  |
| 4    | 1      | 4      | 0.00239  |
| 5    | 1      | 5      | 0.003052 |
| 6    | 1      | 6      | 0.003583 |
| 7    | 100    | 1      | 0.01033  |
| 8    | 100    | 2      | 0.007017 |
| 9    | 100    | 3      | 0.004951 |
| 10   | 100    | 4      | 0.008432 |
| 11   | 100    | 5      | 0.007258 |
| 12   | 100    | 6      | 0.009542 |
| 13   | 1000   | 1      | 0.094041 |
| 14   | 1000   | 2      | 0.055634 |
| 15   | 1000   | 3      | 0.042375 |
| 16   | 1000   | 4      | 0.037539 |
| 17   | 1000   | 5      | 0.032876 |
| 18   | 1000   | 6      | 0.039525 |
| 19   | 10000  | 1      | 0.835913 |
| 20   | 10000  | 2      | 0.553967 |
| 21   | 10000  | 3      | 0.377475 |
| 22   | 10000  | 4      | 0.389515 |
| 23   | 10000  | 5      | 0.332054 |
| 24   | 10000  | 6      | 0.381459 |

从上述表格中可以归纳出如下规律:

1. 对于只有1个任务的情况, 时间线性递增, 是因为有了线程池多个线程创建的开销
2. 对于多任务的情况, 从任务100到任务10000个, 时间从线程1开始递减, 直到线程数为5时时间是最低点, 线程数为6时时间递增.
3. 数据规模线性增大时, 如从1000个任务到10000个任务, 时间几乎是成比例减小的
4. 但是当线程数增大时, 时间没有呈比例减小




