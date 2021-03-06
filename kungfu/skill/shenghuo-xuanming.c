// Code of JHSH
// shenghuo-xuanming.c	明教  圣火玄冥功
// Zhangchi 2/2000

inherit FORCE;

#include <ansi.h>

int valid_enable(string usage)
{
	return usage == "force";
}

int valid_force(string force)
{
        return force == "shenghuo-xinfa";
}

int query_neili_improve(object me)
{
    int lvl;

    lvl = (int)me->query_skill("shenghuo-xuanming", 1);
    return lvl * lvl * 16 * 15 / 100 / 200;
}

int valid_learn(object me)
{
	mapping skill;
	string *skname;
	int i, n = 0;

	skill = this_player()->query_skills();
	skname = sort_array(keys(skill), (: strcmp :) );

	if ((int)me->query_skill("force", 1) < 10 )
		return notify_fail("你的基本内功火候不足，不能学圣火玄冥功。\n");

	if (me->query("gender") == "无性")
		return notify_fail("你无根无性，阴阳不调，难"
                                   "以领会高深的圣火玄冥。\n");

	return ::valid_learn(me);
}

int practice_skill(object me)
{
    return notify_fail("圣火玄冥只能用学的，或是从运用(exert)中增加熟练度。\n");
}

string exert_function_file(string func)
{
	return __DIR__"shenghuo-xuanming/" + func;
}
mapping curing_msg(object me)
{
        return ([
"apply_short" :me->name()+"神情严肃，闭目合掌，周身燃起一团火焰。",
"start_self" :HIR"你盘膝而坐，双手十指张开，举在胸前，作火焰飞腾之状，运起圣火玄冥心法疗伤。\n"NOR,
"on_self" :HIM"你口中默念经文，一股暖流不断在周天游走，伤势慢慢愈合。\n"NOR,
"start_other" :HIM""+me->name()+"口中叽叽咕咕地念叨经文，周身火焰越来越旺。\n"NOR,
"finish_self" :HIR"你感到暖流游遍周天，全身有种说不出的舒服，缓缓站了起来。\n"NOR,
"finish_other" :me->name()+"脸色泛红，周身的火焰渐渐消退，缓缓站了起来。\n",
"unfinish_self" :"你正在以圣火玄冥心法疗伤，忽然觉得内力不济，体内暖流停滞不前，只好作罢，暂缓疗伤。\n",
"unfinish_other":me->name()+"脸色苍白，缓缓站起，周身火焰便即熄灭。\n",
"halt_self" :"你深深呼上几口气，催动内力强行将体内暖流收回。\n",
"halt_other" :me->name() +"深深吸了几口气，站了起来，周身火焰缓缓熄灭。\n",
        ]);
} 
