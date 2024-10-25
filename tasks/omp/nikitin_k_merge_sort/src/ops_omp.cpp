// Copyright 2024 Nikitin Kirill
#include "omp/nikitin_k_merge_sort/include/ops_omp.hpp"

#include <omp.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;
using namespace nikitin_k_merge_sort_omp;

std::vector<int> nikitin_k_merge_sort_omp::getRandomVector(int sz) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> vec(sz);
  for (int i = 0; i < sz; i++) {
    vec[i] = gen() % 100 + 1;
  }
  return vec;
}

std::vector<int> nikitin_k_merge_sort_omp::Merge(const std::vector<int>& firstVector,
                                                 const std::vector<int>& secondVector) {
  std::vector<int> result(firstVector.size() + secondVector.size());
  std::merge(firstVector.begin(), firstVector.end(), secondVector.begin(), secondVector.end(), result.begin());
  return result;
}

std::vector<int> nikitin_k_merge_sort_omp::radixSort(std::vector<int> vector) {
  std::vector<int> freq;
  for (int d = 0, maxElem = *max_element(vector.begin(), vector.end());
       d <= (maxElem == 0 ? 1 : static_cast<int>(log10(abs(maxElem))) + 1); d++) {
    std::vector<int> temp(vector.size());
    int div = static_cast<int>(pow(10, d));
    int min = vector[0] % (div * 10) / div;
    int max = min;
    for (const int num : vector) {
      int curr = num % (div * 10) / div;
      min = min < curr ? min : curr;
      max = max > curr ? max : curr;
    }
    freq.assign(max - min + 1, 0);
    for (const int num : vector) freq[num % (div * 10) / div - min]++;
    for (int i = 0, sum = 0; i < static_cast<int>(freq.size()); i++) sum += freq[i], freq[i] = sum;
    for (int i = static_cast<int>(vector.size()) - 1; i >= 0; i--)
      temp[--freq[vector[i] % (div * 10) / div - min]] = vector[i];
    vector = std::move(temp);
  }
  return vector;
}

bool TestOMPTaskSequential::pre_processing() {
  internal_order_test();
  // Init vectors
  input_ = std::vector<int>(taskData->inputs_count[0]);
  auto* tmp_ptr = reinterpret_cast<int*>(taskData->inputs[0]);
  for (unsigned i = 0; i < taskData->inputs_count[0]; i++) {
    input_[i] = tmp_ptr[i];
  }
  // Init value for output
  res = 1;
  return true;
}

bool TestOMPTaskSequential::validation() {
  internal_order_test();
  // Проверяем, что входные данные заданы и содержат хотя бы один элемент
  return taskData->inputs_count.size() == 1 && taskData->outputs_count.size() == 1 && taskData->inputs.size() == 1 &&
         taskData->outputs.size() == 1 && taskData->inputs[0] != nullptr && taskData->outputs[0] != nullptr &&
         taskData->inputs_count[0] == taskData->outputs_count[0] && taskData->inputs_count[0] >= 0 &&
         taskData->outputs_count[0] >= 0;
}

bool TestOMPTaskSequential::run() {
  internal_order_test();
  try {
    std::vector<int> freq;
    for (int d = 0, maxElem = *std::max_element(input_.begin(), input_.end());
         d <= (maxElem == 0 ? 1 : static_cast<int>(log10(abs(maxElem))) + 1); d++) {
      std::vector<int> temp(input_.size());
      int div = static_cast<int>(pow(10, d));
      int min = input_[0] % (div * 10) / div;
      int max = min;
      for (const int num : input_) {
        int curr = num % (div * 10) / div;
        min = min < curr ? min : curr;
        max = max > curr ? max : curr;
      }
      freq.assign(max - min + 1, 0);
      for (const int num : input_) freq[num % (div * 10) / div - min]++;
      for (int i = 0, sum = 0; i < static_cast<int>(freq.size()); i++) sum += freq[i], freq[i] = sum;
      for (int i = static_cast<int>(input_.size()) - 1; i >= 0; i--)
        temp[--freq[input_[i] % (div * 10) / div - min]] = input_[i];
      input_ = temp;
    }
    return true;
  } catch (...) {
    return false;
  }
}

bool TestOMPTaskSequential::post_processing() {
  internal_order_test();
  auto* outputs = reinterpret_cast<int*>(taskData->outputs[0]);
  for (size_t i = 0; i < input_.size(); i++) {
    outputs[i] = input_[i];
  }
  return true;
}

bool TestOMPTaskParallel::pre_processing() {
  internal_order_test();
  // Init vectors
  try {
    int size = static_cast<int>(taskData->inputs_count[0]);
    input_ = std::vector<int>(reinterpret_cast<int*>(taskData->inputs[0]),
                              reinterpret_cast<int*>(taskData->inputs[0]) + size);
    return true;
  } catch (...) {
    return false;
  }
}

bool TestOMPTaskParallel::validation() {
  internal_order_test();
  // Check count elements of output
  return taskData->inputs_count.size() == 1 && taskData->outputs_count.size() == 1 && taskData->inputs.size() == 1 &&
         taskData->outputs.size() == 1 && taskData->inputs[0] != nullptr && taskData->outputs[0] != nullptr &&
         taskData->inputs_count[0] == taskData->outputs_count[0] && taskData->inputs_count[0] >= 0 &&
         taskData->outputs_count[0] >= 0;
}

bool TestOMPTaskParallel::run() {
  internal_order_test();
  try {
    std::vector<int> result;
    int resultSize = input_.size();
#pragma omp parallel shared(result, resultSize)
    {
      int currentThread = omp_get_thread_num();
      int threadNum = std::min(omp_get_num_threads(), resultSize);
      if (currentThread < resultSize) {
        int step = resultSize / threadNum;
        int left = step * currentThread;
        int right = (currentThread == threadNum - 1) ? resultSize : step * (currentThread + 1);
        std::vector<int> input_Local = radixSort(std::vector<int>(input_.begin() + left, input_.begin() + right));
#pragma omp critical
        result = Merge(result, input_Local);
      }
    }
    input_ = result;
    return true;
  } catch (...) {
    return false;
  }
}

bool TestOMPTaskParallel::post_processing() {
  internal_order_test();
  for (size_t i = 0; i < input_.size(); i++) {
    reinterpret_cast<int*>(taskData->outputs[0])[i] = input_[i];
  }
  return true;
}
