#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "polynomial.h"

void print_expression(Polynomial *poly, char *message){
    char *expression1 = poly_algebraic_expression(poly);
    printf("%s%s\n", message, expression1);
    free(expression1);
}

void polynomial(void){
    // -6X^2 + 2X + 5
    Polynomial *poly1 = poly_init();
    Expression exp1;
    exp1.coefficient = 5;
    exp1.exponent = 0;
    Expression exp2;
    exp2.coefficient = -6;
    exp2.exponent = 3;
    Expression exp3;
    exp3.coefficient = 2;
    exp3.exponent = 1;
    poly_add(poly1, &exp1);
    poly_add(poly1, &exp2);
    poly_add(poly1, &exp3);

    // 18X^5 - 3X^2 - 9X + 7
    Polynomial *poly2 = poly_init();
    Expression exp4;
    exp4.coefficient = -3;
    exp4.exponent = 2;
    Expression exp5;
    exp5.coefficient = 18;
    exp5.exponent = 5;
    Expression exp6;
    exp6.coefficient = -8;
    exp6.exponent = 1;
    Expression exp7;
    exp7.coefficient = 7;
    exp7.exponent = 0;
    poly_add(poly2, &exp4);
    poly_add(poly2, &exp5);
    poly_add(poly2, &exp6);
    poly_add(poly2, &exp7);

    // 按字符串输出多项式代数表达式
    print_expression(poly1, "A:     ");
    print_expression(poly2, "B:     ");

    // 代数值
    float value1 = poly_algebraic_value(poly1, 3);
    float value2 = poly_algebraic_value(poly2, 2);
    printf("%f %f\n", value1, value2);

    // 加减乘
    Polynomial *plus = poly_plus(poly1, poly2);
    print_expression(plus, "A + B: ");
    Polynomial *sub = poly_subtract(poly1, poly2);
    print_expression(sub, "A - B: ");
    Polynomial *multiply = poly_multiply(poly1, poly2);
    print_expression(multiply, "A x B: ");

    // 导数/微商、不定积分、定积分
    Polynomial *diff = poly_differential(poly1);
    print_expression(diff, "D[A]:  ");
    Polynomial *in_inte = poly_in_integral(poly2);
    print_expression(in_inte, "In[B]: ");
    float result = poly_integral(poly2, -5, 15);
    printf("In[B, -5, 15]: %.0f\n", result);

    poly_clear(plus);
    poly_clear(sub);
    poly_clear(multiply);
    poly_clear(diff);
    poly_clear(in_inte);
    poly_clear(poly1);
    poly_clear(poly2);
}

int main() {
    polynomial();
    return 0;
}