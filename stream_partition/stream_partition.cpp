// stream_partition.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<string>
#include"Partitioner.h"
#include"read_graph.hpp"

using namespace std;
string str_cb(char const* a, char const* b)
{
	std::string const& cc = std::string(a) + std::string(b);
	char const* c = cc.c_str();
	return c;
}

int main()
{
	int partition_num = 4;
	vertex_id_t v_num = 10312;
	Partitioner graphPartitioner(partition_num, 1, spt::Algorithms::LDG);

	char graph_path[] = "catalog/edges.data";
	Edge<float>* graph_edges;
	size_t graph_e_num;
	read_edge_by_bin(graph_path, graph_edges, graph_e_num);

	char cn_path[] = "catalog/edges-cn.txt";
	Edge<int>* cn_edges;
	size_t cn_e_num;
	read_edge_by_txt(cn_path, cn_edges, cn_e_num);
	cout << "read graph and cn finish\n";

	FILE* flog = fopen("plog", "w");
	vertex_id_t* vertex_par = new vertex_id_t[v_num];
	for (size_t e_i = 0; e_i < graph_e_num; e_i++)
	{
		cout << e_i << endl;
		partitionedEdge pe = graphPartitioner.addEdge({ to_string(graph_edges[e_i].src), to_string(graph_edges[e_i].dst)});
		vertex_par[graph_edges[e_i].src] = pe[0].second;
		vertex_par[graph_edges[e_i].dst] = pe[1].second;
		//printf( "%d -> pid: %d \t %d -> pid: %d\n", graph_edges[e_i].src, pe[0].second, graph_edges[e_i].dst, pe[1].second);
		fprintf(flog, "%d -> pid: %d \t %d -> pid: %d\n", graph_edges[e_i].src, pe[0].second, graph_edges[e_i].dst, pe[1].second);
	}
	vector<vector<vertex_id_t>>partition_vertex_array(partition_num);
	fprintf(flog,"---------fininal-------------\n");
	for (vertex_id_t i = 0;i< v_num; i++)
	{
		partition_vertex_array[vertex_par[i]].push_back(i);
		fprintf(flog, "%d -> %d \t", i, vertex_par[i]);
	}
	fclose(flog);
	vertex_id_t* vertex_array = new vertex_id_t[v_num]; // 从 new -> old 
	vertex_id_t* vertex_partition_begin = new vertex_id_t[partition_num];
	vertex_id_t* vertex_partition_end = new vertex_id_t[partition_num];
	vertex_id_t v_a_p = 0;
	for (int p_i = 0; p_i < partition_num; p_i++) {
		if (p_i == 0)
		{
			vertex_partition_begin[p_i] = 0;
		}
		else
		{
			vertex_partition_begin[p_i] = vertex_partition_end[p_i - 1];
		}
		vertex_partition_end[p_i] = vertex_partition_begin[p_i] + partition_vertex_array[p_i].size();
		for (vertex_id_t v_i = 0; v_i < partition_vertex_array[p_i].size(); v_i++) {
			vertex_array[v_a_p] = partition_vertex_array[p_i][v_i];
			v_a_p++;
		}
	}
	cout << "partition process\n";
	// 8. 把点的序号 relabel。映射为其在数组中的下标, 把 graph_edges 中都修改好
	vertex_id_t* vertex_map = new vertex_id_t[v_num];// 映射数组，从 old -> new 
	for (vertex_id_t v_i = 0; v_i < v_num; v_i++)
	{
		vertex_map[vertex_array[v_i]] = v_i;
	}
	for (edge_id_t e_i = 0; e_i < graph_e_num; e_i++)
	{
		graph_edges[e_i].src = vertex_map[graph_edges[e_i].src];
		graph_edges[e_i].dst = vertex_map[graph_edges[e_i].dst];
	}
	for (edge_id_t e_i = 0; e_i < cn_e_num; e_i++)
	{
		cn_edges[e_i].src = vertex_map[cn_edges[e_i].src];
		cn_edges[e_i].dst = vertex_map[cn_edges[e_i].dst];
	}
	cout << "relabel finished\n";
	// 9. 把relabel 的图 和 relabel 的cn ，分割方式 输出
	string graph_path_r = str_cb(graph_path, "-r");
	string cn_path_r = str_cb(cn_path, "-r");
	FILE* f_graph_path_r = fopen(graph_path_r.c_str(), "w");
	for (edge_id_t e_i = 0; e_i < graph_e_num; e_i++)
	{
			fprintf(f_graph_path_r, "%d %d %f\n", graph_edges[e_i].src, graph_edges[e_i].dst, graph_edges[e_i].data);
	}
	fclose(f_graph_path_r);
	FILE* f_cn_path_r = fopen(cn_path_r.c_str(), "w");
	for (edge_id_t e_i = 0; e_i < cn_e_num; e_i++)
	{
			fprintf(f_cn_path_r, "%d %d %d\n", cn_edges[e_i].src, cn_edges[e_i].dst, cn_edges[e_i].data);
	}
	fclose(f_cn_path_r);
	string partition_ = str_cb(graph_path, "-p");
	FILE* f_partition_r = fopen(partition_.c_str(), "w");
	for (int p_i = 0; p_i < partition_num; p_i++)
	{
		fprintf(f_partition_r, "%d %d\n", vertex_partition_begin[p_i], vertex_partition_end[p_i]);
	}
	cout << "print finished\n";
	fclose(f_partition_r);
	delete[] vertex_par;

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
