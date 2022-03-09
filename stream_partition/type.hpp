#include<iostream>

#include <assert.h>

#include <random>
#include <chrono>

typedef uint32_t vertex_id_t;
typedef uint64_t edge_id_t;

template <typename edge_data_t>
struct Edge
{
	vertex_id_t src;
	vertex_id_t dst;
	edge_data_t data;
	Edge() {}
	Edge(vertex_id_t _src, vertex_id_t _dst, edge_data_t _data) : src(_src), dst(_dst), data(_data) {}
	bool friend operator == (const Edge<edge_data_t>& a, const Edge<edge_data_t>& b)
	{
		return (a.src == b.src
			&& a.dst == b.dst
			&& a.data == b.data
			);
	}
	// ����Դ�ڵ��Ŀ��ڵ�
	void transpose()
	{
		std::swap(src, dst);
	}
};

template<typename edge_data_t>
struct AdjUnit
{
	vertex_id_t neighbour;// �յ�
	edge_data_t data; // Ȩ��
};

// �ڽӱ�
template<typename edge_data_t>
struct AdjList
{
	// һ��Ԫ�ص���ʼλ�ú���ֹλ�� 
	AdjUnit<edge_data_t>* begin; // ������������ʽ������ָ��ȥָ��
	AdjUnit<edge_data_t>* end;
	void init()
	{
		begin = nullptr;
		end = nullptr;
	}
};

//comprised column row
template<typename edge_data_t>
struct EdgeContainer
{
	// ƫ�������飬��С��v_num
	AdjList<edge_data_t>* adj_lists;
	// �ڽӱ�����
	AdjUnit<edge_data_t>* adj_units;

	vertex_id_t v_num=0;
	edge_id_t e_num=0;
	EdgeContainer() : adj_lists(nullptr), adj_units(nullptr) {}
	~EdgeContainer()
	{
		if (adj_lists != nullptr)
		{
			delete[]adj_lists;
		}
		if (adj_units != nullptr)
		{
			delete[]adj_units;
		}
	}
};

class Timer
{
	std::chrono::time_point<std::chrono::system_clock> _start = std::chrono::system_clock::now();
public:
	void restart()
	{
		_start = std::chrono::system_clock::now();
	}
	double duration()
	{
		std::chrono::duration<double> diff = std::chrono::system_clock::now() - _start;
		return diff.count();
	}
	static double current_time()
	{
		std::chrono::duration<double> val = std::chrono::system_clock::now().time_since_epoch();
		return val.count();
	}
};