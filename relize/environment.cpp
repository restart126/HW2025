#pragma once
#include <vector>

using namespace std;

extern int N, V;

constexpr int tag_size = 1024;
constexpr int zone_size = 512;

class Zone {
private:
    int size = zone_size, position, id;// 
    vector<vector<int>> message;//用来存放数据的id、相对位置和大小
public:
    Zone(int zone_id, int zone_pos) :id(zone_id), position(zone_pos) {};
};

//tag块
//未做：具体的划分方式，如何进行二次划分，初步打算按大小进行划分，但是划分的太细可能导致空间浪费，划分太粗可能导致删除操作时花费时间较多；
//未做：划分后需要保存存在此处的数据id；
//关于未做1：由于大小为1-5，暂时分为125，34两个zone；
class Tag {
private:
    int id, block_size = tag_size, position;
    vector<int> free_node = vector<int>{ 0, tag_size / 2 };
public:
    Tag(const int& num, const int& begin) {
        id = num;
        position = begin;
    }

    int get_id() {
        return id;
    }
};


//每个磁盘有一个磁头可以访问或更新磁头的位置
class Head {
private:
    int position = 0;
public:
    void updata_position(const int& x) {
        position = x;
    }

    int get_position() {
        return position;
    }
};

//cd盘用来划分tag块
//未做：
class CD {
private:
    int block_size = V, free_current = 0;
public:
    Head head;

    bool allocate_tag_block(const int& id) {
        if (block_size - free_current > tag_size) {
            Tag tag(id, free_current);
            free_current += tag_size;
            return 1;
        }
        else return 0;
    }
};

//环境中包含cd盘N个
//未做：环境中需要分别保存需要删除、写入、读取的对象列表以方便管理；
//未做：由于对象删除操作，需要通过对象id寻址，可能需要维护一个大型列表用于对象的id寻址，且由于对象写入会存为3份，空间应为3倍，其中空间和时间如何平衡还没有想法；
class Environment {
private:

public:
    Environment() {
        vector<CD> cd(N);
    }
};
