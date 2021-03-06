#include <ansi.h>
inherit ITEM;
// by dream
int is_enchase_ob() { return 1; }
void create()
{
        set_name(YEL "宝石碎片" NOR, ({ "YEL jewel1","jewel1" }));
        set_weight(20);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", YEL "一片黄色的宝石碎片。\n" NOR);
                set("unit", "片");
                set("value", 80000);
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


                        "con" : 1,           // 增加身法

                        ////////////////////     兵器特有属性

                        "busy_time"     : 1,  // 忙乱敌人3秒
                        "unarmed_damage": 10, // 手法伤害：+10
                        "no_exert"      : 1,  // 内息紊乱3秒

                   ]));

                set("enchase/aspe_data",       // 防具上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性

                        "con" : 1,           // 增加身法

                        ////////////////////     防具特有属性

                        "avoid_poison"  : 10, // 毒性回避：10%
                        "dodge"  : 10,       // 轻功等级：+10
                        "reduce_busy"   : 5, // 化解忙乱：5%

                        "jingli_recover_speed" : 2, // 精力恢复速度：+2%
                   ]));
        }
        setup();
}

int query_autoload()
{
        return 1;
}


