/*
 * @Author: 违心
 * @Date: 2022-10-19
 * @Description: PE 悬空十二炮 挂机冲关脚本
 * @AvZ: 2022_10_12
 * @Version: v2.0.0
 */

// #include "SelectCardsPlus.h"
// #include "WxLogger.h"
#include "avz.h"

constexpr int _CHERRY = 1;
constexpr int _COFFEE = 3;
constexpr int _PUFF_SHROOM = 4;
constexpr int _SCAREDY_SHROOM = 5;
constexpr int _SUN_SHROOM = 6;
constexpr int _FLOWER = 7;
constexpr int _BLOVER = 9;

constexpr int xk12_fight_time = -95;  // 通常波炮发射时间
constexpr int xk12_delay_fight = -55; // 10 20波炮发射时间
constexpr int xk12_wave_len = 601;    // 波长

// constexpr int target_flag = 50000; // 目标选卡数（需x2
// constexpr int log_flag = 2500;     // 输出日志操作之间的间隔

// 全局变量
// 记录已完成轮数
// long int completedFlags = 0;

// 记录当前波数
int game_running_wave;

// _wxvz::Logger wxLogger;

AvZ::PlantFixer pumpkin_fixer;
AvZ::TickRunner tick_runner;

// *** Not In Queue
// 判断场上是否还有存活的僵尸
boolean HaveZombieNotDead();

void TickRun();

// *** 帧运行
// 边路垫红眼
void TickPad();

// *** Not In Queue
// 当卡片可用 同时目标格子也可种时再调用CardNotInQueue种植
// 返回CARD_CD_NOT_ENOUGH表示cd不够
// 其他返回值类型见 pvzfunc.h -> enum PlantReject
int WxSmartCard(int seed_type, int row, float col);
// 卡片cd不够的返回值
constexpr int CARD_CD_NOT_ENOUGH = 9999;

