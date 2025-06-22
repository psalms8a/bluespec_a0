#ifndef TENSOR_OPS_H
#define TENSOR_OPS_H

#include <cstddef>
#include <cstdint>
#include <vector>

namespace tensor {

enum class DataType {
    F32,
    F16,
    BF16,
    I32,
};

class Tensor {
public:
    Tensor() = default;
    Tensor(const std::vector<std::size_t> &shape, DataType type);

    const std::vector<std::size_t> &shape() const;
    DataType type() const;
    std::size_t numel() const;
    std::size_t elem_size() const;

    template <typename T> T *data();
    template <typename T> const T *data() const;

private:
    std::vector<std::size_t> shape_;
    DataType type_{DataType::F32};
    std::vector<unsigned char> buffer_;
};

inline Tensor::Tensor(const std::vector<std::size_t> &shape, DataType type)
    : shape_(shape), type_(type) {
    buffer_.resize(numel() * elem_size());
}

inline const std::vector<std::size_t> &Tensor::shape() const { return shape_; }
inline DataType Tensor::type() const { return type_; }
inline std::size_t Tensor::numel() const {
    std::size_t n = 1;
    for (std::size_t d : shape_)
        n *= d;
    return n;
}
inline std::size_t Tensor::elem_size() const {
    switch (type_) {
    case DataType::F32:
        return sizeof(float);
    case DataType::F16:
        return sizeof(_Float16);
    case DataType::BF16:
        return sizeof(__bf16);
    case DataType::I32:
        return sizeof(int32_t);
    }
    return sizeof(float);
}

template <typename T> inline T *Tensor::data() {
    return reinterpret_cast<T *>(buffer_.data());
}
template <typename T> inline const T *Tensor::data() const {
    return reinterpret_cast<const T *>(buffer_.data());
}

// Tensor operation APIs (unimplemented stubs)
Tensor matmul(const Tensor &a, const Tensor &b);
Tensor add(const Tensor &a, const Tensor &b);
Tensor softmax(const Tensor &a, int axis);
Tensor layer_norm(const Tensor &input, const Tensor &gamma, const Tensor &beta);
Tensor embedding_lookup(const Tensor &table, const Tensor &indices);
Tensor concat(const std::vector<Tensor> &inputs, int axis);

} // namespace tensor

#endif // TENSOR_OPS_H
