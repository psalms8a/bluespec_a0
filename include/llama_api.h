#ifndef LLAMA_API_H
#define LLAMA_API_H

#include <string>
#include <vector>
#include "tensor_ops.h"

namespace llama {

using tensor::Tensor;

struct ModelConfig {
    int num_layers;
    int hidden_size;
    int vocab_size;
};

class Model {
public:
    explicit Model(const ModelConfig &config);

    Tensor token_embedding;
    Tensor output_weight;
    std::vector<Tensor> layer_weights;
};

class Tokenizer {
public:
    std::vector<int> encode(const std::string &text) const;
    std::string decode(const std::vector<int> &tokens) const;
};

class InferenceSession {
public:
    explicit InferenceSession(const Model &model);
    std::vector<int> generate(const std::vector<int> &prompt, int max_tokens);
private:
    const Model &model_;
};

} // namespace llama

#endif // LLAMA_API_H
