#include <stdio.h>

typedef struct
{
    unsigned int seed;
    int brand;
    int ability;
} seed_struct;

char ability_order[14][70] = {
    "��ʡ",
    "��ʡ",
    "ī��",
    "����",
    "����",
    "���м���",
    "sp��������",
    "������ǿ",
    "���ٸ���",
    "������",
    "��ǿ",
    "��ȫЬ",
    "����",
    "�ж�ǿ��",
};
char brand_name[20][70] = {
    "ս������",
    "�����ȷ�",
    "����",
    "�޿ϱ���",
    "���",
    "��Ʒ",
    "ů��",
    "����",
    "����",
    "ʱ��",
    "������",
    "Ͼ��",
    "�޷�����",
    "�ϻ�",
    "������",
    "ɢ��˾",
    "����",
    "�������̻�",
    "�������",
    "amiibo",
};
int brand_ability[20][2] = {
    {11, 0},
    {9, 8},
    {4, 12},
    {3, 4},
    {6, 5},
    {7, 1},
    {1, 2},
    {8, 6},
    {0, 3},
    {12, 13},
    {2, 9},
    {5, 7},
    {1, 6},
    {10, 11},
    {0, 10},
    {13, 10},
    {13, 5},
    {-1, -1},
    {-1, -1},
    {-1, -1},
};

int printfb(unsigned int a)
{
    int i = 0;
    while (1)
    {
        printf("%d", a % 2);
        i++;
        a = a / 2;
        if (a == 0)
            break;
    }
    printf("\n");
    printf("%d\n", i);
}

