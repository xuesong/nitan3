inherit ROOM;

void create()
{
        set("short", "��԰");
        set("long", @LONG
����һƬ�ķ��˵Ļ�԰������������һƬС����������Щ
����֮�࣬���ڳ������Ҳ�ɺ��ˡ���������������ɽ��ɽ
�ϳ�������̦�������Ѿ��ܾ�û���������ˡ�
LONG);
        set("outdoors", "quanzhen");
        set("exits", ([
                "west" : __DIR__"houhuayuan",
        ]));
        set("objects", ([
                __DIR__"npc/wugui" : 1,
        ]));

        setup();
        replace_program(ROOM);
}