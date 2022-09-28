#include "server_manager.h"

int main()
{

    // GroupsManager* gm = create_groups_manager(200);
    // if(gm == NULL){printf("gm == null\n");}

    // char ip[20];
    // GroupsManager_return res = new_group(gm, "yakov", ip);
    // printf("6: %d\n", res);
    // printf("ip: %s\n", ip);

    // res = new_group(gm, "yakov", ip);
    // printf("1: %d\n", res);

    // res = new_group(gm, "yakov hananael", ip);
    // printf("6: %d\n", res);
    // printf("ip: %s\n", ip);

    // res = join_existing_group (gm, "yakov", ip);
    // printf("6: %d\n", res);
    // printf("224.255.255.1: %s\n", ip);

    // res = join_existing_group (gm, "yakov hananael", ip);
    // printf("6: %d\n", res);
    // printf("224.255.255.2: %s\n", ip);

    // res = join_existing_group (gm, "yakov  hananael", ip);
    // printf("2: %d\n", res);

    // res = leave_group (gm, "yakov");
    // printf("6: %d\n", res);

    // //queue_for_each(gm->m_ips, print_ips, NULL);

    // res = leave_group (gm, "yakov");
    // printf("7: %d\n", res);

    // //queue_for_each(gm->m_ips, print_ips, NULL);

    // res = leave_group (gm, "yakov");
    // printf("2: %d\n", res);

    // char list [500];
    // give_all_groups_names(gm, list);
    // printf("list:\n%s", list);


    // char groups_names_list[GRUPS_NAMES_LIST_SIZE] = {0};
    // give_all_groups_names(gm, groups_names_list);

    // if(num_of_groups(gm) < 1)
    //     printf("no groups\n");
    // else
    // {
    //     printf("size: %d\n", sizeof(groups_names_list));
    //    printf("list:\n%s", groups_names_list);
    // }

    //         destroy_groups_manager(gm);


    // SubscribsManager* sm = create_subscribs_manager(200);
    // if(sm == NULL){printf("sm == null\n");}

    // SubscribsManager_return res = new_subscriber(sm, "yakov", "302");
    // printf("0: %d\n", res);

    // res = log_in_chack(sm, "yakov", "302");
    // printf("0: %d\n", res);

    // res = log_in_chack(sm, "yakov", "302 ");
    // printf("10: %d\n", res);

    // res = log_in_chack(sm, "yakav", "302");
    // printf("3: %d\n", res);


    // res = new_subscriber(sm, "hananel", "0545949422");
    // printf("0: %d\n", res);

    // res = log_in_chack(sm, "hananel", "0545949422");
    // printf("0: %d\n", res);

    // res = log_in_chack(sm, "hananel ", "0545949422");
    // printf("3: %d\n", res);

    // res = log_in_chack(sm, "hananel", "045949422");
    // printf("10: %d\n", res);


    // res = new_subscriber(sm, "yakov hananel", "1234");
    // printf("0: %d\n", res);

    // res = log_in_chack(sm, "yakov hananel", "1234");
    // printf("0: %d\n", res);

    // res = log_in_chack(sm, "yakov  hananel", "1234");
    // printf("3: %d\n", res);

    // res = log_in_chack(sm, "yakov hananel", "054594089422");
    // printf("10: %d\n", res);


    // destroy_subscribs_manager(sm);


    // UserManager* um = create_users_manager(200);
    // if(um == NULL){printf("um == null\n");}


    // UserManager_return rese = user_log_in(um, "yakov hananel");
    // printf("0: %d\n", rese);

    //  rese = user_log_in(um, "yakov hananel");
    // printf("6: %d\n", rese);

    //     rese =  user_log_out(um, "yakov hananel");
    // printf("0: %d\n", rese);

    //      rese = user_log_in(um, "yakov hananel");
    // printf("0: %d\n", rese);

    //  rese = user_join_group(um, "yakov hananel", "yyy");
    //  printf("0: %d\n", rese);

    // rese = user_join_group(um, "yakov hananel", "yyy");
    //  printf("5: %d\n", rese);

    // rese = user_leave_group(um, "yakov hananel", "yyy");
    // printf("0: %d\n", rese);


    // rese = user_join_group(um, "yakov hananel", "yyy");
    //  printf("0: %d\n", rese);


    // rese = user_join_group(um, "yakov  hananel", "875");
    //  printf("3: %d\n", rese);

    // rese = user_join_group(um, "yakov hananel", "yyuuy");
    //  printf("0: %d\n", rese);


    // rese = user_leave_group(um, "yakov hananel", "yyy");
    // printf("0: %d\n", rese);

    // rese = user_leave_group(um, "yakov aananel", "yyy");
    // printf("3: %d\n", rese);

    // rese = user_leave_group(um, "yakov hananel", "yyy");
    // printf("4: %d\n", rese);

    // rese = user_leave_group(um, "yakov hananel", "yy98y");
    // printf("4: %d\n", rese);

    // rese = user_leave_group(um, "yakov hananel", "yyuuy");
    // printf("0: %d\n", rese);

    // rese =  user_log_out(um, "yakov hananel");
    // printf("0: %d\n", rese);

    // rese =  user_log_out(um, "yakov hananel");
    // printf("3: %d\n", rese);

    // destroy_users_manager(um);

    ServerManager* sm =  server_manager_create(250, 300, 1000);
    if(sm == NULL)
    {
        printf("server manager not initialized\n");
        return 0;
    }

    while(TRUE)
    {
        char c = getchar();
        if(c == 'q' || c == 27)
        {
            server_manager_destroy(sm);
            break;
        }
    }
}