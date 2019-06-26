//
// Created by Once on 2019/6/26.
//

#include "polynomial.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/**
 * 单个多项式自身的运算，包括初始化、检查是否空或已满、添加表达式、获取单项、清除数据
 * */
Polynomial* poly_init(){
    Polynomial *polynomial = (Polynomial*)malloc(sizeof(Polynomial));
    if(!polynomial){
        perror("init polynomial failed.");
        return NULL;
    }
    polynomial->head = NULL;
    polynomial->tail = NULL;
    polynomial->size = 0;
    return polynomial;
}

int poly_is_empty(Polynomial *polynomial){
    return polynomial->size == 0;
}

int poly_is_full(Polynomial *polynomial){
    Item *item = (Item*)malloc(sizeof(Item));
    if(!item)
        return 1;
    free(item);
    return -1;
}

int poly_add(Polynomial *polynomial, Expression *expression){
    if(polynomial == NULL || expression == NULL)
        return -1;

    Item *item = (Item*)malloc(sizeof(Item));
    if(!item){
        perror("init item failed.");
        return -1;
    }
    item->expression.coefficient = expression->coefficient;
    item->expression.exponent = expression->exponent;
    item->next = NULL;

    if(!polynomial->head){
        polynomial->head = item;
        polynomial->tail = item;
        polynomial->size++;
        return 1;
    }
    Item *node = polynomial->head;
    while(node){
        if(item->expression.exponent == node->expression.exponent){
            node->expression.coefficient += item->expression.coefficient;
            return 1;
        }
        node = node->next;
    }
    polynomial->tail->next = item;
    polynomial->tail = item;
    polynomial->size++;
    return 1;
}

Expression* poly_get(Polynomial *polynomial, float exponent){
    Item *item = polynomial->head;
    while(item){
        if(item->expression.exponent == exponent)
            return &item->expression;
        item = item->next;
    }
    return NULL;
}

int poly_delete(Polynomial *polynomial, float exponent){
    Item *item = polynomial->head;
    if(item->expression.exponent == exponent){
        polynomial->head = item->next;
        free(item);
        polynomial->size--;
        return 1;
    }
    while(item->next){
        Item *temp = item->next;
        if(temp->expression.exponent == exponent){
            item->next = temp->next;
            if(polynomial->tail == temp)
                polynomial->tail = item;
            free(temp);
            polynomial->size--;
            return 1;
        }
        item = item->next;
    }
    return -1;
}

int poly_clear(Polynomial *polynomial){
    Item *item = polynomial->head;
    while(item){
        Item *temp = item->next;
        free(item);
        item = temp;
    }
    return 1;
}


/**
 * 多项式的运算：多项式相加、相减、相乘
 * 单个多项式求导数/微商、积分（定积分和不定积分）
 * */
static void expression_add(Polynomial *polynomial, Item *item){
    while(item){
        Expression expression;
        expression.coefficient = item->expression.coefficient;
        expression.exponent = item->expression.exponent;
        poly_add(polynomial, &expression);
        item = item->next;
    }
}

Polynomial* poly_plus(Polynomial *poly1, Polynomial *poly2){
    if(poly1 == NULL || poly2 == NULL)
        return NULL;
    Polynomial *polynomial = poly_init();
    if(!polynomial)
        return NULL;

    Item *item1 = poly1->head;
    Item *item2 = poly2->head;

    expression_add(polynomial, item1);
    expression_add(polynomial, item2);
    return polynomial;
}

// sub1 - sub2
Polynomial* poly_subtract(Polynomial *sub1, Polynomial *sub2){
    if(sub1 == NULL || sub2 == NULL)
        return NULL;
    Polynomial *polynomial = poly_init();
    if(!polynomial)
        return NULL;

    Item *item1 = sub1->head;
    Item *item2 = sub2->head;
    while(item2){
        item2->expression.coefficient = - item2->expression.coefficient;
        item2 = item2->next;
    }
    item2 = sub2->head;

    expression_add(polynomial, item1);
    expression_add(polynomial, item2);

    item2 = sub2->head;
    while(item2){
        item2->expression.coefficient = - item2->expression.coefficient;
        item2 = item2->next;
    }
    return polynomial;
}

