#pragma once

#include <common.hpp>
#include <detail/traits.hpp>
#include <type_traits>
#include <detail/trigonometric_derivatives.hpp>

template<Axis A1, Axis A2, Axis A3, std::enable_if_t<isTaitBryan<A1,A2,A3>(), int> = 0>
Vect<3> eulerFromRotationMatrix(Mat<3,3> const& R)
{
  using T = TaitBryanTraits<A1,A2,A3>;
  const auto W = Wrap{R};
  const auto lone_v = W.uncorrected(T::a2s);
  const auto thres = 1.0 - std::numeric_limits<float>::epsilon();
  double a1,a2,a3;
  if( lone_v < thres ){
    if( lone_v > - thres ){
      a2 = detail::asin( W(T::a2s));
      a1 = detail::atan2(W(T::a1s), W(T::a1c));
      a3 = detail::atan2(W(T::a3s), W(T::a3c));
    } 
    else { // Gimbal lock. lone value ~= -1
      using TGL = typename T::GimbalLock;
      a2 =  - W.sign(T::a2s) * M_PI / 2.0;
      a1 =  - detail::atan2( W(TGL::a1s), W(TGL::a1c) );
      a3 = 0.;
    }
  }
  else { // Gimbal lock. lone value ~= +1
    using TGL = typename T::GimbalLock;
    a2 =  W.sign(T::a2s) * M_PI / 2.0;
    a1 =  detail::atan2( W(TGL::a1s), W(TGL::a1c) );
    a3 = 0.;
  }
  return (Vect<3>() << a1,a2,a3).finished();
}

template<Axis A1, Axis A2, Axis A3, std::enable_if_t<isTaitBryan<A1,A2,A3>(), int> = 0>
Vect<3> eulerFromRotationMatrix(Mat<3,3> const& R, Eigen::Ref<Mat<3,9>> H)
{
  using T = TaitBryanTraits<A1,A2,A3>;
  H = Mat<3,9>::Zero();
  const auto W = Wrap{R};
  const auto lone_v = W.uncorrected(T::a2s);
  const auto thres = 1.0 - std::numeric_limits<float>::epsilon();
  double a1,a2,a3;
  if( lone_v < thres ){
    if( lone_v > - thres ){
      a2 = detail::asin( W(T::a2s), 
          H(1, T::a2s.c*3 + T::a2s.r));
      H(1, T::a2s.c*3 + T::a2s.r) *= W.sign(T::a2s);

      a1 = detail::atan2(W(T::a1s), W(T::a1c),
          H(0, T::a1s.c*3 + T::a1s.r),
          H(0, T::a1c.c*3 + T::a1c.r));
      H(0, T::a1s.c*3 + T::a1s.r) *= W.sign(T::a1s);
      H(0, T::a1c.c*3 + T::a1c.r) *= W.sign(T::a1c);

      a3 = detail::atan2(W(T::a3s), W(T::a3c),
      H(2, T::a3s.c*3 + T::a3s.r),
      H(2, T::a3c.c*3 + T::a3c.r));
      H(2, T::a3s.c*3 + T::a3s.r) *= W.sign(T::a3s);
      H(2, T::a3c.c*3 + T::a3c.r) *= W.sign(T::a3c);
    } 
    else { // Gimbal lock. lone value ~= -1
      using TGL = typename T::GimbalLock;
      a2 =  - W.sign(T::a2s) * M_PI / 2.0;
      a1 =  - detail::atan2( W(TGL::a1s), W(TGL::a1c) );
      a3 = 0.;
      H = Mat<3,9>::Constant(std::nan(""));
    }
  }
  else { // Gimbal lock. lone value ~= +1
    using TGL = typename T::GimbalLock;
    a2 =  W.sign(T::a2s) * M_PI / 2.0;
    a1 =  detail::atan2( W(TGL::a1s), W(TGL::a1c) );
    a3 = 0.;
    H = Mat<3,9>::Constant(std::nan(""));
  }
  return (Vect<3>() << a1,a2,a3).finished();
}

