# bp-xor
基于C++，使用bp神经网络进行异或检测

## ----------使用工具----------
Visual Studio 2019

## ------------运行------------
修改double x[3] = { 1,1,0 };和	double y = 1;的值；
运行Bp.cpp即可

## 各文件内容如下：
### 1）Bp.h:
 #define RANDOM (2.0*rand() / RAND_MAX - 1.0) / 10.0
#### 随机数函数，随机数fanw:(-0.1,0.1)
定义BPNet类，

### 2) Bp.cpp:
void BPNet::backword(double out_exp)函数:
计算隐含层及输出层的调整值