Polynomial* poly_multiply(Polynomial *poly1, Polynomial *poly2){
    if(poly1 == NULL || poly2 == NULL)
        return NULL;
    Polynomial *polynomial = poly_init();
    if(!polynomial)
        return NULL;

    Item *item1 = poly1->head;
    Item *item2 = poly2->head;

    while(item1){
        while(item2){
            Expression expression;
            expression.coefficient = item1->expression.coefficient * item2->expression.coefficient;
            expression.exponent = item1->expression.exponent + item2->expression.exponent;
            poly_add(polynomial, &expression);
            item2 = item2->next;
        }
        item2 = poly2->head;
        item1 = item1->next;
    }

    return polynomial;
}

Polynomial* poly_differential(Polynomial *poly){
    if(poly == NULL)
        return NULL;
    Polynomial *polynomial = poly_init();
    if(!polynomial)
        return NULL;
    Item *item = poly->head;
    while(item){
        Expression expression;
        expression.coefficient = item->expression.coefficient * item->expression.exponent;
        expression.exponent = item->expression.exponent == 0 ? 0 : item->expression.exponent - 1;
        poly_add(polynomial, &expression);
        item = item->next;
    }
    return polynomial;
}

Polynomial* poly_in_integral(Polynomial *poly){
    if(poly == NULL)
        return NULL;
    Polynomial *polynomial = poly_init();
    if(!polynomial)
        return NULL;

    Item *item = poly->head;
    while(item){
        Expression expression;
        expression.coefficient = item->expression.coefficient / (item->expression.exponent + 1);
        expression.exponent = item->expression.exponent + 1;
        poly_add(polynomial, &expression);
        item = item->next;
    }

    return polynomial;
}

float poly_integral(Polynomial *poly, float lower, float upper){
    Polynomial *polynomial = poly_in_integral(poly);
    if(!polynomial){
        perror("can not solve integral function.");
        return -1;
    }

    float result = poly_algebraic_value(polynomial, upper) - poly_algebraic_value(polynomial, lower);
    poly_clear(polynomial);
    return result;
}

// 计算多项式的代数值
extern float poly_algebraic_value(Polynomial *poly, float value){
    if(poly == NULL){
        perror("null pointer exception.");
        return -1;
    }

    float count = 0;
    Item *item = poly->head;
    while(item){
        count += item->expression.coefficient * powf(value, item->expression.exponent);
        item = item->next;
    }
    return count;
}

// 以字符串的形式输出多项式代数式
// 计算整数位数
int int_bit(int value){
    if(value < 0)
        value = - value;
    int bit = 1, exp = 10;
    while(value / exp > 0){
        bit++;
        exp *= 10;
    }
    return bit;
}

char* poly_algebraic_expression(Polynomial *poly){
    if(poly == NULL)
        return NULL;

    int len = 0;

    Item *item = poly->head;
    int len_arr[poly->size];
    int i = 0;
    while(item){
        int temp = int_bit((int)item->expression.coefficient) + int_bit((int)item->expression.exponent);
        len_arr[i] = temp;
        len = len + temp;
        item = item->next;
        i++;
    }
    item = poly->head;

    len = len + 5 * poly->size;
    char *str = (char*)malloc(len * sizeof(char));
    memset(str, 0, len * sizeof(char));
    i = 0;
    while(item){
        char temp[len_arr[i]];
        char *exp = "%.0fX^%.0f";
        float coefficient = item->expression.coefficient;
        if(coefficient < 0 && i != 0){
            coefficient = - coefficient;
            exp = " - %.0fX^%.0f";
        }
        if(item->expression.coefficient > 0 && i != 0){
            exp = " + %.0fX^%.0f";
        }
        sprintf(temp, exp, coefficient, item->expression.exponent);
        strcat(str, temp);
        item = item->next;
        i++;
    }
    return str;
}