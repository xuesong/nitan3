// leiting.c 「雷霆一击」

#include <ansi.h>
#include <combat.h>

#define LEITING "「" HIY "雷霆一击" NOR "」"

inherit F_SSERVER;

int perform(object me, object target)
{
        int damage;
        string msg;
        int ap, dp;

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail(LEITING "只能对战斗中的对手使用。\n");

        if ((int)me->query_skill("force") < 250)
                return notify_fail("你的内功火候不够，使不出" LEITING "。\n");

        if ((int)me->query_skill("dragon-strike", 1) < 180)
                return notify_fail("你的降龙十八掌不够熟练，不会使用" LEITING "。\n");

        if ((int)me->query("neili") < 500)
                return notify_fail("你的真气不够，无法使用" LEITING "。\n");

	if (me->query_skill_mapped("strike") != "dragon-strike")
		return notify_fail("你没有激发降龙十八掌，无法使用" LEITING "。\n");

        msg = HIC "$N" HIC "默运内功，施展出" LEITING HIC "，全身急速转动起来，"
              "越来越快，就\n犹如一股旋风，骤然间，$N" HIC "已卷向正看"
              "得发呆的" HIC "$n。\n"NOR;  

        ap = me->query_skill("strike") + me->query("str") * 10;
        dp = target->query_skill("dodge") + target->query("dex") * 10;
        me->start_busy(3);
        if (ap / 2 + random(ap) > dp)
        { 
                damage = ap + random(ap / 2);
                me->add("neili", -400);
                msg += COMBAT_D->do_damage(me, target, UNARMED_ATTACK, damage, 70,
                                           HIR "$p" HIR "只见一阵旋风影中陡然现出$P"
                                           HIR "的双拳，根本来不及躲避，被重重击中，\n五"
                                           "脏六腑翻腾不休，口中鲜血如箭般喷出！！！\n" NOR);
        } else
        {
                msg += HIG "可是$p" HIG "看破了$P" HIG "的企图，没"
                       "有收到迷惑，闪在了一边。\n" NOR;
        }
        message_combatd(msg, me, target);

        return 1;
}
