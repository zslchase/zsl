#include "image.hpp"
#include "resize.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>


int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Need 1 argument" << std::endl;
    std::cerr << "Usage: ./resize image.jpg" << std::endl;
    return 0;
  }
  std::string src_name(argv[1]);
  auto image = LoadImage(src_name);
  float ratio = 5.f;

  auto image_after_resize = ResizeImage(image, ratio);

  int name_len = src_name.find_last_of('.');
  std::string dst_name = src_name.substr(0, name_len) + std::string("_5x.jpg");

  StoreImage(image_after_resize, dst_name);

  stbi_image_free(image.data);
  stbi_image_free(image_after_resize.data);
  return 0;
}
// #include <iostream>
// #include "omp.h"

// using namespace std;

// void test()
// {
// 	for (int i = 0; i < 8; i++)
// 	{
//     printf("%d",i);
// 	}
// }

// int main(int argc, char const* argv[]) 
// {
// 	float startTime = omp_get_wtime();

// 	//指定2个线程
// #pragma omp parallel for num_threads(2)
// 	for (int i = 0; i < 80; i++)
// 	{
// 		test();
// 	}
// 	float endTime = omp_get_wtime();
// 	printf("指定 2 个线程，执行时间: %f\n", endTime - startTime);
// 	startTime = endTime;

// 	//指定4个线程
// #pragma omp parallel for num_threads(4)
// 	for (int i = 0; i < 80000; i++)
// 	{
// 		test();
// 	}
// 	endTime = omp_get_wtime();
// 	printf("指定 4 个线程，执行时间: %f\n", endTime - startTime);
// 	startTime = endTime;

// 	//指定8个线程
// #pragma omp parallel for num_threads(8)
// 	for (int i = 0; i < 80000; i++)
// 	{
// 		test();
// 	}
	// endTime = omp_get_wtime();
	// printf("指定 8 个线程，执行时间: %f\n", endTime - startTime);
	// startTime = endTime;

	//不使用OpenMP
// 	for (int i = 0; i < 80; i++)
// 	{
// 		test();
// 	}
// 	endTime = omp_get_wtime();
// 	printf("不使用OpenMP多线程，执行时间: %f\n", endTime - startTime);
// 	startTime = endTime;
// 	return 0;
// }
