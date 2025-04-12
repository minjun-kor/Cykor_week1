
/*  call_stack

    ���� �ý��ۿ����� ������ �޸𸮿� ���������, �� ���������� `int` �迭�� �̿��Ͽ� �޸𸮸� �����մϴ�.
    ������ SFP�� Return Address�� ���� ���� �޸� �ּҰ� ��������, �̹� ���������� -1�� ��ü�մϴ�.

    int call_stack[]      : ���� ������(`int ��`) �Ǵ� `-1` (��Ÿ������ ���п�)�� �����ϴ� int �迭
    char stack_info[][]   : call_stack[]�� ���� ��ġ(index)�� ���� ������ �����ϴ� ���ڿ� �迭

    ==========================call_stack ���� ��Ģ==========================
    �Ű� ���� / ���� ������ push�� ���   : int �� �״��
    Saved Frame Pointer �� push�� ���  : call_stack������ index
    ��ȯ �ּҰ��� push�� ���       : -1
    =======================================================================


    ==========================stack_info ���� ��Ģ==========================
    �Ű� ���� / ���� ������ push�� ���        : ������ ���� ����
    Saved Frame Pointer �� push�� ���  : � �Լ��� SFP����
    ��ȯ �ּҰ��� push�� ���                 : "Return Address"
    ========================================================================
*/
#include <stdio.h>
#define STACK_SIZE 50 // �ִ� ���� ũ��

int     call_stack[STACK_SIZE];         // Call Stack�� �����ϴ� �迭
char    stack_info[STACK_SIZE][20];     // Call Stack ��ҿ� ���� ������ �����ϴ� �迭
char    values_name[4][20] = { "Return Address" ,"arg1", "arg2", "arg3" };
char    func_name[3][10] = {"func1 SFP", "func2 SFP" , "func3 SFP" };
char    name[4][20] = { "var_1", "var_2" , "var_3" , "var_4" };

/*  SP (Stack Pointer), FP (Frame Pointer)

    SP�� ���� ������ �ֻ�� �ε����� ����ŵ�ϴ�.
    ������ ������� �� SP = -1, �ϳ��� ���̸� `call_stack[0]` -> SP = 0, `call_stack[1]` -> SP = 1, ...

    FP�� ���� �Լ��� ���� ������ �������Դϴ�.
    ���� ���� �Լ� ���� �������� sfp�� ����ŵ�ϴ�.
*/
int SP = -1;
int FP = -1;

void push(char name[20], int value);
void pop();
void func1(int arg1, int arg2, int arg3);
void func2(int arg1, int arg2);
void func3(int arg1);

/*
    ���� call_stack ��ü�� ����մϴ�.
    �ش� �Լ��� ��� ������� �������� ���� �ϼ����� ���� �����Դϴ�.
*/
void print_stack()
{
    if (SP == -1)
    {
        printf("Stack is empty.\n");
        return;
    }

    printf("====== Current Call Stack ======\n");

    for (int i = SP; i >= 0; i--)
    {
        if (call_stack[i] != -1)
            printf("%d : %s = %d", i, stack_info[i], call_stack[i]);
        else
            printf("%d : %s", i, stack_info[i]);

        if (i == SP)
            printf("    <=== [esp]\n");
        else if (i == FP)
            printf("    <=== [ebp]\n");
        else
            printf("\n");
    }
    printf("================================\n\n");
}

void push(char name[20], int value) {
    SP += 1;
    for (int i = 0;name[i] != 0;i++) {
        stack_info[SP][i] = name[i];
    }
    call_stack[SP] = value;
}

void pop() {

    for (int i = 0; stack_info[SP][i] != 0; i++) {
        stack_info[SP][i] = 0;
    }
    call_stack[SP] = 0;

    SP -= 1;
}

//func ���δ� �����Ӱ� �߰��ص� ��������, �Ʒ��� ������ �ٲ����� ������
void func1(int arg1, int arg2, int arg3)
{
    int var_1 = 100;

    // func1�� ���� ������ ���� (�Լ� ���ѷα� + push)

    // �Լ� ���ѷα�
    push(values_name[3], arg3);
    push(values_name[2], arg2);
    push(values_name[1], arg1);
    push(values_name[0], -1);
    push(func_name[0], -1);
    FP = SP;

    //���� ���� push
    push(name[0], var_1);

    print_stack();
    func2(11, 13);

    // func2�� ���� ������ ���� (�Լ� ���ʷα� + pop)

    //�Լ� ���ʷα�
    pop();
    FP = call_stack[SP];
    pop();
    pop();
    pop();
    pop();

    print_stack();
}


void func2(int arg1, int arg2)
{
    int var_2 = 200;

    // func2�� ���� ������ ���� (�Լ� ���ѷα� + push)

    // �Լ� ���ѷα�
    push(values_name[2], arg2);
    push(values_name[1], arg1);
    push(values_name[0], -1);
    push(func_name[0], FP);
    FP = SP;

    //���� ���� push
    push(name[1], var_2);

    print_stack();
    func3(77);

    // func3�� ���� ������ ���� (�Լ� ���ʷα� + pop)

    //�Լ� ���ʷα�
    pop();
    pop();
    FP = call_stack[SP];
    pop();
    pop();
    pop();

    print_stack();
}


void func3(int arg1)
{
    int var_3 = 300;
    int var_4 = 400;

    // func3�� ���� ������ ���� (�Լ� ���ѷα� + push)
   
    // �Լ� ���ѷα�
    push(values_name[1], arg1);
    push(values_name[0], -1);
    push(func_name[0], FP);
    FP = SP;

    //���� ���� push
    push(name[2], var_3);
    push(name[3], var_4);

    print_stack();

}


//main �Լ��� ���õ� stack frame�� �������� �ʾƵ� �˴ϴ�.
int main()
{
    func1(1, 2, 3);
    // func1�� ���� ������ ���� (�Լ� ���ʷα� + pop)

    //�Լ� ���ʷα�
    pop();
    FP = call_stack[SP];
    pop();
    pop();
    pop();
    pop();
    pop();

    print_stack();
    return 0;
}