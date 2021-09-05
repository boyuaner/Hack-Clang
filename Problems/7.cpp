//函数调用参数类型不匹配
int f(int a)
{
    return a;
}
int l()
{
    int *a;
    f(a);
    return 0;
}