#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <iostream>
#include <vector>
#include <cmath>

class Expr
{
    public:
        virtual double eval() = 0;
        virtual std::string description() = 0;

        virtual int getPriority()
        {
            return 9;
        }

        virtual bool isLeftHanded()
        {
            std::cout << "Executed isLeftHanded from Expr class - error.\n";
            return true;
        }

        virtual ~Expr(){};
};

class Number : public Expr
{
    public:
        double value;

        Number (double value);

        std::string description();
        double eval();
};

class Variable : public Expr
{
    private:
        static std::vector<std::pair<std::string, double>> variables;

    public:
        static void addVariable(std::string var, double val);
        static void printVariables();
        static void removeVariable(std::string var);

        std::string Variable_name;
        
        Variable(std::string var);

        std::string description();
        double eval();
};

class Const : public Expr
{
    protected:
        double value;
};

class pi : public Const
{
    public:
        std::string description();
        double eval();
        pi();
};

class e : public Const
{
    public:
        std::string description();
        double eval();
        e();
};

class fi : public Const
{
    public:
        std::string description();
        double eval();
        fi();
};

class UnaryOp : public Expr
{
    public:
        Expr *arg1;
        UnaryOp(Expr *arg1);
};

class Opposite : public UnaryOp
{
    public:
        std::string description();
        double eval();
        Opposite(Expr *arg1);
};

class Reciprocal : public UnaryOp
{
    public:
        std::string description();
        double eval();
        Reciprocal(Expr *arg1);
};

class Abs : public UnaryOp
{
    public:
        std::string description();
        double eval();
        Abs(Expr *arg1);
};

class Exp : public UnaryOp
{
    public:
        std::string description();
        double eval();
        Exp(Expr *arg1);
};

class Ln : public UnaryOp
{
    public:
        std::string description();
        double eval();
        Ln(Expr *arg1);
};

class Sin : public UnaryOp
{
    public:
        std::string description();
        double eval();
        Sin(Expr *arg1);
};

class Cos : public UnaryOp
{
    public:
        std::string description();
        double eval();
        Cos(Expr *arg1);
};

class BinaryOp : public UnaryOp
{
    public:
        int getPriority() override
        {
            return 1;
        }

        Expr *arg2;
        BinaryOp(Expr *arg1, Expr *arg2);
        virtual bool isLeftHanded() = 0;
};

class Add : public BinaryOp
{
    public:
        std::string description();
        double eval();

        bool isLeftHanded() override
        {
            return true;
        }

        Add(Expr *arg1, Expr *arg2) : BinaryOp(arg1, arg2) {}
};

class Sub : public BinaryOp
{
    public:
        std::string description();
        double eval();

        bool isLeftHanded() override
        {
            return true;
        }

        Sub(Expr *arg1, Expr *arg2) : BinaryOp(arg1, arg2) {}
};

class Mul : public BinaryOp
{
    public:
        std::string description();
        double eval();

        int getPriority() override
        {
            return 2;
        }

        bool isLeftHanded() override
        {
            return true;
        }

        Mul(Expr *arg1, Expr *arg2) : BinaryOp(arg1, arg2) {}
};

class Div : public BinaryOp
{
    public:
        std::string description();
        double eval();

        int getPriority() override
        {
            return 2;
        }

        bool isLeftHanded() override
        {
            return true;
        }

        Div(Expr *arg1, Expr *arg2) : BinaryOp(arg1, arg2) {}
};

class Pow : public BinaryOp
{
    public:
        std::string description();
        double eval();

        int getPriority() override
        {
            return 3;
        }

        bool isLeftHanded() override
        {
            return true;
        }

        Pow(Expr *arg1, Expr *arg2) : BinaryOp(arg1, arg2) {}
};

class Mod : public BinaryOp
{
    public:
        std::string description();
        double eval();

        int getPriority() override
        {
            return 3;
        }

        bool isLeftHanded() override
        {
            return true;
        }

        Mod(Expr *arg1, Expr *arg2) : BinaryOp(arg1, arg2) {}
};

class Log : public BinaryOp
{
    public:
        std::string description();
        double eval();

        int getPriority() override
        {
            return 3;
        }

        bool isLeftHanded() override
        {
            return true;
        }

        Log(Expr *arg1, Expr *arg2) : BinaryOp(arg1, arg2) {}
};

#endif