void Script()
{
    // 控制脚本循环次数
    // ++completedFlags;
    // if (completedFlags % log_flag == 0) {
    //     wxLogger.log("# / #", completedFlags, target_flag);
    // }
    // if (completedFlags > target_flag) {
    //     wxLogger.log("Finish");
    //     AvZ::__Exit();
    // }

    // 设置游戏倍速
    AvZ::setGameSpeed(10);

    // 多次生效
    AvZ::openMultipleEffective('Q', AvZ::MAIN_UI_OR_FIGHT_UI);

    // // 按a键回复原速
    // KeyConnect('A', [=]()
    //            { AvZ::SetGameSpeed(1); });

    // // 按f键加速
    // KeyConnect('F', [=]()
    //            { AvZ::SetGameSpeed(10); });

    // // 按k键跳帧
    // KeyConnect('K', [=]()
    //            { AvZ::SkipTick(6000, 20); });

    // 获取出怪列表
    auto zombie_type = AvZ::GetZombieTypeList();

    // 选卡
    // _qrmd::SelectCardsPlus({
    AvZ::SelectCards({
        CHERRY_BOMB,    // 樱桃
        M_CHERRY_BOMB,  // 白樱
        COFFEE_BEAN,    // 咖啡豆
        PUFF_SHROOM,    // 小喷菇
        SCAREDY_SHROOM, // 胆小菇
        SUN_SHROOM,     // 阳光菇
        SUNFLOWER,      // 向日葵
        UMBRELLA_LEAF,  // 伞
        ICE_SHROOM,     // 冰
        PUMPKIN,        // 南瓜头
    });

    // 自动补南瓜
    pumpkin_fixer.start(NGT_30, {{3, 9}, {4, 9}});
    pumpkin_fixer.resetFixHp(2665);

    // 每帧运行的操作
    tick_runner.pushFunc(TickRun);

    AvZ::SetTime(-599, 1);
    AvZ::pao_operator.resetPaoList({
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
    AvZ::SkipTick(6000, 20);

    // P6
    // 主体节奏
    for (auto wave : {1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19}) {
        AvZ::SetTime(xk12_fight_time, wave);
        AvZ::pao_operator.pao({{2, 9}, {5, 9}});
    }

    // 10 20波铲炮前
    for (auto wave : {10, 20}) {
        AvZ::SetTime(100, wave);
        AvZ::Shovel({{1, 7}, {1, 8}, {1, 9}, {6, 7}, {6, 8}, {6, 9}});
    }

    AvZ::SetTime(xk12_fight_time + xk12_wave_len, 9);
    AvZ::InsertOperation([=]() {
                        if (AvZ::GetRunningWave() == 9 && HaveZombieNotDead())
                        {
                            AvZ::SetTime(xk12_fight_time + xk12_wave_len, 9);
                            AvZ::pao_operator.pao({{2, 9}, {5, 9}});
                        } });
    AvZ::SetTime(xk12_fight_time + xk12_wave_len * 3, 9);
    AvZ::InsertOperation([=]() {
                        if (AvZ::GetRunningWave() == 9 && HaveZombieNotDead())
                        {
                            AvZ::SetTime(xk12_fight_time + xk12_wave_len*3, 9);
                            AvZ::pao_operator.pao({{2, 9}, {5, 9}});
                        } });
    AvZ::SetTime(xk12_fight_time + xk12_wave_len * 5, 9);
    AvZ::InsertOperation([=]() {
                        if (AvZ::GetRunningWave() == 9 && HaveZombieNotDead())
                        {
                            AvZ::SetTime(xk12_fight_time + xk12_wave_len*5, 9);
                            AvZ::pao_operator.pao({{2, 8}, {5, 8}});
                        } });

    AvZ::SetTime(xk12_fight_time + xk12_wave_len, 19);
    AvZ::InsertOperation([=]() {
                        if (AvZ::GetRunningWave() == 19 && HaveZombieNotDead())
                        {
                            AvZ::SetTime(xk12_fight_time + xk12_wave_len, 19);
                            AvZ::pao_operator.pao({{2, 9}, {5, 9}});
                        } });
    AvZ::SetTime(xk12_fight_time + xk12_wave_len * 3, 19);
    AvZ::InsertOperation([=]() {
                        if (AvZ::GetRunningWave() == 19 && HaveZombieNotDead())
                        {
                            AvZ::SetTime(xk12_fight_time + xk12_wave_len*3, 19);
                            AvZ::pao_operator.pao({{2, 9}, {5, 9}});
                        } });

    // wave 10 樱消延迟
    AvZ::SetTime(xk12_delay_fight, 10);
    AvZ::pao_operator.pao({{2, 9}, {5, 9}});
    // 模仿者319/320
    // 318冰车压9列普通 在此之前需要模仿者变身完成
    AvZ::SetTime(399, 10);
    AvZ::Card(CHERRY_BOMB, 5, 9);
    AvZ::SetTime(399 + (318 - 320 - 1), 10);
    AvZ::Card(M_CHERRY_BOMB, 2, 9);
    // 樱桃爆炸后冰一下 防止僵尸在模仿者变身期间吃掉白樱
    AvZ::SetTime(399 + (318 - 320 - 1) - 100 - 198, 10);
    AvZ::Card(ICE_SHROOM, 1, 1);
    AvZ::Card(COFFEE_BEAN, 1, 1);

    // wave 10 20 种伞
    if (*(zombie_type + BUNGEE_ZOMBIE) != 0) {
        AvZ::SetTime(-750, 10);
        AvZ::Card(UMBRELLA_LEAF, 2, 8);
        AvZ::SetTime(2, 10);
        AvZ::Card(UMBRELLA_LEAF, 5, 8);
        AvZ::SetTime(400, 10);
        AvZ::Shovel({{2, 8}, {5, 8}});

        AvZ::SetTime(-750, 20);
        AvZ::Card(UMBRELLA_LEAF, 2, 8);
        AvZ::SetTime(2, 20);
        AvZ::Card(UMBRELLA_LEAF, 5, 8);
        AvZ::SetTime(400, 20);
        AvZ::Shovel({{2, 8}, {5, 8}});
    }

    // wave 20
    // 咖啡豆种下到完成唤醒198/199cs
    // 冰生效时间100cs 水面冻结时间300cs
    // 珊瑚200cs出现 刷新后50cs开啃 水面冻结期间无法出现
    // 200-198-100-300+1 = -397
    // AvZ::SetTime(-397, 20);
    // 没有其他地方要用冰的话 无需太精确
    AvZ::SetTime(-300, 20);
    AvZ::Card(ICE_SHROOM, 2, 1);
    AvZ::Card(COFFEE_BEAN, 2, 1);

    // 炮消珊瑚
    // AvZ::SetTime(250 - 378 - 1, 20);
    // AvZ::pao_operator.pao(4, 7);

    // 樱代一炮 防止因炮消珊瑚 cd不够
    // 289全伤小丑 但舞王会召唤出伴舞
    // 果然预判炸还是不要用樱代了
    AvZ::SetTime(xk12_delay_fight, 20);
    AvZ::pao_operator.pao({{2, 9}, {5, 9}});
    // AvZ::pao_operator.pao(2, 9);
    // AvZ::SetTime(190, 20);
    // AvZ::Card(CHERRY_BOMB, 5, 9);
    // 补一对炮 消灭残留普僵
    AvZ::SetTime(xk12_delay_fight + xk12_wave_len, 20);
    AvZ::InsertOperation([=]() {
                        if (AvZ::GetRunningWave() == 20 && HaveZombieNotDead())
                        {
                            AvZ::SetTime(xk12_delay_fight + xk12_wave_len, 20);
                            AvZ::pao_operator.pao({{2, 9}, {5, 9}});
                        } });
    // 再补一对炮防过于靠右的铁桶 并使用垫材
    AvZ::SetTime(xk12_delay_fight + xk12_wave_len * 2 + 250, 20);
    AvZ::InsertOperation([=]() {
                        if (AvZ::GetRunningWave() == 20 && HaveZombieNotDead())
                        {
                            AvZ::SetTime(xk12_delay_fight + xk12_wave_len*2 + 250, 20);
                            AvZ::pao_operator.pao({{2, 9}, {5, 9}});
                        } });
    // 最后杀红眼
    AvZ::SetTime(xk12_delay_fight + xk12_wave_len * 3 + 250, 20);
    AvZ::InsertOperation([=]() {
                        if (AvZ::GetRunningWave() == 20 && HaveZombieNotDead())
                        {
                            AvZ::SetTime(xk12_delay_fight + xk12_wave_len*3 + 250, 20);
                            AvZ::pao_operator.pao({{2, 9}, {5, 9}});
                        } });
}

boolean HaveZombieNotDead()
{
    for (auto&& alive_zombie : AvZ::alive_zombie_filter) {
        return true;
    }

    return false;
}

void TickRun()
{
    game_running_wave = AvZ::GetRunningWave();
    TickPad();
}

void TickPad()
{
    if (game_running_wave == 9 || game_running_wave == 19) {
        if (!HaveZombieNotDead()) {
            return;
        }
        int game_now_time = AvZ::NowTime(game_running_wave);
        if (game_now_time >= 825) {
            for (auto seed_type : {PUFF_SHROOM, SUNFLOWER, SUN_SHROOM, SCAREDY_SHROOM}) {
                int card_return = WxSmartCard(seed_type, 1, 9);
                if (card_return != CARD_CD_NOT_ENOUGH) {
                    if (WxSmartCard(seed_type, 6, 9) != Asm::NIL) {
                    } else if (WxSmartCard(seed_type, 1, 8) != Asm::NIL) {
                    } else if (WxSmartCard(seed_type, 6, 8) != Asm::NIL) {
                    } else if (WxSmartCard(seed_type, 1, 7) != Asm::NIL) {
                    } else if (WxSmartCard(seed_type, 6, 7) != Asm::NIL) {
                    }
                }
            }
        }
    }
}

int WxSmartCard(int seed_type, int row, float col)
{
    // 判断植物类型 获取该植物的种子序号
    // 此方式获取的序号若用在CardNotInQueue函数中 需要+1
    int seed_index;
    if (seed_type > IMITATOR) {
        seed_type -= (IMITATOR + 1);
        seed_index = AvZ::GetSeedIndex(seed_type, true);
    } else {
        seed_index = AvZ::GetSeedIndex(seed_type, false);
    }

    auto seed_array = AvZ::GetMainObject()->seedArray();

    // 检测卡片是否恢复 然后再使用卡片
    if (seed_array[seed_index].isUsable()) {
        int plant_reject = Asm::getPlantRejectType(seed_type, row - 1, col - 1);
        if (plant_reject == Asm::NIL) {
            AvZ::CardNotInQueue(seed_index + 1, row, col);
        }
        return plant_reject;
    } else {
        return CARD_CD_NOT_ENOUGH;
    }
}
