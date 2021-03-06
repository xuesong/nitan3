#include <ansi.h>
inherit ITEM;
// by dream
int is_enchase_ob() { return 1; }
void create()
{
        set_name(WHT "稀世宝石" NOR, ({ "WHT jewel3","jewel3" }));
        set_weight(20);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", WHT "一颗白光闪闪的精美宝石，给人以不同寻常的感觉。\n" NOR);
                set("unit", "块");
                set("value", 120000);
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


                        "int" : 4,           // 增加智慧
                        "dex" : 4,           // 增加根骨

                        ////////////////////     兵器特有属性

                        "cold_attack"   : 30, // 冷冻攻击
                        "suck_neili"    : 10, // 吸取内力
                        "suck_jingli"   : 10, // 吸取精力

                   ]));

                set("enchase/aspe_data",       // 防具上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性

                        "int" : 4,           // 增加智慧
                        "dex" : 4,            // 增加根骨
                        ////////////////////     防具特有属性

                        "reduce_cold"   : 30, // 化解冷冻：30%
                        "defense": 30,       // 防御等级：+30
                        "max_jingli"    : 10, // 精力上限：+10%

                        "jing_recover_speed"   : 15, // 精气恢复速度: +15%
                   ]));
        }
        setup();
}

int query_autoload()
{
        return 1;
}

