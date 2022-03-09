#include<iostream>
#include<assert.h>
#include"type.hpp"
#include <vector>
#include <utility>
#include <unordered_map>
#include<iostream>
#include<fstream>
#include<string>
#pragma warning(disable:4996)
using namespace std;

using namespace std;

#include <functional>


// 读一行
bool read_edge_txt(FILE* f, Edge<int>* edge)
{
	return (3 == fscanf(f, "%u %u %u", &edge->src, &edge->dst, &edge->data));
}
bool read_edge_txt(FILE* f, Edge<float>* edge)
{
	char fl[15];
	int s = fscanf(f, "%u %u %s", &edge->src, &edge->dst, fl);
	edge->data =(float)atof(fl);
	cout << edge->src <<" "<< edge->dst<<" " << fl << endl;
	return s == 3;
}

// 按文本读取整个文件
template<typename T>
void read_edge_by_txt(const char* fname, Edge<T>*& edge, size_t& e_num)
{
	FILE * f = fopen(fname, "r");
	assert(f != NULL);

	fseek(f, 0, SEEK_END);
    int end = ftell(f);
	Edge<T> temp;
	e_num = 0;
	fseek(f, 0, SEEK_SET);
	int ft = ftell(f);
	while (ft < end)
	{
		if (read_edge_txt(f, &temp))
		{
			e_num++;
		}
		ft = ftell(f);
	}
	edge = new Edge<T>[e_num];

	size_t e_i = 0;
	fseek(f, 0, SEEK_SET);
	while (ftell(f) < end)
	{
		if (read_edge_txt(f, &temp))
		{
			edge[e_i] = temp;
			e_i++;
		}
	}
	fclose(f);
}

void read_edge_by_bin(const char* fname, Edge<float>*& edge, size_t& e_num)
{
	FILE* f = fopen(fname, "rb");
	assert(f != NULL);
	fseek(f, 0, SEEK_END);
	// 文件总大小，字节数
	size_t total_size = ftell(f);
	// sizeof T is 12
	e_num = total_size / sizeof(Edge<float>);
	edge = new Edge<float>[e_num];
	fseek(f, 0, SEEK_SET);
	int count = 0;
	for (int e = 0; e < e_num; e++)
	{
		int res = fread(&edge[e], sizeof(Edge<float>), 1, f);
		count += res;
	}
	//auto ret = fread(edge, sizeof(Edge<float>), e_num, f);
	//cout << " ret " << ret << " e_num" <<e_num<< endl;
	assert(count == e_num);
	fclose(f);
}


template<typename T>
void build_edge_container(Edge<T>* edges, edge_id_t e_num, vertex_id_t v_num, EdgeContainer<T>* ec, const vector<vertex_id_t>& vertex_out_degree)
{
	// 邻接表有v_num个大小  【整个图的偏移量表，每个元素代表一个顶点，存储的是区间的头尾索引】
	ec->adj_lists = new AdjList<T>[v_num];
	// 邻接单元由e_num 个
	ec->adj_units = new AdjUnit<T>[e_num];
	// 块索引
	edge_id_t chunk_edge_idx = 0;
	// 给本机的顶点区间的偏移量赋值，指向 adj_unit 对应的区间
	// 循环结束后，区间还未构造完成，此时每个区间的首尾指针都指向的是 区间起点
	for (vertex_id_t v_i = 0; v_i < v_num; v_i++)
	{
		ec->adj_lists[v_i].begin = ec->adj_units + chunk_edge_idx;
		ec->adj_lists[v_i].end = ec->adj_lists[v_i].begin; // 区间尾指针也指向的 区间头
		// chunk_edge_idx增长v_t的出度
		chunk_edge_idx += vertex_out_degree[v_i];
	}
	
	for (edge_id_t e_i = 0; e_i < e_num; e_i++)
	{
		auto e = edges[e_i];
		// ep 指向的是 邻接单元数组上的元素
		auto ep = ec->adj_lists[e.src].end++; // ++在后，先赋值再 把 end 后移
		// neighbour就是邻居，其本质是这条边的目标节点
		ep->neighbour = e.dst;
		// 如果有data域的话就加入data域
		ep->data = e.data;
	}
}

template<typename T>
void get_degree(Edge<T>* edges, size_t e_num,  vector<vertex_id_t>& degree,vertex_id_t v_num){
	
	degree.resize(v_num);
	// 对于每台机器上的边，统计每条边源节点的度,相当于统计子图源节点的出度
	for (edge_id_t e_i = 0; e_i < e_num; e_i++)
	{
		degree[edges[e_i].src]++;
	}
}
