#include <iostream>
#include "../include/llama_api.h"

// Example routine showing how the tensor APIs could be used to run a Llama model
// inference. The API functions themselves are not implemented.

using namespace llama;

int main() {
    // Configure a tiny model for demonstration
    ModelConfig cfg{ /*num_layers*/ 2, /*hidden_size*/ 8, /*vocab_size*/ 32000 };
    Model model(cfg);
    Tokenizer tokenizer;
    InferenceSession session(model);

    // Encode input text
    std::vector<int> prompt = tokenizer.encode("Hello Llama");

    // Generate output tokens
    std::vector<int> output_tokens = session.generate(prompt, 16);

    // Decode to string
    std::string output = tokenizer.decode(output_tokens);
    std::cout << output << std::endl;
    return 0;
}
