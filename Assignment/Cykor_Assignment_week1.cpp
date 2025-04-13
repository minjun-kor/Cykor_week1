
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
#include <stdarg.h>
#define STACK_SIZE 50 // �ִ� ���� ũ��

int     call_stack[STACK_SIZE];         // Call Stack�� �����ϴ� �迭
char    stack_info[STACK_SIZE][20];     // Call Stack ��ҿ� ���� ������ �����ϴ� �迭
int     func_values_info[4][3] = {{0,0},{3,1,1},{2,1,2}, {1,2,3}};    // �Լ����� �ʱ�,���� �������� ����,local_value_name_set ó�� ��ġ  ���� ex) func_values_info[2][1] == fun2��  �������� ���� 
char    beginning_values_name_set[4][20] = { "Return Address" ,"arg1", "arg2", "arg3" };
char    func_name_set[4][10] = {"null","func1 SFP", "func2 SFP" , "func3 SFP" };
char    local_value_name_set[5][20] = { "null","var_1", "var_2" , "var_3" , "var_4"};

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

void func_prologue(int fun_num, ...);
void func_epilogue(int fun_num);
void func1(int arg1, int arg2, int arg3);
void func2(int arg1, int arg2);
void func3(int arg1);

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

void func_prologue(int fun_num, ...) {

    va_list values;
    va_start(values, fun_num);

    for (int i = func_values_info[fun_num][0]; i >= 0; i--) {
        int  beginning_value = (i != 0) ? va_arg(values, int) : -1;
        push(beginning_values_name_set[i], beginning_value);
    }
    
    push(func_name_set[fun_num], (FP == -1) ? -1 : FP);
    FP = SP;

    for (int i = 0; i < func_values_info[fun_num][1]; i++) {
        int loc_num = i + func_values_info[fun_num][2];
        push(local_value_name_set[loc_num], va_arg(values, int));
    }

    va_end(values);
}

void func_epilogue(int fun_num)
{
    int total = func_values_info[fun_num][0] + func_values_info[fun_num][1] + 2; // +2�� Return Adress, SFP�� ���� ������ ���� ��
    for (int i = 1; i <= total; i++) {
        pop();
        if (i == func_values_info[fun_num][1]) FP = call_stack[SP];
    }
}

void func1(int arg1, int arg2, int arg3)
{
    int var_1 = 100;

    func_prologue(1,arg3,arg2,arg1,var_1);
    print_stack();

    func2(11, 13);

    func_epilogue(2);
    print_stack();
}


void func2(int arg1, int arg2)
{
    int var_2 = 200;

    func_prologue(2, arg2, arg1, var_2);
    print_stack();

    func3(77);

    func_epilogue(3);
    print_stack();
}


void func3(int arg1)
{
    int var_3 = 300;
    int var_4 = 400;

    func_prologue(3, arg1, var_3, var_4);
    print_stack();

}

int main()
{
    func1(1, 2, 3);

    func_epilogue(1);
    print_stack();

    return 0;
}