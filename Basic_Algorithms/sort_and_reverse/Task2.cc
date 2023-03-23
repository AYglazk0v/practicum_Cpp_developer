#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

pair<bool, double> CalcMedian(vector<double> samples) {
  if (samples.empty()) {
    return {false, 0};
  }

  std::sort(samples.begin(), samples.end());

  if (samples.size() % 2 == 0) {
    return {true, (0.0 + samples[samples.size() / 2 - 1] +
                   samples[samples.size() / 2]) /
                      2};
  }
  return {true, samples[samples.size() / 2]};
}

int main() {
  int size;
  cin >> size;

  vector<double> samples;
  for (int i = 0; i < size; ++i) {
    double sample;
    cin >> sample;
    samples.push_back(sample);
  }

  pair<bool, double> result = CalcMedian(samples);
  if (result.first) {
    cout << result.second << endl;
  } else {
    cout << "Empty vector"s << endl;
  }
}
