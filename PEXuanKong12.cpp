/*
 * @Author: 违心
 * @Date: 2022-07-24
 * @Description: PE 悬空十二炮 挂机冲关脚本
 * @AvZ: 2022_06_30
 */

#include "avz.h"
#include <iostream>

#define _PUFF_SHROOM 4
#define _SCAREDY_SHROOM 5
#define _SUN_SHROOM 6
#define _FLOWER_POT 7

#define pk_fix_hp 1000

using namespace AvZ;

AvZ::PlantFixer pumpkin_fixer;

int SmartCard(int seed_index, int row, float col)
{
    auto seed_array = AvZ::GetMainObject()->seedArray();
    auto plant_array = AvZ::GetMainObject()->plantArray(); // 读出植物的内存块

    // 检测卡片是否恢复 然后再使用卡片
    if (seed_array[seed_index - 1].isUsable())
    {
        // 遍历植物数组 若当前位置有植物 返回-1表示不种植当前植物 防止垫材种在花盆上浪费阳光
        for (int index = 0; index < AvZ::GetMainObject()->plantCountMax(); ++index)
        {
            int pRow = plant_array[index].row();
            int pCol = plant_array[index].col();

            if (plant_array[index].type() == FLOWER_POT && !plant_array[index].isDisappeared() && pRow == row - 1 && pCol > col - 2 && pCol < col)
            {
                return -1;
            }
        }

        AvZ::setInsertOperation(false);
        Card(seed_index, row, col);
        AvZ::setInsertOperation(true);
        return 1;
    }
    return -1;
}

