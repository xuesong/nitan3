// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.
// leidong.c 雷动九天

#include <ansi.h>

inherit F_SSERVER;
int perform(object me, object target)
{
        string msg;
        int improve;
        object weapon = me->query("weapon");
        
        if (! target) target = offensive_target(me);

        if (! target || ! target->is_character() || ! me->is_fighting(target))
                return notify_fail("雷动九天只能对战斗中的对手使用。\n");
                
        if (userp(me) && ! me->query("can_perform/poyu-quan/lei"))
                 return notify_fail("你还没有受过高人指点，无法施展。\n"); 
                 
        if (objectp(me->query_temp("weapon")))
                return notify_fail("你必须空手才能使用雷动九天！\n");  
                
        if ((int)me->query_skill("poyu-quan", 1) < 80)
                return notify_fail("你的破玉拳不够娴熟，不会使用雷动九天！\n");
                                
        if ((int)me->query_skill("zixia-shengong", 1) < 80)
                return notify_fail("你的紫霞神功不够高，不能用出雷动九天伤敌。\n");

        if (me->query_skill_prepared("cuff") != "poyu-quan"
                || me->query_skill_mapped("cuff") != "poyu-quan")
                return notify_fail("你现在无法使用「雷动九天」进行攻击。\n");                                                                                 

        if ((int)me->query("neili") < 200)
                return notify_fail("你现在内力太弱，无法使出雷动九天。\n");

        if (me->query_temp("leidong"))
                return notify_fail("你正在使用雷动九天！\n");

        msg = MAG "$N" MAG "使出劈石破玉拳的绝学雷动九天，顿时天空电闪雷鸣！\n" NOR;
        message_combatd(msg, me, target); 
      
        improve = (int)me->query("dex");
        improve += (int)me->query("str");
        me->add_temp("apply/dex", improve);
        me->add_temp("apply/str", improve);
        me->set_temp("leidong", 1);  
        
        me->add("neili", -150); 
        target->start_busy(random((int)me->query_skill("leidong") / 20 + 2));      
        me->start_call_out((: call_other, __FILE__, "remove_effect", me, improve :),
                            me->query_skill("poyu-quan", 1) / 5);
        return 1;
}

void remove_effect(object me, int improve)
{
        if ((int)me->query_temp("leidong"))  
        {
                me->add_temp("apply/dex", - improve);
                me->add_temp("apply/str", - improve);
                me->delete_temp("leidong");
        }
}


