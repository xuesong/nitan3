// logind.c
// updated by Lonely for mysql and cruise

#include <ansi.h>
#include <command.h>
#include <login.h>
#include <config.h>
#include <getconfig.h>
#include <mudlib.h>

#ifndef UNREG_IDLE
#define UNREG_IDLE      60
#endif
#define MUDLIST_CMD     "/cmds/usr/mudlist"
#define REBOOT_CMD      "/cmds/arch/reboot"
#define INVENTORY_CMD   "/cmds/usr/inventory"
#define MULTI_LOGIN     1
#define NO_FEE
#define NO_FEE_WARNING  "$$【注意】为保护会员利益，如果非会员在 " + UNREG_IDLE + " 天之内没有任何活动记录，人物将被系统自动清除。$$"
inherit F_DBASE;

#include "/adm/etc/database.h"

nosave int wiz_lock_level = 0;
nosave int cruise_lock_status = 0;

nosave mapping blocks = ([]);
nosave string *allows = ({});

string *banned_name = ({
        "你", "你", "我", "他", "她", "它",
        "爸", "爷", "　", "妈",
        "屎", "尿", "粪",
});

string *banned_id = ({
        "domain", "root", "mudlib", "quit",
        "arch", "wizard", "apprentice",
        "immortal", "player", "fuck", "doing", "admin",
        "all", "none", "cancel",
});

mapping day2num = ([
        "Mon"   : 1, "Tue"   : 2,
        "Wed"   : 3, "Thu"   : 4,
        "Fri"   : 5, "Sat"   : 6,
        "Sun"   : 7
]);

// 内部调用的函数
protected void get_id(string arg, object ob);
protected void get_version(string arg, object ob);
protected void confirm_cruiseid(string yn, object ob);
protected void get_cruisepwd(string pass, object ob);
protected void get_passwd(string pass, object ob);
protected void get_ad_passwd(string pass, object ob);
protected void check_ok(object ob);
protected void confirm_id(string yn, object ob);
private   void init_new_player(object user);

protected void client_connect(object);
protected void client_check(string, object);

// 可以被外部调用的函数
object    make_body(object ob);
object    find_body(string name);
varargs   void enter_world(object ob, object user, int silent);
varargs   void reconnect(object ob, object user, int silent);
int       check_legal_id(string arg);
int       check_legal_name(string arg, int maxlen);
int       blocks_ip(string ip, int time, string reason);
int       remove_blocks(string ip);
int       remove_allows(string ip);
int       player_day, player_week, day;

void create() 
{
        seteuid(ROOT_UID);
        set("channel_id", "连线精灵");
        if (intp(WIZ_LOCK_LEVEL))
                wiz_lock_level = WIZ_LOCK_LEVEL;
        else
                wiz_lock_level = 0;
}

void logon(object ob)
{
        //object *usr, *login_usr;
        object *usr;
        int i, ip_num, wiz_cnt, ppl_cnt, login_cnt;
        int iplimit;
       // int user_num;
        int now_time;
        //string msg;
       // mapping temp_time;
        string *counter = ({});
        string ipname;

        if (uptime() < 30)
        {
                write(sprintf("%s正在启动过程中，请稍候再来。\n", LOCAL_MUD_NAME()));
                destruct(ob);
                return;
        }
        
        now_time = time() & 0xFFFFFFFE;
        if (BAN_D->is_banned(query_ip_number(ob)) == 1)
        {
                write("你的地址在本 MUD 不受欢迎。\n");
                destruct(ob);
                return;
        }

/*
        if (mapp(temp_time = blocks[query_ip_number(ob)]))
        {
                if (now_time - temp_time["time"] < temp_time["punish"])
                {
                        write(temp_time["reason"] + "请过" + 
                              appromix_time(temp_time["time"] + temp_time["punish"] - now_time) + "再尝试连线。\n");
                        destruct(ob);
                        return;
                }
                remove_blocks(query_ip_number(ob));
        }
        
        login_usr = filter_array(children(LOGIN_OB), (: query_ip_number($1) == $2 :), query_ip_number(ob));

        if (sizeof(login_usr) > 20)
        {
                blocks_ip(query_ip_number(ob), 1800, "您的IP产生了大量废弃连接！");

                for (i = 0; i < sizeof(login_usr); i ++)
                                destruct(login_usr[i]);

                return;
        }
*/
        
        write(ESC "[2J");
        write(color_filter(read_file(WELCOME))); 

        catch(MUDLIST_CMD->main());
        UPTIME_CMD->main();

        if (ob->query_temp("tomud"))
                write(ALERT("经典文字MUD游戏『nt3』，群号『44135826』，本游戏为免费游戏！$$" +
                        "请到 http://www.mudbuilder.com 浏览本游戏主页和论坛、新闻。$$"+
                        "如果有任何疑问，请到本游戏论坛专区提问，或联系QQ 150867 咨询。$$"));

        if (! VERSION_D->is_version_ok() && ! VERSION_D->query_temp("operator"))
                write(HIY "现在本站正在同步版本，如果你不是巫师，请稍候再登录。\n\n" NOR);
        else
        if (REBOOT_CMD->is_rebooting())
                write(HIY "现在本站正在准备重新启动，如果你不是巫师，请稍候再登录。\n\n" NOR);

        usr = users() + filter(children(LOGIN_OB), (: interactive :));
        wiz_cnt = 0;
        ppl_cnt = 0;
        login_cnt = 0;
        iplimit = 0;       
        for (i = 0; i < sizeof(usr); i++)
        {
                ipname = query_ip_number(usr[i]);
                if (member_array(ipname, counter) == -1)
                        counter += ({ ipname });
                if (ipname == query_ip_number(ob)) 
                        iplimit++;
                if (base_name(usr[i]) == LOGIN_OB)
                        login_cnt++;
                else if (wizardp(usr[i]))
                {
                        if (! usr[i]->query("env/invisible")) wiz_cnt++;
                }
                else ppl_cnt++;
        }

        ip_num = atoi(read_file("/adm/etc/ips", 1));
        if (sizeof(counter) > ip_num)
                write_file("/adm/etc/ips", sprintf("%d", sizeof(counter)), 1);
        
#ifndef MULTI_LOGIN     
        if ((iplimit > 12 || (iplimit > 4 && ! BAN_D->is_netclub(query_ip_number(ob)))) && query_ip_number(ob) != "124.42.121.251")
        {
                write("对不起，" + LOCAL_MUD_NAME() + "限制相同ip多重登录。\n");
                destruct(ob);
                return;
        }
#endif
        if (file_size("/adm/etc/players") > 0)
        {
                sscanf(read_file("/adm/etc/players", 1, 1), "%d %d %d",
                       player_week, player_day, day);
                        
                ip_num = atoi(read_file("/adm/etc/ips", 1));
                
                write(sprintf ("自开站以来最高在线IP数" CYN " %d " NOR 
                        "个，今日上线 " CYN "%d" NOR " 人次，本周上线 " CYN "%d" NOR " 人次。\n",
                        ip_num, player_day, player_week));
        }
  
        write(sprintf("目前共有 " CYN "%d" NOR " 位巫师、" CYN "%d" NOR " 位玩家"
                      "在线上，以及 "CYN "%d" NOR " 位使"
                      "用者尝试连线中。\n", wiz_cnt, ppl_cnt, login_cnt));
               
        if (ob->query_temp("tomud"))
        {
                write(TMI("version"));
                input_to("get_version", ob);
        }
        if (! ob->query_temp("big5"))
                write("Select 国标码 GB or き絏 BIG5 (gb/big5)：\n");
        else
                write("您的英文名字(新玩家可以选择一喜欢的名字)：\n");
                
        input_to("get_id", ob);
}

