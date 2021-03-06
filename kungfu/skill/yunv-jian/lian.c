// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.

#include <ansi.h>

#define LIAN "「" HIW "连环剑术" NOR "」"

inherit F_SSERVER;
#include <balance.h>

int perform(object me, object target)
{
        object weapon;
        string msg;
        int ap, dp;
        int count;
        int i, attack_time;
 
        if (userp(me) && ! me->query("can_perform/yunv-jian/lian"))
                return notify_fail("你未得高人指点，不知该如何施展" LIAN "。\n");

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail(LIAN "只能对战斗中的对手使用。\n");

        if (! objectp(weapon = me->query_temp("weapon"))
           || (string)weapon->query("skill_type") != "sword")
                return notify_fail("你所使用的武器不对，难以施展" LIAN "。\n");

        if ((int)me->query_skill("yunv-jian", 1) < 120)
                return notify_fail("你玉女剑法不够娴熟，难以施展" LIAN "。\n");

        if ((int)me->query_skill("dodge", 1) < 100)
                return notify_fail("你的轻功修为不够，难以施展" LIAN "。\n");

        if (me->query_skill_mapped("sword") != "yunv-jian")
                return notify_fail("你没有激发玉女剑法，难以施展" LIAN "。\n");

        if (me->query("neili") < 200)
                return notify_fail("你现在的真气不够，难以施展" LIAN "。\n");

        if (! living(target))
                return notify_fail("对方都已经这样了，用不着这么费力吧？\n");

        msg = HIM "\n$N" HIM "身子轻轻一转，霎时向$n" HIM "劈出了数剑！剑招看"
             "似简单，但却迅猛非常，令人匪夷所思！" NOR;

        message_sort(msg, me, target);

        ap = ap_power(me, "sword");
        dp = dp_power(target, "dodge"); 

        attack_time = 4;

        if (ap / 2 + random(ap * 2) > dp)
        {
                msg = HIR "结果$n" HIR "被$N" HIR "攻了个措手不及，$n"
                      HIR "慌忙招架，心中叫苦。\n" NOR;
                count = ap / 8;
                attack_time += random(ap / 40);
                me->add_temp("apply/attack", count * 2);
        } else
        {
                msg = HIC "$n" HIC "见$N" HIC "这几剑招式凌厉，凶猛异"
                      "常，只得苦苦招架。\n" NOR;
                count = 0;
        }
        message_combatd(msg, me, target);

        if (attack_time > 8)
                attack_time = 8;

        me->add("neili", -attack_time * 20);

        for (i = 0; i < attack_time; i++)
        {
                if (! me->is_fighting(target))
                        break;

                COMBAT_D->do_attack(me, target, weapon, 20, 20);
        }
        me->add_temp("apply/attack", -count * 2);
        me->start_busy(1 + random(attack_time));

        return 1;
}
