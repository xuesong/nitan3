//liuyi

inherit NPC;

#include <ansi.h>
#include "bajiao.h"

void create()
{
	set_name(HIG "����" NOR, ({ "lu yi" }) );
	set("title", CYN "Ⱥ��˽�" NOR);
	set("gender", "Ů��" );
	set("age", 22);
	set("str", 30);
	set("per", 40);
	set("long", @LONG
һ��ȫ�����´��ŷ�װ����¶���ص���Ů��
LONG);
	set("combat_exp", 10);
	set("attitude", "friendly");

	setup();
	carry_object("/d/changan/npc/obj/skirt")->wear();
}