// Room: /d/suzhou/tielingguan.c
// Date: May 31, 98  Java

inherit ROOM;

void create()
{
	set("short", "铁岭关");
        set("long", @LONG
关呈长方形，砖石结构，正中的三层敌楼为新建。铁岭关地处水陆要冲，登敌
楼，既可了望，又能放炮射箭。这关垣、运河、石桥所组成的联合工事，当时是
为了防御合抵抗侵扰的倭寇。
LONG );
	set("outdoors", "suzhou");
//	set("no_clean_up", 0);
	set("exits", ([
		"south" : "/d/suzhou/fengqiao",
	]));
	set("objects", ([
		"/d/city/npc/wujiang" : 1,
		"/d/city/npc/bing" : 3,
	]));
	set("coor/x", 140);
	set("coor/y", -190);
	set("coor/z", 0);
	setup();
	replace_program(ROOM);
}

