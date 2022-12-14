#ifndef RESIZE_H_
#define RESIZE_H_

#include "utils.hpp"
#include <cmath>
#include <thread>


const float a = -0.5f;
const float A1=a+2;
const float A2=a+3;
const float B1=5*a;
const float B2=8*a;
const float B3=-4*a;
float coeffx[4];
void WeightCoeff(float u,float *coe) {
  float x;
  for(int i=0;i<4;i++)
  {
    x=fabs(u-i);
    if (x <= 1) {
    coe[i]= 1 - A2 * x * x + A1 * x * x * x;
  } else if (x < 2) {
    coe[i]=B3+ B2* x - B1* x * x + a * x * x * x;
  }
  }
}
void CalcCoeff4x4(float x, float y, float *coeff) {

  float v = y - floor(y);

  v += 1;
  float coeffy[4];
  WeightCoeff(v,coeffy);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      coeff[i * 4 + j] =
          coeffx[i]*coeffy[j];
    }
  }
}


unsigned char BGRAfterBiCubic(RGBImage src, float x_float, float y_float,
                               int d) {
  float coeff[16];

  int x0 = floor(x_float) - 1;
  int y0 = floor(y_float) - 1;
  CalcCoeff4x4(x_float, y_float, coeff);

  float sum = .0f;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      sum += coeff[i * 4 + j] *
            src.data[((x0 + i) * src.cols + y0 + j) * 3 + d];
    }
  }
  return static_cast<unsigned char>(sum);
}

// const float a = -0.5f;
//   float a_mul_4 = (a + a) + (a + a);   //4a
//   float a_mul_5 = a_mul_4 + a;         //5a
//   float a_mul_8 = a_mul_4 + a_mul_4;   //8a
//   float a_add_3 = a + 3;
//   float a_add_2 = a + 2;
//   __m128 a_m = _mm_set1_ps(a);
//   __m128 m_1 = _mm_set1_ps(1.0);
//   __m128 a_mul_4_m = _mm_set1_ps(a_mul_4);
//   __m128 a_mul_5_m = _mm_set1_ps(a_mul_5);
//   __m128 a_mul_8_m = _mm_set1_ps(a_mul_8);
//   __m128 a_add_3_m = _mm_set1_ps(a_add_3);
//   __m128 a_add_2_m = _mm_set1_ps(a_add_2);

// unsigned char cubic_inner_SSE( RGBImage src, float x_float, float y_float,int channels,int d)
// {
//   //计算权重系数
//   float u = x_float - floor(x_float);
//   float v = y_float - floor(y_float);

//   __m128 C30_A30 = _mm_set_ps(2 - v, 1 + v, 2 - u, 1 + u);   //C3 C0 A3 A0
//   __m128 C21_A21 = _mm_set_ps(1 - v, v, 1 - u, u);   //C2 C1 A2 A1


//   __m128 tmp0 = _mm_sub_ps(_mm_mul_ps(a_m, C30_A30), a_mul_5_m);   //a*xx - a_mul_5
//   tmp0 = _mm_add_ps(a_mul_8_m, _mm_mul_ps(C30_A30, tmp0));       //a_mul_8 + xx*(a*xx - a_mul_5)
//   tmp0 = _mm_sub_ps(_mm_mul_ps(C30_A30, tmp0), a_mul_4_m);    //xx*(a_mul_8 + xx*(a*xx - a_mul_5)) - a_mul_4  = C3 C0 A3 A0


//   __m128 tmp1 = _mm_sub_ps(_mm_mul_ps(a_add_2_m, C21_A21), a_add_3_m);   //a_add_2*xx - a_add_3
//   tmp1 = _mm_mul_ps(_mm_mul_ps(C21_A21, C21_A21), tmp1);    //xx*xx*(a_add_2*xx - a_add_3)
//   tmp1 = _mm_add_ps(m_1, tmp1);     //1 + xx*xx*(a_add_2*xx - a_add_3) = C2 C1 A2 A1


//   __m128 A_m = _mm_unpacklo_ps(tmp0, tmp1);    //tmp1[1] tmp0[1] tmp1[0] tmp0[0] = A2 A3 A1 A0
//   __m128 C_m = _mm_unpackhi_ps(tmp0, tmp1);    //tmp1[3] tmp0[3] tmp1[2] tmp0[2] = C2 C3 C1 C0
//   A_m = _mm_shuffle_ps(A_m, A_m, _MM_SHUFFLE(2, 3, 1, 0));   //A3 A2 A1 A0
//   C_m = _mm_shuffle_ps(C_m, C_m, _MM_SHUFFLE(2, 3, 1, 0));   //C3 C2 C1 C0


//   float C[4] __attribute__((aligned(32)));
//   _mm_store_ps(C, C_m);


//   __m128 m128_C = _mm_set1_ps(C[0]);
//   __m128 coeff0 = _mm_mul_ps(A_m, m128_C);


