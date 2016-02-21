// jingcisi.c
// Date: Nov.1997 by Venus
#include <room.h>
inherit ROOM;
void create()
{
        set("short", "净慈寺");
        set("long", @LONG
净慈寺，原名慧日永明院，又叫兴教寺。净慈寺面向西湖，後拥苍然挺秀的
南屏山。往北就回到大道上。
LONG );
        set("exits", ([
            "north"   : "/d/hangzhou/road16",
            "enter"   : "/d/hangzhou/jingci",
        ]));
        set("no_clean_up", 0);
        setup();
        replace_program(ROOM);
}
