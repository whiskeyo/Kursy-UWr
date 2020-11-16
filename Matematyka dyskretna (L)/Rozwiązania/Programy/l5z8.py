import scipy.special # binomial coefficient

def max2d_iter(n):
    return 1 + (n * (n + 1)) / 2

def max2d_rec(n):
    if n == 0:
        return 1
    else:
        return max2d_rec(n - 1) + n

def max3d_iter(n):
    return (n**3 + 5*n + 6) / 6

def max3d_rec(n):
    if n == 0:
        return 1
    else:
        return max3d_rec(n - 1) + max2d_rec(n - 1)

def max4d_iter(n):
    return int((n**4 - 2*n**3 + 11*n**2 + 14*n + 24) / 24)

def max4d_rec(n):
    if n == 0:
        return 1
    else:
        return max4d_rec(n - 1) + max3d_rec(n - 1)

def maxNd_rec(N, n):
    if N == 0: # bez wymiarow jest tez niby tylko 1 region (?)
        return 1
    elif n == 0: # to jest zawsze prawdziwe (1 region bez podzialow)
        return 1
    else:
        return maxNd_rec(N, n - 1) + maxNd_rec(N - 1, n - 1)

def maxNd_iter(N, n):
    result = 0
    for i in range(0, N + 1):
        result = result + scipy.special.binom(n, i)
    return int(result)

max1d = []
max2d = []
max3d = []
max3dnew = []
max4drec  = []
max4diter = []
max4dnew  = []
maxNn = []
maxNniter = []

for i in range(0, 10):
    max1d.append(maxNd_rec(1, i))
    max2d.append(maxNd_rec(2, i))
    max3d.append(maxNd_rec(3, i))
    # max4drec.append(max4d_rec(i))
    # max4diter.append(max4d_iter(i))
    max4dnew.append(maxNd_rec(4, i))
    maxNn.append(maxNd_rec(10, i))
    maxNniter.append(maxNd_iter(10, i))
    max3dnew.append(maxNd_iter(3, i))

print("1d:",    max1d)
print("2d:",    max2d)
print("3d:",    max3d)
print("3dNEW:", max3dnew)
print("4d:",    max4dnew)
print("10dREC:", maxNn)
print("10dITER:", maxNniter)
