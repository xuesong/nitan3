// ding-dodge.c ���ֲ� 

inherit SKILL;

string *dodge_msg = ({
        "$n�������ĵ���������һ�����պñܹ�$N���������ơ�\n",
        "����$nǡ����������һ���������$N��һ�С�\n",
});

int valid_enable(string usage) { return (usage == "dodge"); }

int valid_learn(object me) { return 1; }

string query_dodge_msg()
{
        return dodge_msg[random(sizeof(dodge_msg))];
}

int practice_skill(object me)
{
        if( (int)me->query("qi") < 50 )
                return notify_fail("�������̫���ˣ����������ֲ���\n");
        if( (int)me->query("neili") < 40 )
                return notify_fail("�������������\n");
        me->receive_damage("qi", 40);
        me->add("neili", -30);
        return 1;
}

