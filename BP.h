#pragma once
#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;

#define RANDOM (2.0*rand() / RAND_MAX - 1.0) / 10.0 //随机数生成函数
const int layer_Max = 5; //定义5层神经网络
const int neuron_number[layer_Max] = { 3,3,3,3,1 }; //定义各层神经元个数
const int neuron_max = 3; //定义各层最大神经元个数

class BPNet
{
public:
	BPNet();
	double simgoid(double x); //激励函数
	void update_weight(); //更新权值函数
	double output(); //计算每个神经元输出
	double train(double x[3],double y); //训练函数
	void backword(double out_exp);//计算调整量
	double cost(double out, double exp); //损失函数
	void show();
	virtual ~BPNet();

private:
	double weight[layer_Max][neuron_max][neuron_max];
	//weight[i][j][k]表示第i层第j个神经元连接到第i-1层第k个神经元的权值
	double bias[layer_Max][neuron_max];
	//bias[i][j]表示第i层第j个神经元的阈值
	double input[3]; //样本输入
	double out_exp; //期望输出
	double layer_node[layer_Max][neuron_max];
	//layer_node[i][j]表示第i层第j个神经元的输出
	double delta[layer_Max][neuron_max];
	//E[i][j]表示第i层第j个神经元的误差的梯度
	double learningrate; //学习速率
	double error; //误差
};
