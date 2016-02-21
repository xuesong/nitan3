// Room: /d/xiangyang/guofugate.c
// Date: Jan. 8 1999 by Winder

inherit ROOM;
#include <ansi.h>;
void create()
{
	set("short", "郭府大门");
	set("long", 
"一座建筑古朴的大宅院出现在你的眼前。只见门楼高挑，上书两个大字\n\n"
HIG"                     郭   府\n\n"NOR
"全真教第二代弟子耶律齐和几个家丁站在门前接待前来保卫襄阳的各方义士。\n"
 );
        set("outdoors", "xiangyang");
	set("exits", ([
		"south" : "/d/xiangyang/westjie1",
		"north" : "/d/xiangyang/guofuyuan",
	]));
	set("objects", ([
		"/d/xiangyang/npc/yelvqi" : 1,
		"/d/xiangyang/npc/jiading" : 2,
	]));
	set("coor/x", -300);
	set("coor/y", -80);
	set("coor/z", 0);
	setup();
	replace_program(ROOM);
}

