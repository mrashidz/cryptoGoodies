/*Following code has been developed for Cryptography Course, the code could be optimized in many ways, but in order to keep the readability performance might have been dissmissed.*/
/*After all this is not a programming course.*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    unsigned int base;
    unsigned long long expo;
    unsigned int modu;
}stu_me;

typedef struct {
    unsigned int remi;
    unsigned int modu;
}stu_crt;

typedef struct {
    unsigned int degrees[10];
    int coefici[10];
    unsigned char size;
}stu_polynomial;

typedef struct {
    int x;
    int y;
    bool special;
}stu_point;
unsigned int euclidean(unsigned int _a, unsigned int _b, bool _verboose) {
    unsigned int gcd = 0,r =0,r_prime=0, rem = 1, dev = 0;
    if (_a > _b) {
        r = _a;
        r_prime = _b;
    } else {
        r = _b;
        r_prime = _a;
    }
    if (_verboose) {
        printf("Euclidean Algorithm to calculated GCD.\n");
        printf("Initing!... ");
        printf("[ r = %d, r' = %d ]\n",r,r_prime);
    }
    while(rem > 0) {
        gcd = rem; //if x becomes zero we dont come here so we just take our thing in case ",
        rem = r % r_prime;
        dev = r/r_prime;//here only for printing
        if (_verboose) {
        printf("%d = (%d * %d) + %d\n--------\n",r,r_prime,dev,rem);
        printf("shifting... ");
        }
        r = r_prime;//shifts for the next round
        r_prime = rem;
        if (_verboose)
            printf("[ r = %d, r' = %d ]\n",r,r_prime);
    }
    if (_verboose)
        printf("[r' == %d ], GCD of %d and %d has been computed... WoW\n",r_prime,_a,_b);
    return gcd;
}

int ext_euclidean(unsigned int _number, unsigned int _size_of_set, bool _verboose) {
    unsigned int gcd = 0,r =0,r_prime=0, x = 1, x_prime = 0,y=0,y_prime=1,dev=0,tmp = 0;
    int reverse = -1;
    r = _size_of_set;
    r_prime = _number%_size_of_set;
    if (_verboose) {
    printf("Extended Euclidean Algorithm to findout x | 1 = %d * x mod %d .\n",_number,_size_of_set);
    printf("gcd(%d,%d) should be 1.\n",_size_of_set,_number);
    }
    unsigned char i = 1;
    while (r_prime != 0) {
        dev = r/r_prime;
        if (_verboose) {
        printf("STEP %d:\n",i++);
        printf("[(r,r')=(%d,%d), dev=%d, (x,x')=(%d,%d), (y,y')=(%d,%d)]\n",r,r_prime,dev,x,x_prime,y,y_prime);
        }
        tmp = r;
        if (_verboose)
        printf("[r  = r' = %d]\t",r_prime);
        r = r_prime;
        if (_verboose)
        printf("[r' = r - (dev * r') = %d - (%d * %d)]\n",tmp,dev,r_prime);
        r_prime = tmp - (dev*r_prime);

        tmp = x;
        if (_verboose)
        printf("[x  = x' = %d]\t",x_prime);
        x = x_prime;
        if (_verboose)
        printf("[x' = x - (dev * x') = %d - (%d * %d)]\n",tmp,dev,x_prime);
        x_prime = tmp - (dev*x_prime);

        tmp = y;
        if (_verboose)
        printf("[y  = y' = %d]\t",y_prime);
        y = y_prime;
        if (_verboose)
        printf("[y' = y - (dev * y') = %d - (%d * %d)]\n",tmp,dev,y_prime);
        y_prime = tmp - (dev*y_prime);
    }

    if (_verboose) {
    printf("\nr' reached Zero, so gcd(r,r') =  x*r + y*r'\n");
    printf("[(r,r')=(%d,%d), (x,y)=(%d,%d)]\n",_size_of_set,_number,x,y);
    }
    //since we are not using these variable anymore, using them as temp to print nice formula
    r = _size_of_set*x;
    r_prime=(_number%_size_of_set)*y;
    if (_verboose)
    printf("gcd(%d,%d) =  %d*%d + %d*%d = (%d) + (%d) = %d\n",_size_of_set,_number,_size_of_set,x,_number,y,r,r_prime,r+r_prime);
    if (r+r_prime != 1) {
        printf("gcd(%d,%d) !=  1, %d has no inverse in the set. \n",_size_of_set,_number,_number);
    } else {
        if (_verboose) {
        printf("1 =  %d*%d + %d*%d\t",_size_of_set,x,_number,y);
        printf("Reduce both sides of the equation mod %d (devide by %d and keep the remainder)\n",_size_of_set,_size_of_set);
        printf("in mod %d , %d * %d = 0)\n",_size_of_set,_size_of_set,x);
        }
        printf("1 = %d * %d  mod %d\n",_number,y,_size_of_set,(_number*y),_size_of_set,r);
        while (reverse < 0)
            reverse = (y+=_size_of_set)%_size_of_set; //get rid of the minus
    }
    return reverse;
}

bool is_gen(unsigned int *_elements, unsigned int _set_size, unsigned int _gen, unsigned int _mod) {
    bool gen = false;
    unsigned int i = 0,p=1;
    unsigned int gen_elements[_set_size];
    printf("Calculating exponents of %d - we will stop when we reach 1 since %d to the power of zero is one,and 1 = N+1 mod N that means we have done a round complete.\n",_gen,_gen);
    while (i!=1)
    {
        i = ((unsigned int)pow(_gen,p))%_mod;
        printf("%d\t^%d\t=%d\n",_gen,p,i);
        gen_elements[p-1] = i;
//        printf("%d",gen_elements[p-1]);
        ++p;
    }
    printf("The generated set by the generator:{");
    for (i = 0; i < p-1; i++)
        printf("%d,",gen_elements[i]);
    printf("}\nSize of generated set is %d and original set size is %d.\n",p-1,_set_size);

    if (p-1==_set_size)
    {
        int j;
        bool elem_missing = false;
        for (i = 0; i < _set_size;i++)
        {
            bool found = false;
            for (j = 0; (j < _set_size && !found); j++)
                if (_elements[i]==gen_elements[j])
                    found = true;
            if (!found)
            {
                elem_missing = true;
                break;
            }

        }
        if (!elem_missing)
            gen = true;
    }
    return gen;
}

unsigned int compute_order_and_check_generator(unsigned int _n, bool _show_elements, bool _check_gen, unsigned int *_gen, unsigned int _no_of_gens) {
    unsigned int i,c=0;
    unsigned int elements[_n-1];//max if n is prime
    if (_show_elements)
        printf("Z*%d = {",_n);
    for (i = 1; i < _n; i++) //no need to check n-1 since their gcd is 1 for sure, but ...
        if (euclidean(i,_n,false)==1) {
            elements[c] = i;
            c++;
            if (_show_elements)
                printf("%d,",i);
        }
    if (_show_elements)
        printf("}\n",i);
    if (_check_gen)
        for (i = 0; i < _no_of_gens; i++)
        {
            if (is_gen(elements,c,_gen[i],_n))
                printf("++++++++ %d IS   a   generator for the list.\n\n",_gen[i]);
            else
                printf("++++++++ %d IS NOT a generator for the list.\n\n",_gen[i]);
        }

    return c;
}
unsigned int modular_exponentiation(stu_me _me, bool _verboose) {
    if (_verboose)
        printf("Base = %d, Exponent=%lli, Modulus=%d.\n",_me.base,_me.expo,_me.modu);
    unsigned int result = 1;
    unsigned char bits[64];//64 bit integer at the max long long
    unsigned long long val =  _me.expo;
    unsigned char size = 0;
    while (val != 0) {
           if((val & 1) == 1)
               bits[size]=1;
           else
               bits[size]=0;
           //next bit
           val >>= 1;
           size++;
    }
    int i = size-1;
    if (_verboose) {
        printf("Exponent %lli binary format is:\t [",_me.expo);
        for (i; i>-1; i--)
            printf("%d ",bits[i]);
        printf("]\nWe start by initializing the result as one. and we go through the binary representation (lmsb) of the Exponent showed above.");
        printf("\nWhere we have one  we square the result in modulus given, then multiply it to the base modulus.");
        printf("\nWhere we have zero we square the result in modulus given. Please have look into the source code.\n");
    }
    i = size -1;
    for (i;i>-1; i--) {
        if (bits[i]) {//if one
            result = (unsigned int)(pow(result,2))%_me.modu;
            result = (result*_me.base)%_me.modu;
        } else//if zero
            result = (unsigned int)(pow(result,2))%_me.modu;
    }
    return result;
}

int crt(stu_crt *_crt,unsigned char _size) {
    if (_size < 1)
        return -1;

    int i , j, crt = 0;
    unsigned int Mi[_size];
    unsigned int yi[_size];
    unsigned int M = 1;
    printf("Calculatin CRT based on the following:\nGCD of all Modulus compare to the others should be one.\n");
    //GCDs / since we are going through each elements we caluclate M here as well
    for (i=0; i < _size; i++) {
        printf("X = %d mod %d\n",_crt[i].remi,_crt[i].modu);
        M = _crt[i].modu*M;
        for (j = 0; j < _size;j++) // lots of redundant gcd calculation here, since the size is 3 we can ignire that
            if (i!=j && euclidean(_crt[i].modu,_crt[j].modu,false) != 1) // gcd should be 1 between all devider to CRT to work.
                return -1;
    }
    printf("M = %d\n----------\n",M);
    //calculating Mi and yi
    for (i=0; i < _size; i++) {
         Mi[i] = M/_crt[i].modu;
         yi[i] = ext_euclidean(Mi[i],_crt[i].modu,false);
         printf("M[%d]=%d\t y[%d]=%d\n----------\n",i,Mi[i],i,yi[i]);
    }
    //calculating X
    for (i=0; i < _size; i++)
    {
        printf("a[%d] * M[%d] * yi[%d] = %d * %d * %d = %d \n",i,i,i,_crt[i].remi,Mi[i],yi[i],_crt[i].remi*Mi[i]*yi[i]);
        crt += _crt[i].remi*Mi[i]*yi[i];
    }
    printf("X = %d mod %d\n",crt,M);
    return crt%M;
}
void recover_rsa(unsigned int _n,unsigned int _e,unsigned int _p, unsigned int _c) {
    printf("n = %d, p = %d, e= %d\n",_n,_p,_e);
    if(_n%_p)
    {
        printf("Given the values, cannot compute the other prime -- Aborted!");
        return;
    }
    unsigned int q =_n/_p;
    printf("n = pq => %d = %d * q => q = %d / %d = %d  -- this is the other prime.\n",_n,_p,_n,_p,q);
    unsigned int s =(_p-1)*(q-1);
    printf("s = (p-1)(q-1) = %d\n",s);
    printf("d = 1/e mod s  or 1 = e*d mod s - we use extended euclidean algorithm to calculate the reverse, steps are hidden, please refer to source code.\n");
    unsigned int d = ext_euclidean(_e,s,false);
    printf("d = %d -- this is the secret key!\n",d);
    printf("Decryption means solving C^d mod n => %d^%d mod %d -- Calculation steps are hidden, please refer to source code\n",_c,d,_n);
    stu_me me;
    me.base = _c;
    me.expo = d;
    me.modu = _n;
    printf("++++++++ Message is %d \n\n",modular_exponentiation(me,false));
}
void demo_el_g(unsigned int _g, unsigned int _sk, unsigned int _mod, unsigned int _y, unsigned int _msg) {
    printf("Demo El Gamal given the following variables:\n");
    printf("Generator:%d\tModulus:%d\n",_g,_mod);
    printf("Message:%d\tSecret Key:%d\n",_msg,_sk);
    stu_me me;
    me.base=_g;
    me.expo=_sk;
    me.modu=_mod;
    unsigned int h = modular_exponentiation(me,false);
    printf("Random:%d\th = g^sk mod m = %d^%d mod %d = %d\n",_y,_g,_sk,_mod,h);
    printf("Public Key (Modulus,Generator,h)(%d,%d,%d)\n",_mod,_g,h);
    me.base = _g;
    me.expo = _y;
    unsigned int c1 = modular_exponentiation(me,false);
    printf("C1 = Generator^y mod m = %d^%d mod %d = %d\n",_g,_y,_mod,c1);
    me.base = h;
    me.expo = _y;
    unsigned int c2 = (_msg*modular_exponentiation(me,false))%_mod;
    printf("C2 = h^y mod m = %d^%d mod %d = %d\n",h,_y,_mod,c2);
    printf("Transmitter now sends (C1,C2) = (%d,%d) to receiver.\n------\n",c1,c2);
    printf("Receiver HAS the secret key, and obviously the public key as well. She tries to Calculate S.\n");
    me.base = c1;
    me.expo = _sk;
    unsigned int s = modular_exponentiation(me,false);
    printf("S = C1^sk mod m = %d^%d mod %d = %d\nCalculating the inverse: ",c1,_sk,_mod,s);

    unsigned int s_inv = ext_euclidean(s,_mod,false);
    printf("S^-1 = %d\n",s_inv);
    unsigned decrypted_msg = (c2*s_inv)%_mod;
    printf("Message = C2 * S^-1 = %d * %d mod %d = %d\n",c2,s_inv,_mod,decrypted_msg);
}
stu_polynomial results[2];//first Quotient and second remainder
#define Q 0//QUOTIENT
#define R 1//REMAINDR
void empty_result(){
    unsigned char i = 0;
    for (i;i<10;i++)
    {
        results[Q].coefici[i]=0;
        results[Q].degrees[i]=0;
        results[R].coefici[i]=0;
        results[R].degrees[i]=0;
    }
    results[Q].size = 0;
    results[R].size = 0;
}
stu_polynomial polynomi_min(stu_polynomial _a, stu_polynomial _b, unsigned int _mod) {
    stu_polynomial result;
//    int i,j;
//    for (i = 0; i <_a.size;i++)
//        for (j = 0)
    return result;
}

stu_polynomial polynomi_mul(stu_polynomial _a, stu_polynomial _b, unsigned int _mod) {
    stu_polynomial result;
    return result;

}

stu_polynomial *polynomi_dev(stu_polynomial _divid, stu_polynomial _divis, unsigned int _mod) {
    empty_result();
//    bool still_shit_to_divide = true;
//    stu_polynomial tmp = _divid;
//    stu_polynomial tmp2;
//    unsigned char i = 0;
//    while (tmp.degrees[0] >= _divis.degrees[0]) {
//        results[Q].degrees[results[Q].size] = tmp.degrees[0] - _divis.degrees[0];
//        results[Q].coefici[results[Q].size] = tmp.degrees[0] / _divis.degrees[0];
//        results[Q].size ++;



//    }


//    Divi
    return results;
}

void ext_euclidean_polynomi(stu_polynomial _divid, stu_polynomial _divis, unsigned int _mod) {

//    stu_polynomial gcd = 0,r =0,r_prime=0, x = 1, x_prime = 0,y=0,y_prime=1,dev=0,tmp = 0;
//    stu_polynomial reverse = -1;
//    r = _size_of_set;
//    r_prime = _number%_size_of_set;
//    if (_verboose) {
//    printf("Extended Euclidean Algorithm to findout x | 1 = %d * x mod %d .\n",_number,_size_of_set);
//    printf("gcd(%d,%d) should be 1.\n",_size_of_set,_number);
//    }
//    unsigned char i = 1;
//    while (r_prime != 0) {
//        dev = r/r_prime;
//        if (_verboose) {
//        printf("STEP %d:\n",i++);
//        printf("[(r,r')=(%d,%d), dev=%d, (x,x')=(%d,%d), (y,y')=(%d,%d)]\n",r,r_prime,dev,x,x_prime,y,y_prime);
//        }
//        tmp = r;
//        if (_verboose)
//        printf("[r  = r' = %d]\t",r_prime);
//        r = r_prime;
//        if (_verboose)
//        printf("[r' = r - (dev * r') = %d - (%d * %d)]\n",tmp,dev,r_prime);
//        r_prime = tmp - (dev*r_prime);

//        tmp = x;
//        if (_verboose)
//        printf("[x  = x' = %d]\t",x_prime);
//        x = x_prime;
//        if (_verboose)
//        printf("[x' = x - (dev * x') = %d - (%d * %d)]\n",tmp,dev,x_prime);
//        x_prime = tmp - (dev*x_prime);

//        tmp = y;
//        if (_verboose)
//        printf("[y  = y' = %d]\t",y_prime);
//        y = y_prime;
//        if (_verboose)
//        printf("[y' = y - (dev * y') = %d - (%d * %d)]\n",tmp,dev,y_prime);
//        y_prime = tmp - (dev*y_prime);
//    }

}

stu_point curve_add(stu_point _1, stu_point _2, unsigned int _mod) {
    stu_point result;
    if (_1.special||_2.special|| (_1.x == _2.x && _1.y == -_2.y )) {
        result.special = true;
        return result;
    }
    result.special = false;
    printf("Adding P(%d * %d) and Q(%d,%d):\t",_1.x, _1.y,_2.x,_2.y);
    unsigned int landa;
    if ((_2.y - _1.y) == 1)
        landa = ext_euclidean((_2.x - _1.x),_mod,false);
    else
        landa = ((_2.y - _1.y)/(_2.x - _1.x))%_mod;
    printf("Landa: %d:\n",landa);
    result.x = (unsigned int)(pow(landa,2) - _1.x - _2.x);
    while (result.x < 0)
        result.x+=_mod;
    if (result.x > _mod)
        result.x = result.x % _mod;

    result.y = landa * (_1.x - result.x) - _1.y;
    while (result.y < 0)
        result.y+=_mod;
    if (result.y > _mod)
        result.y = result.y % _mod;

    return result;
}
stu_point curve_dob(stu_point _p, unsigned int _a, unsigned int _mod) {
    stu_point result;
    if (_p.special) {
        result.special = true;
        return result;
    }
    result.special = false;
    printf("Doubling P(%d * %d)\t",_p.x, _p.y);
    unsigned int landa;
    if (((pow(_p.x,2)*3)+_a) == 1) {
        landa = ext_euclidean((2*_p.y),_mod,false);
    } else {
        landa = (unsigned int)(((pow(_p.x,2)*3)+_a)/(2*_p.y)) % _mod;
    }
    printf("Landa: %d\n",landa);
    result.x = (unsigned int)(pow(landa,2) - (2*_p.x))/* % _mod*/; //doesnt work like this....dont know why
    printf("x: %d, mod: %d\n",result.x,_mod);
    while (result.x < 0)
        result.x+=_mod;
