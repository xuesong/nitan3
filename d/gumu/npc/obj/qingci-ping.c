// qingci-ping.c 青瓷瓶，用来装玉蜂蜜

#include <ansi.h>

inherit ITEM;

inherit F_LIQUID;

#define RAN 60	// 浆有效果的概率

void create()
{
	set_name(CYN "青瓷瓶" NOR, ({"qingci ping", "ping", "bottle" }));
	set_weight(100);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("long", "古墓派用来装玉蜂蜜的青瓷小瓶。\n");
		set("unit", "个");
		set("base_unit", "个");
		set("max_liquid", 10);
		set("no_give",0);
	}

	set("liquid", ([
		"type": "mi",
		"name": "玉蜂蜜",
		"remaining": 10,
		"drunk_apply": 1,
	]));

}

void init()
{
	add_action("do_drink", "he");
	add_action("do_drink", "drink");
	add_action("do_fill", "fill");
	add_action("do_fill", "zhuang");
	add_action("do_add", "add");
}


int do_drink(string arg)
{
	object pl = this_player();
	object ping = this_object();
	object *inv;
	string item;
	int num,i,j;

	if(!arg) return 0;
	if( !living(pl) ) return 0;
	if( pl->is_busy() )
		return notify_fail("你上一个动作还没有完成。\n");
	if( (int)pl->query("water") >= (int)pl->max_water_capacity() )
		return notify_fail("你已经喝太多了，再也灌不下一滴水了。\n");

	if (sscanf(arg, "%s %d", item, num)==2){
		if( !id(item) ) return 0;
		if(num){
			inv = all_inventory(pl);
			j = 0;
			for(i=0; i<sizeof(inv); i++){
				if( inv[i]->query("name") == CYN"青瓷瓶"NOR ){
					j += 1;
					if( j == num ) ping = inv[i];
				}
			}
		}
	}
	else if( !id(arg) ) return 0;
	
	if( !ping->query("liquid/remaining") )
		return notify_fail( name() + (ping->query("liquid/name") ? "已经被喝得一滴也不剩了。\n":"是空的。\n"));

	ping->add("liquid/remaining", -1);
	message_vision("$N拿起" + name() + "呷了一小口" + ping->query("liquid/name") + "。\n",pl);
	pl->add("water", 5);
	pl->add("jingli", 5);

	if( ping->query("liquid/name") == "玉蜂蜜" ) {
		pl->add("jing",30);
		if ((int)pl->query_condition("yufengdu")) {
		    pl->apply_condition("yufengdu", (int)pl->query_condition("yufengdu") - 5);
			tell_object(pl, HIW "玉蜂毒的奇痒减轻了一些！\n" NOR );
		}
	}
	if( ping->query("liquid/name") == "玉蜂蜜水" ) {
		pl->add("jingli",25);
		if ((int)pl->query_condition("yufengdu")) {
		    pl->apply_condition("yufengdu", (int)pl->query_condition("yufengdu") - 2);
			tell_object(pl, HIW "玉蜂毒的奇痒只减轻了一点点！\n" NOR );
		}
	}
	if( ping->query("liquid/name") == "玉蜂王浆" ) {
		if (pl->query_skill("qufeng",1) > 60
			&& pl->query_skill("yunv-xinfa",1) > 30
			&& pl->query("family/family_name") == "古墓派"){
			tell_object(pl, HIW "你潜运功力，任王浆阴柔之气畅行于经脉。\n" NOR );
			if ( random(100) < RAN ){
			        if ((int)pl->query("max_jingli") < (int)pl->query_current_jingli_limit())
                                {
					pl->add("max_jingli", 1);
					pl->add("eff_jingli", 1);
				}	
			}
			else if ( pl->query("jingli") < pl->query("max_jingli") )
				pl->set("jingli",pl->query("max_jingli"));
			if ((int)pl->query_condition("yufengdu")>=20) 
				pl->apply_condition("yufengdu", 20);
			else
				pl->apply_condition("yufengdu", 
					(int)pl->query_condition("yufengdu") + 2);
		}
		else pl->apply_condition("yufengdu", 
			(int)pl->query_condition("yufengdu") + 20);
	}
	if( ping->query("liquid/name") == "古墓玉浆" ) {
		if (pl->query_skill("qufeng",1) > 120
			&& pl->query_skill("yunu-xinjing",1) > 90
			&& pl->query("family/family_name") == "古墓派"){
			tell_object(pl, HIW "你潜运玉女心经，借玉浆阴柔之气冲击经脉，似乎有些作用。\n" NOR );
			if ( random(100) < RAN ){
			        if ((int)pl->query("max_neili") < (int)pl->query_current_neili_limit())
					pl->add("max_neili", 1);
			}
			else if ( pl->query("neili") < pl->query("max_neili") )
				pl->set("neili",pl->query("max_neili"));
			if ((int)pl->query_condition("yufengdu")>=30) 
				pl->apply_condition("yufengdu", 30);
			else
			    pl->apply_condition("yufengdu", 
					(int)pl->query_condition("yufengdu") + 3);
		}
		else pl->apply_condition("yufengdu", 
			(int)pl->query_condition("yufengdu") + 30);
	}
	if( ping->query("liquid/name") == "古墓圣浆" ) {
		if ( pl->query_skill("qufeng",1) > 150
			&& pl->query_skill("yunu-xinjing",1) > 120
			&& pl->query("family/family_name") == "古墓派"){
			tell_object(pl, HIW "你潜运玉女心经，借圣浆阴柔之气畅行经脉，通体舒泰！\n" NOR );
			if ( random(100) < RAN ){
			        if ((int)pl->query("max_neili") < (int)pl->query_current_neili_limit())
					pl->add("max_neili", 1);
			}
			else 
			if ((int)pl->query("max_jingli") < (int)pl->query_current_jingli_limit()){
				pl->add("max_jingli", 1);
				pl->add("eff_jingli", 1);
			}
			if ((int)pl->query_condition("yufengdu")>=30) 
				pl->apply_condition("yufengdu", 30);
			else
			    pl->apply_condition("yufengdu", 
					(int)pl->query_condition("yufengdu") + 3);
		}
		else pl->apply_condition("yufengdu", 
			(int)pl->query_condition("yufengdu") + 30);
	}

//	if( pl->query("jingli") > pl->query("max_jingli")) 
//	this_player()->set("jingli", this_player()->query("max_jingli"));

	if( pl->is_fighting() ) pl->start_busy(2);
	if( !ping->query("liquid/remaining") ) {
		write("你已经将" + name() + "里的" + ping->query("liquid/name")
			+ "喝得一滴也不剩了。\n");
		if ( ping->query("worthless") )  set("value", 0);
	}

	// This allows customization of drinking effect.
	if( ping->query("liquid/drink_func") ) return 1;

	switch(ping->query("liquid/type")) {
		case "alcohol":
			pl->apply_condition("drunk",
				(int)pl->query_condition("drunk") 
				+ (int)ping->query("liquid/drunk_apply"));
			break;
	}
	
	return 1;
}

