// guiying.c
// smallbear 11/1/2002

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

int perform(object me, object target)
{
      object weapon;
      int skill;
    if( !target ) target = offensive_target(me);

    if( !target || !me->is_fighting(target) )
      return notify_fail("「无形鬼影」只能在战斗中使用。\n");

    if ((int)me->query_skill("yixing-bufa", 1) < 100 )
        return notify_fail("你的意形步法等级不够，不能使出「无形鬼影」！\n");    
    if ((int)me->query_skill("dodge", 1) < 100 )
        return notify_fail("你的基本轻功极不够娴熟，不能使出「无形鬼影」！\n");    
    if ( me->query_skill_mapped("dodge") != "yixing-bufa"
             || me->query_skill_mapped("force") != "busi-shenlong") 
                return notify_fail("你现在无法使出「无形鬼影」进行牵制！\n"); 
    if ((int)me->query("neili") < 300 )
        return notify_fail("你现在真气太弱，不能使出「无形鬼影」！\n");
    if ((int)me->query("neili") < 1000 )
        return notify_fail("你现在内力太弱，不能使出「无形鬼影」！\n");
    if ( (int)me->query_temp("鬼影", 1) ) 
        return notify_fail("你已经使出「无形鬼影」步法！\n");
    
    message_vision(HIB"$N前后游走，身形闪动，脚踏意形步法，化出数个身形。
$n被此「无形鬼影」所迷惑，手足无措，不知哪个$N是真，哪个是假。\n"NOR, me, target);
    target->start_busy(4);
    me->start_busy(2);
    me->add("neili", -200);    
    skill = me->query_skill("yixing-bufa", 1)/3;
    me->set_temp("鬼影", 1);
    me->start_call_out( (: call_other, __FILE__, "remove_effect", me:), skill+1);
    return 1;
}

void remove_effect(object me)
{
    message_vision(HIB"$N长吁一声，放慢步法，分身立即消失不见！\n"NOR, me);
    me->delete_temp("鬼影");
    return;
}

