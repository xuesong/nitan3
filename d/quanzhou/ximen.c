// Room: /d/quanzhou/ximen.c
// Date: May 12,1998 Java

inherit ROOM;

void create()
{
	set("short", "泉州西门");
	set("long", @LONG
这里是南方第一港泉州的西门。西门外是一座吊桥。
LONG );
	set("exits", ([
                "east" : __DIR__"zhongxin",
                "west" : __DIR__"westbridge",
	]));
	set("no_clean_up", 0);
	set("outdoors", "quanzhou");
	set("coor/x", 140);
	set("coor/y", -180);
	set("coor/z", 0);
	setup();
	replace_program(ROOM);
}