//   m128_C = _mm_set1_ps(C[1]);
//   __m128 coeff1 = _mm_mul_ps(A_m, m128_C);


//   m128_C = _mm_set1_ps(C[2]);
//   __m128 coeff2 = _mm_mul_ps(A_m, m128_C);


//   m128_C = _mm_set1_ps(C[3]);
//   __m128 coeff3 = _mm_mul_ps(A_m, m128_C);
  
//   ///


//   int x0 = floor(x_float) - 1;
//   int y0 = floor(y_float) - 1;


//   __m128 sum_m = _mm_setzero_ps();
  
//   __m128 src_m = _mm_set_ps(src.data[((x0+3) * src.cols + y0) * channels + d], src.data[((x0+2) * src.cols + y0) * channels + d], src.data[((x0+1) * src.cols + y0) * channels + d], src.data[((x0) * src.cols + y0) * channels + d]);
//   sum_m = _mm_add_ps(sum_m, _mm_mul_ps(src_m, coeff0));


//   src_m = _mm_set_ps(src.data[((x0+3) * src.cols + y0+1) * channels + d], src.data[((x0+2) * src.cols + y0+1) * channels + d], src.data[((x0+1) * src.cols + y0+1) * channels + d], src.data[((x0) * src.cols + y0+1) * channels + d]);
//   sum_m = _mm_add_ps(sum_m, _mm_mul_ps(src_m, coeff1));



//   src_m = _mm_set_ps(src.data[((x0+3) * src.cols + y0+2) * channels + d], src.data[((x0+2) * src.cols + y0+2) * channels + d], src.data[((x0+1) * src.cols + y0+2) * channels + d], src.data[((x0) * src.cols + y0+2) * channels + d]);
//   sum_m = _mm_add_ps(sum_m, _mm_mul_ps(src_m, coeff2));


//   src_m = _mm_set_ps(src.data[((x0+3) * src.cols + y0+3) * channels + d], src.data[((x0+2) * src.cols + y0+3) * channels + d], src.data[((x0+1) * src.cols + y0+3) * channels + d], src.data[((x0) * src.cols + y0+3) * channels + d]);
//   sum_m = _mm_add_ps(sum_m, _mm_mul_ps(src_m, coeff3));


//   float *p = (float *)&sum_m;
//   unsigned char sum = (unsigned char)(p[0] + p[1] + p[2] + p[3]);


//   return sum;
// }

void fun(RGBImage &src,int d,unsigned char *res,int ratio)
{
  int resize_rows = src.rows * ratio;
  int resize_cols = src.cols * ratio;
   auto check_perimeter = [src](float x, float y) -> bool {
    return x < src.rows - 2 && x > 1 && y < src.cols - 2 && y > 1;
  };
  for (int i = 0; i < resize_rows; i++) {
    float src_x = i / ratio;
    float u = src_x - floor(src_x);
    u += 1;
    WeightCoeff(u,coeffx);
    for (int j = 0; j < resize_cols; j++) {
      float src_y = j / ratio;
      if (check_perimeter(src_x, src_y)) {
        // for (int d = 0; d < channels; d++) {
          res[((i * resize_cols) + j) * 3 + d] =
               BGRAfterBiCubic(src, src_x, src_y, d);
              // cubic_inner_SSE(src, src_x, src_y, channels, d);
        // }
      }
    }
  }
}
RGBImage ResizeImage(RGBImage src, float ratio) {
  const int channels = src.channels;
  Timer timer("resize image by 5x");
  int resize_rows = src.rows * ratio;
  int resize_cols = src.cols * ratio;

  printf("resize to: %d x %d\n", resize_rows, resize_cols);

  // auto check_perimeter = [src](float x, float y) -> bool {
  //   return x < src.rows - 2 && x > 1 && y < src.cols - 2 && y > 1;
  // };

  auto res = new unsigned char[channels * resize_rows * resize_cols];
  std::fill(res, res + channels * resize_rows * resize_cols, 0);
  // for (int i = 0; i < resize_rows; i++) {
  //   for (int j = 0; j < resize_cols; j++) {
  //     float src_x = i / ratio;
  //     float src_y = j / ratio;
  //     if (check_perimeter(src_x, src_y)) {
  //       for (int d = 0; d < channels; d++) {
  //         res[((i * resize_cols) + j) * channels + d] =
  //             BGRAfterBiCubic(src, src_x, src_y, channels, d);
  //       }
  //     }
  //   }
  // }
  std::thread th2(fun, std::ref(src),0,res, ratio);
  std::thread th3(fun, std::ref(src),1,res, ratio);
  std::thread th4(fun, std::ref(src),2,res, ratio);
  th2.join();
  th3.join();
  th4.join();
  return RGBImage{resize_cols, resize_rows, channels, res};
}

#endif