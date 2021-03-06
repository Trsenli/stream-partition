#include <map>
#include <set>
#include <unordered_set>
#include <vector>

#ifndef JASMINE_PARTITION
#define JASMINE_PARTITION

class Partition {
    /*
    What is set good for? (Source: http://lafstern.org/matt/col1.pdf)
        - The collection can potentially grow so large that the difference between O(N) and O(log N) is important.
        - The number of lookups is the same order of magnitude as the number of insertions; there aren't so few
            insertions that insertion speed is irrelevant.
        - Elements are inserted in random order, rather than being inserted in order.
        - Insertions and lookups are interleaved; we don't have distinct insertion and lookup phases.
    */
    std::map<std::string, std::set<std::string>> edgeList; // 每一个键值对代表一个点和它所有的邻居，所以 edgeList 代表一些点
    /**
     * Edge cuts data structure
     * [id]                    [id]                 ...       [id]
     *  |                       |                              |
     *  ↓                       ↓                              ↓
     * {res->(foreign,..)} {res->(foreign,..) }     ...   {res->(foreign,..) }
     * 
     * **/
    std::vector<std::map<std::string, std::unordered_set<std::string>>> edgeCuts;// 边切分，每个partition的 edgeList
    int id;
    int numberOfPartitions;  // Size of the cluster TODO: can be removed

   public:
    Partition(int id, int numberOfPartitions) {
        this->id = id;
        this->numberOfPartitions = numberOfPartitions;
        for (size_t i = 0; i < numberOfPartitions; i++) {
            this->edgeCuts.push_back({});
        }
    };
    void addEdge(std::pair<std::string, std::string> edge);
    std::set<std::string> getNeighbors(std::string);
    double partitionScore(std::string vertex);
    double getEdgesCount();
    double getVertextCount();
    void addToEdgeCuts(std::string resident, std::string foreign, int partitionId);
    float edgeCutsRatio();
    template <typename Out>
    static void _split(const std::string &s, char delim, Out result);
    static std::vector<std::string> _split(const std::string &s, char delim);
    long edgeCutsCount();
    void printEdgeCuts();
    void printEdges();
    bool isExist(std::string);
};

#endif