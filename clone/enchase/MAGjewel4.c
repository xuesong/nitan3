#include <ansi.h>
inherit ITEM;
int is_enchase_ob() { return 1; }
void create()
{
        set_name(MAG "帝之宝石" NOR, ({ "MAG jewel4","jewel4" }));
        set_weight(30);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", MAG "一块闪烁这奇异光芒的紫色宝石，"
                    "让人感到一阵阵的心悸。\n" NOR);
                set("unit", "块");
                set("value", 140000);
                set("no_identify", 1);
                set("wear_msg", HIC "$N" HIC "轻轻地把$n"
                    HIC "戴在头上。\n" NOR);
                set("remove_msg", HIC "$N" HIC "轻轻地把$n"
                    HIC "从头上摘了下来。\n" NOR);
                    
                set("enchase/type", "all");   // 可镶嵌的道具类型"all"或"sword"或"blade"或"cloth"等
                set("enchase/cur_firm", 90);  // 抗磨损度
                set("enchase/wspe_data",      // 兵器上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性


                        "con" : 4,           // 增加身法

                        ////////////////////     兵器特有属性

                        "perferm_ap"    : 4,  // 绝招命中：+5%
                        "suck_eff_jing" : 15, // 伤敌精力上限
                        "suck_jingli"   : 15, // 吸取精力
                        "suck_neili"    : 15, // 吸取内力

                   ]));

                set("enchase/aspe_data",       // 防具上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性

                        "con" : 4,           // 增加身法

                        ////////////////////     防具特有属性

                        "defense": 40,       // 防御等级：+40
                        "armor"  : 40,       // 防御能力：+40
                        "perform_dp"    : 4,  // 绝招防御：+4%
                        "reduce_damage" : 20, // 化解伤害：20%
                        "reduce_busy"   : 20, // 化解忙乱：20%

                   ]));
        }
        setup();
}

int query_autoload()
{
        return 1;
}