protected void get_version(string arg, object ob)
{
        // CURRENT_CLIENT_VERSION
/*
        if (! arg || arg[0..3] != "2060")
        {
                write(HIY "请从下列地址下载Tomud客户端\n" NOR);
                write(HIG "    http://www.mudbuilder.com/down/tomudsetup.exe\n" NOR);
                write(HIY "或使用Zmud客户端选择2000端口重新连接。\n\n" NOR);
                destruct(ob);
                return;
        }
*/
        input_to("get_id", ob);
}

protected void get_id(string arg, object ob)
{
        object ppl, *usr;
        //string where;
       // string *res;  
        int    i, flag;
                      
        arg = lower_case(arg);

        if (arg == "big5")
        {
                ob->set_temp("big5", 1);
                logon(ob);
                return;
        }
        
        if (! check_legal_id(arg))
        {
                ob->add_temp("fake_id", 1);
                if (ob->query_temp("fake_id") > 20)
                {
                        blocks_ip(query_ip_number(ob), 300, "您的IP持续登录无效ID！");
                        destruct(ob);
                        return;
                }
                
                write("您的英文名字(新玩家可以选择一喜欢的名字)：\n");
                input_to("get_id", ob);
                return;
        }

        // 检查最大连接人数
        if (intp(MAX_USERS) && MAX_USERS > 0)
        {
                if ((string)SECURITY_D->get_status(arg) == "(player)" &&
                    sizeof(users()) >= MAX_USERS)
                {
                        ppl = find_body(arg);
                        // Only allow reconnect an interactive player when MAX_USERS exceeded.
                        if (! ppl)
                        {
                                write("对不起，" + LOCAL_MUD_NAME() +
                                      "的使用者已经太多了，请待会再来。\n");
                                destruct(ob);
                                return;
                        }
                }
        }

        if (wiz_level(arg) < 1)
        {
                if (! VERSION_D->is_version_ok() && ! VERSION_D->query_temp("operator"))
                {
                        write("现在本站正在同步版本中，暂时不能登录，请稍候再尝试。\n");
                        destruct(ob);
                        return;
                } else
                if (REBOOT_CMD->is_rebooting() && ! find_player(arg))
                {
                        write("现在本站正准备重新启动，暂时不能登录，请稍候再尝试。\n");
                        destruct(ob);
                        return;
                }
        }

        if (wiz_level(arg) < wiz_lock_level)
        {
                write(sprintf("对不起，%s目前限制巫师等级 %d 以"
                              "上的人才能连线。\n",
                              LOCAL_MUD_NAME(), wiz_lock_level));
                destruct(ob);
                return;
        }
        
        if (AHACK_D->query_block(arg))
        {
                write(HIY "这个账号被暂时封锁了，请用 guest 人物登入与在线巫师联系。\n" NOR);
                destruct(ob);
                return;
        }
        
        if ((string)ob->set("id", arg) != arg)
        {
                write("Failed setting user name.\n");
                destruct(ob);
                return;
        }

        if (arg == "guest")
        {
                // If guest, let them create the character.
                confirm_id("Yes", ob);
                return;
        } 
        
#ifdef DB_SAVE
        res = DATABASE_D->do_sql("select online, on_time, fee_time,
                                  last_station from users where id = '" + arg + "'");
        flag = arrayp(res);
        if (flag)
        {
                if ((int)res[0] == 3)
                {
                        write(sprintf("[%s] 这个 ID 目前正在漫游中，请稍后再尝试登陆。。\n", arg));
                        destruct(ob);
                        return;
                }

                where = res[3];
                if (wiz_level(arg) < 1 && stringp(where) && where != LOCAL_STATION)
                {
                        if (res[0] > 0)
                        {
                                if ((int)res[0] == 2)
                                        write(sprintf("[%s] 这个 ID 目前正在%s闭关，如想漫游，请先将该 ID 从%s站退出。\n",
                                                      arg, ! undefinedp(stations[where]) ? stations[where] : "其他站",
                                                      ! undefinedp(stations[where]) ? stations[where] : "其他站"));  
                                else
                                if ((int)res[0] == 1)
                                        write(sprintf("[%s] 这个 ID 目前正在%s连线，如想漫游，请先将该 ID 从%s站退出。\n",
                                                      arg, ! undefinedp(stations[where]) ? stations[where] : "其他站",
                                                      ! undefinedp(stations[where]) ? stations[where] : "其他站"));   
                                
                                destruct(ob);
                                return;
                        }
                        write(sprintf("[%s] 这个 ID 数据目前在%s，你想将该 ID 数据漫游过来(y/n)？：\n",
                                      arg, ! undefinedp(stations[where]) ? stations[where] : "其他站"));
                        write("如果该 ID 不是你注册的或者你不想漫游，请输入(n/N)来重新注册或登陆其他 ID。\n" NOR);
                                input_to("confirm_cruiseid", 1, ob);
                        return;
                }                       
                ob->set_temp("res", res);
        }
#else
        flag = (file_size(ob->query_save_file() + __SAVE_EXTENSION__) >= 0);  
#endif              
        if (flag)
        {
                if (ob->restore())
                {
                        if ((string)SECURITY_D->get_status(ob->query("id")) != "(player)") 
                        {
                                if (ob->query("wizpwd")) 
                                {
                                        write("请输入巫师专用密码：\n");
                                        input_to("get_wizpwd", 1, ob);
                                        return;
                                }
                                
                                write("\n警告！您没有设置巫师专用密码！请登陆后用" HIW " wizpwd " NOR "命令设置。\n");
                                write("请输入密码：\n");
                                input_to("get_passwd", 1, ob);
                                return;
                        }                       
                        write("这个角色已经存在，请输入该角色的密码：\n");
                        input_to("get_passwd", 1, ob);
                        return;
                }
                write("您的人物储存挡出了一些问题，请利用 guest 人物通知巫师处理。\n");
                destruct(ob);
                return;
        }
        
#ifdef DB_SAVE 
        if (! DATABASE_D->query_db_status())
        {
                write("对不起，由于连接不上数据库所在服务器，目前" + MUD_NAME + "暂时不接受数据漫游或新玩家注册。\n"
                      "请访问游戏论坛：http://www.mudbuilder.com 获得详细的讯息，或重新尝试连接。\n");
                destruct(ob);
                return;
        } 
#endif

#ifdef REFUSE_NEW_PLAYER
        if (! VERSION_D->is_release_server())
        {
                write("对不起，目前" + MUD_NAME + "暂时不接受新玩家注册。\n请登陆总站(NITAN.NET 2000)注册。\n");
                destruct(ob);
                return;
        }
#endif

        // 防止两个以上的玩家用同一帐号
        usr = filter_array(children(LOGIN_OB), (: interactive :));
        i = sizeof(usr);
        while (i--)
                if (usr[i] != ob && usr[i]->query("id") == arg)
                        break;
        if (i >= 0)
        {
                write("有其他玩家正在使用这个 ID，请重新输入。\n");
                write("您的英文名字：\n");
                input_to("get_id", ob);
                return;
        }
        
        if (ob->query_temp("tomud"))
                write(ALERT(NO_FEE_WARNING)+"使用 " + (string)ob->query("id") + 
                        " 这个名字将会创造一个新的人物，您确定吗(y/n)？");
        else
                write("使用 " + (string)ob->query("id") + " 这个名字将会创造一个新的人物，您确定吗(y/n)？\n");               
        input_to("confirm_id", ob);
}

protected void get_wizpwd(string pass, object ob)
{
        string old_pass;
        
        write("\n");
        old_pass = ob->query("wizpwd");

        if (crypt(pass, old_pass) == old_pass) 
        {
                write(HIG "身份确认完成，巫师专用密码验证通过！\n" NOR);
                write("请输入密码：\n");
                input_to("get_passwd", 1, ob);
                return;
        } else 
        { 
                write("密码错误！\n");
                destruct(ob);
                return;
        }
}

protected void confirm_cruiseid(string yn, object ob)
{
        //int t;
        
        write("\n");
        if (upper_case(yn) == "Y")  
        {
                if (cruise_lock_status)
                {
                        write("对不起，目前该站已被巫师临时设置为禁止漫游，如想要漫游，请和巫师联系。\n");
                        destruct(ob);
                        return;
                }

                write("请输入该 ID 管理密码：\n");
                input_to("get_cruisepwd", 1, ob);
                return;  
        }    
        write("你选择重新注册或登陆 ID，请重新输入。\n");         
        write("您的英文名字：\n");
        input_to("get_id", ob);
        return;
}

protected void get_cruisepwd(string pass, object ob)
{
	write("\n"); 
#ifdef DB_SAVE    
        string sites;
        string passwd;
        passwd = DATABASE_D->db_query_user(ob->query("id"), "ad_password");
        if (! stringp(passwd) || crypt(pass, passwd)[0..15] != passwd)
        {
                write("密码错误！\n");
                destruct(ob);
                return;
        }
                                  
        write("人物身份确认完成，开始连接站点服务器获取人物的数据信息。\n");
        sites = DATABASE_D->db_query_user(ob->query("id"), "last_station");
        CRUISE_D->authenticate_user(ob, sites);

        destruct(ob);
        return;
#else
        write("密码错误！\n");
        destruct(ob);
        return;
#endif
}

protected void get_passwd(string pass, object ob)
{
        string ad_pass;
        string my_pass;

        my_pass = ob->query("password");
        ad_pass = ob->query("ad_password");
        if (! stringp(my_pass) || crypt(pass, my_pass) != my_pass)
        {
                if (! stringp(ad_pass) || crypt(pass, ad_pass) != ad_pass)
                {
                        write("密码错误！\n");
                        destruct(ob);
                        return;
                }

                write(HIR "\n你采用了管理密码进入游戏，"
                      "因此请你先修改你的普通密码。\n" NOR);

                // 做标记：表示目前流程是修改密码分支。
                ob->set_temp("reset_password", 1);
                write("\n请重新设定您的普通密码：\n");
                input_to("new_password", 1, ob);
                return;
        }

        if (! stringp(ad_pass))
        {
                write(HIC "\n请注意：你的ID目前还没有管理密码，请设置你的管理密码。\n\n" NOR);
                write(HIW "在你普通密码丢失的情况下，你可以输入管理密码进入，并修改普通\n"
                      "密码，所以请你设置一个可靠的并且与普通密码不同的管理密码，用\n"
                      "来保护你的ID。平时请使用普通密码登陆，避免过于频繁的使用管理\n"
                      "密码以导致潜在的泄漏风险。\n\n" NOR);
                write("请输入你的管理密码：\n");
                input_to("reset_ad_password", 1, ob);
                return;
        }

        check_ok(ob);
}

protected void reset_ad_password(string pass, object ob)
{
        string my_pass;
        write("\n");

        if (strlen(pass) < 5)
        {
                write("管理密码的长度至少要五个字元，请重设您的管理密码：\n");
                input_to("reset_ad_password", 1, ob);
                return;
        }

        my_pass = ob->query("password");
        if (stringp(my_pass) && crypt(pass, my_pass) == my_pass)
        {
                write(HIR "为了安全起见，系统要求管理密码的密码和普通密码不能相同。\n\n" NOR);
                write("请重新输入你的管理密码：\n");
                input_to("reset_ad_password", 1, ob);
                return;
        }

        ob->set_temp("ad_password", crypt(pass, 0));
        write("请再输入一次您的管理密码，以确认您没记错：\n");
        input_to("confirm_reset_ad_password", 1, ob);
}

protected void confirm_reset_ad_password(string pass, object ob)
{
        //mapping my;
        string old_pass;
        //string passwd;
        
        write("\n");
        old_pass = ob->query_temp("ad_password");
        if (crypt(pass, old_pass) != old_pass)
        {
                write("您两次输入的管理密码并不一样，请重新设定一次管理密码：\n");
                input_to("reset_ad_password", 1, ob);
                return;
        }

        ob->set("ad_password", old_pass);
        check_ok(ob);
}

protected void check_ok(object ob)
{
        object user;
        //string *res, str, str1, str2;
        //int    on, fee, i, rec = 0;
        int rec=0;
        
        // Check if we are already playing.
        user = find_body(ob->query("id"));
        if (! objectp(user)) 
                user = make_body(ob);
        else
                rec = 1;

        if (! objectp(user)) 
        {
                write(HIR "无法创建该玩家，你可以尝试重新登录或是和巫师联系。\n" NOR);
                return ;
        }

        if (rec != 1 && ! user->restore()) // 必须要判断rec的值，否则会把玩家数据返回到前一次存储的数据
        {
                write(HIR "无法读取该玩家数据，你可以尝试重新登录或是和巫师联系。\n" + rec + NOR);
                return ;
        }

#ifdef NO_FEE
        if (ob->query_temp("tomud"))
                write(ALERT("文字MUD游戏『nt2』将不定期限免费开放！请新老玩家相互转告！\n"));
#else
        string *res, str, str1, str2;
        int    on, fee, i;
        // 如果是月费用户，则不需要再判断点数了。
        if (user->query("monfee") < time() && arrayp(res = ob->query_temp("res")))
        {
                str1 = res[1] + "0"; 
                str2 = res[2] + "0";
                if (sscanf(str1, "%d", on) == 1 && sscanf(str2, "%d", fee) == 1)
                {
                        on /= 10;
                        fee /= 10;
                        i = onl / 3600;
                        str = "你总共购买游戏时间： " + fee + " 个小时$$" +
                              "你已经在游戏中进行了：" + i + "小时" + ((on % 3600) / 60) + "分钟$$";
                        if (i >= fee)
                                str += "你购买的游戏时间已经使用完了，请联系『nt2』小组继续购买游戏时间！$$游戏主页： http://www.mudbuilder.com $$";
                        else if ((fee - i) < 18)
                                str+="你购买的游戏时间已经只有 " + (fee - i - 1) + " 小时多一点了$$请赶快去 http://www.mudbuilder.com 给你的帐号续费吧$$";
                        else if (fee == i)
                                str += "你购买的时间已经不到一个小时了，请尽快去 http://www.mudbuilder.com 给你的帐号续费$$";
                        if (fee == 18)
                                str += NO_FEE_WARNING;
                        write(ALERT(str) + ((fee > 18) ? TMI("regur good") : "") + TMI("dlurl 123.12586.com/images/face32/"));
                        if (i >= fee)
                        {
                                destruct(ob);
                                destruct(user);
                                return;
                        }
                        // write(ALERT("a"));
                }
                // else
                //         write(ALERT("b"));
        }
        // else
        //      write(ALERT("c") + "FLAG : " + user->query("monfee") + " " + time() + " " + arrayp(res = ob->query_temp("res")));

        // Check if we are already playing.
        // user = find_body(ob->query("id"));
        // if (user)
        // {
#endif
                        
        if (rec)
        {
                if (user->is_net_dead())
                {
                        reconnect(ob, user);
                        return;
                }
                write("您要将另一个连线中的相同人物赶出去，取而代之吗？(y/n)\n");
                input_to("confirm_relogin", ob, user);
                return;
        }
        /*
        user = MESSAGE_D->find_chatter(ob->query("id"));
        if (objectp(user))
        {
                write("你把正在聊天的ID踢了出去。\n");
                MESSAGE_D->user_logout(user, user->name(1) + "从" +
                                       query_ip_number(ob) + "连线进入世界，"
                                       "把你踢了出去。\n");
        }

        if (objectp(user = make_body(ob)))
        {
                if (user->restore())
                */
                else
                {
                        mixed err;
                        string msg;

                        log_file("usage", sprintf("%s(%s) loggined from %s (%s)\n",
                                 user->query("name"), user->query("id"),
                                 query_ip_number(ob), ctime(time()) ) );

                        if (ob->query("last_on") < time() &&
                            ob->query("last_on") > time() - 30 &&
                            ! wiz_level(user))
                        {
                                write("你距上一次退出时间只有" + chinese_number(time() - ob->query("last_on")) +
                                      "秒钟，请稍候再登录。\n");
                                destruct(user);
                                destruct(ob);
                                return;
                        }

                        user->set_temp("logon_time", time());
                        user->set("last_save", time());
                        if (err = catch(enter_world(ob, user)))
                        {
                                user->set_temp("error", err);
                                msg = HIR "\n你无法进入这个世界，可能你的档"
                                      "案出了一些问题，需要和巫师联系。\n\n" NOR;
                                if (mapp(err))
                                        msg += MASTER_OB->standard_trace(err, 1);
                                user->set_temp("error_message", msg);
                                tell_object(user, msg);
                        }
                        return;
                } 
                /*
                else
                {
                        destruct(user);
                        write(HIR "\n无法读取你的数据档案，您需要和巫师联系。\n" NOR);
                        if (CONFIG_D->query_int("ask_recreate"))
                        {
                                write(WHT "你可以选择重新创造玩家(y/n)：\n" NOR);
                                input_to("create_new_player", ob);
                        } else
                                destruct(ob);
                }
        } else
                write(HIR "无法创建该玩家，你可以尝试重新登录或是和巫师联系。\n" NOR);
        */
}

protected void create_new_player(string yn, object ob)
{
        if (yn != "y" && yn != "Y")
        {
                write("\n那好吧，本次连接登录失败了。\n");
                destruct(ob);
                return;
        }

        write(HIW "\n现在开始重新创造 " HIY + ob->query("id") +
              HIW " 这个玩家。\n" NOR);
        confirm_id(yn, ob);
}

protected void confirm_relogin(string yn, object ob, object user)
{
        object old_link;

        if (! yn || yn=="")
        {
                write("您要将另一个连线中的相同人物赶出去，取而代之吗？(y/n)\n");
                input_to("confirm_relogin", ob, user);
                return;
        }       

        if (yn[0]!='y' && yn[0]!='Y')
        {
                write("好吧，欢迎下次再来。\n");
                destruct(ob);
                return;
        }

        if (user)
        {
                tell_object(user, "有人从别处( " + query_ip_number(ob)
                                + " )连线取代你所控制的人物。\n");
                log_file("usage", sprintf("%s(%s) replaced by %s (%s)\n",
                         user->query("name"), user->query("id"),
                         query_ip_number(ob), ctime(time())));

                // Kick out tho old player.
                old_link = user->query_temp("link_ob");
                if (old_link)
                {
                        exec(old_link, user);
                        destruct(old_link);
                }
        } else
        {
                write("在线玩家断开了连接，你需要重新登陆。\n");
                destruct(ob);
                return;
        }

        reconnect(ob, user);
}

protected void confirm_id(string yn, object ob)
{
        if (yn == "")
        {
                write("\n使用这个名字将会创造一个新的人物，您确定吗(y/n)？\n");
                input_to("confirm_id", ob);
                return;
        }       

        if (yn[0] != 'y' && yn[0] != 'Y')
        {
                write("\n好吧，那么请重新输入您的英文名字：\n");
                input_to("get_id", ob);
                return;
        }
        write(@TEXT

请输入您的高姓大名，由于这个名字代表你的人物，而且以后不能更改，
务必慎重择名（不雅观的姓名将被删除）。另：请不要选择金庸小说中
已有人物姓名。

首先输入的是你的姓，比如你想扮演的角色叫张三的话，先输入“张”，
然后电脑会询问你的名字，那时你再输入“三”。如果你想扮演的角色
的名字比较怪，比如叫“老虎”，可以不输入姓，直接敲回车略过。但
是建议你还是输入一个比较像样子的姓名，而这些比较怪的称号可以用
昵称代替，否则某些地方的称呼可能会比较怪。如果你要加入世家，那
么你就不必输入姓了，因为一旦加入世家将自动选择家传祖姓，不是你
自己决定的。

TEXT
        );
        write("您的中文" HIG "姓氏" NOR "(不要超过两个汉字)：\n");
        input_to("get_surname", ob);
}

protected void get_surname(string arg, object ob)
{
        if (arg && strlen(arg) > 0)
        {
                if (ob->query_temp("big5")) 
#ifdef LONELY_IMPROVED
                        // arg = big5togb(arg);
                        arg = LANGUAGE_D->toGB(arg);
#else                   
                        arg = LANGUAGE_D->toGB(arg);
#endif                          
                if (! check_legal_name(arg, 4))
                {
                        write("您的中文" HIG "姓氏" NOR "(不要超过两个汉字)：\n");
                        input_to("get_surname", ob);
                        return;
                }
                ob->set("surname", arg);
        } else
        {
                write("您的中文" HIG "姓氏" NOR "(不可以没有姓氏)：\n");
                input_to("get_surname", ob);
                return;
        }
        write("您的中文" HIY "名字" NOR "(不要超过两个汉字)：\n");
        input_to("get_name", ob);
}

protected void get_name(string arg, object ob)
{
        string fname;
        string result;

        if (ob->query_temp("big5")) 
#ifdef LONELY_IMPROVED
                // arg = big5togb(arg);
                arg = LANGUAGE_D->toGB(arg);
#else
                arg = LANGUAGE_D->toGB(arg);
#endif                
        if (! check_legal_name(arg, 4))
        {
                write("您的中文" HIY "名字" NOR "(不要超过两个汉字)：\n");
                input_to("get_name", ob);
                return;
        }
        ob->set("purename", arg);

        fname = ob->query("surname");
        if (! stringp(fname)) fname = "";
        fname += arg;

        if (strlen(fname) < 4)
        {
                write("对不起，你的中文名字（姓和名的组合）至少要有两个汉字。\n");
                write("\n请重新输入您中文" HIG "姓氏" NOR "：\n");
                input_to("get_surname", ob);
                return;
        }

        if (stringp(result = NAME_D->invalid_new_name(fname)) ||
            stringp(result = NAME_D->invalid_new_name(arg)))
        {
                write("对不起，" + result + "\n");
                write("\n请重新输入您中文" HIG "姓氏" NOR "：\n");
                input_to("get_surname", ob);
                return;
        }
        
#ifdef DB_SAVE
        if (DATABASE_D->db_find_user("name", fname) ||
            DATABASE_D->db_find_user("name", arg))
        {
                write("对不起，你的姓名和其他站玩家的姓名重复或类似！\n");
                write("\n请重新输入您中文" HIG "姓氏" NOR "：\n");
                input_to("get_surname", ob);
                return;
        }        
#endif       
                
        if (arg == ob->query("surname"))
        {
                write(HIW "\n系统发现你输入的姓和名字相同，这样你的名字将是：“"
                      HIY + arg + arg + HIW "”。\n"
                      "你是否是因为不了解本系统而且没有仔细阅读前面的帮助而错误的输入\n"
                      "了你的姓名，如果是，请你重新连接并输入你的名字，否则请你输入一\n"
                      "遍你的全名，表明你的确想使用这个名字。\n\n" NOR);
                write("请输入您的全名(即姓和名字的组合)：\n");
                input_to("input_full_name", ob, arg + arg);
                return;
        }

        write(HIW "\n为了保证你的人物的安全，游戏要求你设置两个密码。一个是管理密\n"
              "码，这个密码可以在你遗失了普通密码时登录游戏，并用来修改普通\n"
              "密码。平时请使用普通密码登陆，避免过于频繁的使用管理密码以导\n"
              "致潜在的泄漏风险。\n\n" NOR);
        write("请设定您的管理密码：\n");
        input_to("new_ad_password", 1, ob);
}

protected void input_full_name(string arg, object ob, string fname)
{
        if (arg != fname)
        {
                write(WHT "\n你输入的全名并不是你姓和名字的的组合，系统认为你开始没有理解输\n"
                      "入名字的要求，" HIR "请你仔细阅读所有提示的条款，不要自作聪明"
                      NOR WHT "。包括在\n"
                      "今后的游戏过程中请仔细阅读各种帮助和公告，避免因为自以为是而导\n"
                      "致浪费时间或是遭受不必要的损失。\n" NOR);
                confirm_id("Yes", ob);
                return;
        }

        write("\n请设定您的管理密码：\n");
        input_to("new_ad_password", 1, ob);
}

protected void new_ad_password(string pass, object ob)
{
        write("\n");
        if (strlen(pass) < 5)
        {
                write("管理密码的长度至少要五个字元，请重设您的管理密码：\n");
                input_to("new_ad_password", 1, ob);
                return;
        }
        
#ifdef ENABLE_MD5_PASSWORD 
        ob->set_temp("ad_password", crypt(pass, sprintf("$1$%d", random(99999999)))); 
#else 
        ob->set_temp("ad_password", crypt(pass, 0));
#endif 

        write("请再输入一次您的管理密码，以确认您没记错：\n");
        input_to("confirm_ad_password", 1, ob);
}

protected void confirm_ad_password(string pass, object ob)
{
        //mapping my;
        string old_pass;
        //string passwd;

        write("\n");
        old_pass = ob->query_temp("ad_password");
        if (crypt(pass, old_pass) != old_pass)
        {
                write("您两次输入的管理密码并不一样，请重新设定一次管理密码：\n");
                input_to("new_ad_password", 1, ob);
                return;
        }

        ob->set("ad_password", old_pass);

        write(HIW "普通密码是你平时登录游戏时使用的，游戏中可以通过 PASSWD 命令\n"
              "来修改这个密码。\n\n" NOR);
        write("请输入你的普通密码：\n");
        input_to("new_password", 1, ob);
}

protected void new_password(string pass, object ob)
{
        string ad_pass;

        write("\n");
        if (strlen(pass) < 3)
        {
                write("密码的长度至少要三个字元，请重设您的密码：\n");
                input_to("new_password", 1, ob);
                return;
        }

        if (stringp(ad_pass = ob->query("ad_password")) &&
            crypt(pass, ad_pass) == ad_pass)
        {
                write(HIR "请注意，为了安全起见，系统要求你的管理密码和普通密码不能相同。\n\n" NOR);
                write("请重新设置您的普通密码：\n");
                input_to("new_password", 1, ob);
                return;
        }
        
#ifdef ENABLE_MD5_PASSWORD 
        ob->set_temp("password", crypt(pass, sprintf("$1$%d", random(99999999)))); 
#else 
        ob->set_temp("password", crypt(pass, 0));
#endif 

        write("请再输入一次您的密码，以确认您没记错：\n");
        input_to("confirm_password", 1, ob);
}

protected void confirm_password(string pass, object ob)
{
       // mapping my;
        string old_pass;
        //string passwd;

        write("\n");
        old_pass = ob->query_temp("password");
        if (crypt(pass, old_pass) != old_pass)
        {
                write("您两次输入的密码并不一样，请重新设定一次密码：\n");
                input_to("new_password", 1, ob);
                return;
        }

        ob->set("password", old_pass);
        if (ob->query_temp("reset_password"))
        {
                // 这是用户修改自己的密码分支流程，不是创建
                // 用户分支，因此转移到 check_ok 函数继续执
                // 行。
                ob->save();
                check_ok(ob);
                return;
        }

        write("您要扮演男性(m)的角色或女性(f)的角色？\n");
        input_to("get_gender", ob);
}

protected void get_gender(string gender, object ob)
{
        object user;

        write("\n");
        if (gender == "")
        {
                write("您要扮演男性(m)的角色或女性(f)的角色？\n");
                input_to("get_gender", ob, user);
                return;
        }

        if (gender[0]=='m' || gender[0]=='M')
                ob->set_temp("gender", "男性");
        else if (gender[0]=='f' || gender[0]=='F')
                ob->set_temp("gender", "女性" );
        else
        {
                write("对不起，您只能选择男性(m)或女性(f)的角色：\n");
                input_to("get_gender", ob, user);
                return;
        }

        if (find_body(ob->query("id")))
        {
                write(HIR "这个玩家现在已经登录到这个世界上了，请"
                      "退出重新连接。\n" NOR);
                destruct(ob);
                return;
        }
        
#ifdef DB_SAVE
        if (! DATABASE_D->query_db_status())
                DATABASE_D->connect_to_database();   

        if (DATABASE_D->db_find_user("id", ob->query("id")))   
        {
                write(HIR "\n这个玩家现在已经在其他站注册了，请退出重新连接。\n" NOR);
                destruct(ob);
                return;
        }
#endif        
        ob->set("body", USER_OB);
        if (! objectp(user = make_body(ob)))
        {
                write(HIR "\n你无法登录这个新的人物，请重新选择。\n" NOR);
                destruct(ob);
                return;
        }
        user->set("str", 14);
        user->set("dex", 14);
        user->set("con", 14);
        user->set("int", 14);
        user->set("per", 20);
        user->set("kar", 20);
        user->set("gender", ob->query_temp("gender"));
        ob->set("registered", 0);
        user->set("registered", 0);
        log_file("usage", sprintf("%s(%s) was created from %s (%s)\n",
                          user->query("name"), user->query("id"),
                          query_ip_number(ob), ctime(time())));

        init_new_player(user);
        user->set_temp("newplayer", 1);
        enter_world(ob, user);
        write("\n");
}

object make_body(object ob)
{
        string err;
        object user;
        //int n;

        if (! is_root(previous_object()))
        {
                log_file("static/security",
                         sprintf("%s try to create player(%s) on %s.\n",
                                 (string)geteuid(previous_object()),
                                 (string)ob->query("id"),
                                 ctime(time())));
                write("你没有权限创建玩家。\n");
                return 0;
        }

        seteuid(getuid());
        user = new(USER_OB);
        if (! user)
        {
                write("现在可能有人正在修改使用者物件的程式，无法进行复制。\n");
                write(err+"\n");
                return 0;
        }
        seteuid(ob->query("id"));
        export_uid(user);
        export_uid(ob);
        seteuid(getuid());
        user->set("id", ob->query("id"));
        user->set("surname", ob->query("surname"));
        user->set("purename", ob->query("purename"));
        user->set_name(0, ({ ob->query("id")}));
        return user;
}

private void init_new_player(object user)
{
        // 初始化必要属性
        user->set("title", "普通百姓");
        user->set("birthday", time());
        user->set("combat_exp", "100");
        user->set("potential", 99);
        user->set("food", (user->query("str") + 10) * 10);
        user->set("water", (user->query("str") + 10) * 10);
        user->set("channels", ({ "chat", "rumor", "party",
                                 "bill", "sos", "family",
                                 "nt", "rultra" }));
        user->set("chann", (["sing" : 1, "chat" : 1, "bill" : 1 ])); // 默认显示在主窗口的频道

        // 记录名字
        NAME_D->map_name(user->query("name"), user->query("id"));

        // 设置月度会员
        user->set("monfee", time() + 8640000);
        // 设置必要的环境参数
        // user->set("env/prompt", "time");
        user->set("env/wimpy", 60);
        user->set("env/roomlist", 1);        
}
                
varargs void enter_world(object ob, object user, int silent)
{
        //object cloth, shoe, room, login_ob;
        object cloth,shoe,login_ob;
        string startroom;
        string ipname;
        mapping marry;
        object *users;

        if (! is_root(previous_object()))
                return;

        user->set_temp("link_ob", ob);
        ob->set_temp("body_ob", user);
        ob->set("registered", user->query("registered"));
        user->set_temp("time", time());
        user->set_temp("logon_time", time());
        user->set("env/show_reply", "all");
        if (ob->query_temp("big5")) 
                user->set_temp("big5", 1);
        if (ob->query_temp("tomud")) 
                user->set_temp("tomud", 1);
        if (interactive(ob)) exec(user, ob);

        write("\n目前权限：" + wizhood(user) + "\n");

        user->setup();
        if (user->query("age") == 14)
        {
                user->set("food", user->max_food_capacity());
                user->set("water", user->max_water_capacity());
        }

        // In case of new player, we save them here right aftre setup 
        // compeleted.
        // 在这里创建一个数据库记录
#ifdef DB_SAVE
        if (user->query_temp("newplayer"))
        {
                if (! DATABASE_D->query_db_status())
                        DATABASE_D->connect_to_database();
                        
                DATABASE_D->db_new_player(ob, user);
        }
#endif
        user->save();
        user->set("last_save", time());
        ob->save();

        // check the user
        UPDATE_D->check_user(user);

        if (wizhood(user) == "(player)")
        {
                if (user->query("class") == "bonze")
                {
                        if (user->query("gender") == "女性")
                        {
                                cloth = new("/clone/cloth/ni-cloth.c");
                                shoe = new("/clone/cloth/ni-xie.c");
                        }
                        else
                        {
                                cloth = new("/clone/cloth/seng-cloth.c");
                                shoe = new("/clone/cloth/seng-xie.c");
                        }
                } else
                {
                        if (user->query("class") == "taoist")
                        {
                                if (user->query("gender") == "女性")
                                {
                                        cloth = new("/clone/cloth/daogu-cloth.c");
                                        shoe = new("/clone/cloth/dao-xie.c");
                                } else
                                {
                                        cloth = new("/clone/cloth/dao-cloth.c");
                                        shoe = new("/clone/cloth/dao-xie.c");
                                }
                        } else
                        {
                                if (user->query("gender") == "女性")
                                {
                                        shoe = new("/clone/cloth/female-shoe.c");
                                        cloth = new(sprintf("/clone/cloth/female%d-cloth.c", 1 + random(8)));
                                } else
                                {
                                        shoe = new("/clone/cloth/male-shoe.c");
                                        cloth = new(sprintf("/clone/cloth/male%d-cloth.c", 1 + random(8)));
                                }
                        }
                }

                cloth->move(user);
                catch(cloth->wear());
                shoe->move(user);
                catch(shoe->wear());
        } else
        {
                if (user->query("gender") == "女性")
                {
                        cloth = new("/clone/cloth/yunshang");
                        cloth->move(user);
                        cloth->wear();
                } else
                {
                        cloth = new("/clone/cloth/jinduan");
                        shoe  = new("/clone/cloth/xianlv");
                        cloth->move(user);
                        cloth->wear();
                        shoe->move(user);
                        shoe->wear();
                }
        }

        if (cloth && (! environment(cloth) || ! cloth->query("equipped")))
                destruct(cloth);

        if (shoe && (! environment(shoe) || ! shoe->query("equipped")))
                destruct(shoe);

        if (! silent)
        {
                if (! user->query("registered"))
                        write(color_filter(read_file(UNREG_MOTD))); 
                else
                        write(color_filter(read_file(MOTD))); 

                write("你连线进入" + LOCAL_MUD_NAME() + "。\n\n");

                if (! user->query("registered"))
                {
                        if (user->is_ghost())
                                user->reincarnate();
                        user->set("startroom", REGISTER_ROOM);
                } else
                if (! stringp(user->query("born")))
                {
                        if (user->is_ghost())
                                user->reincarnate();
                        user->set("startroom", BORN_ROOM);
                }
                
                if ((string)SECURITY_D->get_status(ob->query("id")) != "(player)") 
                        startroom = WIZARD_ROOM;
                
                else        
                if (user->is_in_prison())
                        startroom = user->query_prison();
                else
                if (user->is_ghost())
                        startroom = DEATH_ROOM;
                else
                if (! stringp(startroom = user->query("startroom")) ||
                    file_size(startroom + ".c") < 0)
                        startroom = START_ROOM;

                if (! catch(load_object(startroom)))
                        user->move(startroom);
                else
                {
                        user->move(START_ROOM);
                        startroom = START_ROOM;
                        user->set("startroom", START_ROOM);
                }
                tell_room(startroom, user->query("name") +
                          "连线进入这个世界。\n", ({user}));
        }

        login_ob = new(LOGIN_OB);
        login_ob->set("id", user->query("id"));
        login_ob->restore();
        if (login_ob->query("registered"))
        {
                if (! intp(login_ob->query("login_times")))
                {
                        write(NOR "\n你是第一次光临" + LOCAL_MUD_NAME() + "。\n");
                        login_ob->set("login_times", 1);
                        // show rules
                } else
                {
                        login_ob->add("login_times", 1);
                        write("\n你上次光临" + LOCAL_MUD_NAME() + "是 " + HIG +
                                ctime(login_ob->query("last_on")) + NOR + " 从 " + HIR +
                                login_ob->query("last_from") + NOR + " 连接的。\n");
                }
        }

        destruct(login_ob);

        if (!(wizardp(user)&&user->query("env/no_login_msg")))
        CHANNEL_D->do_channel(this_object(), "sys",
                sprintf("%s(%s)由%s连线进入。",
                user->name(), user->query("id"),
                (ipname = query_ip_number(user)) ? ipname : "未知地点"));

#ifdef DB_SAVE
        DATABASE_D->db_set_player(user->query("id"), "online", 1);
        DATABASE_D->db_set_player(user->query("id"), "last_station", LOCAL_STATION);
#endif
        
#if INSTALL_EXAMINE
        EXAMINE_D->examine_player(user);
#endif

        if (day ==6 && day2num[ctime(time())[0..2]] == 7)
                player_week = 0;
        if (day != day2num[ctime(time())[0..2]]) 
        {
                player_day = 0;
                day = day2num[ctime(time())[0..2]];
        }
        write_file("/adm/etc/players", sprintf( "%d %d %d", player_week + 1, player_day + 1, day), 1);
          
        if (wizhood(user) != "(admin)" &&
            EXAMINE_D->query("log_by/" + user->query("id")))
                user->start_log();

        if (BAN_D->is_welcome(user->query("id")) && ! BAN_D->is_netclub(query_ip_number(user)))
                BAN_D->open_club_line(user);    // 自动开放网吧限制

        // notice user the user of this mud
        NEWS_D->prompt_user(user);
        ANALECTA_D->prompt_user(user);
        TOPTEN_D->topten_checkplayer(user);
        if (mapp(marry = user->query("couple")) && ! undefinedp(marry["couple_id"]))
        {
                string tmpstr, tmpstr1;
                object couple_ob = find_player(marry["couple_id"]);
                if (couple_ob)
                {
                        if ((string)user->query("gender") != "男性")
                        {
                                tmpstr1 = "夫君"; 
                                tmpstr = "爱妻";
                        }
                        else
                        {
                                tmpstr1 = "爱妻";
                                tmpstr = "夫君";
                        }
                        write(sprintf(WHT HIM "\n你的%s%s已经在这等了你好久了，快去找%s吧。。。\n" NOR,
                                      tmpstr1, couple_ob->name(), gender_pronoun(couple_ob->query("gender"))));
                        tell_object(couple_ob, sprintf(WHT HIM "\n你的%s%s抑制不住对你的思念跑来看你啦，快去和%s打个招呼吧。。。\n" NOR,
                                                       tmpstr,user->name(),gender_pronoun(user->query("gender"))));
                }
        }

        if ((user->query("qi") < 0 || user->query("jing") < 0) && living(user))
                user->unconcious();

        users = filter_array(users(), (: $1->query_temp("tomud") :));
        message("system", ADD2(user), users);
        if (user->query_temp("tomud"))
        {
                foreach (object player in users())
                        tell_object(user, ADD2(player));
                user->force_me("inventory");
                user->force_me("wear all");
                user->force_me("cmdbar");
        }
}

varargs void reconnect(object ob, object user, int silent)
{
        user->set_temp("link_ob", ob);
        ob->set_temp("body_ob", user);
        
        if (ob->query_temp("big5"))
                user->set_temp("big5", 1);
        else    
                user->delete_temp("big5");
                
        if (ob->query_temp("tomud"))
                user->set_temp("tomud", 1);
        else    
                user->delete_temp("tomud");

        exec(user, ob);

        user->reconnect();
        if (! silent && (! wizardp(user) || ! user->query("env/invisible"))) {
                tell_room(environment(user), user->query("name") + "重新连线回到这个世界。\n",
                ({user}));
        }

        if (user->query_temp("tomud"))
        {
                user->force_me("look");
                user->force_me("inventory");
                user->force_me("cmdbar");
        }
#ifdef DB_SAVE
        DATABASE_D->db_set_player(user->query("id"), "online", 1);
#endif
        
        if (! (wizardp(user) && user->query("env/no_login_msg"))) 
        CHANNEL_D->do_channel( this_object(), "sys",
                sprintf("%s(%s)由%s重新连线进入。", user->query("name"),
                        user->query("id"), query_ip_number(user)));
}

int check_legal_id(string id)
{
        int i;

        if (member_array(id, banned_id) != -1)
        {
                write("对不起，" HIC"“" + id + "”" NOR"这个词有着特"
                      "殊的含意，不能用做英文名字。\n");
                return 0;
        }

        i = strlen(id);

        if ((strlen(id) < 3) || (strlen(id) > 10))
        {
                write("英文名字必须是 3 到 10 个英文字母,请注意格式。\n");
                return 0;
        }

        while (i--)
        {
                if (id[i] < 'a' || id[i] > 'z' )
                {
                        write("对不起，你的英文名字只能用英文字母。\n");
                        return 0;
                }
        }

        return 1;
}

int check_legal_name(string name, int maxlen)
{
        int i;

        i = strlen(name);
        
        if ((strlen(name) < 2) || (strlen(name) > maxlen))
        {
                write("对不起，你的中文姓名不能太长或太短。\n");
                return 0;
        }

        if (! is_chinese(name))
        {
                write("对不起，请您用「中文」取名字。\n");
                return 0;
        }

        if (member_array(name, banned_name) % 2 == 0)
        {
                write("对不起，这种姓名会造成其他人的困扰。\n");
                return 0;
        }

        return 1;
}

object find_body(string name)
{
        return find_player(name);
}

int set_wizlock(int level)
{
        if (wiz_level(this_player(1)) <= level)
                return 0;

        if (geteuid(previous_object()) != ROOT_UID)
                return 0;

        wiz_lock_level = level;
        return 1;
}

int set_cruiselock(int level)
{
        if (wiz_level(this_player(1)) <= 4)
                return 0;

        cruise_lock_status = 1;
        return 1;
}

int can_login(int level)
{
        if (level < wiz_lock_level)
                return 0;

        return 1;
}

int blocks_ip(string ip, int time, string reason)
{
        mapping temp_ban = ([]);

        temp_ban["reason"] = reason;
        temp_ban["time"] = time();
        temp_ban["punish"] = time;
        map_delete(blocks, ip);
        blocks[ip] = temp_ban;

        return 1;
}

int remove_blocks(string ip)
{
        if (ip == "all")
                blocks = ([]);
        else
                map_delete(blocks, ip);

        return 1;
}

mapping query_blocks()
{
        return blocks;
}

int allows_id(string id)
{
        allows += ({ id });
        return 1;
}

int remove_allows(string id)
{
        if (id == "all")
                allows = ({});
        else
                allows -= ({ id });

        return 1;
}