void Script()
{

    // 设置游戏倍速
    AvZ::setGameSpeed(10);

    // 多次生效
    AvZ::openMultipleEffective('Q', AvZ::MAIN_UI_OR_FIGHT_UI);

    // 选卡
    SelectCards({
        ICE_SHROOM,     // 寒冰菇
        M_ICE_SHROOM,   // 白冰
        COFFEE_BEAN,    // 咖啡豆
        PUFF_SHROOM,    // 小喷菇
        SCAREDY_SHROOM, // 胆小菇
        SUN_SHROOM,     // 阳光菇
        FLOWER_POT,     // 花盆
        BLOVER,         // 三叶草
        DOOM_SHROOM,    // 核
        PUMPKIN,        //南瓜头
    });

    // 自动补南瓜
    pumpkin_fixer.start(NGT_30, {{3, 9}, {4, 9}});
    pumpkin_fixer.resetFixHp(2500);

    SetTime(-599, 1);
    pao_operator.resetPaoList({
        {1, 5},
        {2, 5},
        {3, 1},
        {4, 1},
        {3, 3},
        {4, 3},
        {3, 5},
        {4, 5},
        {3, 7},
        {4, 7},
        {5, 5},
        {6, 5},
    });

    // 跳帧
    auto condition = [=]()
    {
        // 判断有无春哥去世
        std::vector<int> results_1;
        GetPlantIndices({{1, 5}, {2, 5}, {5, 5}, {6, 5}}, YMJNP_47, results_1);

        for (auto result : results_1)
        {
            if (result < 0)
            {
                return false;
            }
        }

        return true;
    };

    auto callback = [=]()
    {
        ShowErrorNotInQueue("春哥无了，嘤嘤嘤");
    };

    SkipTick(condition, callback);

    // P6
    // 主体节奏
    for (auto wave : {1, 2, 3, 4, 5, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17, 18})
    {
        SetTime(-95, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
    }

    // 10 20波铲炮前
    for (auto wave : {10, 20})
    {
        SetTime(100, wave);
        Shovel(1, 7);
        Shovel(1, 8);
        Shovel(1, 9);
        Shovel(6, 7);
        Shovel(6, 8);
        Shovel(6, 9);
    }

    // wave 9 19 多发几炮杀红眼
    SetTime(-95, 9);
    pao_operator.pao({{2, 9}, {5, 9}});
    SetTime(-95 + 600, 9);
    pao_operator.pao({{2, 9}, {5, 9}});
    SetTime(-95 + 1800, 9);
    InsertOperation([=]()
                    {
                        if (GetRunningWave() == 9)
                        {
                            SetTime(-95 + 1800, 9);
                            pao_operator.pao({{2, 9}, {5, 9}});
                        } });
    SetTime(-95 + 3000, 9);
    InsertOperation([=]()
                    {
                        if (GetRunningWave() == 9)
                        {
                            SetTime(-95 + 3000, 9);
                            pao_operator.pao({{2, 8}, {5, 8}});
                        } });

    SetTime(-95, 19);
    pao_operator.pao({{2, 9}, {5, 9}});
    SetTime(-95 + 600, 19);
    pao_operator.pao({{2, 9}, {5, 9}});
    SetTime(-95 + 1800, 19);
    InsertOperation([=]()
                    {
                        if (GetRunningWave() == 19)
                        {
                            SetTime(-95 + 1800, 19);
                            pao_operator.pao({{2, 9}, {5, 9}});
                        } });

    // 9 19波边路垫红眼
    int time_far_1 = 300;
    int time_far_2 = 675;
    for (int i = 1; i < 20; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            SetTime(time_far_2 + i * 150, 9 + j * 10);
            InsertOperation([=]()
                            {
                                if (GetRunningWave() == 9+j*10)
                                {
                                    SetTime(time_far_2+i*150, 9+j*10);
                                    SmartCard(_PUFF_SHROOM, 1, 9);
                                    SmartCard(_FLOWER_POT, 6, 9);
                                    SmartCard(_SCAREDY_SHROOM, 1, 9);
                                    SmartCard(_SUN_SHROOM, 6, 9);
                                    SmartCard(_PUFF_SHROOM, 1, 8);
                                    SmartCard(_SUN_SHROOM, 6, 8);
                                    SmartCard(_SCAREDY_SHROOM, 1, 8);
                                    SmartCard(_FLOWER_POT, 6, 8);
                                    SmartCard(_PUFF_SHROOM, 1, 7);
                                    SmartCard(_SUN_SHROOM, 6, 7);
                                    SmartCard(_SCAREDY_SHROOM, 1, 7);
                                    SmartCard(_FLOWER_POT, 6, 7);
                                } });
        }
    }

    // wave 10
    SetTime(-55, 10);
    pao_operator.pao({{2, 9}, {5, 9}});

    // 核消刷怪延迟
    SetTime(851, 10);
    InsertOperation([=]()
                    {
                    if (GetRunningWave() == 10)
                    {
                        SetTime(852, 10);
                        Card(DOOM_SHROOM, 2, 8);
                        Card(COFFEE_BEAN, 2, 8);
                    } });

    // 冰杀小偷
    SetTime(100, 10);
    Card(ICE_SHROOM, 1, 1);
    Card(COFFEE_BEAN, 1, 1);

    // wave 20
    // 咖啡豆种下到完成唤醒198/199cs
    // 冰生效时间100cs 水面冻结时间300cs
    // 珊瑚200cs出现
    SetTime(-397, 20);
    Card(ICE_SHROOM, 2, 1);
    Card(COFFEE_BEAN, 2, 1);

    // 固定发一对炮
    SetTime(-55, 20);
    pao_operator.pao({{2, 9}, {5, 9}});
    // 补一对炮 消灭残留普僵
    SetTime(-55 + 600);
    pao_operator.recoverPao({{2, 9}, {5, 9}});
    // 再补一对炮防过于靠右的铁桶 并使用垫材
    SetTime(-55 + 600 + 600 + 250);
    pao_operator.recoverPao({{2, 9}, {5, 9}});

    // 冰杀小偷（最晚653冰生效
    SetTime(-397);
    Card(M_ICE_SHROOM, 5, 1);
    SetTime(-397 + 751);
    Card(COFFEE_BEAN, 5, 1);
}
