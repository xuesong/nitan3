// qinglindong.c
// Date: Nov.1997 by Venus
#include <room.h>
inherit ROOM;
void create()
{
        set("short", "清林洞");
        set("long", @LONG
又叫老虎洞。只见靠右雕有弭陀，观音，大势至三尊小佛。洞口东侧壁上还
刻着一组『庐舍那佛会』浮雕，它上刻花头龛，中间坐着释迦牟尼，左右分列文
殊，普贤，还有天王佛像。西边是下山的路。小路往上延伸向东。
LONG );
        set("exits", ([
                "eastup"    : "/d/hangzhou/cuiweiting",
                "westdown"  : "/d/hangzhou/feilaifeng",
        ]));
        set("outdoors", "hangzhou");
        set("no_clean_up", 0);
        setup();
        replace_program(ROOM);
}
