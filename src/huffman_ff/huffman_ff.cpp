
#include "huffman_ff.h"

#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include <unordered_map>

#include "../utimer.cpp"

using namespace std;
using namespace ff;
struct Task {
  int task_id;
  int n_encoders;
  std::string* seq;
  std::vector<std::vector<bool>*>* chunk;
  std::unordered_map<char, std::vector<bool>*>* codes;

  Task(int task_id, std::string* seq, int n_encoders,
       std::vector<std::vector<bool>*>* chunk,
       std::unordered_map<char, std::vector<bool>*>* codes) {
    this->task_id = task_id;
    this->seq = seq;
    this->codes = codes;
    this->chunk = chunk;
    this->n_encoders = n_encoders;
  }
};

class Emitter : public ff::ff_monode_t<Task> {
 private:
  int n_encoders;
  std::string seq;
  std::unordered_map<char, std::vector<bool>*> codes;
  encoded_t partial_res;

 public:
  Emitter(int n_encoders, std::unordered_map<char, std::vector<bool>*> codes,
          const std::string& seq)
      : n_encoders(n_encoders) {
    this->seq = seq;
    this->codes = std::move(codes);
  }
  Task* svc(Task*) override {
    for (int i = 0; i < n_encoders; i++) {
      Task* t = new Task(i, &seq, n_encoders, nullptr, &codes);
      ff_send_out(t);
    }
    return EOS;
  }
};

class Collector : public ff::ff_node_t<Task> {
 private:
  encoded_t* partial_res;

 public:
  Task* svc(Task* t) override {
    partial_res->at(t->task_id) = t->chunk;
    delete t;
    return GO_ON;
  }
  explicit Collector(encoded_t* partial_res) {
    // get pointer to result vector
    this->partial_res = partial_res;
  }
};

Task* Worker(Task* t, ff::ff_node* nn) {
  auto n_encoders = t->n_encoders;
  auto tid = t->task_id;
  auto size = t->seq->length();
  auto start = tid * (size / n_encoders);
  auto stop = (tid == n_encoders - 1) ? size : (tid + 1) * (size / n_encoders);

  // allocating memory for chunk -> make memory allocation parallel time.
  t->chunk = new std::vector<std::vector<bool>*>();
  t->chunk->reserve(stop - start);

  for (auto i = start; i < stop; i++) {
    auto seq = t->seq->at(i);
    t->chunk->push_back(t->codes->at(seq));
  }
  return t;
}

unordered_map<char, unsigned int> huffman_ff::count_frequency() {
  auto res = unordered_map<char, unsigned int>();
  auto map_f = [&](const long i, unordered_map<char, unsigned>& tempsum) {
    tempsum[text[i]]++;
  };

  auto red_f = [&](unordered_map<char, unsigned>& a,
                   const unordered_map<char, unsigned>& b) {
    for (auto& it : b) a[it.first] += it.second;
  };
  auto pf = ParallelForReduce<unordered_map<char, unsigned>>((long)num_threads);
  pf.parallel_reduce(res, unordered_map<char, unsigned>(), 0, (long)text.size(),
                     1, map_f, red_f, num_threads);
  return res;
}

encoded_t* huffman_ff::encode_string() {
  auto results = new encoded_t(num_threads);
  // auto emitter = Emitter((int)num_threads, codes, text);
  // auto collector = Collector(results);

  // create FF farm with n_encoders workers
  // ff::ff_Farm<Task> farm(Worker, (long)num_threads);
  // farm.add_emitter(emitter);
  // farm.add_collector(collector);
  // farm.run_and_wait_end();
  ParallelFor pf(num_threads);
  auto Map = [&](const long start, const long stop, const int thid) {
    auto chunk = new std::vector<std::vector<bool>*>();
    chunk->reserve(stop - start);
    for (auto i = start; i < stop; i++) {
      chunk->push_back(codes[text[i]]);
    }
    results->at(thid) = chunk;
  };
  pf.parallel_for_idx(0, text.size(), 1, 0, Map, num_threads);
  return results;
}
