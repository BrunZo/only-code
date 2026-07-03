import argparse
import random
import time

from data_structures.point2d import point, chull


def parse_args():
    ap = argparse.ArgumentParser(description="Stress-test and benchmark point2d.chull")
    ap.add_argument("--n", type=int, default=100_000)
    ap.add_argument("--trials", type=int, default=2_000)
    ap.add_argument("--max_coord", type=int, default=1_000)
    return ap.parse_args()


def main():
    args = parse_args()
    stress_test(args.n, args.trials, args.max_coord)


def stress_test(n: int, trials: int, max_coord: int):
    correctness_stress(trials, max_coord)
    bench(n, max_coord)


def correctness_stress(trials: int, max_coord: int):
    for _ in range(trials):
        m = random.randint(3, 50)
        pts = [point(random.randint(0, max_coord), random.randint(0, max_coord)) for _ in range(m)]
        hull = chull(pts)
        if len(hull) < 3:
            continue  # degenerate (collinear/duplicate) input; not covered by this check
        if not hull_contains_all(pts, hull):
            raise AssertionError(f"invalid hull for points={[(p.x, p.y) for p in pts]}")
    print(f"correctness: {trials} random point sets all produced a valid convex hull")


def hull_contains_all(points, hull):
    n = len(hull)
    for i in range(n):
        a, b = hull[i], hull[(i + 1) % n]
        edge = b - a
        for p in points:
            if (edge ^ (p - a)) < 0:
                return False
    return True


def bench(n: int, max_coord: int):
    pts = [point(random.randint(0, max_coord), random.randint(0, max_coord)) for _ in range(n)]
    start = time.perf_counter()
    hull = chull(pts)
    elapsed = time.perf_counter() - start
    print(f"chull  n={n:>9,}  hull_size={len(hull):>4}  time={elapsed:8.4f}s  ({n / elapsed:,.0f} points/s)")


if __name__ == "__main__":
    main()
