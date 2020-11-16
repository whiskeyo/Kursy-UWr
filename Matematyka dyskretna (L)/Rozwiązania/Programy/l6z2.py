import math

def a(n):
    if n == 0 or n == 1:
        return 1
    else: 
        return abs(math.sqrt(a(n-1)**2 + a(n-2)**2))

a_res = []
a_res_squared = []
for i in range(1, 20):
    a_res.append(a(i))
    a_res_squared.append(int(a(i)**2))

print(a_res)
print(a_res_squared)