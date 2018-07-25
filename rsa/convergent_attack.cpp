#include <iostream>
#include <vector>
#include <gmpxx.h>
#include <sys/random.h>
const char *N_string = "8F033CAE74F88BA10D2BEA35FFB0EDD3";
const char *e_string = "7ABCBD7B7C84A2226ED49339DAAFA72F";
const char *d_string = "4BF39D3E2E07B10F997CF64F523BBAC2";
struct FRAC{
    mpz_class numerator;
    mpz_class denomerat;
    FRAC(mpz_class n,mpz_class d):numerator(n),denomerat(d){}
};
typedef struct FRAC frac;
frac dom_calc(mpz_class a_i,std::vector<mpz_class> a_iz)
{
    if (a_iz.size()>1) {
        std::vector<mpz_class> new_a_i(a_iz.begin()+1,a_iz.end());
        frac res = dom_calc(a_iz[0],new_a_i);
        mpz_class n = a_i*res.numerator;
        n+=res.denomerat;
        return frac(n,res.numerator);
    } else {
        mpz_class n = a_i * a_iz[0];
        n++;
        return frac(n,a_iz[0]);
    }
}
int main()
{    
    mpz_class N(N_string,16);
    mpz_class e(e_string,16);
    mpz_class d(d_string,16);
    mpz_class d_estimation;
    mpz_root(d_estimation.get_mpz_t(),N.get_mpz_t(),4);
    mpz_tdiv_q_ui(d_estimation.get_mpz_t(),d_estimation.get_mpz_t(),3);
    std::cout << "Value of the private key shall be less than: "<<d_estimation << std::endl;
    mpz_class q=0,r=0,d_p,a_0,n_p,sample_cipher,sample_message,i=e,j=N,what,tmp;
    gmp_randstate_t rand_state;
    gmp_randinit_mt(rand_state);
    unsigned long seed = 0;
    getrandom(&seed,sizeof seed,GRND_NONBLOCK);
    gmp_randseed_ui(rand_state,seed);
    do {

        mpz_urandomm(sample_message.get_mpz_t(),rand_state,N.get_mpz_t());
        mpz_gcd(tmp.get_mpz_t(),sample_message.get_mpz_t(),N.get_mpz_t());
    } while (tmp.get_ui() != 1);
    mpz_powm(sample_cipher.get_mpz_t(),sample_message.get_mpz_t(),e.get_mpz_t(),N.get_mpz_t());
    std::cout << "Sample Message: "<<sample_message << " and encryption of the message using given N and e: " <<sample_cipher <<std::endl<<std::endl;
    std::vector<mpz_class> a_i;
    bool finished = false;
    int c = 0;
    d_p=1;
    while (!finished && mpz_cmp(d_estimation.get_mpz_t(),d_p.get_mpz_t())>0) {
        ++c;
        q = i/j;
        r = i%j;
        i=j;
        j=r;
        if (c>1){
            a_i.push_back(q);
            frac  s = dom_calc(a_0,a_i);
            n_p = s.numerator;
            d_p= s.denomerat;
        } else {
            n_p = a_0 = q;
        }

        std::cout <<"+++++ Converget "<<c<<": " <<n_p<<"/"<< d_p << std::endl;
        if(d_p<d_estimation&&c>1)
        {
            mpz_powm(what.get_mpz_t(),sample_cipher.get_mpz_t(),d_p.get_mpz_t(),N.get_mpz_t());
            int asd = mpz_cmp(what.get_mpz_t(),sample_message.get_mpz_t());
            std::cout <<"----- Result of deciphering the sample message using this convergent denominator: "<<  what<<std::endl;
            if (!asd) {
                mpz_powm(what.get_mpz_t(),d.get_mpz_t(),d_p.get_mpz_t(),N.get_mpz_t());
                std::cout << "***** Private key is found and it is: " << d_p <<std::endl;
                std::cout << "***** Decryption of the cipher text in the assignment: " << d_p <<std::endl;
                finished = true;
            }
        }
    }
    if(!finished)
        std::cout << "!!!!! Was not able to find the private key." <<std::endl;
    return 0;
}
