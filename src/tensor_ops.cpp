#include "tensor_ops.h"
#include <stdexcept>
#include <cmath>
#include <algorithm>

using namespace tensor;

static float get_value(const Tensor &t, std::size_t idx) {
    switch (t.type()) {
    case DataType::F32:
        return t.data<const float>()[idx];
    case DataType::F16:
        return static_cast<float>(t.data<const _Float16>()[idx]);
    case DataType::BF16:
        return static_cast<float>(t.data<const __bf16>()[idx]);
    case DataType::I32:
        return static_cast<float>(t.data<const int32_t>()[idx]);
    }
    return 0.f;
}

static void set_value(Tensor &t, std::size_t idx, float v) {
    switch (t.type()) {
    case DataType::F32:
        t.data<float>()[idx] = v;
        break;
    case DataType::F16:
        t.data<_Float16>()[idx] = (_Float16)v;
        break;
    case DataType::BF16:
        t.data<__bf16>()[idx] = (__bf16)v;
        break;
    case DataType::I32:
        t.data<int32_t>()[idx] = static_cast<int32_t>(v);
        break;
    }
}

Tensor tensor::matmul(const Tensor &a, const Tensor &b) {
    if (a.shape().size() != 2 || b.shape().size() != 2)
        throw std::runtime_error("matmul requires 2D tensors");
    std::size_t m = a.shape()[0];
    std::size_t k = a.shape()[1];
    if (b.shape()[0] != k)
        throw std::runtime_error("matmul shape mismatch");
    std::size_t n = b.shape()[1];
    Tensor out({m, n}, DataType::F32);
    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            float sum = 0.f;
            for (std::size_t p = 0; p < k; ++p) {
                sum += get_value(a, i * k + p) * get_value(b, p * n + j);
            }
            set_value(out, i * n + j, sum);
        }
    }
    return out;
}

Tensor tensor::add(const Tensor &a, const Tensor &b) {
    if (a.shape() != b.shape())
        throw std::runtime_error("add shape mismatch");
    Tensor out(a.shape(), DataType::F32);
    std::size_t n = a.numel();
    for (std::size_t i = 0; i < n; ++i)
        set_value(out, i, get_value(a, i) + get_value(b, i));
    return out;
}

Tensor tensor::softmax(const Tensor &a, int axis) {
    if (a.shape().size() != 2)
        throw std::runtime_error("softmax only supports 2D");
    std::size_t rows = a.shape()[0];
    std::size_t cols = a.shape()[1];
    Tensor out(a.shape(), DataType::F32);
    if (axis == 0) {
        for (std::size_t j = 0; j < cols; ++j) {
            float maxv = -1e9;
            for (std::size_t i = 0; i < rows; ++i)
                maxv = std::max(maxv, get_value(a, i * cols + j));
            float sum = 0.f;
            for (std::size_t i = 0; i < rows; ++i) {
                float e = std::exp(get_value(a, i * cols + j) - maxv);
                sum += e;
                set_value(out, i * cols + j, e);
            }
            for (std::size_t i = 0; i < rows; ++i) {
                float val = get_value(out, i * cols + j) / sum;
                set_value(out, i * cols + j, val);
            }
        }
    } else {
        for (std::size_t i = 0; i < rows; ++i) {
            float maxv = -1e9;
            for (std::size_t j = 0; j < cols; ++j)
                maxv = std::max(maxv, get_value(a, i * cols + j));
            float sum = 0.f;
            for (std::size_t j = 0; j < cols; ++j) {
                float e = std::exp(get_value(a, i * cols + j) - maxv);
                sum += e;
                set_value(out, i * cols + j, e);
            }
            for (std::size_t j = 0; j < cols; ++j) {
                float val = get_value(out, i * cols + j) / sum;
                set_value(out, i * cols + j, val);
            }
        }
    }
    return out;
}

Tensor tensor::layer_norm(const Tensor &input, const Tensor &gamma, const Tensor &beta) {
    if (input.shape() != gamma.shape() || input.shape() != beta.shape())
        throw std::runtime_error("layer_norm shape mismatch");
    Tensor out(input.shape(), DataType::F32);
    std::size_t n = input.numel();
    float mean = 0.f;
    for (std::size_t i = 0; i < n; ++i)
        mean += get_value(input, i);
    mean /= n;
    float var = 0.f;
    for (std::size_t i = 0; i < n; ++i) {
        float diff = get_value(input, i) - mean;
        var += diff * diff;
    }
    var /= n;
    float inv_std = 1.0f / std::sqrt(var + 1e-5f);
    for (std::size_t i = 0; i < n; ++i) {
        float norm = (get_value(input, i) - mean) * inv_std;
        float val = norm * get_value(gamma, i) + get_value(beta, i);
        set_value(out, i, val);
    }
    return out;
}

Tensor tensor::embedding_lookup(const Tensor &table, const Tensor &indices) {
    if (table.shape().size() != 2 || indices.shape().size() != 1)
        throw std::runtime_error("embedding_lookup shape");
    std::size_t vocab = table.shape()[0];
    std::size_t dim = table.shape()[1];
    std::size_t count = indices.shape()[0];
    Tensor out({count, dim}, DataType::F32);
    for (std::size_t i = 0; i < count; ++i) {
        int idx = static_cast<int>(get_value(indices, i));
        if (idx < 0 || static_cast<std::size_t>(idx) >= vocab)
            throw std::runtime_error("embedding index out of range");
        for (std::size_t j = 0; j < dim; ++j) {
            float v = get_value(table, idx * dim + j);
            set_value(out, i * dim + j, v);
        }
    }
    return out;
}

Tensor tensor::concat(const std::vector<Tensor> &inputs, int axis) {
    if (inputs.empty())
        throw std::runtime_error("concat empty");
    auto shape = inputs[0].shape();
    std::size_t rank = shape.size();
    for (const auto &t : inputs)
        if (t.shape().size() != rank)
            throw std::runtime_error("concat rank mismatch");
    std::vector<std::size_t> out_shape = shape;
    for (std::size_t i = 1; i < inputs.size(); ++i) {
        for (std::size_t d = 0; d < rank; ++d) {
            if (d == static_cast<std::size_t>(axis))
                out_shape[d] += inputs[i].shape()[d];
            else if (inputs[i].shape()[d] != shape[d])
                throw std::runtime_error("concat shape mismatch");
        }
    }
    Tensor out(out_shape, DataType::F32);
    std::size_t inner = 1;
    for (std::size_t d = axis + 1; d < rank; ++d)
        inner *= shape[d];
    std::size_t outer = 1;
    for (std::size_t d = 0; d < static_cast<std::size_t>(axis); ++d)
        outer *= shape[d];
    std::size_t offset = 0;
    for (std::size_t i = 0; i < outer; ++i) {
        for (const auto &t : inputs) {
            std::size_t len = t.shape()[axis] * inner;
            for (std::size_t j = 0; j < len; ++j) {
                set_value(out, offset++, get_value(t, i * len + j));
            }
        }
    }
    return out;
}

