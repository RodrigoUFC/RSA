#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define	MAXDIGITS	1000		/* maximum length largeInt */

typedef struct {
    char digits[MAXDIGITS];         /* Represent the number */
    int lastDigit;			/* Index of highest-order digit */
} largeInt;

void create_largeInt(largeInt *n, char digits[], int sizeM)
{
    sizeM--;
    int i;
    n->lastDigit = 0;
    for (i=0; i<MAXDIGITS; i++)
        n->digits[i] = 0;
    for (i=0; i<sizeM; i++)
        n->digits[i] = digits[(sizeM-1)-i];
    n->lastDigit = i-1;
}

void initialize_largeInt(largeInt *n)
{
    int i;
    n->lastDigit = 0;
    for (i=0; i<MAXDIGITS; i++)
        n->digits[i] = 0;
}

void char_to_int(largeInt *n)
{
    for (int j=n->lastDigit; j>=0; j--)
    {
        n->digits[j] = n->digits[j] - '0';
    }
}

void int_to_char(largeInt *n)
{
    for (int j=n->lastDigit; j>=0; j--)
    {
        n->digits[j] = n->digits[j] + '0';
    }
}

void subtract_largeInt(largeInt *a, largeInt *b, largeInt *c)
{
    int borrow = 0, i = 0;

    char_to_int(a);
    char_to_int(b);
    for (i=b->lastDigit+1; i<MAXDIGITS; i++)
        b->digits[i] = 0;
    initialize_largeInt(c);

    c->lastDigit = ((a->lastDigit) > (b->lastDigit) ? (a->lastDigit) : (b->lastDigit));//Allows enough space to be allocated

    for(i = 0; i <= c->lastDigit; i++)
    {
        if (a->digits[i]<b->digits[i])
        {
                borrow = 10;
                a->digits[i+1] = a->digits[i+1] - 1;
        } else borrow = 0;
        c->digits[i] = (borrow + a->digits[i] - b->digits[i]); //Module 10 of the sum
    }
    while ((c->lastDigit > 0) && ((c->digits[c->lastDigit] == 0)))
        c->lastDigit--;
}

void add_largeInt(largeInt *a, largeInt *b, largeInt *c)
{
    int carry = 0, i = 0;

    char_to_int(a);
    char_to_int(b);

    initialize_largeInt(c);

    c->lastDigit = ((a->lastDigit) > (b->lastDigit) ? (a->lastDigit) : (b->lastDigit)) + 1;//Allows enough space to be allocated

    for(i = 0; i <= c->lastDigit; i++)
    {
        if(i==c->lastDigit)
            c->digits[i] = carry;
        else if (i>a->lastDigit)
            c->digits[i] = (carry + b->digits[i]) % 10; //Module 10 of the sum
        else if (i>b->lastDigit)
            c->digits[i] = (carry + a->digits[i]) % 10; //Module 10 of the sum
        else
            c->digits[i] = (carry + a->digits[i] + b->digits[i]) % 10; //Module 10 of the sum
        carry = (carry + a->digits[i] + b->digits[i]) / 10;
    }
    while ((c->lastDigit > 0) && ((c->digits[c->lastDigit] == 0)))
		c->lastDigit--;
}

void digit_shift(largeInt *n, int d)		/* multiply n by 10^d */
{
	int i;				/* counter */

	if (((n->lastDigit == 0) && (n->digits[0] == 0)) || d == 0) return;

	for (i=n->lastDigit; i>=0; i--)
		n->digits[i+d] = n->digits[i];

	for (i=0; i<d; i++) n->digits[i] = 0;

	n->lastDigit = n->lastDigit + d;
}

void multiply_largeInt(largeInt *a, largeInt *b, largeInt *c)
{
	largeInt row;			/* represent shifted row */
	largeInt intResult;			/* intermediate result largeInt */
	int carry = 0;

	initialize_largeInt(c);
	initialize_largeInt(&intResult);

    char_to_int(a);
    char_to_int(b);

	for (int i=0; i<=b->lastDigit; i++) {
        c->lastDigit = ((a->lastDigit) > (b->lastDigit) ? (a->lastDigit) : (b->lastDigit)) + 1;
		for (int j=0; j<=c->lastDigit; j++) {
            c->digits[j] = ((carry + (a->digits[j] * b->digits[i])) % 10); //Module 10 of the sum
            carry = (carry + (a->digits[j] * b->digits[i])) / 10;
		}

        digit_shift(c,i);

		int_to_char(c);
		int_to_char(&intResult);

		add_largeInt(c,&intResult,&row);

		intResult = row;
	}
	c->lastDigit = intResult.lastDigit;
	for (int j=0; j<=intResult.lastDigit; j++)
        c->digits[j] = intResult.digits[j];
    while ((c->lastDigit > 0) && (c->digits[c->lastDigit] == 0))
        c->lastDigit--;
}

int bigger_value(largeInt *a, largeInt *b)
{
    if(a->lastDigit > b->lastDigit)
        return 1;
    else if(a->lastDigit < b->lastDigit)
        return 2;
    else
    {
        for(int i = a->lastDigit; i >= 0; i--)
        {
            if(a->digits[i] > b->digits[i])
                return 1;
            if(a->digits[i] < b->digits[i])
                return 2;
        }
        return 0;
    }
}

