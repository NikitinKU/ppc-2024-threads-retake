// Copyright 2024 Nikitin Kirill
#include <gtest/gtest.h>

#include <vector>

#include "tbb/nikitin_k_merge_sort/include/ops_tbb.hpp"

TEST(nikitin_k_merge_sort, Test_Sequential_Sort) {
  std::vector<int> vec = nikitin_k_merge_sort_tbb::getRandomVector(100);
  std::vector<int> sorted_ref = vec;  // Создаем копию для сравнения

  // Ожидаемый результат сортировки
  std::sort(sorted_ref.begin(), sorted_ref.end());

  // Создаем TaskData для последовательной задачи
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataSeq->inputs_count.emplace_back(vec.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataSeq->outputs_count.emplace_back(vec.size());

  // Создаем Task для последовательной сортировки
  nikitin_k_merge_sort_tbb::TestTBBTaskSequential testTbbTaskSequential(taskDataSeq);
  ASSERT_EQ(testTbbTaskSequential.validation(), true);
  testTbbTaskSequential.pre_processing();
  testTbbTaskSequential.run();
  testTbbTaskSequential.post_processing();

  // Проверяем результат
  ASSERT_EQ(vec, sorted_ref);
}

TEST(nikitin_k_merge_sort, Test_Parallel_Sort) {
  std::vector<int> vec = nikitin_k_merge_sort_tbb::getRandomVector(100);
  std::vector<int> sorted_ref = vec;  // Создаем копию для сравнения

  // Ожидаемый результат сортировки
  std::sort(sorted_ref.begin(), sorted_ref.end());

  // Создаем TaskData для параллельной задачи
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataPar->inputs_count.emplace_back(vec.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataPar->outputs_count.emplace_back(vec.size());

  // Создаем Task для параллельной сортировки
  nikitin_k_merge_sort_tbb::TestTBBTaskParallel testTbbTaskParallel(taskDataPar);
  ASSERT_EQ(testTbbTaskParallel.validation(), true);
  testTbbTaskParallel.pre_processing();
  testTbbTaskParallel.run();
  testTbbTaskParallel.post_processing();

  // Проверяем результат
  ASSERT_EQ(vec, sorted_ref);
}

TEST(nikitin_k_merge_sort, Test_Small_Vector_Sequential_Sort) {
  std::vector<int> vec = nikitin_k_merge_sort_tbb::getRandomVector(100);
  std::vector<int> sorted_ref = vec;  // Создаем копию для сравнения

  // Create data
  std::sort(sorted_ref.begin(), sorted_ref.end());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataPar->inputs_count.emplace_back(vec.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataPar->outputs_count.emplace_back(vec.size());

  // Create Task
  nikitin_k_merge_sort_tbb::TestTBBTaskSequential testTbbTaskParallel(taskDataPar);
  ASSERT_EQ(testTbbTaskParallel.validation(), true);
  ASSERT_TRUE(testTbbTaskParallel.pre_processing());
  ASSERT_TRUE(testTbbTaskParallel.run());
  ASSERT_TRUE(testTbbTaskParallel.post_processing());

  ASSERT_EQ(vec, sorted_ref);
}

TEST(nikitin_k_merge_sort, test1) {
  std::vector<int> vec = nikitin_k_merge_sort_tbb::getRandomVector(40);
  std::vector<int> sorted_ref = vec;

  // Create data
  std::sort(sorted_ref.begin(), sorted_ref.end());

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataPar->inputs_count.emplace_back(vec.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataPar->outputs_count.emplace_back(vec.size());

  // Create Task
  nikitin_k_merge_sort_tbb::TestTBBTaskSequential testTbbTaskParallel(taskDataPar);
  ASSERT_EQ(testTbbTaskParallel.validation(), true);
  ASSERT_TRUE(testTbbTaskParallel.pre_processing());
  ASSERT_TRUE(testTbbTaskParallel.run());
  ASSERT_TRUE(testTbbTaskParallel.post_processing());

  ASSERT_EQ(vec, sorted_ref);
}

TEST(nikitin_k_merge_sort, Test_Small_Vector_Parallel_Sort) {
  std::vector<int> vec = nikitin_k_merge_sort_tbb::getRandomVector(10);
  std::vector<int> sorted_ref = vec;  // Создаем копию для сравнения

  // Ожидаемый результат сортировки
  std::sort(sorted_ref.begin(), sorted_ref.end());

  // Создаем TaskData для параллельной задачи
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataPar->inputs_count.emplace_back(vec.size());
  taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t *>(vec.data()));
  taskDataPar->outputs_count.emplace_back(vec.size());

  // Создаем Task для параллельной сортировки
  nikitin_k_merge_sort_tbb::TestTBBTaskParallel testTbbTaskParallel(taskDataPar);
  ASSERT_EQ(testTbbTaskParallel.validation(), true);
  testTbbTaskParallel.pre_processing();
  testTbbTaskParallel.run();
  testTbbTaskParallel.post_processing();

  // Проверяем результат
  ASSERT_EQ(vec, sorted_ref);
}
// int main(int argc, char **argv) {
//   ::testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }
