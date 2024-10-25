// Copyright 2024 Nikitin Kirill
#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

namespace nikitin_k_merge_sort_omp {

std::vector<int> getRandomVector(int sz);
std::vector<int> Merge(const std::vector<int>& firstVector, const std::vector<int>& secondVector);
std::vector<int> radixSort(std::vector<int> vector);

class TestOMPTaskSequential : public ppc::core::Task {
 public:
  explicit TestOMPTaskSequential(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> input_;
  int res{};
  std::string ops;
};

class TestOMPTaskParallel : public ppc::core::Task {
 public:
  explicit TestOMPTaskParallel(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)){}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> input_;
  int res{};
  std::string ops;
};

}  // namespace nikitin_k_merge_sort_omp
