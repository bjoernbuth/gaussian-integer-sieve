from tqdm import tqdm
import math
import matplotlib.pyplot as plt
import sys
import os


def simple_sieve(x, display=False, sort=False):
    """Simple sieve of Eratosthenes in the Gaussian integers.

    This function implements the basic sieve of Eratosthenes in the Gaussian
    integers. Returns a list of Gaussian primes a + bi as tuples (a, b) with
    norm less than or equal to x.  Here a > 0 and b >= 0, and so the returned
    list contains exactly one copy of each Gaussian prime a + bi up to the
    action of the unit group. This choice of a and b includes the positive
    real-axis and excludes the positive imaginary-axis.

    The primes used to sieve the sieve array G are generated dynamically. As in
    the most basic version of the sieve of Eratosthenes, the smallest uncrossed
    number from the sieve array is deemed prime, then progress continues as its
    multiples are crossed off. This approach is taken here.
    """
    if display:
        print('Initializing...')
    G = initialize_sieve(x)
    N = sort_by_norm(isqrt(x))
    if display:
        print('Done initializing sieve array.')
        mem_size = sys.getsizeof(G)  # tons of overhead in native python
        for g in G:
            mem_size += sys.getsizeof(g)
            mem_size += sum(sys.getsizeof(v) for v in g)  # True takes 28 bytes to store in memory
        print('Sieve array uses {} GB of memory.'.format(mem_size / 10**9))
        print('Sieve array has {} M total entries'.format(sum([len(g) for g in G]) / 10**6))
        print('Starting to sieve...')

    # Using PNT to make crude progress bar. The constant 0.8 seemed somewhat accurate.
    with tqdm(total=.8 * x * math.log(math.log(x))) as pbar:
        for a, b in N:
            if G[a][b]:  # so a + bi is actually prime
                p = a * a + b * b
                cross_off_multiples2(x, G, a, b)
                # We just crossed off a + bi, but we want it to be marked prime.
                G[a][b] = True
                pbar.update(x / p)  # updating the progress bar

    if display:
        print('Done sieving.')
        print('Gathering primes...')
    P = gather_primes(G)
    if display and sort:
        print('Sorting primes...')
    if sort:
        P.sort(key=lambda ab: ab[0] ** 2 + ab[1] ** 2)
    if display:
        print('Finished!')
    return P


