// Room: /d/xiangyang/westjie3.c
// Date: Jan. 8 1999 by Winder

inherit ROOM;

void create()
{
	set("short", "西大街");
        set("long", @LONG
这是一条宽阔的青石街道，向东西两头延伸。西面是白虎内门，东边是一个
大十字街口，只见人来人往，络绎不绝。南边是大校场。北面是襄阳城的西兵营，
隐隐能听见里面传来的宋兵的操练声。
LONG );
        set("outdoors", "xiangyang");
	set("no_clean_up", 0);

	set("exits", ([
		"east"  : "/d/xiangyang/westjie2",
		"west"  : "/d/xiangyang/westgate1",
		"south" : "/d/xiangyang/dajiaochang",
		"north" : "/d/xiangyang/bingying4",
	]));
	set("coor/x", -330);
	set("coor/y", -80);
	set("coor/z", 0);
	setup();
	replace_program(ROOM);
}

