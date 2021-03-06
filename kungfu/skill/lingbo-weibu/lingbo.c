// lingbo.c  洛神凌波

#include <ansi.h>

inherit F_SSERVER;

int perform(object me, object target)
{
        string msg;
	int level;

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail("你一个人跳甚么？\n");

        if ((level = me->query_skill("lingbo-weibu", 1)) < 120)
                return notify_fail(HIY "你的凌波微步还不够熟练。\n" NOR);

        if ((int)me->query("neili") < 400)
                return notify_fail(HIY "你现在真气不足。\n" HIY);

        if (target->is_busy())
                return notify_fail(target->name() + "目前正自顾不暇，放胆攻击吧！\n");

        if( me->query("gender") == "女性")
               msg = HIC "$N" HIC "提起真气，依照先天伏羲六十"
                     "四卦，不理$n" HIC "如何出招，自顾自的将"
                     "\n凌波微步由头到尾迅速无比的走了一次。$N"
                     HIC "仿如洛神再生，微步凌波，\n$n" HIC
                     "却是一点皮毛也沾不上。$N" HIC
                     "真个将逍遥二字发挥得淋漓尽至。\n" NOR;
        else   msg = HIG "$N" HIG "提起真气，依照先天伏羲六十"
                     "四卦，不理$n" HIG "如何出招，自顾自的将"
                     "\n凌波微步由头到尾迅速无比的走了一次。"
                     "只见$N" HIG "意态飘逸，影子乱飞，\n$n" HIG
                     "却是一点皮毛也沾不上。$N" HIG "真个将逍"
                     "遥二字发挥得淋漓尽至。\n" NOR;

        if (random(level) > (int)target->query_skill("dodge", 1) / 2)
        {
                msg += HIW "结果$p被$P围着转得头晕眼花！\n" NOR;
                target->start_busy(level / 16);
                me->add("neili", -100);
                me->start_busy(1);
        } else
        {
                msg += HIY "$p" HIY "只是勉强的逃了开去。\n" NOR;
                me->add("neili", -50);
                me->start_busy(2);
        }
        message_combatd(msg, me, target);

        return 1;
}
