#ifndef TENSOR_OPS_H
#define TENSOR_OPS_H

#include <cstddef>
#include <vector>

namespace tensor {

enum class DataType {
    F32,
    F16,
    I32,
};

class Tensor {
public:
    Tensor() = default;
    Tensor(const std::vector<std::size_t> &shape, DataType type);

    const std::vector<std::size_t> &shape() const;
    DataType type() const;

private:
    std::vector<std::size_t> shape_;
    DataType type_{DataType::F32};
};

inline Tensor::Tensor(const std::vector<std::size_t> &shape, DataType type)
    : shape_(shape), type_(type) {}

inline const std::vector<std::size_t> &Tensor::shape() const { return shape_; }
inline DataType Tensor::type() const { return type_; }

// Tensor operation APIs (unimplemented stubs)
Tensor matmul(const Tensor &a, const Tensor &b);
Tensor add(const Tensor &a, const Tensor &b);
Tensor softmax(const Tensor &a, int axis);
Tensor layer_norm(const Tensor &input, const Tensor &gamma, const Tensor &beta);
Tensor embedding_lookup(const Tensor &table, const Tensor &indices);
Tensor concat(const std::vector<Tensor> &inputs, int axis);

} // namespace tensor

#endif // TENSOR_OPS_H
