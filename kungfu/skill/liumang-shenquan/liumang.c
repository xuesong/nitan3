// liumang.c 流氓神拳
// By Alf, Last Update 2001.11

#include <ansi.h>

inherit F_SSERVER;
int perform(object me, object target)
{
        int damage;
        string msg;

        if(!target) target = offensive_target(me);

        if(!target || !target->is_character() || !me->is_fighting(target))
                return notify_fail("你只能在战斗中使用这一绝招。\n");

    if((int)me->query_skill("liumang-shenquan", 1) < 80)
                return notify_fail("你的流氓神拳还不够纯熟，无法使出这一绝招。\n");

        if((int)me->query("neili") < 200)
                return notify_fail("你的内力修为还不够高。\n");

        msg = HIG"$N深深吸了口气，气运丹田，陡然间张嘴一声狂喝：\n"NOR;
        msg += HIC"
I@@L      @@@@       I@@@ ezz,_zzzzzz_     @@@L     I@@I       zzzL  @@@' Izzz  
`@@@_I--zz@@~~-----  I@@@ I@@I     I@@I    `~~~ ezz_a@@bzz_,   `~~~,I@@@ I~~~'  
 `~~~ _@@@' `-@@L   zzzzzzI@@I     I@@I  '~~@@@ I@@'I@@II@@I  ~~~~~=@@@~~~~~~~~'
I@@L I@@@_____@@@L   @@@  I@@M~~@@@@~~     I@@P I@@ I@@II@@I ~~~~@@@@~~~-@@@~~~~
`-@@_ =@@~=== _=@~'  @@@  I@@I  I@@I      I@@@@bI@@~@@@@-@@I  ._a@@@=__d@@@@@L, 
  ==='I@@ I@@ @@@    @@@  I@@I--I@@I---  I@@@@M@a@@ I@@II@@I '~~~'   I@@I  ~~~~'
 d@@' I@@ I@@ @@@    @@@  I@@I  I@@L       I@@I I@@_I@@II@@P  `~~~~~~I@@I~~~~~' 
I@@@  I@@ I@@ @@@ .  @@@  I@@I  I@@I  _    I@@I '~~'I@@I~~'  ~~~~~~~~@@@@~~~~~~~
I@@I  a@@ I@@ @@@_%  ~-@bz`@@@zz-I@@zd~    I@@I     I@@I        I-z_z@@@P       \n"NOR;

        msg += HIY"\n跟着拳随声至，势如急风骤雨一般，迳直击往$n的胸腹要害！！！\n"NOR;  

        if(random(me->query("combat_exp")) > (int)target->query("combat_exp")/2)
        { 
                me->start_busy(2+random(2));
                target->start_busy(4);

                damage = (int)me->query_skill("liumang-shenquan", 1)/3+(int)me->query_skill("unarmed",1)/3;
                damage = damage + random(damage);

                target->receive_damage("qi", damage);
                target->receive_wound("qi", damage/2);
                me->add("neili", -200);
                msg += HIR"$n耳际如遭雷殛，懵然不知闪避，被$N这招重重击实，不由得长声惨呼！\n"NOR;
        }

        else
        {
                msg += YEL"$n急忙作势斜扑，一个就地十八滚，狼狈万状的躲开了$N这招。\n"NOR;
                me->start_busy(1);
        }

        message_vision(msg, me, target);

        if(!target->is_fighting(me)) target->fight_ob(me);
        if(userp(target)) target->fight_ob(me);
        else if( !target->is_killing(me) ) target->kill_ob(me);
        return 1;
}


