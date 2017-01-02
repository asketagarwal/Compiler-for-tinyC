int printi(int num);
int prints(char * c);
int readi(int *eP);

int f(int *a)
{
    int x;
    x = *a;
    x = x + 5;
    return x;
}
int main()
{
    int a,q;
    int *e;
    
    
    
    q = 3;
    e = &q;

    prints("Example of passing pointer to function\n");
    prints("Value passed : ");
    printi(q);
    prints("\n");
    a = f(e);
    prints("Value returned  is: ");
    printi(a);
    prints("\n");
    
    prints("Read an integer!!");
    prints("\n");
    q = readi(e);
    prints("The integer  read is:");
    printi(q);
    prints("\n");
    
    return 0;
}
