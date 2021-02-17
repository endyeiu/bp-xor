#pragma once
#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;

#define RANDOM (2.0*rand() / RAND_MAX - 1.0) / 10.0 //��������ɺ���
const int layer_Max = 5; //����5��������
const int neuron_number[layer_Max] = { 3,3,3,3,1 }; //���������Ԫ����
const int neuron_max = 3; //������������Ԫ����

class BPNet
{
public:
	BPNet();
	double simgoid(double x); //��������
	void update_weight(); //����Ȩֵ����
	double output(); //����ÿ����Ԫ���
	double train(double x[3],double y); //ѵ������
	void backword(double out_exp);//���������
	double cost(double out, double exp); //��ʧ����
	void show();
	virtual ~BPNet();

private:
	double weight[layer_Max][neuron_max][neuron_max];
	//weight[i][j][k]��ʾ��i���j����Ԫ���ӵ���i-1���k����Ԫ��Ȩֵ
	double bias[layer_Max][neuron_max];
	//bias[i][j]��ʾ��i���j����Ԫ����ֵ
	double input[3]; //��������
	double out_exp; //�������
	double layer_node[layer_Max][neuron_max];
	//layer_node[i][j]��ʾ��i���j����Ԫ�����
	double delta[layer_Max][neuron_max];
	//E[i][j]��ʾ��i���j����Ԫ�������ݶ�
	double learningrate; //ѧϰ����
	double error; //���
};
