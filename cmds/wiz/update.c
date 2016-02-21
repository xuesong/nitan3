// update.c
// updated by doing

#include <runtime_config.h>
#include <getconfig.h>
#include <ansi.h>
inherit F_DBASE;
inherit F_CLEAN_UP;

protected int update_player(object me);

void create()
{
        seteuid(getuid());
        set("name", "����ָ��");
        set("id", "update");            
        set("channel_id",HIY"���¾���");
}


int main(object me, string file)
{
	int i;
	object obj, *inv;
	string err;
        string bin_file;

        if (! SECURITY_D->valid_grant(me, "(wizard)"))
                return 0;

	seteuid(geteuid(me));

	if (! file) file = me->query("cwf");
	if (! file)
		return notify_fail("��Ҫ���±���ʲ�ᵵ����\n");

	if ((obj = present(file, environment(me))) && playerp(obj) ||
            (obj = find_player(file)) && playerp(obj))
		return update_player(obj);

	if (file == "me")
		return update_player(me);
	else
	{
		file = resolve_path(me->query("cwd"), file);
		if (! sscanf(file, "%*s.c")) file += ".c"; 
	}

	if (file_size(file) == -1)
		return notify_fail("û�����������\n");

	me->set("cwf", file);

	if (obj = find_object(file))
	{
		if (obj == environment(me))
		{
			if (file_name(obj) == VOID_OB)
				return notify_fail("�㲻���� VOID_OB �����±��� VOID_OB��\n");
			inv = all_inventory(obj);
			i = sizeof(inv);
			while (i--)
				if (playerp(inv[i]) || inv[i]->is_chatter() ||
                                    inv[i]->query_temp("owner"))
                                        // ��ҡ�������ǡ���������Ʒ������
					inv[i]->move(VOID_OB, 1);
				else    inv[i] = 0;
		}

                if (obj == find_object(VERSION_D) &&
                    VERSION_D->is_release_server())
                {
                        // ����ǰ汾�����ķ���������ɾ��
                        // VERSION_D �� bin ���룬��Ϊ��û��
                        // VERSION_D ��ʱ�� driver �ǲ������ġ�
			bin_file = "/binaries" + base_name(obj) + ".b";
			write("Remove: " + bin_file + "\n");
                        rm(bin_file);
                }
                seteuid(getuid());
		destruct(obj);
	}

	if (obj) return notify_fail("�޷�����ɳ�ʽ�롣\n");

        write("���±��� " + file + "��");
	err = catch(call_other(file, "???"));
	if (err)
		write(sprintf( "��������\n%s\n", err));
	else
	{
		write("�ɹ���\n");
		if ((i = sizeof(inv)) && (obj = find_object(file)))
			while (i--)
				if (inv[i]) inv[i]->move(obj, 1);
	}
		
	return 1;
}

protected int update_player(object me)
{
	object env, link_ob, obj;
        object *ob, *dob;
        mapping equip;
        int i;

	if (me->is_chatter())
		return notify_fail("�㲻�ܸ�������ID��\n");

        if (wiz_level(me) > wiz_level(this_player()))
                return notify_fail("��û��Ȩ�޸���������\n");

        seteuid(getuid());
	env = environment(me);

	// First, create the new body.
	link_ob = me->query_temp("link_ob");
        if (! link_ob)
        {
                link_ob = new(LOGIN_OB);
                link_ob->set("id", me->query("id"));
                if (! link_ob->restore())
                {
                        write("�Ҳ�������ҵĴ������ݡ�\n");
                        return 1;
                }
		if (interactive(me))
			link_ob->set_temp("ip_number", query_ip_number(me));
        }

        if (link_ob->query("id") != getuid(me))
        {
                write (sprintf("���Ӷ����ID(%s)���û���ID(%s)����ͬ������"
                       "���¸���ҡ�\n", link_ob->query("id"), getuid(me)));
                return 1;
        }

	obj = LOGIN_D->make_body(link_ob);
	if (! obj) return 0;

	// Save the data and exec the player to his/her link object.
        ob = all_inventory(me);
        equip = ([ ]);
        for (i = 0; i < sizeof(ob); i++)
                equip += ([ ob[i] : ob[i]->query("equipped") ]);
        ob->move(VOID_OB);

        // �����û��Ľ���
	me->save();

        // ������ߣ��ƶ���������
	if (interactive(me))
		exec(link_ob, me);
	destruct(me);

	// Restore new body and exec to it via enter_world in LOGIN_D
	obj->restore();
	LOGIN_D->enter_world(link_ob, obj, 1);
        dob = all_inventory(obj);
        for (i = 0; i < sizeof(dob); i++)
                destruct(dob[i]);

	write("���������ϡ�\n\n");
	obj->move(env);
        ob->move(obj);

	if (! interactive(obj))
		destruct(link_ob);

        for (i = 0; i < sizeof(ob); i++)
	{
                if (equip[ob[i]] == "worn")
                        ob[i]->wear();
                if (equip[ob[i]] == "wielded")
                        ob[i]->wield();
        }
	obj->write_prompt();

	return 1;
}

int help(object me)
{
  write(@HELP
ָ���ʽ : update <����|here|me|�����>
 
���ָ����Ը��µ���, �����µ������������������. ��Ŀ��Ϊ
'here' ��������ڻ���. ��Ŀ��Ϊ 'me' ������Լ�������. ��Ŀ
��Ϊ�����ɸ���������.
 
HELP
    );
    return 1;
}