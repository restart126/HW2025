#pragma once
#include <vector>

using namespace std;

//外部定义：N -- cd磁盘的数量；V -- cd磁盘的总大小
extern int N, V;

//tag块、zone块的固定大小
constexpr int tag_size = 1024;
constexpr int zone_size = 512;

//zone块
class Zone {
private:
    int size = zone_size, position, id;//zone大小、起始位置、id
    vector<vector<int>> message;//用来存放数据的id、相对位置和大小
public:
    //初始化zone区域的起始位置和id
    Zone(const int& begin, const int& idx) {
    }
};

//tag块用来划分zone块
//未做：具体的划分方式，如何进行二次划分，初步打算按大小进行划分，但是划分的太细可能导致空间浪费，划分太粗可能导致删除操作时花费时间较多；
//未做：划分后需要保存存在此处的数据id；
//关于未做1：由于大小为1-5，暂时分为125，34两个zone；
class Tag {
private:
    int id, block_size = tag_size, position; //tag的id、大小、起始位置
    vector<int> free_node = vector<int>{ 0, tag_size / 2 }; //
public:
    //初始化tag块的id、起始位置
    Tag(const int& num, const int& begin) {
    }
    
    //获得tag块的id
    int get_id() {
    }
};


//每个磁盘有一个磁头可以访问或更新磁头的位置
class Head {
private:
    int position = 0; //磁头的实时位置
public:
    //更新磁头位置
    void updata_position(const int& x) {
    }

    //获取磁头当前位置
    int get_position() {
    }
};

//cd盘用来划分tag块
//未做：
class CD {
private:
    int block_size = V, free_current = 0; //cd盘的大小和当前未划分区域的起始位置
public:
    //分配一个磁头
    Head head;

    //从cd内存中分配一个tag块
    bool allocate_tag_block(const int& id) {
    }
};

//环境中包含cd盘N个
//未做：环境中需要分别保存需要删除、写入、读取的对象列表以方便管理；
//未做：由于对象删除操作，需要通过对象id寻址，可能需要维护一个大型列表用于对象的id寻址，且由于对象写入会存为3份，空间应为3倍，其中空间和时间如何平衡还没有想法；
class Environment {
private:
    
public:
    Environment() {
    }
};
