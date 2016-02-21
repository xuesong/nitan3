#include <ansi.h>
#include <combat.h>

#define XIAN "��" HIY "������ӡ��" NOR "��"

inherit F_SSERVER;
#include <balance.h>

int perform(object me, object target)
{
        object weapon;
        int damage;
        string msg;
        int ap, dp;

        if (userp(me) && ! me->query("can_perform/hujia-daofa/xian"))
                return notify_fail("�㻹û���ܹ�����ָ�㣬�޷�ʩչ" XIAN "��\n");

        if (! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if (! target || ! me->is_fighting(target))
                return notify_fail(XIAN "ֻ�ܶ�ս���еĶ���ʹ�á�\n");
 
        if (! objectp(weapon = me->query_temp("weapon")) ||
            (string)weapon->query("skill_type") != "blade")
                return notify_fail("��ʹ�õ��������ԣ�����ʩչ" XIAN "��\n");

        if ((int)me->query_skill("force") < 160)
                return notify_fail("����ڹ���򲻹�������ʩչ" XIAN "��\n");

        if ((int)me->query_skill("hujia-daofa", 1) < 120)
                return notify_fail("��ĺ��ҵ����������ң�����ʩչ" XIAN "��\n");

        if (me->query_skill_mapped("blade") != "hujia-daofa")
                return notify_fail("��û�м������ҵ���������ʩչ" XIAN "��\n");

        if ((int)me->query("neili") < 200)
                return notify_fail("�����������������ʩչ" XIAN "��\n");

        if (! living(target))
                return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");

        msg = HIY "$N" HIY "�������" + weapon->name() +  HIY "��ʩ����������"
              "ӡ�ơ���˫���յ���������������$n" HIY "�����ƽ���\n"NOR;

        ap = ap_power(me, "blade") + me->query_skill("force", 1);
        dp = dp_power(target, "force") + target->query_skill("force");
             
        if (ap / 2 + random(ap) > dp)
        {
                damage = da_power(me, "blade");
                me->add("neili", -150);
                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 25,
                                           HIR "$n" HIR "����������˸����ĵ�����"
                                           "һ���ҽУ��������������֣���Ѫ�Ĵ��罦��\n" NOR);
                me->start_busy(2);
                if (ap / 3 + random(ap / 2) > dp && ! target->is_busy())
                        target->start_busy(ap / 100 + 3);
        } else 
        {
                msg += CYN "$p" CYN "���һ����Ҳ���мܣ��������"
                       "�������ߣ��ܿ���$P" CYN "��һ�С�\n" NOR;
                me->add("neili", -50);
                me->start_busy(3);
        }
        message_combatd(msg, me, target);

        return 1;
}