// ������һ��������
unsigned int seed_advance(unsigned int seed)
{
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

//���㵱ǰƷ�����Ӷ�Ӧ�ļ��� 28 = 2*14 35 = 12 * 2 + 1 + 10
int ability_solve(unsigned int seed, int brand)
{
    int i;
    int seed_;
    if (brand > 16 || brand < 0)
    {
        seed_ = seed % 28;
        for (i = 0; i < 14; i++)
        {
            seed_ = seed_ - 2;
            if (seed_ < 0)
                return i;
        }
    }
    seed_ = seed % 35;
    for (i = 0; i < 14; i++)
    {
        if (i == brand_ability[brand][0])
            seed_ = seed_ - 10;
        else if (i == brand_ability[brand][1])
            seed_ = seed_ - 1;
        else
            seed_ = seed_ - 2;
        if (seed_ < 0)
            return i;
    }
}

//���������ϵ�����£���������û���е�����µļ���
int ability_solve_dd(seed_struct seed, int drink)
{
    int n, seed_, i;
    if (brand_ability[seed.brand][0] == -1)
    {
        n = 26;
    }
    else if (brand_ability[seed.brand][0] == drink)
    {
        n = 25;
    }
    else if (brand_ability[seed.brand][1] == drink)
    {
        n = 34;
    }
    else
        n = 33;
    seed_ = seed.seed % n;
    for (i = 0; i < 14; i++)
    {
        if (i == drink)
            continue;
        if (i == brand_ability[seed.brand][0])
            seed_ = seed_ - 10;
        else if (i == brand_ability[seed.brand][1])
            seed_ = seed_ - 1;
        else
            seed_ = seed_ - 2;
        if (seed_ < 0)
            return i;
    }
}

//���������ϵ�����µ����Ӽ���
seed_struct ability_solve_drink(seed_struct seed, int drink)
{
    int ability, seed_;
    seed.seed = seed_advance(seed.seed);
    if (drink == -1 || drink == 14)
    {
        seed.ability = ability_solve(seed.seed, seed.brand);
        return seed;
    }
    seed_ = seed.seed % 100;
    if (seed_ < 30)
    {
        seed.ability = drink;
        return seed;
    }
    seed.seed = seed_advance(seed.seed);
    seed.ability = ability_solve_dd(seed, drink);
    return seed;
}

//������һλ�����Ƿ������������
int search_seed_one(seed_struct seed, int target, int drink)
{
    int i = drink;
    seed_struct seed_next;
    while (1)
    {
        seed_next = ability_solve_drink(seed, i);
        if (seed_next.ability == target)
            return i;
        i = (i + 1) % 15;
        if (i == drink)
        {
            return -2;
        }
    }
}
//�����е�����������Ѱ��Ŀ�꼼������
int search_seed(seed_struct seed, int target1, int target2, int drink)
{
    int i = drink;
    int j;
    int k[2] = {0, 0};
    seed_struct seed_next;
    while (1)
    {
        seed_next = ability_solve_drink(seed, i);
        if (seed_next.ability == target1)
        {
            k[0] = i;
            if (target2 == -1)
                return k[0] * 100 + 14;
            k[1] = search_seed_one(seed_next, target2, i);
            if (k[1] != -2)
                return k[0] * 100 + k[1];
        }
        i = (i + 1) % 15;
        if (i == drink)
        {
            return -2;
        }
    }
}

//���㻨�Ѽ�������ȯ
int cost_solve(int i, int two)
{
    int cost = 0;
    int d1, d2;
    d1 = two / 100;
    d2 = two % 100;
    if (i != 14)
        cost++;
    if (i != d1 && d1 != 14)
        cost++;
    if (d1 != d2 && d2 != 14)
        cost++;
    return cost;
}

/*
���˼·�������������ܻ�û�����ϵ�����������ܻ�����
�����0��ʼ�ܻ���ÿ�ܻ�һ������������ӵĻ����ϣ���13�ֲ�ͬ�������ܻ�����������������
����Щ�ݻ�������14��3����������Ѱ����Ŀ�꼼�ܵ�һ����ͬ�ļ���
����ҵ����Ǿ���������ӵĻ����ϼ���Ѱ�Һ��������ϵĿ������µ���һ�������뼼��
����ڶ�������������Ŀ������ƥ��
������ҵ�Ŀ������
���Ҽ�����������ʱ���ѵ�����ȯ
֮��ԭʼ���м����ܻ���ͬ�ֻ��ѵ�����ȯֻ���·�����ٵ����
*/
int main()
{
    int i, j, k, ii, jj, kk;
    int two_step;
    int cost;
    int res_cost = 4;
    int target_code[3] = {3, 3, 3};
    seed_struct seed_begin, seed_ori, seed_brance;

    //һЩ�������
    printf("��ӭʹ��splatoonװ����������\n�˳����ɱ���һ�����,ѧ�ղ���,��bugʵ������,��������\nҲ����ʹ�����ɵķ���֮ǰ��֤һ��\n");
    printf("����������������ȯ�Ӷൽ������ÿ�����ĵ�����ȯֻ������һ�ֽ��\n");
    printf("���������뷴������\nQQ:1464243589\nBվID:Ҫ�����ķ�\n");
    printf("�����뵱ǰװ������(����:0x915cfc31):\n");
    scanf("%x", &seed_begin.seed);
    printf("�밴�������װ��Ʒ�Ʊ�ţ����뵱ǰװ��Ʒ�Ʊ�ţ�\n");
    for (i = 0; i < 20; i++)
    {
        printf("%d : %s\n", i, brand_name[i]);
    }
    scanf("%d", &seed_begin.brand);
    printf("�밴������ļ��ܱ�ţ���������Ŀ�꼼������(��:0 0 0)\n");
    printf("���ֻ��Ҫ��������,Ҫ�Լ���ף����һ��λ����-1(��:1 2 -1)\n");
    printf("�����֪��ˢʲô����������-1 -1 -1 �鿴ˢ���м��ܵ�����\n");
    printf("������֮�����ÿո����\n");
    for (i = 0; i < 14; i++)
    {
        printf("%d : %s\n", i, ability_order[i]);
    }

    //�����Ŀ������
    // target_loop_num����������Ѱ��ȫ����·��ʱ��־Ѱ�ҵ��ĸ����ܵ�����
    scanf("%d %d %d", &target_code[0], &target_code[1], &target_code[2]);
    int target_loop_num = -1;
    if (target_code[0] == -1)
    {
        target_loop_num = 0;
        target_code[0] = target_loop_num;
        target_code[1] = target_loop_num;
        target_code[2] = target_loop_num;
    }
    seed_begin.ability = -1;
    int target_num = 15;
    int max_num = target_num * 2;
    seed_ori = seed_begin;
    seed_struct seed_res[max_num];
    //����Ѱ�����м��ܿ����Եĳ��򣬷ֱ�Ŀ���������ó����м��ܵ�0.3��0.2�������г���
    if (target_loop_num != -1)
    {
        int find = 0;
        int drink_num = 0;
        int drink_all[4];
        while (1)
        {
            drink_num = 0;
            find = 0;
            seed_ori = seed_begin;
            for (j = 0; j < max_num - 5; j++)
            {
                if (find != 0)
                    break;
                for (jj = 0; jj < 3; jj++)
                {
                    if (find != 0)
                        break;
                    for (i = 14; i > -1; i--)
                    {
                        if (find != 0)
                            break;
                        seed_brance = seed_ori;
                        for (ii = -1; ii < jj; ii++)
                        {
                            seed_brance = ability_solve_drink(seed_brance, i); //�ں����ϵ�����½��������ܻ�
                        }
                        if (seed_brance.ability == target_code[0])
                        {
                            two_step = search_seed(seed_brance, target_code[1], target_code[2], i);
                            if (two_step != -2 || ((two_step / 100 == two_step % 100) && (two_step / 100 == i)))
                            {
                                find = 1;
                                drink_num = 0;
                                cost = cost_solve(i, two_step);
                                if (i != 14)
                                {
                                    drink_all[drink_num] = i;
                                    drink_num++;
                                }
                                if (two_step / 100 != 14 && two_step / 100 != i)
                                {
                                    drink_all[drink_num] = two_step / 100;
                                    drink_num++;
                                }
                                if (two_step % 100 != 14 && two_step / 100 != two_step % 100)
                                {
                                    drink_all[drink_num] = two_step % 100;
                                    drink_num++;
                                }
                                if (cost != drink_num)
                                {
                                    printf("error drink number\n");
                                    printf("cost = %d", cost);
                                    printf("drink_num = %d", drink_num);
                                    printf("i = %d\n", i);
                                    printf("two_step = %d\n", two_step);
                                    printf("drink_all = %d %d %d\n", drink_all[0], drink_all[1], drink_all[2]);
                                    return 0;
                                }
                                if (target_loop_num < 14)
                                {
                                    printf("%d: <%s> 0.3������Ҫˢ %d �����ܣ���Ҫ %d ������ȯ �ֱ��� \\", target_loop_num, ability_order[target_loop_num], j + jj + 3, drink_num);
                                    for (ii = 0; ii < drink_num; ii++)
                                    {
                                        printf("%s\\", ability_order[drink_all[ii]]);
                                    }
                                    printf("\n");
                                    if (target_loop_num == 13)
                                        printf("\n\n");
                                }
                                if (target_loop_num > 13)
                                {
                                    printf("%d: <%s> 0.2������Ҫˢ %d �����ܣ���Ҫ %d ������ȯ �ֱ��� \\", target_loop_num - 14, ability_order[target_loop_num - 14], j + jj + 2, drink_num);
                                    for (ii = 0; ii < drink_num; ii++)
                                    {
                                        printf("%s\\", ability_order[drink_all[ii]]);
                                    }
                                    printf("\n");
                                }
                            }
                        }
                    }
                }
                seed_ori = ability_solve_drink(seed_ori, -1);
                seed_res[j] = seed_ori;
            }
            target_loop_num = target_loop_num + 1;
            if (target_loop_num == 28)
            {
                scanf("%d", &i);
                return 0;
            }
            if (target_loop_num < 14)
            {
                target_code[0] = target_loop_num;
                target_code[1] = target_loop_num;
                target_code[2] = target_loop_num;
            }
            else
            {
                target_code[0] = target_loop_num - 14;
                target_code[1] = target_loop_num - 14;
                target_code[2] = -1;
            }
        }
    }

    //����Ѱ���ض���Ŀ�����еĳ���
    for (j = 0; j < max_num - 5; j++)
    {
        //��ÿһ��ԭʼ�������ж�Ҫ�ܻ�ʮ�Ĵν������Ŀ����ԣ��ֱ��ܻ����Σ�Ѱ���Ƿ����Ŀ������
        for (jj = 0; jj < 3; jj++)
        {
            for (i = 14; i > -1; i--)
            {
                seed_brance = seed_ori; //����֧��Ѱ��
                for (ii = -1; ii < jj; ii++)
                {
                    seed_brance = ability_solve_drink(seed_brance, i); //�ں����ϵ�����½��������ܻ�
                    seed_res[j + ii + 1] = seed_brance;                //��¼���ܵ�·��
                }
                if (seed_brance.ability == target_code[0])
                {                                                                           //ƥ����Ŀ�����еĵ�һ������
                    two_step = search_seed(seed_brance, target_code[1], target_code[2], i); //Ѱ�Һ����������Ƿ�ƥ��
                    if (two_step != -2 || ((two_step / 100 == two_step % 100) && (two_step / 100 == i)))
                    {
                        cost = cost_solve(i, two_step); //���㷽��������������
                        if (res_cost <= cost)           //�жϴ�ʱ���ѵ����������Ƿ�����֮ǰ�ķ�������������ڣ�����
                        {
                            continue;
                        }
                        seed_res[j + jj + 1] = ability_solve_drink(seed_res[j + jj], two_step / 100);
                        seed_res[j + jj + 2] = ability_solve_drink(seed_res[j + jj + 1], two_step % 100);
                        if (res_cost == 4)
                        {
                            printf("����  <<���>>  �ķ�������Ҫ���� %d ������ȯ\n", cost);
                        }
                        else
                        {
                            printf("�˷�����Ҫ���� %d ������ȯ\n", cost);
                        }
                        res_cost = cost;
                        for (ii = 0; ii < j; ii++)
                        {
                            printf("  �� %d �����ܲ��� %s   ��ʱ������Ϊ  0x%x\n", ii + 1, ability_order[seed_res[ii].ability], seed_res[ii].seed);
                        }
                        if (i != 14)
                            printf("<<<�� %s ����>>>\n", ability_order[i]);
                        for (ii = 0; ii < jj; ii++)
                        {
                            printf("  �� %d �����ܲ��� %s   ��ʱ������Ϊ  0x%x\n", j + ii + 1, ability_order[seed_res[j + ii].ability], seed_res[j + ii].seed);
                        }
                        printf("<<0.3��ϴ�·�>> <<0.2�İ�����>> ���������ļ��ܾ���Ŀ������\n");
                        printf("  �� %d �����ܲ��� %s   ��ʱ������Ϊ  0x%x\n", j + jj + 1, ability_order[seed_res[j + jj].ability], seed_res[j + ii].seed);
                        if (two_step / 100 != i)
                        {
                            if (two_step / 100 == 14)
                                printf("<<<ȡ��>>>����Ч��\n");
                            else
                                printf("<<<�� %s ����>>>\n", ability_order[two_step / 100]);
                        }
                        printf("  �� %d �����ܲ��� %s   ��ʱ������Ϊ  0x%x\n", j + jj + 2, ability_order[seed_res[j + jj + 1].ability], seed_res[j + jj + 1].seed);
                        if (target_code[2] != -1)
                        {
                            if (two_step % 100 != two_step / 100)
                            {
                                if (two_step % 100 == 14)
                                    printf("<<<ȡ��>>>����Ч��\n");
                                else
                                    printf("<<<�� %s ����>>>\n", ability_order[two_step % 100]);
                            }
                            printf("  �� %d �����ܲ��� %s   ��ʱ������Ϊ  0x%x\n", j + jj + 3, ability_order[seed_res[j + ii + 2].ability], seed_res[j + jj + 2].seed);
                        }
                        printf("\n\n\n");
                    }
                }
            }
        }
        // printf("0x%x \n", seed_ori.seed);
        seed_ori = ability_solve_drink(seed_ori, -1);
        seed_res[j] = seed_ori;
    }
    if (res_cost == 4)
    {
        printf("Sorry,�Ҳ���Ŀ�����");
    }
    scanf("%d", &i);
    return 0;
}
