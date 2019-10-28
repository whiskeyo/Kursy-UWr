#include <iostream>
#include <vector>
#include "expressions.hpp"

int main()
{    
    Variable::addVariable("x", 0.01);
    Expr *w1 = new Div(
        new Mul(
            new Sub(
                new Variable("x"),
                new Number(1)),
            new Variable("x")),
        new Number(2));
    std::cout << w1->eval() << "\t" << w1->description() << std::endl;
    Variable::removeVariable("x");
    Variable::addVariable("x", 0.5);
    std::cout << w1->eval() << "\t" << w1->description() << std::endl;
    Variable::removeVariable("x");
    Variable::addVariable("x", 1);
    std::cout << w1->eval() << "\t" << w1->description() << std::endl;

    Expr *w2 = new Div(
        new Add(new Number(3), new Number(5)),
        new Add(new Number(2),
                  new Mul(new Variable("x"), new Number(7))));
    Variable::removeVariable("x");
    Variable::addVariable("x", 0.01);
    std::cout << w2->eval() << "\t" << w2->description() << std::endl;
    Variable::removeVariable("x");
    Variable::addVariable("x", 0.5);
    std::cout << w2->eval() << "\t" << w2->description() << std::endl;
    Variable::removeVariable("x");
    Variable::addVariable("x", 1);
    std::cout << w2->eval() << "\t" << w2->description() << std::endl;

    Expr *w3 = new Sub(
        new Add(
            new Number(2),
            new Mul(new Variable("x"), new Number(7))),
        new Add(
            new Mul(new Variable("y"), new Number(3)),
            new Number(5)));

    Variable::removeVariable("x");
    Variable::addVariable("x", 0.01);
    Variable::addVariable("y", 0.01);
    std::cout << w3->eval() << "\t" << w3->description() << std::endl;
    Variable::removeVariable("x");
    Variable::addVariable("x", 0.5);
    Variable::removeVariable("y");
    Variable::addVariable("y", 0.5);
    std::cout << w3->eval() << "\t" << w3->description() << std::endl;
    Variable::removeVariable("x");
    Variable::addVariable("x", 1);
    Variable::removeVariable("y");
    Variable::addVariable("y", 1);
    std::cout << w3->eval() << "\t" << w3->description() << std::endl;

    Expr *w4 = new Div(
        new Cos(new Mul(
            new Add(new Variable("x"), new Number(1)),
            new Variable("x"))),
        new Pow(
            new Pow(new e(), new Variable("x")),
            new Number(2))

    );

    Variable::removeVariable("x");
    Variable::addVariable("x", 0.01);
    std::cout << w4->eval() << "\t" << w4->description() << std::endl;
    Variable::removeVariable("x");
    Variable::addVariable("x", 0.5);
    std::cout << w4->eval() << "\t" << w4->description() << std::endl;
    Variable::removeVariable("x");
    Variable::addVariable("x", 1);
    std::cout << w4->eval() << "\t" << w4->description() << std::endl;

    return 0;
}