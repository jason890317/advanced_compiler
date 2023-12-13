void icpp4() 
{
    int a, b, c, d, e, x, y, f, g, h, i;
    int *p,**pp;
    
    a  = b + c; //0
    p  = &y;    //1
    d  = b + c; //2
    f  = a + d + x + y; //3
    g  = a + d + x + y; //4
    *p = i + 2;  //5
    h  = a + d + x + y; //6
    f  = y + 1; //7 
    **pp=b; //8
    b=c;
    *pp=p; //9 //
    *pp=&a; //10
    pp=&p;  //11
    **pp=*p; //12
    p=*pp; //13
    a=**pp; //14    
}
