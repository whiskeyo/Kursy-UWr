import matplotlib.pyplot as plt
import numpy as np
import math
import decimal
import scipy, scipy.integrate

# np.set_printoptions(suppress=True)

def f(x):
    return np.exp(-x**2 / 2)


def draw_whole_plot(func, a, b, interval, name):
    x = np.linspace(a, b, interval)
    y = np.exp(-x**2 / 2)

    fig = plt.figure()
    # ax = fig.add_subplot(1, 1, 1)
    # ax.spines['left'].set_position('center')
    # ax.spines['bottom'].set_position('center')
    # ax.spines['right'].set_color('none')
    # ax.spines['top'].set_color('none')
    # ax.xaxis.set_ticks_position('bottom')
    # ax.yaxis.set_ticks_position('left')

    plt.xlim([a, b])
    plt.ylim([0, 1.1])

    section = np.arange(a, 1, 1/100.)
    plt.fill_between(section, f(section), alpha=0.4)

    plt.plot(x, y, 'g')
    # plt.show()
    plt.savefig("./zadanie1b.png")

a = -50
b = 1

I1 = np.sqrt(2 * math.pi)
I2 = 2 * math.pi

# res = scipy.integrate.romberg(f, a, b, tol=1e-08, divmax=50, show=True)
# print(f"I1 - res = {I1 - res}")
# print(f"I1/2 - res = {I1/2 - res}")

def solutionA(func, a, b, printromberg=False):
    return scipy.integrate.romberg(func, a, b, tol=1e-08, divmax=50, show=printromberg)

def solutionB(func, k, printromberg=False):
    int = scipy.integrate.romberg(func, 0, k, tol=1e-08, divmax=50, show=printromberg)
    if k >= 0:
        return I1 / 2 + int
    else:
        return I1 / 2 - int

def phi(funcres):
    return funcres * np.sqrt(2 * math.pi) 


solA = solutionA(f, a, b)
solB = solutionB(f, b)

print(f"solA: {solA}")
print(f"solB: {solB}")
print(f"diff: {np.abs(solA - solB)}")

phiA = phi(solA)
phiB = phi(solB)

print(f"phiA: {phiA}")
print(f"phiB: {phiB}")
print(f"diff: {np.abs(phiA - phiB)}")



# draw_whole_plot(f, -5, 5, 1000000, 'r')