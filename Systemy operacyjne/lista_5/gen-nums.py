#!/usr/bin/env python3

import random
import sys


if __name__ == '__main__':
    nelem = int(sys.argv[1])
    print(nelem)
    for i in range(nelem):
        print(random.randint(-nelem * 10, nelem * 10))
