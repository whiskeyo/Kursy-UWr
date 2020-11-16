def max_regions(n):
    if n == 1:
        return 2
    else:
        return max_regions(n - 1) + 2*n - 2

def max_regions_simple(n):
    return n*n - n + 2

max_reg = []
max_reg_simple = []
for i in range(1, 500):
    max_reg.append(max_regions(i))
    max_reg_simple.append(max_regions_simple(i))
    # print("n = " + str(i) + ", max regions = " + str(max_reg[i-1]))

print(max_reg == max_reg_simple)
