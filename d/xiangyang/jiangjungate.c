// Room: /d/xiangyang/jiangjungate.c
// Date: Jan. 8 1999 by Winder

inherit ROOM;

void create()
{
	set("short", "将军府大门");
        set("long", @LONG
这是将军府的大门，门板不知是以什么材料制成，看上去黝黑一片，似乎极
为沉重，上面有两个擦得晶亮的铜环；在大门的正上面有个夹着一个高大的木牌
坊，上书「将军府」三个大字，字迹娟秀飘逸，只是有些模糊了，想必是岁月留
下的痕迹。门内有一株老柏，生得苍劲挺拔，枝叶茂密。门前东西两边各肃立着
两个腰配大刀的宋兵。
LONG );
        set("outdoors", "xiangyang");

	set("exits", ([
		"south" : "/d/xiangyang/eastjie1",
		"north" : "/d/xiangyang/jiangjunyuan",
	]));
	set("objects", ([
		"/d/xiangyang/npc/bing" : 2,
	]));
	set("coor/x", -280);
	set("coor/y", -80);
	set("coor/z", 0);
	setup();
}

int valid_leave(object me, string dir)
{
        if (dir == "north" &&
                objectp(present("song bing", environment(me))))
           return notify_fail("宋兵向你喝道：什么人胆敢擅闯守备府衙门！\n");

        return ::valid_leave(me, dir);
}