def isqrt(n):
    """Integer square root based on Babylonian method.

    Returns largest integer m such that m * m <= n.
    """

    if not isinstance(n, int):
        raise TypeError('Method requires integer input.')
    if n < 0:
        raise ValueError('Method requires non-negative input.')

    x = n
    y = (x + 1) // 2
    while y < x:
        x = y
        y = (x + n // x) // 2
    return x


def initialize_sieve(x):
    """Create boolean array indexed by Gaussian integers in the first quadrant.

    Returns G, a list of lists with boolean values initialized to True. The
    index of G[a][b] corresponds to a Gaussian integer a + bi. Both indices a
    and b start at 0 and run up to a^2 + b^2 <= x. We initialize the entries of
    G corresponding to the Gaussian integers 0, 1, and i to False because these
    are neither prime nor composite.
    """

    G = []
    for a in range(isqrt(x) + 1):
        G.append([True] * (isqrt(x - a * a) + 1))  # 0 <= b <= sqrt(x - a^2)
    G[0][0] = G[0][1] = G[1][0] = False
    return G


def sort_by_norm(x):
    """Sort Gaussian integers by norm up to x.

    Returns a list N of pairs (a, b) corresponding to Gaussian integers a + bi
    sorted according to norm. Using the same convention as in the function
    simple_sieve(), we constrain a > 0 and b >= 0.
    """

    N = []  # holds tuples (a, b)
    for a in range(1, isqrt(x) + 1):
        for b in range(isqrt(x - a * a) + 1):
            N.append((a, b))
    # Sorting this list according to norm.
    N.sort(key=lambda ab: ab[0] ** 2 + ab[1] ** 2)
    print(N)
    return N


def cross_off_multiples(x, G, a, b):
    """Cross off multiples of a + bi in the sieve array.

    The Z[i]-lattice generated by a + bi is equivalent to the Z-lattice
    generated by the points (a, b) and (-b, a). Thinking of Gaussian integers
    as points, we let
                    (u, v) = c(a, b) + d(-b, a),
    so that
                    u = ac - bd and v = ad + bc.

    With N(a + bi) = p, then we must have N(c + di) <= x / p in order to
    remain within the sieve array. To avoid computing many multiplications
    in determining u and v, we additively step through various u and v for
    each choice of c and d.
    """

    p = a * a + b * b

    outer_u, outer_v = 0, 0  # these will hold the products ac and ad respectively
    for c in range(isqrt(x // p) + 1):
        u, v = outer_u, outer_v
        for d in range(isqrt(x // p - c * c) + 1):
            if u > 0:
                G[u][v] = False
            else:
                # u + vi is in second quadrant; multiplication by -i brings it back
                G[v][-u] = False

            u -= b
            v += a
        outer_u += a
        outer_v += b


def cross_off_multiples2(x, G, a, b):
    """Cross off multiples of a + bi in the sieve array.

    This algorithm is only guaranteed to workin the case that a + bi is prime.
    If a + bi is a degree 1 prime over a rational prime p, multiples of a + bi
    form a subgroup of order p in the quotient group Z[i] / pZ[i]. We construct
    elements of this subgroup and translate them by multiples of p. In the code
    below, s + it is a reduced representative in this subgroup. We let
                    u = s + kp and v = t + jp
    be the real and imaginary parts of the translates of s + it. We loop over k
    and j to obtain all possible u and v.
    """

    if b:  # degree 1 prime
        p = a * a + b * b
        subgroup_size = p
    else:  # degree 2 prime
        p = a
        subgroup_size = 1

    # Building additive subgroup generated by a + bi in Z[i]/pZ[i].
    s, t = a, b
    for _ in range(subgroup_size):
        u = s
        # Using that (s + kp)^2 + (t + jp)^2 <= x then solving for k, j
        # u = s + kp and v = t + jp
        for k in range((isqrt(x - t * t) - s) // p + 1):
            v = t
            for j in range((isqrt(x - u * u) - t) // p + 1):
                G[u][v] = False
                v += p
            u += p
        s = (s + a) % p
        t = (t + b) % p


def gather_primes(G):
    """Gather Gaussian primes as pairs from sieve array."""

    P = []
    for a in range(1, len(G)):
        for b in range(len(G[a])):
            if G[a][b]:
                P.append((a, b))
    return P


def visualize(x, P, full_disk=False):
    """Plot Gaussian primes with Matplotlib."""

    X = [p[0] for p in P]
    Y = [p[1] for p in P]
    plt.subplots(figsize=(8, 8))
    plt.axhline(0, color='red')
    plt.axvline(0, color='red')
    if full_disk:
        # Appending integers on positive real-axis to positive imaginary-axis.
        real_axis = [p[0] for p in P if p[1] == 0]
        X += [0] * len(real_axis)
        Y += real_axis

        neg_X = [-t for t in X]
        neg_Y = [-t for t in Y]
        plt.plot(X, Y, 'bo', markersize=100 / isqrt(x))
        plt.plot(neg_X, Y, 'bo', markersize=100 / isqrt(x))
        plt.plot(X, neg_Y, 'bo', markersize=100 / isqrt(x))
        plt.plot(neg_X, neg_Y, 'bo', markersize=100 / isqrt(x))
    else:
        plt.plot(X, Y, 'bo', markersize=200 / isqrt(x))
    plt.show()


# To run from command line and save list of "small" primes to a file.
if __name__ == "__main__":
    if not os.path.exists('../data/'):
        os.mkdir('../data/')
    P = simple_sieve(int(sys.argv[1]))
    with open('../data/small_primes.txt', 'w') as f:
        for p in P:
            f.write(str(p) + '\n')

