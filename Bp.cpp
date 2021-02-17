#include "BP.h"
using namespace std;

BPNet::~BPNet()
{
}
//���캯������ʼ��
BPNet::BPNet()
{
	srand(time(NULL));
	//Ȩֵ����ֵ��(-0.1,0.1)���������ʼ��
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
	learningrate = 0.5; //�趨ѧϰ����Ϊ0.5
	error = 0.0;
}

//�����
double BPNet::simgoid(double x)
{
	return (1.0 / (1 + exp(-x)));
}

//��ʧ����
double BPNet::cost(double out, double exp)
{
	return (pow(out - exp, 2));
}

//����ÿһ������ֵ
double BPNet::output()
{
	int i, j, k;
	double in_node[layer_Max][neuron_max];
	//���������������
	for (i = 0; i < 3; i++)
	{
		in_node[0][i] = input[i];
		layer_node[0][i] = in_node[0][i];
	}
	//���������㼰���������
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
	//����������ƽ�����
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

//�Ը����Ȩֵ����ֵ����
void BPNet::update_weight()
{
	int i, j, k;
	for (i = 0; i < layer_Max; i++)
	{
		for (j = 0; j < neuron_number[i]; j++)
		{
			for (k = 0; k < neuron_number[i - 1]; k++)
			{
				//����Ȩֵ
				weight[i][j][k] = weight[i][j][k] - learningrate * delta[i][j] * layer_node[i - 1][k];
			}
			bias[i][j] -= learningrate * delta[i][j];//������ֵ
		}
	}
}

void BPNet::show()
{
	printf("����㣺");
	for (int i = 0; i < neuron_number[0]; i++)
	{
		printf("%f,", layer_node[0][i]);
	}
	printf("\n");
	printf("�����:%f\n", layer_node[layer_Max-1][0]);
	printf("ƽ�����:%f\n", error);
}

double BPNet::train(double x[3], double y)
{
	BPNet();
	//��������
	for (int i = 0; i < 3; i++)
		input[i] = x[i];

	for (int i = 1; i <= 5000; i++)
	{
		double delta;
		printf("��%d��ѵ��\n", i);
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