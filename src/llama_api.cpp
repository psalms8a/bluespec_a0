#include "llama_api.h"
#include <random>
#include <sstream>
#include <unordered_map>

using namespace llama;
using namespace tensor;

static std::unordered_map<std::string, int> word2id;
static std::unordered_map<int, std::string> id2word;

Model::Model(const ModelConfig &config) {
    token_embedding = Tensor({(std::size_t)config.vocab_size,
                              (std::size_t)config.hidden_size},
                             DataType::F32);
    output_weight = Tensor({(std::size_t)config.hidden_size,
                            (std::size_t)config.vocab_size},
                           DataType::F32);
    layer_weights.resize(config.num_layers);
    for (auto &w : layer_weights)
        w = Tensor({(std::size_t)config.hidden_size,
                    (std::size_t)config.hidden_size},
                   DataType::F32);

    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(-0.5f, 0.5f);
    auto init = [&](Tensor &t) {
        for (std::size_t i = 0; i < t.numel(); ++i)
            t.data<float>()[i] = dist(gen);
    };
    init(token_embedding);
    init(output_weight);
    for (auto &w : layer_weights)
        init(w);
}

std::vector<int> Tokenizer::encode(const std::string &text) const {
    std::vector<int> tokens;
    std::istringstream iss(text);
    std::string word;
    while (iss >> word) {
        auto it = word2id.find(word);
        if (it == word2id.end()) {
            int id = word2id.size();
            word2id[word] = id;
            id2word[id] = word;
            tokens.push_back(id);
        } else {
            tokens.push_back(it->second);
        }
    }
    return tokens;
}

std::string Tokenizer::decode(const std::vector<int> &tokens) const {
    std::ostringstream oss;
    for (std::size_t i = 0; i < tokens.size(); ++i) {
        auto it = id2word.find(tokens[i]);
        if (it != id2word.end())
            oss << (i ? " " : "") << it->second;
        else
            oss << (i ? " " : "") << "?";
    }
    return oss.str();
}

InferenceSession::InferenceSession(const Model &model) : model_(model) {}

static int argmax(const Tensor &t) {
    float maxv = -1e9f;
    int idx = 0;
    for (std::size_t i = 0; i < t.numel(); ++i) {
        float v = t.data<const float>()[i];
        if (v > maxv) {
            maxv = v;
            idx = i;
        }
    }
    return idx;
}

std::vector<int> InferenceSession::generate(const std::vector<int> &prompt,
                                            int max_tokens) {
    std::vector<int> out_tokens = prompt;
    for (int t = 0; t < max_tokens; ++t) {
        int token = out_tokens.back();
        Tensor idx_tensor({1}, DataType::I32);
        idx_tensor.data<int32_t>()[0] = token;
        Tensor hidden = embedding_lookup(model_.token_embedding, idx_tensor);
        for (const Tensor &w : model_.layer_weights) {
            hidden = matmul(hidden, w);
            hidden = softmax(hidden, 1);
        }
        Tensor logits = matmul(hidden, model_.output_weight);
        logits = softmax(logits, 1);
        int next = argmax(logits);
        out_tokens.push_back(next);
    }
    return out_tokens;
}

