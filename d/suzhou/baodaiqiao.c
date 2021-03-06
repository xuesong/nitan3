// Room: /d/suzhou/baodaiqiao.c
// Date: May 31, 98  Java

inherit ROOM;

void create()
{
	set("short", "宝带桥");
        set("long", @LONG
宝带桥横跨于苏州的运河之上，又因桥形宛若长带，被称为长桥。桥面宽阔，
行人在桥上来往，桥边有城里的小贩吆喝、叫卖着，熙熙攘攘，热闹非凡。宝带
桥卧于碧波之上，相应生辉，分外多娇。这里是中秋赏月的一处绝佳之地。
LONG );
	set("outdoors", "suzhou");
	set("exits", ([
		"northeast" : "/d/suzhou/dongdajie1",
		"northwest" : "/d/suzhou/xidajie1",
		"north"     : "/d/suzhou/canlangting",
		"south"     : "/d/suzhou/nandajie1",
                "west"     : "/d/suzhou/yingbin-lou1.c",
	]));
	set("objects", ([
		"/d/suzhou/npc/seller": 1,
		// "/adm/npc/jiaoshi" : 1,
                "/clone/npc/walker"  : 1, 
	]));
	set("coor/x", 200);
	set("coor/y", -220);
	set("coor/z", 0);
	setup();
	replace_program(ROOM);
}

