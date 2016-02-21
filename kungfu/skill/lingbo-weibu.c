#include <ansi.h>

inherit SKILL;

string *dodge_msg = ({
        "����$n����һ�ã�$N��һ�����˸��ա�\n",
        "ȴ��$n�㲻��أ����Դܿ����ߣ����˿�ȥ��\n",
        "$n����΢�Σ��о����յرܿ���$N��һ�С�\n",
        "$n���������������Ī�⣬��$N��һ��û�з����κ����á�\n",
        "$n�Թ��Ե��߳���������Ȼ����$N���й���δ���\n",
        "$N��һ���ۿ���Ҫ���У�����$n���Ա�һ�ã�ȥ��֮�ʵ���Ƿ�����˼��\n",
        "$n��ǰһ������Ȼ���ˣ�ǡǡ�ܿ�$N��һ�У��о����ա�\n",
});

int valid_enable(string usage) 
{ 
        return usage == "dodge";
}

int valid_learn(object me)
{
        int lvl;

        lvl = me->query_skill("lingbo-weibu", 1);
        if (lvl > 300) lvl = 300;

       if (me->query("dex") < 21) 
                return notify_fail("����������̫��޷�ѧϰ�貨΢����\n");

        if (me->query("max_neili") < 3000 + lvl * 20)
                return notify_fail("�����������������پ���"
                                   "�ڷ���֮�����������������á�\n");
        return 1;
}

string query_dodge_msg(string limb)
{
        return dodge_msg[random(sizeof(dodge_msg))];
}

mixed valid_damage(object ob, object me, int damage, object weapon)
{
        mixed result;
        int ap, dp, mp;

        if ((int)me->query_skill("lingbo-weibu", 1) < 100 ||
            ! living(me))
                return;

        mp = ob->query_skill("count", 1);
        ap = ob->query_skill("dodge") + mp;
        dp = me->query_skill("dodge", 1) / 2 +
             me->query_skill("lingbo-weibu", 1);

        if (ap / 2 + random(ap) < dp)
        {
                result = ([ "damage": -damage ]);

                switch (random(4))
                {
                case 0:
                        result += (["msg" : HIW "$n" HIW "��������ζ���$N" HIW
                                            "��ǰ��ʱ������������$n" HIW "�Ļ�Ӱ����$N"
                                            HIW "��ȫ�޷������ʵ��\n" NOR]);
                        break;
                case 1:
                        result += (["msg" : HIW "$n" HIW "���Ա�����бб������һ����ȴǡ"
                                            "����$N" HIW "�Ĺ���ʧ֮���塣\n" NOR]);
                        break;
                case 2:
                        result += (["msg" : HIW "$N" HIW "��һ�����ĺÿ죬Ȼ��$n"
                                            HIW "һ�����ƺ����Ѱ��������ȴ��$N"
                                            HIW "��һ�иպñܿ���\n" NOR]);
                        break;
                default:
                        result += (["msg" : HIW "$n" HIW "����һԾ���Ѳ�������Ӱ��$N"
                                            HIW "���д󺧣�ȴ�ּ�$n" HIW "���籼����"
                                            "��������˼�����ǡ�\n" NOR]);
                        break;
                }
                return result;
        } else
        if (mp >= 100)
        {
                switch (random(4))
                {
                case 0:
                        result = HIY "ֻ��$n" HIY "΢΢һЦ����������ζ���"
                                 "��ʱ������������$n" HIY "�Ļ�Ӱ������$N"
                                 HIY "��ͨ��������������ʵ������������\n" NOR;
                        break;
                case 1:
                        result = HIY "$n" HIY "���Ա�����бб������һ����Ȼ��$N"
                                 HIY "�������棬��λ���岻�\n" NOR;
                        break;
                case 2:
                        result = HIY "$n" HIY "һ�����ƺ����Ѱ��������ȴ��$N"
                                 HIY "��һ�иպñܿ�������$N"
                                 HIY "�������У�����ȴ�ǲ�ֹ��\n" NOR;
                        break;
                default:
                        result = HIY "����$n" HIY "����һԾ���Ѳ�������Ӱ��$N"
                                 HIY "����˼�����������У����������޷���\n" NOR;
                        break;
                }
                COMBAT_D->set_bhinfo(result);
        }
}

int query_effect_dodge(object attacker, object me)
{
        int lvl;

        lvl = me->query_skill("lingbo-weibu", 1);
        if (lvl < 50)  return 0;
        if (lvl < 100) return 30;
        if (lvl < 150) return 50;
        if (lvl < 200) return 80;
        if (lvl < 250) return 100;
        if (lvl < 300) return 120;
        if (lvl < 350) return 140;
        return 150;
}

int practice_skill(object me)
{
        if ((int)me->query("qi") < 70)
                return notify_fail("�������̫���ˣ�������ϰ�貨΢����\n");

        if ((int)me->query("neili") < 70)
                return notify_fail("������������ˣ��޷���ϰ�貨΢����\n");

        me->receive_damage("qi", 65);
        me->add("neili", -65);
        return 1;
}

string perform_action_file(string action)
{
        return __DIR__"lingbo-weibu/" + action;
}
void skill_improved(object me)
{
        if (me->query_skill("lingboweibu",1) == 80 && ! me->query("lbwb") 
        &&  me->query_skill("literate", 1) > 80)
        {
                tell_object(me, HIY "$Nһ������������ֻ���������泩����֪���������������ۡ�\n" NOR);
                me->add("max_neili", random(30)+20);
                me->add("lbwb",1);
        }
        if (me->query_skill("lingboweibu", 1) == 120 && me->query("lbwb") == 1 
        &&  me->query_skill("literate", 1) > 100)
        {
                tell_object(me, HIM "$N��ʮ�������꣬�պ�����һ����Ȧ���ص�ԭ�أ��������\n" NOR);
                me->add("max_neili", random(30)+20);
                me->add("lbwb", 1);
        }
        if (me->query_skill("lingboweibu", 1) == 160 && me->query("lbwb") == 2 
        &&  me->query_skill("literate", 1) > 120)
        {
                tell_object(me, CYN "$NԽ��Խ�죬��Ϣ���Ų�����ס��ת������Ȼ�Լ��������Ѿ�������ߣ�\n" NOR);
                me->add("max_neili", random(30)+20);
                me->add("lbwb", 1);
        }
        if (me->query_skill("lingboweibu", 1) == 200 && me->query("lbwb") == 3 
        &&  me->query_skill("literate", 1) > 140)
        {
                message_vision(RED "$N������죬����̤�����Ų���Բ��ֻ��������ˬ��ȫ������������\n" NOR, me);
                me->add("max_neili", random(30)+20);
                me->add("lbwb", 1);
        }
}