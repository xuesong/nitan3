//Room: road1.c
//Date: Feb.14 1998 by Java

inherit ROOM;
void create()
{
	set("short","青石大道");
        set("long", @LONG
这是一条通往成都的大青石道，两旁绿树荫蔽，空气十分清新。成都平原向
称天府之国，这里四周田野稻浪滚滚，平平展展的大地上，偶尔会出现一大丛翠
竹浓荫，细细认去，都是人家居处。
LONG );
	set("outdoors", "emei");
	set("exits",([ /* sizeof() == 1 */
	    "east"       : "/d/emei/qsjie1",
	    "northwest"  : "/d/city3/road2",
	]));
	set("no_clean_up", 0);
	set("coor/x", -370);
	set("coor/y", -190);
	set("coor/z", 0);
	setup();
	replace_program(ROOM);
}

