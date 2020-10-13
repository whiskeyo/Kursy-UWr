#define FILL CONCAT(fill_, VARIANT)
#define INDEX CONCAT(index_, VARIANT)
#define QUERY CONCAT(query_, VARIANT)

void FILL(T *arr) {
  for (long y = 0; y < N; y++)
    for (long x = 0; x < N; x++)
      arr[INDEX(x, y)] = fast_random();
}

long QUERY(T *arr, long xs, long ys, long xe, long ye, long *sum_p) {
  if (ys > ye) {
    long xt = xs; xs = xe; xe = xt;
    long yt = ys; ys = ye; ye = yt;
  }

  long dx, step;

  if (xs < xe) {
    dx = xe - xs;
    step = 1;
  } else {
    dx = xs - xe;
    step = -1;
  }

  long dy = ye - ys;

  long x = xs;
  long y = ys;

  long r = 0;

  if (dx < dy) {
    long dg2 = dx * 2;
    long dg = dg2 - dy;
    long dg1 = dg - dy;
    int n = dy;

    do {
      r += arr[INDEX(x, y)];
      x += dg > 0 ? step : 0;
      dg += dg > 0 ? dg1 : dg2;
      y++;
    } while (--n);

    *sum_p += r;
    return dy;
  } else {
    long dg2 = dy * 2;
    long dg = dg2 - dx;
    long dg1 = dg - dx;
    int n = dx;

    do {
      r += arr[INDEX(x, y)];
      y += dg > 0;
      dg += dg > 0 ? dg1 : dg2;
      x += step;
    } while (--n);

    *sum_p += r;
    return dx;
  }
}

#undef FILL
#undef QUERY
#undef INDEX
#undef VARIANT
