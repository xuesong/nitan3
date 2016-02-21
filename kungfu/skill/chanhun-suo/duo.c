// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;
#include <balance.h>

#define DUO "��" HIC "�����" NOR "��"

int perform(object me, object target)
{
        object weapon;
        string msg;
        int ap, dp;
        int damage;
        /*
        if (userp(me) && ! me->query("can_perform/chanhun-suo/duo"))
                return notify_fail("�㻹û���ܹ�����ָ�㣬�޷�ʩչ" DUO "��\n");
        */
        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail(DUO "ֻ����ս���жԶ���ʹ�á�\n");

        if (! objectp(weapon = me->query_temp("weapon")) ||
            weapon->query("skill_type") != "whip")
                return notify_fail("��ʹ�õ��������ԡ�\n");

        if (me->query_skill("force", 1) < 130)
                return notify_fail("����ڹ���򲻹���ʹ����" DUO "��\n");

        if (me->query_skill("chanhun-suo", 1) < 100)
                return notify_fail("��Ĳ���������̫ǳ��ʹ����" DUO "��\n");

        if (me->query("neili") < 220)
                return notify_fail("��������������޷�ʹ��" DUO "��\n");

        if (me->query_skill_mapped("whip") != "chanhun-suo")
                return notify_fail("��û�м�������������ʹ����" DUO "��\n");

       if (! living(target))
              return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");

        msg = HIY "$N" HIY "�����һЦ������" + weapon->name() +
              HIY "���ñ�ֱ������$n" HIY "���󣬿��Ǵ�����;ȴ��ͣ�ͣ�ת��$n"
              HIY "�ؿڻ�ȥ��\n" NOR;

        ap = ap_power(me, "whip");
        dp = dp_power(target, "parry");

        if (ap / 2 + random(ap) > dp)
        {
                damage = da_power(me, "whip");
                me->add("neili", -180);
                me->start_busy(2);

                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 75,
                                           HIR "���$n" HIR "һ���ҽУ�δ�ܿ���$N"
                                           HIR "����ͼ������һ��Ӳ�����ؿڣ���Ѫ��"
                                           "����Ƥ��������\n" NOR);
                message_combatd(msg, me, target);
                if (ap > 1000) ap = 1000;
                dp += target->query_skill("martial-cognize", 1) * 2;
                if (! target->is_busy() &&
                    weapon->query("material") == "white silk" &&
                    ap / 2 + random(ap) > dp)
                {
                        target->start_busy(1 + random(4));
                        message_vision(HIW "$N" HIW "һ�������е�" + weapon->name() +
                                       HIW "��ಡ��Ĵ��˳���������Ȼ����$n"
                                       HIW "��\n$n" HIW "���һ������æ�˺���ȥ��һ�С�\n" NOR,
                                       me, target);
                }
        } else
        {
                me->add("neili", -100);
                me->start_busy(3);
                msg += CYN "����$p" CYN "����һԾ���𣬶�ܿ���"
                       CYN "$P" CYN "�Ĺ�����\n" NOR;
                message_combatd(msg, me, target);
        }

        return 1;
}
