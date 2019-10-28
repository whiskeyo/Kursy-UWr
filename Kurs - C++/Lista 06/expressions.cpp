#include "expressions.hpp"
std::vector<std::pair<std::string, double>> Variable::variables;

Number::Number(double value)
{
    this->value = value;
}

std::string Number::description()
{
    std::string str = std::to_string(value);
    int offset = 1;
    
    if (str.find_last_not_of('0') == str.find('.'))
        offset = 0;
    
    str.erase(str.find_last_not_of('0') + offset, std::string::npos);

    return str;
}

double Number::eval()
{
    return value;
}

Variable::Variable(std::string var)
{
    this->Variable_name = var;
}

std::string Variable::description()
{
    return Variable_name;
}

double Variable::eval()
{
    try
    {
        int how_many = Variable::variables.size();
        for (int i = 0; i < how_many; i++)
            if (Variable::variables[i].first == Variable_name)
                return Variable::variables[i].second;

        std::string err = "Variable not found.";
        throw err;
    }
    catch(std::string err_msg)
    {
        std::cerr << "Error in static variables: " << err_msg << " while executing eval.\n";
        std::cerr << "Returning 1 instead of " << Variable_name << ".\n";
        return 1;
    }
}

void Variable::addVariable(std::string var, double val)
{
    int how_many = Variable::variables.size();
    for (int i = 0; i < how_many; i++)
    {
        if (Variable::variables[i].first == var)
        {
            std::cout << "Variable " << var << " already exists, its value equals "
            << Variable::variables[i].second << std::endl;
            return;
        }
    }

    Variable::variables.push_back(make_pair(var, val));
}

void Variable::removeVariable(std::string var)
{
    try
    {
        int how_many = Variable::variables.size();
        for (int i = 0; i < how_many; i++)
        {
            if (Variable::variables[i].first == var)
            {
                Variable::variables.erase(Variable::variables.begin() + i);
                return;
            }
        }
        std::string err = "Variable not found.";
        throw err;
    }
    catch (std::string err)
    {
        std::cout << "Error in static variables: " << err << " while executing removeVariable.\n";
        std::cout << "Array of variables does not change." << std::endl;
    }
}

void Variable::printVariables()
{
    int how_many = Variable::variables.size();

    std::cout << "Stored variables: " << std::endl;
    for (int i = 0; i < how_many; i++)
        std::cout << Variable::variables[i].first << "\t" << Variable::variables[i].second << std::endl;
}

pi::pi()
{
    value = 3.141592;
}

std::string pi::description()
{
    return "pi";
}

double pi::eval()
{
    return value;
}

e::e()
{
    value = 2.718281;
}

std::string e::description()
{
    return "e";
}

double e::eval()
{
    return value;
}

fi::fi()
{
    value = 3.141592;
}

std::string fi::description()
{
    return "fi";
}

double fi::eval()
{
    return value;
}

UnaryOp::UnaryOp(Expr *arg1)
{   
    this->arg1 = arg1;
}

Opposite::Opposite(Expr *arg1) : UnaryOp(arg1) {}

std::string Opposite::description()
{
    return "-(" + arg1->description() + ")";
}

double Opposite::eval()
{
    return (-1) * arg1->eval();
}

Reciprocal::Reciprocal(Expr *arg1) : UnaryOp(arg1) {}

std::string Reciprocal::description()
{
    return "1 / " + arg1->description();
}

double Reciprocal::eval()
{
    return 1.0 / arg1->eval();
}

Abs::Abs(Expr *arg1) : UnaryOp(arg1) {}

std::string Abs::description()
{
    return "|" + arg1->description() + "|";
}

double Abs::eval()
{
    if (arg1->eval() < 0)
        return (-1) * arg1->eval();
    else
        return arg1->eval();
}

Exp::Exp(Expr *arg1) : UnaryOp(arg1) {}

std::string Exp::description()
{
    return "exp(" + arg1->description() + ")";
}

double Exp::eval()
{
    return exp(arg1->eval());
}

Ln::Ln(Expr *arg1) : UnaryOp(arg1) {}

std::string Ln::description()
{
    return "ln(" + arg1->description() + ")";
}

double Ln::eval()
{
    return log(arg1->eval());
}

Sin::Sin(Expr *arg1) : UnaryOp(arg1) {}

std::string Sin::description()
{
    return "sin(" + arg1->description() + ")";
}

double Sin::eval()
{
    return sin(arg1->eval());
}

Cos::Cos(Expr *arg1) : UnaryOp(arg1) {}

std::string Cos::description()
{
    return "cos(" + arg1->description() + ")";
}

double Cos::eval()
{
    return cos(arg1->eval());
}

BinaryOp::BinaryOp(Expr *arg1, Expr *arg2) : UnaryOp(arg1)
{
    this->arg2 = arg2;
}

