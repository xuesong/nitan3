#include <ansi.h>
inherit ITEM;
// by dream
int is_enchase_ob() { return 1; }
void create()
{
        set_name("魔之宝石", ({ "BLK jewel6","jewel6" }));
        set_weight(40);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", "一块闪烁这奇异光芒的黑色宝石，"
                    "让人感到一阵阵的心悸。\n");
                set("unit", "块");
                set("value", 180000);
                set("no_identify", 1);
                set("wear_msg", HIC "$N" HIC "轻轻地把$n"
                    HIC "戴在头上。\n" NOR);
                set("remove_msg", HIC "$N" HIC "轻轻地把$n"
                    HIC "从头上摘了下来。\n" NOR);
                
                set("enchase/SN", 1);
                set("enchase/type", "all");   // 可镶嵌的道具类型"all"或"sword"或"blade"或"cloth"等
                set("enchase/cur_firm", 90);  // 抗磨损度
                set("enchase/wspe_data",      // 兵器上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性


                          "str" : 8,           // 增加膂力

                        ////////////////////     兵器特有属性

                        "poison_attack" : 70, // 毒焰攻击
                        "draw_dragon"   : 1,  // 召唤神龙
                        "xuruo_status"  : 3,  // 战力削弱(虚弱)1秒

                   ]));

                set("enchase/aspe_data",       // 防具上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性
 
                          "str" : 8,           // 增加膂力

                        ////////////////////     防具特有属性

                        "reduce_poison" : 70, // 抗毒效果：70%
                        "avoid_poison"  : 70, // 毒性回避：70%
                        "reduce_damage" : 40, // 化解伤害：40%
                        "reduce_busy"   : 40, // 化解忙乱：40%
                        "reduce_poison" : 70, // 抗毒效果：70%
                        "parry"  : 80,       // 招架等级：+80

                   ]));
        }
        setup();
}

int query_autoload()
{
        return 1;
}