int do_fill(string arg)
{
	object pl = this_player();
	object ping = this_object();
	object *inv;
	string item;
	int num,i,j;

	if(!arg) return 0;
	if( !living(pl) ) return 0;
	if( pl->is_busy() )
		return notify_fail("你上一个动作还没有完成。\n");
	if( !environment(pl)->query("resource/water") )
		return notify_fail("这里没有地方可以装水。\n");

	if (sscanf(arg, "%s %d", item, num)==2){
		if( !id(item) ) return 0;
		if(num){
			inv = all_inventory(pl);
			j = 0;
			for(i=0; i<sizeof(inv); i++){
				if( inv[i]->query("name") == CYN"青瓷瓶"NOR ){
					j += 1;
					if( j == num ) ping = inv[i];
				}
			}
		}
	}
	else if( !id(arg) ) return 0;

	if ( ping->query("liquid/remaining")==ping->query("max_liquid") 
		&& ping->query("liquid/name")=="玉蜂蜜" ){
		message_vision("$N将水缓缓注入" + ping->name() + "里，慢慢摇匀。\n", pl);
		ping->set("liquid/type", "mi-water");
		ping->set("liquid/name", "玉蜂蜜水");
		ping->set("liquid/remaining", query("max_liquid"));
		ping->set("liquid/drink_func", 0);
	}
	else {
		if( ping->query("liquid/remaining") ) 
			message_vision("$N将" + name() + "里剩下的" + query("liquid/name") + "倒掉。\n", pl);
		message_vision("$N将" + name() + "装满清水。\n", pl);

		ping->set("liquid/type", "water");
		ping->set("liquid/name", "清水");
		ping->set("liquid/remaining", query("max_liquid")); //*query_amount());
		ping->set("liquid/drink_func", 0);
	}

	if( pl->is_fighting() ) pl->start_busy(2);

	return 1;
}

int do_add(string arg)
{
	object me = this_player();
	int i,max,remaining;

	if( !wizardp(me) ) return 0;
	if(!arg) return 0;

	sscanf(arg, "%d", i);
	add("liquid/remaining", i);
	remaining = query("liquid/remaining");
	max = query("max_liquid");	//*query_amount();
	tell_object(me, "满瓶"+chinese_number(max)+"，现在"	+ chinese_number(remaining)+"。\n");
	
	return 1;
}
