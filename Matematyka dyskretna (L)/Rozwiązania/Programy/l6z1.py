def sum_iter(k, n):
    result = 0
    for i in range(1, n + 1):
        result = result + k**i

    return result

def sum_rec(k, n):
    if n == 1:
        return k
    else:
        return k**n + sum_rec(k, n - 1)

def sum_short(k, n):
    if n == 1:
        return k
    else:
        return int((k**(n+1) - 1) / (k - 1) - 1)

def sum_short2(k, n):
    if n == 1:
        return k
    else:
        return int(3 * (1 - k**n) / (1 - k))

def h(n):
    if n == 1:
        return 1
    else:
        return h(n-1) + (-1)**(n+1) * n


iter3  = []
rec3   = []
short3 = []
shortd = []
hn     = []

for i in range (1, 30):
    iter3.append(sum_iter(3, i))
    rec3.append(sum_rec(3, i))
    short3.append(sum_short(3, i))
    shortd.append(sum_short2(3, i))
    hn.append(h(i))

print("iterative: ", iter3)
print("recursive: ", rec3)
print("closedform:", short3)
print("another:   ", shortd)
print("h:         ", hn)
