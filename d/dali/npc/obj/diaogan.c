#include <weapon.h>
inherit WHIP;

void create()
{
	set_name("����", ({ "diao gan", "diao", "gan" }));
	set_weight(5000);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "��");
		set("long", "����һ��ϸ�������ˣ���Ȼһ�������Ǹ���ͨ�����õĵ��ˡ�\n");
		set("value", 1500);
		set("rigidity",100);
		set("material", "steel");
		set("wield_msg", "$N���һ��$n�������С�\n");
		set("unwield_msg", "$N�����е�$n������䡣\n");
	}
  	init_whip(30);
	setup();
}