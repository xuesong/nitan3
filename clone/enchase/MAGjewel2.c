#include <ansi.h>
inherit ITEM;
int is_enchase_ob() { return 1; }
void create()
{
        set_name(MAG "宝石" NOR, ({ "MAG jewel2","jewel2" }));
        set_weight(20);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", MAG "一颗紫光闪闪的精美宝石，华丽异常。\n" NOR);
                set("unit", "块");
                set("value", 100000);

                set("wear_msg", HIC "$N" HIC "轻轻地把$n"
                    HIC "戴在头上。\n" NOR);
                set("remove_msg", HIC "$N" HIC "轻轻地把$n"
                    HIC "从头上摘了下来。\n" NOR);
                set("no_identify", 1);                    
                set("enchase/type", "all");   // 可镶嵌的道具类型"all"或"sword"或"blade"或"cloth"等
                set("enchase/cur_firm", 90);  // 抗磨损度
                set("enchase/wspe_data",      // 兵器上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性


                        "con" : 2,           // 增加身法

                        ////////////////////     兵器特有属性

                        "perferm_ap"    : 2,  // 绝招命中：+5%
                        "suck_eff_jing" : 6, // 伤敌精力上限
                        "suck_jingli"   : 6, // 吸取精力
                        "suck_neili"    : 6, // 吸取内力

                   ]));

                set("enchase/aspe_data",       // 防具上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性

                        "con" : 2,           // 增加身法

                        ////////////////////     防具特有属性

                        "defense": 20,       // 防御等级：+20
                        "armor"  : 20,       // 防御能力：+20
                        "perform_dp"    : 2,  // 绝招防御：+2%
                        "reduce_damage" : 10, // 化解伤害：10%
                        "reduce_busy"   : 10, // 化解忙乱：10%

                   ]));
        }
        setup();
}

int query_autoload()
{
        return 1;
}


