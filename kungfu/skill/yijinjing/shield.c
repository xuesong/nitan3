// shield.c 易筋经金刚不坏体

#include <ansi.h>

inherit F_CLEAN_UP;

void remove_effect(object me, int amount);

int exert(object me, object target)
{
        int skill;


        if (me->query("family/family_name") != "少林派")
                return notify_fail("你不是少林弟子，无法使用易筋经绝技。\n");

        if (target != me)
                return notify_fail("你只能用易筋经来激发金刚不坏体 \n");

        if ((int)me->query("neili") < 100)
                return notify_fail("你的真气不够。\n");

        if ((int)me->query_skill("yijinjing", 1) < 40)
                return notify_fail("你的易筋经等级不够。\n");

        if ((int)me->query_temp("shield"))
                return notify_fail("你已经在运功中了。\n");

        skill = me->query_skill("force");
        me->add("neili", -100);
        me->receive_damage("qi", 0);

        message_combatd(HIY "$N" HIY "双目突然精芒暴涨,身上蓦地涌起"
                        HIY "一股凛冽金光。\n" NOR, me);

        me->add_temp("apply/armor", skill * 5 / 2);
        me->set_temp("shield", 1);

        me->start_call_out((: call_other, __FILE__, "remove_effect", me, skill * 5 / 2 :), skill);


        if (me->is_fighting()) me->start_busy(2);

        return 1;
}

void remove_effect(object me, int amount)
{
        if (me->query_temp("shield"))
        {
                me->add_temp("apply/armor", -amount);
                me->delete_temp("shield");
                tell_object(me, "金光隐隐退去,你恢复原来的样子。\n");
        }
}