template<Axis A1, Axis A2, Axis A3, std::enable_if_t<isProperEuler<A1,A2,A3>(), int> = 0>
Vect<3> eulerFromRotationMatrix(Mat<3,3> const& R)
{
  using T = ProperEulerTraits<A1,A2>; 
  const auto W = Wrap{R};
  const auto lone_v = W.uncorrected(T::a2c);
  const auto thres = 1.0 - std::numeric_limits<float>::epsilon();
  double a1,a2,a3;
  if( lone_v < thres ){
    if( lone_v > - thres ){
      a2 = detail::acos( W(T::a2c));
      a1 = detail::atan2(W.uncorrected(T::a1s), W(T::a1c));
      a3 = detail::atan2(W.uncorrected(T::a3s), W(T::a3c));
    } 
    else { // Gimbal lock. lone value ~= -1
      using TGL = typename T::GimbalLock;
      a2 =  M_PI;
      a1 =  - detail::atan2( W(TGL::a1s), W(TGL::a1c) );
      a3 = 0.;
    }
  }
  else { // Gimbal lock. lone value ~= +1
    using TGL = typename T::GimbalLock;
    a2 =  0.0;
    a1 =  detail::atan2( W(TGL::a1s), W(TGL::a1c) );
    a3 = 0.;
  }
  return (Vect<3>() << a1,a2,a3).finished();
}

template<Axis A1, Axis A2, Axis A3, std::enable_if_t<isProperEuler<A1,A2,A3>(), int> = 0>
Vect<3> eulerFromRotationMatrix(Mat<3,3> const& R, Eigen::Ref<Mat<3,9>> H)
{
  using T = ProperEulerTraits<A1,A2>; 
  H = Mat<3,9>::Zero();
  const auto W = Wrap{R};
  const auto lone_v = W.uncorrected(T::a2c);
  const auto thres = 1.0 - std::numeric_limits<float>::epsilon();
  double a1,a2,a3;
  if( lone_v < thres ){
    if( lone_v > - thres ){
      a2 = detail::acos( W(T::a2c), 
          H(1, T::a2c.c*3 + T::a2c.r));
      H(1, T::a2c.c*3 + T::a2c.r) *= W.sign(T::a2c);

      a1 = detail::atan2(W.uncorrected(T::a1s), W(T::a1c),
          H(0, T::a1s.c*3 + T::a1s.r),
          H(0, T::a1c.c*3 + T::a1c.r) );
      H(0, T::a1c.c*3 + T::a1c.r) *= W.sign(T::a1c);
      a3 = detail::atan2(W.uncorrected(T::a3s), W(T::a3c),
          H(2, T::a3s.c*3 + T::a3s.r),
          H(2, T::a3c.c*3 + T::a3c.r) );
      H(2, T::a3c.c*3 + T::a3c.r) *= W.sign(T::a3c);
    } 
    else { // Gimbal lock. lone value ~= -1
      using TGL = typename T::GimbalLock;
      a2 =  M_PI;
      a1 =  - detail::atan2( W(TGL::a1s), W(TGL::a1c) );
      a3 = 0.;
      H = Mat<3,9>::Constant(std::nan(""));
    }
  }
  else { // Gimbal lock. lone value ~= +1
    using TGL = typename T::GimbalLock;
    a2 =  0.0;
    a1 =  detail::atan2( W(TGL::a1s), W(TGL::a1c) );
    a3 = 0.;
    H = Mat<3,9>::Constant(std::nan(""));
  }
  return (Vect<3>() << a1,a2,a3).finished();
}

template<Axis A1, Axis A2, Axis A3, std::enable_if_t<isMalformed<A1,A2,A3>(), int> = 0>
void eulerFromRotationMatrix(Mat<3,3> const&)
{
  constexpr bool alwaysFalse = ( A1 != A1);
  static_assert( alwaysFalse, "Passed a malformed rotation sequence. Please choose either a propor euler sequence or a Tait-Bryan sequence");
}

