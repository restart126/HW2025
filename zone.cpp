//
// Created by 24762 on 2025/3/18.
//
#include"environment.h"
#include<algorithm>
#include <iostream>

using namespace std;

bool Zone::test_write(int size) {
    int flag = 0;
    if (size > 2 && size < 5)
        flag = 1;
    else if (size == 5)
        flag = 2;
    if (free_size[flag] / size < 2)
        return false;
    else
        return true;
}

int Zone::next_alloc_guide() {
    if (free_size[0] / 2 < free_size[1] / 3)
        return ALLOC_GUIDE_F;
    else
        return ALLOC_GUIDE_S;
}

int Zone::write_data(int id, const int size) {
    if (free_size[0] < 2 || free_size[1] < 4 || free_size[2] < 5)
        return next_alloc_guide();
    int absolute_pos = position + hash_pos(id, size);
    int flag = 0;
    if (size > 2 && size < 5)
        flag = 1;
    else if (size == 5)
        flag = 2;

    int absolute_wall = position + hash_pos_end(id, size);
    // cout<<absolute_pos<<" "<<absolute_wall - absolute_pos + 1<<endl;
 //    if(flag == 0)
 //        absolute_wall += wall[0];
 //    else
 //        absolute_wall += wall[flag] - wall[flag-1] - 1;

 //    for (int i = absolute_pos; i <= absolute_wall; i += size) {
 //        if (to_cd[i] == -1) {
 //            if (absolute_wall - i + 1>= size) {
 //                fill(to_cd.begin() + i, to_cd.begin() + i + size,id);
 //                free_size[flag] -= size;
 //                return i;
 //            }
 //            return next_alloc_guide();
 //        }
 //    }
 //    for (int i = absolute_pos; i + size - 1 <= absolute_wall; i++) {
 //        bool canWrite = true;
 //        for (int off = 0; off < size; off++) {
 //            if (to_cd[i + off] != -1) {
 //                canWrite = false;
 //                break;
 //            }
 //        }
 //        if (canWrite) {
 //            // 全都 -1，说明这段是空闲的
 //            fill(to_cd.begin() + i, to_cd.begin() + i + size, id);
 //            free_size[flag] -= size;
 //            return i;
 //        }
 //    }
    int consecutive = 0;    // 当前已连续发现多少个 -1
    int start_index = -1;   // 若发现连续空闲时的起始位
    // 注意要确保 i+size-1 不超过 absolute_wall
    for (int i = absolute_pos; i <= absolute_wall; i++) {
        if (to_cd[i] == -1) {
            // 如果是连续空闲序列的开端，记录一下
            if (consecutive == 0) {
                start_index = i;
            }
            consecutive++;
            // 如果已连续发现 size 个 -1，说明可以整段写入
            if (consecutive == size) {
                // 在 [start_index..start_index+size-1] 填入数据
                fill(to_cd.begin() + start_index,
                    to_cd.begin() + start_index + size,
                    id);

                free_size[flag] -= size;
                return start_index; // 返回写入位置
            }
        }
        else {
            // 碰到非 -1，连续计数清 0，重新找下一段
            consecutive = 0;
        }
    }
    return next_alloc_guide();
}

int Zone::get_data(int id, const int size) {
    int absolute_pos = position + hash_pos(id, size);
    int flag = 0;
    if (size > 2 && size < 5)
        flag = 1;
    else if (size == 5)
        flag = 2;

    int absolute_wall = position + hash_pos_end(id, size);

    auto it = find(to_cd.begin() + absolute_pos, to_cd.begin() + absolute_wall + 1, id);
    if (it == to_cd.begin() + absolute_wall + 1)
        return ERROR_ZONE_FOUND;
    else
        return it - to_cd.begin();
}

int Zone::del_data(int id, int size) {
    int absolute_pos = position + hash_pos(id, size);
    int flag = 0;
    if (size > 2 && size < 5)
        flag = 1;
    else if (size == 5)
        flag = 2;

    int absolute_wall = position + hash_pos_end(id, size);

    // 1. 先在 [absolute_pos..absolute_wall] 找到任意一个单元 == id
    auto it = find(
        to_cd.begin() + absolute_pos,
        to_cd.begin() + absolute_wall + 1,
        id
    );
    // 如果没找到，返回错误
    if (it == to_cd.begin() + absolute_wall + 1) {
        return ERROR_ZONE_FOUND;
    }

    // 2. 检查一下越界风险：确保能往后写 size 个单元
    //    （一般应该能通过您的插入逻辑保证，但额外防守一下也行）
    size_t start_index = std::distance(to_cd.begin(), it);
    if (start_index + size - 1 > (size_t)(absolute_wall)) {
        // 说明这个 id 之后不够 size 个单元，数据本身就不完整
        return ERROR_ZONE_FOUND;
    }

    // 3. 就地将这段恢复为 -1
    std::fill(it, it + size, -1);

    // 4. 更新剩余可用空间
    free_size[flag] += size;

    //    auto it = find(to_cd.begin() + absolute_pos, to_cd.begin() + absolute_wall + 1, id);
    //    if (it == to_cd.begin() + absolute_wall + 1)
    //        return ERROR_ZONE_FOUND;
    //    // 计算目标区间：左移 (it + size 到 absolute_wall + 1左移到it位置)
    //    rotate(it, it + size, to_cd.begin() + absolute_wall + 1);
    //    // 置空最后 `size` 个元素
    //    fill(to_cd.begin() + absolute_wall + 1 - size, to_cd.begin() + absolute_wall + 1, -1);
    //    free_size[flag] += size;
    return 0;
}

int Zone::hash_pos(int id, int size) {
    if (size < 3)
        return (id % hashzone_num) * 64;
    else if (size < 5)
        return (id % hashzone_num) * 64 + wall[0] + 1;
    else
        return (id % hashzone_num) * 64 + wall[1] + 1;
}

int Zone::hash_pos_end(int id, int size) {
    if (size < 3)
        return (id % hashzone_num) * 64 + wall[0];
    else if (size < 5)
        return (id % hashzone_num) * 64 + wall[1];
    else
        return (id % hashzone_num) * 64 + wall[2];
}
