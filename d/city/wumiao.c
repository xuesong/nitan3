// wumiao.c
#include <room.h>
inherit ROOM;
int is_chat_room() { return 1; }

void create()
{
        set("short", "����");
        set("long", @LONG
����������������������������������Ϸ������������顺���Һ�ɽ��
�ĺ��ҡ����������Ƕ�š����ұ������ĸ����֡����˵��ˣ��������������Ϲ�
�Ͼ���������ݡ��Ա��ƺ���һ�����š�
LONG );
	set("no_fight", 1);
	set("no_steal", 1);
	set("no_sleep_room", 1);

        set("valid_startroom", 1);
	set("objects", ([
		__DIR__"obj/box" : 1,
	]));

	set("exits", ([
		"east" : __DIR__"beidajie2",
		"up"   : __DIR__"wumiao2",
                "northwest": "/d/wizard/guest_room",     
	]));
        create_door("northwest", "����", "southeast", DOOR_CLOSED);
	set("coor/x", -10);
	set("coor/y", 20);
	set("coor/z", 0);
	setup();

        ("/adm/npc/nanxian")->come_here();
}

int valid_leave(object me, string dir)
{
        if (dir == "northwest" && ! playerp(me) && ! me->is_chatter())
                return 0;
                
        return ::valid_leave(me, dir);
}