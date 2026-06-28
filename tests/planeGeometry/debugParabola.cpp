#include <algorithm>
#include <cmath>
#include <cstdio>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int g_passed = 0, g_failed = 0;

void recover(double a, double b, double C, double d, double e, double f,
             double& cx, double& cy, double& p) {
  // Recover s, c (both >= 0; sign encoded in p)
  double sr, cr;
  if (std::abs(C) > 1e-15) {
    sr = std::sqrt(std::max(a, 0.0));
    cr = std::sqrt(std::max(b, 0.0));
  } else {
    sr = std::sqrt(std::max(a, 0.0));
    cr = std::sqrt(std::max(b, 0.0));
  }

  // Compute p from the formula (derived by solving the d, e equations)
  // When sc != 0: p = -(c*d + s*e) / 4
  // When s == 0 (a == 0): p = -d/(4c)
  // When c == 0 (b == 0): p = -e/(4s)
  double pval;
  if (std::abs(sr) < 1e-15) {
    pval = -d / (4.0 * cr);
  } else if (std::abs(cr) < 1e-15) {
    pval = -e / (4.0 * sr);
  } else {
    pval = -(cr * d + sr * e) / 4.0;
  }
  p = pval;

  // Recover (cx, cy) depending on the case
  if (std::abs(sr * cr) < 1e-15) {
    // s == 0 or c == 0
    if (std::abs(sr) < 1e-15) {
      // opens along x-axis: s = 0
      cy = -e / (2.0 * b);
      cx = (f - b * cy * cy) / (4.0 * p * cr);
    } else {
      // opens along y-axis: c = 0
      cx = -d / (2.0 * a);
      cy = (f - a * cx * cx) / (4.0 * p * sr);
    }
  } else if (std::abs(sr * sr - cr * cr) < 1e-12 * (sr * sr + cr * cr)) {
    // s^2 == c^2 (45-degree rotation): solve tangent + F = 0 directly.
    // Tangent parallel to (c, -s) with s = ±c gives either x + y = const or x - y = const.
    // Use the gradient parallel-to-axis condition:
    //   s*F_x = c*F_y  ->  simplifies to a linear constraint.
    // With s^2 = c^2, the constraint becomes: s*F_x = c*F_y.
    //   F_x = 2ax + Cy + d, F_y = Cx + 2by + e
    // With a = b = s^2 = c^2, C = -2sc = -2s^2 (assuming sign(s)=sign(c))
    //   s(2ax + Cy + d) = c(Cx + 2by + e)
    //   2as x + sC y + sd = cC x + 2bc y + ce
    //   2s^3 x - 2s^3 y + sd = -2s^3 x + 2s^3 y + se  (since C = -2s^2)
    //   Rearr: 4s^3 (x - y) = se - sd
    //   x - y = (e - d) / (4s^2)  [when s = c]
    // Then F(x, y) = 0: s^2 (x-y)^2 - 4s^2 (x+y) * p' / s^2 ... 
    // Actually use the original F: F = (s(x-cx) - c(y-cy))^2 - 4p(c(x-cx) + s(y-cy))
    //   = s^2 (x-y)^2 - 4p s (x+y)  (with s = c)
    // Solve (x-y) = K1 and (x+y) = K2 from F = 0.
    double K1 = (e - d) / (4.0 * sr * sr);
    double ssq = sr * sr;
    // s^2 K1^2 - 4p s * (x+y) = 0  =>  x+y = s * K1^2 / (4p)
    double K2 = sr * K1 * K1 / (4.0 * p);
    cx = (K1 + K2) / 2.0;
    cy = (K2 - K1) / 2.0;
  } else {
    // General case: K = (se - cd) / (4sc)
    double K = (sr * e - cr * d) / (4.0 * sr * cr);
    double ssq = sr * sr + cr * cr;  // should be 1 by convention
    cx = sr * K + cr * K * K / (4.0 * p);
    cy = -cr * K + sr * K * K / (4.0 * p);
  }
}

void testCase(const char* name, double cx0, double cy0, double p0, double rot) {
  double s = std::sin(rot), c = std::cos(rot);
  double a = s*s, b = c*c, C = -2*s*c;
  double d = -4*p0*c - 2*s*s*cx0 + 2*s*c*cy0;
  double e = -4*p0*s - 2*c*c*cy0 + 2*s*c*cx0;
  double f = s*s*cx0*cx0 + c*c*cy0*cy0 - 2*s*c*cx0*cy0 + 4*p0*(c*cx0 + s*cy0);

  double cxr, cyr, pr;
  try {
    recover(a, b, C, d, e, f, cxr, cyr, pr);
  } catch (...) {
    std::printf("%s: EXCEPTION\n", name);
    g_failed++;
    return;
  }
  double Fv = a*cxr*cxr + b*cyr*cyr + C*cxr*cyr + d*cxr + e*cyr + f;

  // Expected p may differ by sign - but sign should encode direction.
  // So we compare |p|.
  bool ok_cx = std::abs(cxr - cx0) < 1e-6;
  bool ok_cy = std::abs(cyr - cy0) < 1e-6;
  bool ok_p = std::abs(std::abs(pr) - std::abs(p0)) < 1e-6;
  bool ok_F = std::abs(Fv) < 1e-6;

  // Check focus direction: focus should be on the correct side
  // For the formula focus = vertex + p * (c, s), p should have same sign as p0.
  bool ok_psign = (pr * p0) > 0 || std::abs(p0) < 1e-9;

  if (ok_cx && ok_cy && ok_p && ok_F && ok_psign) {
    std::printf("%s OK: (%.4f, %.4f, p=%.4f)\n", name, cxr, cyr, pr);
    g_passed++;
  } else {
    std::printf("%s FAIL: exp (%.3f,%.3f,p=%.3f) got (%.4f,%.4f,p=%.4f) Fv=%.3e psign_ok=%d\n",
                name, cx0, cy0, p0, cxr, cyr, pr, Fv, ok_psign);
    g_failed++;
  }
}

int main() {
  testCase("t0: rot=0, (1,2), p=2", 1.0, 2.0, 2.0, 0.0);
  testCase("t1: rot=pi/4, (0.5,0.5), p=1", 0.5, 0.5, 1.0, M_PI/4);
  testCase("t2: rot=pi/2, (1,2), p=2", 1.0, 2.0, 2.0, M_PI/2);
  testCase("t3: rot=0, (0,0), p=1", 0.0, 0.0, 1.0, 0.0);
  testCase("t4: rot=0, (3,-1), p=2.5", 3.0, -1.0, 2.5, 0.0);
  testCase("t5: rot=pi, (1,2), p=2", 1.0, 2.0, 2.0, M_PI);
  testCase("t6: rot=pi/3, (2,3), p=1.5", 2.0, 3.0, 1.5, M_PI/3);
  testCase("t7: rot=0, (0,0), p=3", 0.0, 0.0, 3.0, 0.0);
  testCase("t8: rot=pi/6, (-1,-2), p=0.5", -1.0, -2.0, 0.5, M_PI/6);

  std::printf("\n%d passed, %d failed\n", g_passed, g_failed);
  return g_failed > 0 ? 1 : 0;
}
