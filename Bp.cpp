#include "BP.h"
using namespace std;

BPNet::~BPNet()
{
}
//构造函数，初始化
BPNet::BPNet()
{
	srand(time(NULL));
	//权值和阈值在(-0.1,0.1)间随机数初始化
	for (int i = 1; i < layer_Max; i++)
	{
		for (int j = 0; j < neuron_number[i]; j++)
		{
			//bias[i][j] = 0.5;
			bias[i][j] = RANDOM;
			for (int k = 0; k < neuron_number[i - 1]; k++)
			{
				weight[i][j][k] = RANDOM;
			}
		}
	}
	learningrate = 0.5; //设定学习速率为0.5
	error = 0.0;
}

//激活函数
double BPNet::simgoid(double x)
{
	return (1.0 / (1 + exp(-x)));
}

//损失函数
double BPNet::cost(double out, double exp)
{
	return (pow(out - exp, 2));
}

//计算每一层的输出值
double BPNet::output()
{
	int i, j, k;
	double in_node[layer_Max][neuron_max];
	//输入层的输出即输入
	for (i = 0; i < 3; i++)
	{
		in_node[0][i] = input[i];
		layer_node[0][i] = in_node[0][i];
	}
	//计算隐含层及输出层的输出
	for (i = 1; i < layer_Max; i++)
	{
		for (j = 0; j < neuron_number[i]; j++)
		{
			layer_node[i][j] = 0.0;
			for (k = 0; k < neuron_number[i - 1]; k++)
			{
				layer_node[i][j] += layer_node[i - 1][k] * weight[i-1][j][k];
			}
			layer_node[i][j] +=  bias[i][j];
			layer_node[i][j] = simgoid(layer_node[i][j]);
		}
	}
	return layer_node[layer_Max - 1][0];
}

void BPNet::backword(double out_exp)
{
	int i, j, k;
	double t;
	//计算输出层的平方误差
	error =  cost(layer_node[layer_Max - 1][0],out_exp);
	//
	delta[layer_Max - 1][0] = layer_node[layer_Max - 1][0] * (1 - layer_node[layer_Max - 1][0]) * (layer_node[layer_Max - 1][0] - out_exp);
	for (i = layer_Max - 1; i > 0; i--)
	{
		for (j = 0; j < neuron_number[i - 1]; j++)
		{
			t = 0;
			for (k = 0; k < neuron_number[i]; k++)
			{
				t = t + delta[i][k]*weight[i-1][j][k];
			}
			delta[i - 1][j] = layer_node[i - 1][j] * (1 - layer_node[i - 1][j]) * t;
		}
	}
}

//对个层的权值和阈值修正
void BPNet::update_weight()
{
	int i, j, k;
	for (i = 0; i < layer_Max; i++)
	{
		for (j = 0; j < neuron_number[i]; j++)
		{
			for (k = 0; k < neuron_number[i - 1]; k++)
			{
				//更新权值
				weight[i][j][k] = weight[i][j][k] - learningrate * delta[i][j] * layer_node[i - 1][k];
			}
			bias[i][j] -= learningrate * delta[i][j];//更新阈值
		}
	}
}

void BPNet::show()
{
	printf("输入层：");
	for (int i = 0; i < neuron_number[0]; i++)
	{
		printf("%f,", layer_node[0][i]);
	}
	printf("\n");
	printf("输出层:%f\n", layer_node[layer_Max-1][0]);
	printf("平方误差:%f\n", error);
}

double BPNet::train(double x[3], double y)
{
	BPNet();
	//样本输入
	for (int i = 0; i < 3; i++)
		input[i] = x[i];

	for (int i = 1; i <= 5000; i++)
	{
		double delta;
		printf("第%d次训练\n", i);
		output();
		backword(y);
		update_weight();
		show();
	}
	return 0;
}
int main()
{
	BPNet bp;
	double x[3] = { 1,1,0 };
	double y = 1;
	bp.train(x, y);
	return 0;
}