//    if (result.x > _mod)
        result.x = result.x % _mod;
    result.y = (landa * (_p.x - result.x) - _p.y ) /*% _mod*/;
    printf("y: %d, mod: %d\n",result.y,_mod);
    while (result.y < 0)
        result.y+=_mod;
//    if (result.y > _mod)
        result.y = result.y % _mod;



    return result;
}
bool point_on_the_curve(stu_point _p, stu_polynomial _y, stu_polynomial _x, unsigned int _mod) {

    if (_p.special)
        return false;
    unsigned int x = 0,y = 0,i;

    for (i = 0;i<_x.size;i++)
        x += (pow(_p.x,_x.degrees[i])*_x.coefici[i]);
    x = x%_mod;
    for (i = 0;i<_y.size;i++)
        y += (pow(_p.y,_y.degrees[i])*_y.coefici[i]);


    y = y%_mod;
    printf("Calculating both side of the polynomial for (%d,%d) => x:%d  y:%d.\n",_p.x,_p.y,x,y);
    return x == y;

}

int main(int argc, char *argv[]) {
    (void *)argc;
    (void *)argv;
    unsigned int a = 28,b = 98, n = 0;
    printf("*****************************************************************************\n");
    printf("(1.a) Compute GCD of %d and %d using Euclidean Alg.\n\n",a,b);
    printf("++++++++ GCD of %d and %d: %d\n\n",a,b,euclidean(a,b,true));
    printf("\n----------------------------------------------------------------------------\n");
    unsigned char z = 26,c;
    int inverse = 0;
    printf("(1.b) Calculate inverse of 5, 6, and 7 in the given set (N=%d).\n\n",z);
    for (c=5;c<8;c++) {
        inverse = ext_euclidean(c,z,true);
        (inverse > 0)?
                    printf("++++++++ Inverse of %d is %d\n\n",c,inverse):
                    printf("++++++++ %d has no inverse in the set since gcd(%d,%d)!=1\n\n",c,z,c);
    }
    z = 43;
    c = 35;
    printf("\n----------------------------------------------------------------------------\n");
    printf("(1.c) Compute the orders of cyclic Z %d and %d.\n\n",z,c);
    printf("The order of cyclic group is Phi or the size of the set.\n");
    printf("It could be also the highest number the generator needs as exponatation.\n");
    printf("To generate all the elements of the list.\n");
    printf("For prime number, P we have Phi = p-1.\n");
    printf("Since 43 is prime Phi(43)=43-1=42.\n");
    printf("But for the fun, we calculate both 43 and 35, by finding the elements which their\ngcd with set field (43 and 35) is 1.\n");
    printf("i.e. we are looking for 0<i<N | gcd (i,N) = 1.  \n");
    printf("Here we use Euclidean Alg. to calculate gcd and find elements -- Euclidean steps are hidden, check source code\n");
    printf("++++++++ Order of %d is %d\n\n",c,compute_order_and_check_generator(c,false,false,NULL,0));
    printf("++++++++ Order of %d is %d\n\n",z,compute_order_and_check_generator(z,false,false,NULL,0));
    printf("\n----------------------------------------------------------------------------\n");
    printf("(1.d) Calculate Modular Exponentiation.\n\n",z,c);
    stu_me me;
    me.base = 3;
    me.expo = 12000027;
    me.modu = 35;
    printf("++++++++ %d to the power off  %lli in modulus %d is: %d\n\n",me.base,me.expo,me.modu,modular_exponentiation(me,true));
    me.base = 4;
    me.expo = 72000000126;
    me.modu = 77;
    printf("++++++++ %d to the power off  %lli in modulus %d is: %d\n\n",me.base,me.expo,me.modu,modular_exponentiation(me,true));
    z=21;
    printf("\n----------------------------------------------------------------------------\n");
    printf("(1.e) List all elements of Z%d\n\n",z);
    printf("Here we use Euclidean Alg. to calculate gcd and find elements -- Euclidean steps are hidden, check source code\n");
    printf("++++++++ The size of Z%d is %d\n\n",z,compute_order_and_check_generator(z,true,false,NULL,0));

    printf("\n*****************************************************************************\n");
    printf("(2.a) Find the number of apples.\n\n",z);
    stu_crt _crt[3];
    _crt[0].modu = 9;
    _crt[0].remi = 4;
    _crt[1].modu = 5;
    _crt[1].remi = 2;
    _crt[2].modu = 11;
    _crt[2].remi = 3;
//    LECTURE EXAMPLE
//    _crt[0].modu = 7;
//    _crt[0].remi = 5;
//    _crt[1].modu = 11;
//    _crt[1].remi = 3;
//    _crt[2].modu = 13;
//    _crt[2].remi = 10;
    printf("++++++++ Number of Apples: %d\n\n",crt(_crt,3));
    printf("\n----------------------------------------------------------------------------\n");
    z = 23;
    printf("(2.b) Are 2 and 20 generators to  Z*23.\n\n",z);
    unsigned int gen[2] = {2,20};
    compute_order_and_check_generator(z,true,true,gen,2);
    printf("\n*****************************************************************************\n");
    printf("(3.a) Recover Secret Key and the Message.\n\n",a,b);
    n = 7387;
    a = 83;
    z = 3;
    b = 6920;
    recover_rsa(n,z,a,b);

    printf("\n*****************************************************************************\n");
    printf("(4.a) Demostrate El Gamal.\n\n",a,b);
    printf("Please note modular exponentiation and extended Euclidean Alg details are hidden, please refer to the source code.\n");
    z = 4; // g
    a = 107; // mod
    c = 14; //y
    b = 04; //sk
    n = 0b100111;  //msg
//  LECTURE EXAMPLE
//    z = 3; // g
//    a = 139; // mod
//    c = 52; //y
//    b = 12; //sk
//    n = 100;  //msg
    demo_el_g(z,b,a,c,n);
    printf("\n----------------------------------------------------------------------------\n");
    printf("(4.c) Calculate 5P.\n\n",a,b);
    stu_polynomial x;
    stu_polynomial y;
    x.size = 3;
    x.degrees[0] = 3;
    x.coefici[0] = 1;
    x.degrees[1] = 1;
    x.coefici[1] = 3;
    x.degrees[2] = 0;
    x.coefici[2] = -4;
    y.size = 1;
    y.degrees[0] = 2;
    y.coefici[0] = 1;
    stu_point p;
    p.special = false;
    p.x = 3;
    p.y = 7;
    z = 17; //mod
    a = 3; //a

//    x.size = 3;
//    x.degrees[0] = 3;
//    x.coefici[0] = 1;
//    x.degrees[1] = 1;
//    x.coefici[1] = 1;
//    x.degrees[2] = 0;
//    x.coefici[2] = 5;

//    y.size = 1;
//    y.degrees[0] = 2;
//    y.coefici[0] = 1;
//    stu_point p;
//    p.special = false;
//    p.x = 0;
//    p.y = 7;
//    z = 11; //mod
//    a = 1; //a
    printf("It is obvious that 5P = 2*(2P) + P.\nTherefore, we first find the value of 2P.\n",a,b);
    printf("First we check if P is on the curve.\n",a,b);
    if (point_on_the_curve(p,y,x,z)) {
        printf("P(%d,%d) special(%d) is on the curve.\n",p.x,p.y,p.special);
        printf("Calculate 2P:\n");
        stu_point _2p = curve_dob(p,a,z);
        printf("2P = (%d,%d) special(%d) and we need to check if it is on the curve.\n",_2p.x,_2p.y,_2p.special);
        if (point_on_the_curve(_2p,y,x,z)) {
            printf("2P(%d,%d) special(%d) is on the curve.\n",_2p.x,_2p.y,_2p.special);
            printf("Calculate 4P:\n");
            stu_point _4p = curve_dob(_2p,a,z);
            printf("4P = (%d,%d) special(%d) and we need to check if it is on the curve.\n",_4p.x,_4p.y,_4p.special);
            if (point_on_the_curve(_4p,y,x,z)) {
                printf("4P(%d,%d) special(%d) is on the curve.\n",_4p.x,_4p.y,_4p.special);
                printf("Calculate 5P = 4P + P:\n");
                stu_point _5p = curve_add(_5p,p,z);
                printf("5P = (%d,%d) special(%d) and we need to check if it is on the curve.\n",_5p.x,_5p.y,_5p.special);
                if (point_on_the_curve(_5p,y,x,z))
                    printf("5P(%d,%d) special(%d) is on the curve.\n",_5p.x,_5p.y,_5p.special);
                else
                    printf("5P(%d,%d) special(%d) is NOT on the curve.\n",_5p.x,_5p.y,_5p.special);


            } else
                printf("4P(%d,%d) special(%d) is NOT on the curve.\n",_4p.x,_4p.y,_4p.special);


        } else
         printf("2P(%d,%d) special(%d) is NOT on the curve.\n",_2p.x,_2p.y,_2p.special);

    } else
        printf("P(%d,%d) special(%d) is NOT on the curve.\n",p.x,p.y,p.special);






    stu_polynomial divid;

    return 0;
}

