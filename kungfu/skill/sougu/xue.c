// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.

#include <ansi.h>
#include <combat.h>

#define JU "「" HIR "赤血爪" NOR "」"

inherit F_SSERVER;
#include <balance.h>

int perform(object me, object target)
{
        int damage;
        string msg;
        int ap, dp;

       if (userp(me) && ! me->query("can_perform/sougu/xue"))
                return notify_fail("你还没有受过高人指点，无法施展" JU "。\n"); 

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail(JU "只能对战斗中的对手使用。\n");

        if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
                return notify_fail(JU "只能空手施展。\n");

         if ((int)me->query_skill("sougu", 1) < 60)
                return notify_fail("你搜骨鹰爪功不够娴熟，难以施展" JU "。\n");

       if (me->query_skill_mapped("claw") != "sougu")
                return notify_fail("你没有激发搜骨鹰爪功，难以施展" JU "。\n");

        if (me->query_skill_prepared("claw") != "sougu")
                return notify_fail("你没有准备毒搜骨鹰爪功，难以施展" JU "。\n");

        if (me->query_skill("force") < 80)
                return notify_fail("你的内功修为不够，难以施展" JU "。\n");

        if ((int)me->query("neili") < 220)
                return notify_fail("你现在的真气不够，难以施展" JU "。\n");

        if (! living(target))
                return notify_fail("对方都已经这样了，用不着这么费力吧？\n");

        ap = ap_power(me, "claw");
        dp = dp_power(target, "parry");

        msg = HIW "\n$N" HIW "运转真气，双手忽成爪行，施出绝招「" HIR "赤"
              "血爪" HIW "」，迅猛无比地抓向$n" HIW "。\n" NOR;

        if (ap / 2 + random(ap) > dp)
        {
                damage = da_power(me, "claw") * 3 / 2;
                
                msg += COMBAT_D->do_damage(me, target, UNARMED_ATTACK, damage, 55,
                                           HIR "但见$N" HIR "双爪划过，$n" HIR "已闪避不及，胸口被$N" HIR
                                           "抓出十条血痕。\n" NOR);

                me->start_busy(3);
                me->add("neili", -200);
        } else
        {
                msg += CYN "$n" CYN "奋力招架，竟将$N" CYN "这招化解。\n" NOR;

                me->start_busy(4);
                me->add("neili", -100);
        }
        message_sort(msg, me, target);
        return 1;
}