void modulo_largeInt(largeInt *a, largeInt *b, largeInt *c)
{
    largeInt intResult;

    initialize_largeInt(&intResult);
    initialize_largeInt(c);

    if(bigger_value(a,b) == 1)
    {
        c->lastDigit = a->lastDigit;
        for (int j=0; j <= a->lastDigit; j++){
            c->digits[j] = a->digits[j];
        }
        while(bigger_value(c,b) != 2)
        {
            subtract_largeInt(c,b,&intResult);
            int_to_char(&intResult);
            int_to_char(b);

            c->lastDigit = intResult.lastDigit;
            for (int j=0; j<=intResult.lastDigit; j++)
                c->digits[j] = intResult.digits[j];
        }
        char_to_int(c);
    } else if (bigger_value(a,b) == 2)
    {
        c->lastDigit = a->lastDigit;
        for (int j=0; j<=a->lastDigit; j++)
            c->digits[j] = a->digits[j];
        char_to_int(c);
    }
}

void modularSum_largeInt(largeInt *a, largeInt *b, largeInt *m, largeInt *c)
{
    largeInt moduloA;
    largeInt moduloB;
    largeInt sumModulo;

    initialize_largeInt(&moduloA);
    initialize_largeInt(&moduloB);
    initialize_largeInt(&sumModulo);
    initialize_largeInt(c);

    modulo_largeInt(a,m,&moduloA);
    modulo_largeInt(b,m,&moduloB);

    int_to_char(&moduloA);
    int_to_char(&moduloB);

    add_largeInt(&moduloA,&moduloB,&sumModulo);

    int_to_char(&sumModulo);

    modulo_largeInt(&sumModulo,m,c);
}

void modularSubtract_largeInt(largeInt *a, largeInt *b, largeInt *m, largeInt *c)
{
    largeInt moduloA;
    largeInt moduloB;
    largeInt sumModulo;

    initialize_largeInt(&moduloA);
    initialize_largeInt(&moduloB);
    initialize_largeInt(&sumModulo);
    initialize_largeInt(c);

    modulo_largeInt(a,m,&moduloA);
    modulo_largeInt(b,m,&moduloB);

    int_to_char(&moduloA);
    int_to_char(&moduloB);

    subtract_largeInt(&moduloA,&moduloB,&sumModulo);

    int_to_char(&sumModulo);
    modulo_largeInt(&sumModulo,m,c);
}

int main()
{
    largeInt a;
    largeInt b;
    largeInt c;
    largeInt m;
    //char n1[] = "9230674085814934751107402307045227470354027117063142732318790172446966357515515421947785380455793822284099022569246906027295607848040292901094567664230197"; //512 bit number p
    //char n2[] = "11629004838073748156647564488681050840370021607699951657371680300260223119319218030318292642364868044043065118640339011082831826711696088399013225837436991"; //512 bit number q
    //char n1[] = "45059";
    //char n2[] = "26279";
    char modulo[] = "91271981396293721816284051075950197960572157851148300684300308702564059302132740702344608165968565759415739329105642091505483167616017628766568477896999098903463007958209579909332057913619466570559779924680974106397215944447195079650763284528642426285528637591995708954175893628937019413161031783989286930607"; //1024 bit number e
    char n1[] = "32219906772714416866643912866710021116381418394442883817140318333012160784511807280979808521271738286434244559391047187447211536416938872403288519745096647518363829727783638014519944120778140629268010939374829729147389802359678459349561771478019959834308231312072161696434507773354139846425767705906193872329"; //1024 bit number p
    char n2[] = "118514897135112702832131584298191703869303122337588769666013383278029821053629225225628019665965684275733110409691382238312506707030292346324557723179621061903788794840097883236205413339380810367504452908890550320395338754910941038316882266040172628460109540918825477843915461268428218453255724540136628387757"; //1024 bit number q

    create_largeInt(&a,n1,sizeof(n1));
    create_largeInt(&b, n2, sizeof(n2));
    create_largeInt(&m, modulo, sizeof(modulo));

    clock_t begin = clock();

    for (int i=0;i<10000;i++) {
        modularSum_largeInt(&a,&b,&m,&c);
        create_largeInt(&a,n1,sizeof(n1));
        create_largeInt(&b, n2, sizeof(n2));
        create_largeInt(&m, modulo, sizeof(modulo));
    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    //add_largeInt(&a, &b, &c);
    //subtract_largeInt(&b, &a, &c);
    //multiply_largeInt(&a, &b, &c);
    //modulo_largeInt(&b,&m,&c);
    //modularSum_largeInt(&a,&b,&m,&c);
    //modularSubtract_largeInt(&a,&b,&m,&c);
    //printf("\n%d ",d);
    printf("A: ");
    for (int j=a.lastDigit; j>=0; j--)
    {
        printf("%d",a.digits[j]-'0');
    }
    printf("\n\nB: ");
    for (int j=b.lastDigit; j>=0; j--)
    {
        printf("%d",b.digits[j]-'0');
    }
    printf("\n\nAddition Modulaire: ");
    for (int j=c.lastDigit; j>=0; j--)
    {
        printf("%d",c.digits[j]);
    }
    printf("\nRuntime: %f",time_spent/10000);
    return 0;
}

