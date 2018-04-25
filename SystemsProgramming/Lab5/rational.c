#include <stdio.h> // for printf()
#include <stdlib.h> // for exit()
#include<math.h>

#include "rational.h"

static Rational rtnl_simplify(Rational rtnl);
int GCD(Rational rtnl);

int main(int argc, char *argv[])
{
    int num = 17*258;
    int denom = 64*258;
    Rational rtnl0 = rtnl_init(42, 19);
    Rational rtnl1 = rtnl_init(22, 13);
    char buf0[RTNL_BUF_SIZE], buf1[RTNL_BUF_SIZE], buf2[RTNL_BUF_SIZE];
    int ipow;

    printf("simplification of %d/%d = %s  (should be 17/64)\n",
           num, denom, rtnl_asStr(rtnl_init(num, denom), buf0));
    printf("%s + %s = %s\n", rtnl_asStr(rtnl0, buf0), rtnl_asStr(rtnl1, buf1),
           rtnl_asStr(rtnl_add(rtnl0, rtnl1), buf2));
    printf("%s - %s = %s\n", rtnl_asStr(rtnl0, buf0), rtnl_asStr(rtnl1, buf1),
           rtnl_asStr(rtnl_sub(rtnl0, rtnl1), buf2));
    printf("%s * %s = %s\n", rtnl_asStr(rtnl0, buf0), rtnl_asStr(rtnl1, buf1),
           rtnl_asStr(rtnl_mul(rtnl0, rtnl1), buf2));
    printf("%s / %s = %s\n", rtnl_asStr(rtnl0, buf0), rtnl_asStr(rtnl1, buf1),
           rtnl_asStr(rtnl_div(rtnl0, rtnl1), buf2));
    printf("warning: integer overflow about to occur\n"
           "  watch for negative signs\n");
    for (ipow = 0; ipow < 10; ipow++) {
        printf("%s**%d = %s\n", rtnl_asStr(rtnl0, buf0), ipow,
               rtnl_asStr(rtnl_ipow(rtnl0, ipow), buf1));
    }
    exit(EXIT_SUCCESS);
}
int GCD(Rational rtnl)
{
	int tmp;
	int a = rtnl.num;
	int b = rtnl.denom;

	a = abs(a);
	b = abs(b);

	while(b != 0)
	{
		tmp = b;
		b = a % b;
		a = tmp;
	}

	return a;
}
Rational rtnl_simplify(Rational rtnl)
{
	int gcd;

	if(rtnl.num < 0 || rtnl.denom < 0)
	{
		if(rtnl.denom < 0)
		{
			rtnl.num *= -1;
			rtnl.denom *= -1;
		}
	}
	
	/*
	if(rtnl.num < 0 && rtnl.denom > 0)
	{
		rtnl.num *= -1;
		rtnl.denom *= -1;
	}
	else if(rtnl.num > 0 && rtnl.denom < 0)
	{
		rtnl.num *= -1;
		rtnl.denom *= -1;
	}
	else if(rtnl.num < 0 && rtnl.denom < 0)
	{
		rtnl.num *= -1;
		rtnl.denom *= -1;
	}
	*/

	gcd = GCD(rtnl);

	rtnl.num /= gcd;
	rtnl.denom /= gcd;
	
	return rtnl;
}

extern Rational rtnl_add(Rational rtnl0, Rational rtnl1)
{
	Rational rtnl;

	rtnl.num = (rtnl0.num * rtnl1.denom);
	rtnl.num += (rtnl0.denom * rtnl1.num);
	rtnl.denom = (rtnl0.denom * rtnl1.denom);

	rtnl = rtnl_simplify(rtnl);

	return rtnl;
}
extern Rational rtnl_sub(Rational rtnl0, Rational rtnl1)
{
	rtnl1.num *= -1;

	return (rtnl_add(rtnl0, rtnl1));
}
extern Rational rtnl_mul(Rational rtnl0, Rational rtnl1)
{
	Rational rtnl;

	rtnl.num = (rtnl0.num * rtnl1.num);
	rtnl.denom = (rtnl0.denom * rtnl1.denom);

	rtnl = rtnl_simplify(rtnl);

	return rtnl;
}
extern Rational rtnl_div(Rational rtnl0, Rational rtnl1)
{
	int temp = rtnl1.num;
	rtnl1.num = rtnl1.denom;
	rtnl1.denom = temp;

	return rtnl_mul(rtnl0, rtnl1);
}
extern Rational rtnl_ipow(Rational rtnl, int ipow)
{
	Rational base = rtnl;
	int p = abs(ipow);
	int i;
	rtnl = rtnl_simplify(rtnl);
	base = rtnl_simplify(base);

	if(ipow == 0)
	{
		rtnl.num = 1;
		rtnl.denom = 1;
	}
	else
	{
		for(i = 1; i < p; i++)
		{
			rtnl = rtnl_mul(base, rtnl);
		}
	}

	if(ipow < 0)
	{
		base.num = rtnl.num;
		rtnl.num = rtnl.denom;
		rtnl.denom = base.num;
	}

	return rtnl;
}
extern Rational rtnl_init(int num, int denom)
{
	Rational rtnl;

	rtnl.num = num;
	rtnl.denom = denom;

	rtnl = rtnl_simplify(rtnl);

	return rtnl;
}
extern char *rtnl_asStr(Rational rtnl, char buf[RTNL_BUF_SIZE])
{
	sprintf(buf, "(%d/%d)", rtnl.num, rtnl.denom);

	return buf;
}



















