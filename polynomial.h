//
// Created by Once on 2019/6/26.
//

#ifndef DATALGORITHM_POLYNOMIAL_H
#define DATALGORITHM_POLYNOMIAL_H

/**
 * 实现多项式ADT
 * a0*X^n + a1*X^(n-1) + ... + an-1*X
 * 主要存储ai系数和n指数
 * */

// 多项式的每一项表达式
typedef struct expression{
    float coefficient; // 系数
    float exponent; // 指数
} Expression;

// 多项式项结点
typedef struct item{
    Expression expression;
    struct item *next;
} Item;

// 多项式ADT对外接口，使用单链表实现
typedef struct polynomial{
    Item *head; // 头结点
    Item *tail; // 尾结点
    unsigned int size; // 多项式的项数
} Polynomial;

/**
 * 单个多项式自身的运算，包括初始化、检查是否空或已满、添加表达式、获取单项、清除数据
 * */
extern Polynomial* poly_init();
extern int poly_is_empty(Polynomial *polynomial);
extern int poly_is_full(Polynomial *polynomial);
extern int poly_add(Polynomial *polynomial, Expression *expression);
extern Expression* poly_get(Polynomial *polynomial, float exponent);
extern int poly_delete(Polynomial *polynomial, float exponent);
extern int poly_clear(Polynomial *polynomial);

/**
 * 多项式的运算：多项式相加、相减、相乘
 * 单个多项式求导数/微商、积分（定积分和不定积分）
 * */
extern Polynomial* poly_plus(Polynomial *poly1, Polynomial *poly2);
extern Polynomial* poly_subtract(Polynomial *sub1, Polynomial *sub2);
extern Polynomial* poly_multiply(Polynomial *poly1, Polynomial *poly2);

extern Polynomial* poly_differential(Polynomial *poly);
extern Polynomial* poly_in_integral(Polynomial *poly);
extern float poly_integral(Polynomial *poly, float lower, float upper);

// 计算多项式的代数值
extern float poly_algebraic_value(Polynomial *poly, float value);

// 以字符串的形式输出多项式代数式
extern char* poly_algebraic_expression(Polynomial *poly);

#endif //DATALGORITHM_POLYNOMIAL_H
