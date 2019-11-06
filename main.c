/* Reverse polish (postfix) notation computation 
Программа для интерпретации обратной польской записи и её последующего вычисления.
В командной строке консоли задается:
<имя_программы> X1 X2 X3 .. Xn P1 P2 P3 ..Pn-1
где имя_программы - имя исполняемого файла
    X1,... - операнды
    P1,... - арифметические операторы из списка: * / + -
Запись преобразуется к виду
((Xn P1 Xn-1) P2 Xn-2) P3 Xn-3) и т.д.
Число операторов должно быть на единицу меньше числа операндов, прочие операторы отбрасываются.
Программа обрабатывает не более 50 операндов.

Для операндов допустимы целые и вещественные числа вида:
1.23
-2
2E+06 (это 2*10^6)
-3.2e+2
7.34e-10

Примеры задания строк:
Строка вида
2 3 +
Считывается как
3.00+2.00
Строка 
2.2 4E+2 -3 + *
Cчитывается как
(-3.00+400.00)*2.20 
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

char help_str_ru[] = "Вычисление по обратной польской записи.\n\n\
<имя_программы> X1 X2 X3 .. Xn P1 P2 P3 ..Pn-1\n\n\
    <имя_программы>  имя исполняемого файла\n\
    X1,...           операнды-числа, к примеру: 1.23 -2 2E+06 -3.2e+2 7.34e-10\n\
    P1,...           арифметические операторы из списка: * / + - \n\
    Число операндов не более 50,\nоператоров должно быть на единицу меньше.\n\n\
Выражение вычисляется согласно виду:\n\
((Xn P1 Xn-1) P2 Xn-2) P3 Xn-3) ... \n\
";

char help_str_en[] = "Computation according to reverse polish (postfix) notation\n\n\
<program_name> X1 X2 X3 .. Xn P1 P2 P3 .. Pn-1\n\n\
    <program_name>  executive module name\n\
    X1,...          operands: 1.23 -2 2E+06 -3.2e+2 7.34e-10\n\
    P1,...          arithmetic operators: * / + - \n\n\
Quantity of operands shall be not more than 50,\noperators shall be less by one.\n\
Notation is computed in the following order:\n\
((Xn P1 Xn-1) P2 Xn-2) P3 Xn-3) ...\n\
";

enum HELP_LANG {RUS_HELP, EN_HELP};

int isoperand(char c);
inline static int print_help(int l)
{
    return l == RUS_HELP ? printf("%s", help_str_ru) : printf("%s", help_str_en);
}


int main(int argc, char *argv[])
{
	int i, j, m, k, operand_exist;
        #define L 50
	char s[50] = "", c;
	double ops[L], r = 0.00;
	char op[L] = "";
  
        /* вывод справки при необходимости */
        if (argc == 1)
        {       
            if (setlocale (LC_ALL, "ru_RU.UTF-8") == NULL)
                print_help(EN_HELP);
            else
                print_help(RUS_HELP);
            return 0;
        }
              
	/* вывод аргументов */
	printf("The result of:\n");
	for (i = 1; i < argc; i++)
	{
		printf("%s ", argv[i]);
	}
	printf("\n");

	/* чтение операндов */
	m = 0;
	operand_exist = 0;
	for (i = 1; i < argc; i++)
	{
		c = *argv[i];
		j = 0;
		if (c == '-' && isdigit(*(argv[i]+1)))
		{
			s[j] = *(argv[i]+j);
			j++;
			c = *(argv[i]+j);
		}
		if (isdigit(c))
		{
			while (c != '\0' && isdigit(c))
			{
				s[j] = *(argv[i]+j);
				j++;
				c = *(argv[i]+j);
			}
			if (c == '.' && isdigit(*(argv[i]+j+1)))
			{
				s[j] = c;
				j++;
				c = *(argv[i]+j);
				while (c != '\0' && isdigit(c))
				{
					s[j] = c;
					j++;
					c = *(argv[i]+j);
				}
			}
			if ((c == 'e' || c == 'E') && (*(argv[i]+j+1) == '+' || *(argv[i]+j+1) == '-') && isdigit(*(argv[i]+j+2))) 
			{
				s[j] = c;
				j++;
				c = *(argv[i]+j);
				s[j] = c;
				j++;
				c = *(argv[i]+j);
				while (c != '\0' && isdigit(c))
				{
					s[j] = c;
					j++;
					c = *(argv[i]+j);
				}
			}
			if (c != '\0' && !isdigit(c))
			{
				printf("\nError. Incorrect number.");
				getchar();
				return -1;
			}
			s[j] = '\0';
			ops[m++] = atof(s);
			operand_exist = 1;
		}
		else
			break;
	}
	if (m > 0)
		m--;

	/* чтение операторов */
	k = 0;
	for (; i < argc; i++)
	{
		if (isoperand(*argv[i]) && *(argv[i]+1) == '\0')
		{
			op[k++] = *argv[i];
			if (k == m)
				break;
		}
		else
		{
			printf("\nError. Incorrect operator.");
			getchar();
			return -2;
		}
	}
	if (k > 0)
		k--;

	/* осуществление операций и вывод на экран последовательности действий */
	printf("will be:\n");
	if (m > 0 && op[0] != '\0')
	{
		r = ops[m];
		printf("%.2f", ops[m]); 
		m--;
		for (; m >= 0; m--)
		{
			switch (op[k-m])
			{
			case '*':
				r = r * ops[m];
				printf(" %c ", op[k-m]);
				break;
			case '-':
				r = r - ops[m];
				printf(" %c ", op[k-m]);
				break;
			case '+':
				r = r + ops[m];
				printf(" %c ", op[k-m]);
				break;
			case '/':
				if (ops[m] != 0)
				{
					r = r / ops[m];
					printf(" %c ", op[k-m]);
				}
				else
				{
					printf("\nError. Div by null.");
					getchar();
					return -3;
				}
				break;
			default:
				break;
			}
			printf("%.2f = %.2f\n", ops[m], r);
		}
	}
	getchar();
	return 0;
}

int isoperand(char c)
{
	switch (c)
	{
	case '*': case '+': case '-': case '/':
		return c;		
	default:
		return 0;	
	}
}