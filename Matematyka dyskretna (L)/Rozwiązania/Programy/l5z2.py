def find_all(start, end):
    counter = 0
    for i in range(start, end + 1):
        if (i % 6 == 0 or i % 8 == 0) and i % 7 != 0:
            counter = counter + 1

    return counter

def divisible_by_a_or_b(start, end, a, b):
    counter = 0
    for i in range(start, end + 1):
        if i % a == 0 or i % b == 0:
            counter = counter + 1

    return counter

def divisible_by_a_and_b(start, end, a, b):
    counter = 0
    for i in range(start, end + 1):
        if i % a == 0 and i % b == 0:
            counter = counter + 1

    return counter

def divisible_by_all_abc(start, end, a, b, c):
    counter = 0
    for i in range(start, end + 1):
        if i % a == 0 and i % b == 0 and i % c == 0:
            counter = counter + 1

    return counter

print("divisible by 6 or 8:",  divisible_by_a_or_b(1, 800, 6, 8))
print("divisible by 6 and 7:", divisible_by_a_and_b(1, 800, 6, 7))
print("divisible by 7 and 8:", divisible_by_a_and_b(1, 800, 7, 8))
print("divisible by 6, 7, 8:", divisible_by_all_abc(1, 800, 6, 7, 8))
print(find_all(1, 800))
