// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.
// yijinjing.c 少林 易筋经神功


#include <ansi.h>
inherit FORCE;

int valid_enable(string usage) { return usage == "force"; }

int valid_force(string force) { return 1; }     

int query_neili_improve(object me)   
{
        int lvl;  
        lvl = (int)me->query_skill("yijinjing", 1);  
        return (lvl / 20) * (lvl / 10) * 25 * 15 /100; 
}

int valid_learn(object me)
{
        if (me->query("con") < 20)
                return notify_fail("你研究了一会儿，只觉得眼前金星乱冒，太阳穴突突的跳。\n");
                
        if (me->query_skill("force", 1) < 100)
                return notify_fail("你的基本内功火候不够，无法学习易筋经神功！\n");

        /*
        if (me->query_skill("freezing-force", 1))
                return notify_fail("你已经将易筋经神功和冰蚕寒毒合炼成了一门新内功了。\n");
        */

        if (me->query_skill("force", 1) < me->query_skill("yijinjing", 1))
                return notify_fail("你的基本内功水平不够，难以领悟更高深的易筋经神功！\n");

        if (me->query("gender") != "男性")
                return notify_fail("你不是男性，无法练习易筋经神功。\n");

        return ::valid_learn(me);
}

void skill_improved(object me)
{
        int skill, lit;
        
        skill = me->query_skill("yijinjing", 1);
        lit = me->query_skill("literate", 1);
        
        if (skill == 200 && ! me->query("sl/yijinjing"))
        {
                tell_object(me, HIW "\n忽然，你感道体内内息奔走，越转越快，瞬时间已经在全身上下运行了一周天，复汇聚于丹田之中！\n" NOR);
                me->add("max_neili", random(lit + skill));
                if (me->query("max_neili") > me->query_neili_limit()) 
                        me->set("max_neili", me->query_neili_limit()); 
                me->set("sl/yijinjing", 1);
        }
        if (skill == 400)
        {
                tell_object(me, HIW "\n忽然，你感道体内内息奔走，越转越快，瞬时间已经在全身上下运行了十周天，复汇聚于丹田之中！\n" NOR);
                tell_object(me, HIW "你修炼成百毒不侵神功了。\n" NOR);
                me->set_temp("nopoison", 1);
        }
}

int practice_skill(object me)
{
        return notify_fail("易筋经神功只能用学(learn)的来增加修为。\n");
}

string exert_function_file(string func)
{
        return __DIR__"yijinjing/" + func;
}

mapping exercise_msg(object me)
{
        return ([
                "status_msg" : "脸上红光隐现，气象庄严",
                "start_my_msg" : "五心向天，排除一切杂念，内息顺经脉缓缓流动。\n",
                "start_other_msg" : "五心向天，一会工夫，从口鼻处缓缓吐出白气，白气越吐越浓，渐渐笼罩了全身。\n",
                "halt_msg" : "长出一口气，将内息急速退了回去，站了起来。\n",
                "end_my_msg" : "将内息走了个小周天，流回丹田，收功站了起来。\n",
                "end_other_msg" : "只见笼罩"+me->name()+"的白气渐渐消散，又从口鼻处吸了回去。\n",
                "heal_msg" : "双手合什，盘膝而坐，口中念起“往生咒”，开始运功疗伤。\n",
                "heal_finish_msg" : "缓缓站起，只觉全身说不出的舒服畅快，便道：“善哉！善哉！本门筋经当真是天下绝学！”\n",
                "heal_unfinish_msg" : "吐出瘀血，缓缓站起，但脸色苍白，看来还有伤在身。\n",
                "heal_halt_msg" : "一声：“阿弥陀佛”双袖挥动，压下内息，站起身来。\n",
        ]);
}

string *fanzhen_msg = ({
        "只震得$N胸内腹中，有如五脏一齐翻转！\n",
        "抢先反震，将$N震得脸如白纸，全无血色！\n",
        "震得$N心肺欲裂，腾腾腾连退数步！\n",
        "功力已布满全身，将$N的力量反震回去！\n",
        "震得$N天旋地转，腾腾腾连退七八步！\n",
        "和$N一撞，只震的$p胸口气血翻涌！\n",
        "$N便如撞在一堵棉花作面，钢铁为里的厚墙上一般，震退数步！\n",
        "$N刚碰到$p，突然身子一震，登时飞了出去！\n",
});

mixed valid_damage(object ob, object me, int damage, object weapon)
{
        mixed result;
        string msg;
        int ap, dp;
        int jiali;
        object weapon1;
        
        if (me->query("family/family_name") != "少林派")
                return;
                
        if ((int) me->query_skill("yijinjing", 1) < 200 ||
            ! living(me))
                return;                      

        if ((jiali = ob->query("jiali")) < 1)
                return;

        ap = ob->query_skill("force") + ob->query("neili");
        dp = me->query_skill("force") + me->query("neili");                        

        if (ap / 2 + random(ap) < dp)
        {
                if (! objectp(weapon1 = ob->query_temp("weapon")))
                {
                        result = ([ "damage" : -damage ]);
                        
                        msg = random(2) ? HIR "$n身上生出一股反震之力，" : HIR "$n身上金刚不坏体神功随心而起，";   
                              
                        msg += fanzhen_msg[random(sizeof(fanzhen_msg))] + NOR;             
                
                        ob->receive_damage("qi", damage, me);
                        ob->receive_wound("qi", damage / 2, me);                              
                        
                        result += ([ "msg" : msg ]);
                        
                        return result;
                } else
                {
                        result = ([ "damage" : -damage ]);

                        switch (random(5))
                        {
                        case 0:  
                        case 1:
                        case 2:
                        case 3:                                                
                                result += ([ "msg" : HIR "好似有一堵无形的墙在$n面前阻挡着，结果"
                                                     "$N" HIR "一下子被反弹，震得$N" 
                                                     HIR "手臂发麻！\n" NOR ]);
                                break;
                        default:
                                result += ([ "msg" : HIR "结果$n身上生出一股反震之力，$N只觉虎口一热，" + 
                                                     weapon1->name() + HIR "顿时脱手而出，飞出丈外！\n" NOR]);
                                weapon1->unequip();
                                weapon1->move(environment(ob));
                                ob->reset_action();
                                if (! ob->is_busy())
                                        ob->start_busy(1);
                                break;
                        }

                        return result;
                }
        }
}

mixed hit_ob(object me, object victim, int damage_bonus, int factor)
{
        int skill;
        int i = 1,j;    

        if (me->query("family/family_name") != "少林派")
                return;
                
        if (me->query_skill("yijinjing", 1) < 400 ||
            me->is_busy())
                return;
                
        skill = me->query_skill("force");

        i = damage_bonus * (skill + 1) / 1000;
        i = i/2+ random(i);
        if (me->query_temp("weapon"))
                i = i*3;
        if (me->query_temp("apply/damage")>=100)
                i = i/3;

        j = me->query_skill("yijinjing", 1) / 20;  
        message_combatd(HIR"$N"+HIR+"高喧一声佛号，将体内九阳真气聚于双臂，狭雷霆之势攻向$n！\n" NOR, me,victim);

        return i;
} 

string query_description()  
{
        return
        "易筋经乃少林寺头等绝技，由内及外，为无上伏魔神功。此内功是少林镇寺"
        "之宝，毅力一般者无法学成，其三阶段分别为：0-200，200-400，>400。初"
        "级易筋经可强身健体，提高修为；中级可降妖伏魔，成金刚之体；高级可砍"
        "妖除魔于无形，御力修身，百毒不侵。";
}

