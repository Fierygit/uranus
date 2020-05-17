//
// Created by Firefly on 2020/5/17.
//


#include <thread>
#include <zconf.h>
#include <random>
#include "../common/WaitGroup.h"

int main() {

    WaitGroup waitGroup;
    int cnt = 5;
    waitGroup.Add(cnt);
    for (int i = 0; i < cnt; i++) {
        std::thread t{[&waitGroup, i] {
//            std::default_random_engine generator(time(nullptr));
//            std::uniform_int_distribution<int> distribution(0, 10);
//            int w = distribution(generator);
            std::default_random_engine random;
            int w = random() % 6;   // 执行太快了都获取不到随机数
            sleep(i);
            printf("%d wait %d  over\n", i, i);
            waitGroup.Done();
        }};
        t.detach();
    }
    waitGroup.Wait();
    printf("all  over\n");
}

/*
梅森旋转算法（Mersenne twister）是一个伪随机数发生算法。由松本真和西村拓士[1]在1997年开发，基于有限二进制字段上的矩阵线性递归。
 可以快速产生高质量的伪随机数， 修正了古典随机数发生算法的很多缺陷。

Mersenne Twister这个名字来自周期长度取自梅森素数的这样一个事实。这个算法通常使用两个相近的变体，
 不同之处在于使用了不同的梅森素数。一个更新的和更常用的是MT19937, 32位字长。 还有一个变种是64位版的MT19937-64。
 对于一个k位的长度，Mersenne Twister会在的区间之间生成离散型均匀分布的随机数。

应用
梅森旋转算法是R,Python,Ruby,IDL,Free Pascal,PHP,Maple,Matlab,GMP和GSL的默认伪随机数产生器。
 从C++11开始，C++也可以使用这种算法。在Boost C++,Glib和NAG数值库中，作为插件提供。
 在SPSS中，梅森选旋转算法是两个PRNG中的一个：另一个是产生器仅仅为保证旧程序的兼容性，
 梅森旋转被描述为”更加可靠“。梅森旋转在SAS中同样是PRNG中的一个，另一个产生器是旧时的且已经被弃用。

优点
最为广泛使用Mersenne Twister的一种变体是MT19937，可以产生32位整数序列。具有以下的优点:

有219937 − 1的非常长的周期。在许多软件包中的短周期—232 随机数产生器在一个长周期中不能保证生成随机数的质量。[2]
在1 ≤ k ≤ 623的维度之间都可以均等分布(参见定义).
除了在统计学意义上的不正确的随机数生成器以外， 在所有伪随机数生成器法中是最快的(当时)[3]

由此可以看出，VS2010上正是用了MT19937这种算法。
在使用的时候，构造函数可以传参，我们传了个time(0),其实就是个long整数。就根据这个参数产生种子。具体的随机算法暂时不研究了。
然后我们用了uniform_int_distribution这个均匀分布的东东，可以在构造时指定min,max；
关键是：
int n = distribution(generator);

这句是怎么产生出随机数的呢？
生成器generator：能够产生离散的等可能分布数值
分布器distributions: 能够把generator产生的均匀分布值映射到其他常见分布，如均匀分布uniform，
 正态分布normal，二项分布binomial，泊松分布poisson;
所以就明白了就是把generator产生的随机数，再次投影到均匀分布上。
————————————————
版权声明：本文为CSDN博主「xiaolomg」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/boyxiaolong/article/details/23297433
 */