std::string Add::description()
{
    std::string left, right;

    if (arg1->getPriority() > getPriority())
        left = arg1->description();
    else if (arg1->getPriority() < getPriority())
        left = "(" + arg1->description() + ")";
    else if (arg1->isLeftHanded() == true)
        left = arg1->description(); // the same priority, checks whether it is left-sided operator
    else
        left = "(" + arg1->description() + ")";

    if (arg2->getPriority() > getPriority())
        right = arg2->description();
    else if (arg2->getPriority() < getPriority())
        right = "(" + arg2->description() + ")";
    else if (arg2->isLeftHanded() == true)
        right = arg2->description(); // the same priority, checks whether it is left-sided operator
    else
        right = "(" + arg2->description() + ")";

    return left + " + " + right;
}

double Add::eval()
{
    return arg1->eval() + arg2->eval();
}

std::string Sub::description()
{
    std::string left, right;

    if (arg1->getPriority() > getPriority())
        left = arg1->description();
    else if (arg1->getPriority() < getPriority())
        left = "(" + arg1->description() + ")";
    else if (arg1->isLeftHanded() == true)
        left = arg1->description(); // the same priority, checks whether it is left-sided operator
    else
        left = "(" + arg1->description() + ")";

    if (arg2->getPriority() > getPriority())
        right = arg2->description();
    else if (arg2->getPriority() < getPriority())
        right = "(" + arg2->description() + ")";
    else if (arg2->isLeftHanded() == true)
        right = arg2->description(); // the same priority, checks whether it is left-sided operator
    else
        right = "(" + arg2->description() + ")";

    return left + " - " + right;
}

double Sub::eval()
{
    return arg1->eval() - arg2->eval();
}

std::string Mul::description()
{
    std::string left, right;

    if (arg1->getPriority() > getPriority())
        left = arg1->description();
    else if (arg1->getPriority() < getPriority())
        left = "(" + arg1->description() + ")";
    else if (arg1->isLeftHanded() == true)
        left = arg1->description(); // the same priority, checks whether it is left-sided operator
    else
        left = "(" + arg1->description() + ")";

    if (arg2->getPriority() > getPriority())
        right = arg2->description();
    else if (arg2->getPriority() < getPriority())
        right = "(" + arg2->description() + ")";
    else if (arg2->isLeftHanded() == true)
        right = arg2->description(); // the same priority, checks whether it is left-sided operator
    else
        right = "(" + arg2->description() + ")";

    return left + " * " + right;
}

double Mul::eval()
{
    return arg1->eval() * arg2->eval();
}

std::string Div::description()
{
    std::string left, right;

    if (arg1->getPriority() > getPriority())
        left = arg1->description();
    else if (arg1->getPriority() < getPriority())
        left = "(" + arg1->description() + ")";
    else if (arg1->isLeftHanded() == true)
        left = arg1->description(); // the same priority, checks whether it is left-sided operator
    else
        left = "(" + arg1->description() + ")";

    if (arg2->getPriority() > getPriority())
        right = arg2->description();
    else if (arg2->getPriority() < getPriority())
        right = "(" + arg2->description() + ")";
    else if (arg2->isLeftHanded() == true)
        right = arg2->description(); // the same priority, checks whether it is left-sided operator
    else
        right = "(" + arg2->description() + ")";

    return left + " / " + right;
}

double Div::eval()
{
    return arg1->eval() / arg2->eval();
}

std::string Pow::description()
{
    std::string left, right;

    if (arg1->getPriority() > getPriority())
        left = arg1->description();
    else if (arg1->getPriority() < getPriority())
        left = "(" + arg1->description() + ")";
    else if (arg1->isLeftHanded() == true)
        left = arg1->description(); // the same priority, checks whether it is left-sided operator
    else
        left = "(" + arg1->description() + ")";

    if (arg2->getPriority() > getPriority())
        right = arg2->description();
    else if (arg2->getPriority() < getPriority())
        right = "(" + arg2->description() + ")";
    else if (arg2->isLeftHanded() == true)
        right = arg2->description(); // the same priority, checks whether it is left-sided operator
    else
        right = "(" + arg2->description() + ")";

    return left + " ^ " + right;
}

double Pow::eval()
{
    return pow(arg1->eval(), arg2->eval());
}

std::string Mod::description()
{
    std::string left, right;

    if (arg1->getPriority() > getPriority())
        left = arg1->description();
    else if (arg1->getPriority() < getPriority())
        left = "(" + arg1->description() + ")";
    else if (arg1->isLeftHanded() == true)
        left = arg1->description(); // the same priority, checks whether it is left-sided operator
    else
        left = "(" + arg1->description() + ")";

    if (arg2->getPriority() > getPriority())
        right = arg2->description();
    else if (arg2->getPriority() < getPriority())
        right = "(" + arg2->description() + ")";
    else if (arg2->isLeftHanded() == true)
        right = arg2->description(); // the same priority, checks whether it is left-sided operator
    else
        right = "(" + arg2->description() + ")";

    return left + " % " + right;
}

double Mod::eval()
{
    return fmod(arg1->eval(), arg2->eval());
}

std::string Log::description()
{
    return "log(" + arg1->description() + ", " + arg2->description() + ")";
}

double Log::eval()
{
    double e1, e2;

    e1 = log (arg1->eval());
    e2 = log (arg2->eval());

    return e2 / e1;
}