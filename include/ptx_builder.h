#ifndef PTX_BUILDER_H
#define PTX_BUILDER_H

#include <string>
#include <vector>
#include <sstream>

namespace ptx {

// Simple representation of a PTX register
class Reg {
    std::string name_;
public:
    explicit Reg(const std::string &name) : name_(name) {}
    const std::string &name() const { return name_; }
};

// Helper class to build PTX instruction strings
class Builder {
    std::vector<std::string> lines_;
public:
    void add(const Reg &d, const Reg &a, const Reg &b, const std::string &t = "f32");
    void mul(const Reg &d, const Reg &a, const Reg &b, const std::string &t = "f32");
    void mad(const Reg &d, const Reg &a, const Reg &b, const Reg &c, const std::string &t = "f32");
    void fma(const Reg &d, const Reg &a, const Reg &b, const Reg &c, const std::string &t = "f32");
    void ld_global(const Reg &d, const Reg &addr, const std::string &t = "f32");
    void st_global(const Reg &addr, const Reg &s, const std::string &t = "f32");
    void mov(const Reg &d, const Reg &s, const std::string &t = "f32");
    void ret();
    std::string str() const;
};

inline void Builder::add(const Reg &d, const Reg &a, const Reg &b, const std::string &t)
{
    lines_.emplace_back("add." + t + " " + d.name() + ", " + a.name() + ", " + b.name() + ";");
}

inline void Builder::mul(const Reg &d, const Reg &a, const Reg &b, const std::string &t)
{
    lines_.emplace_back("mul." + t + " " + d.name() + ", " + a.name() + ", " + b.name() + ";");
}

inline void Builder::mad(const Reg &d, const Reg &a, const Reg &b, const Reg &c, const std::string &t)
{
    lines_.emplace_back("mad." + t + " " + d.name() + ", " + a.name() + ", " + b.name() + ", " + c.name() + ";");
}

inline void Builder::fma(const Reg &d, const Reg &a, const Reg &b, const Reg &c, const std::string &t)
{
    lines_.emplace_back("fma." + t + " " + d.name() + ", " + a.name() + ", " + b.name() + ", " + c.name() + ";");
}

inline void Builder::ld_global(const Reg &d, const Reg &addr, const std::string &t)
{
    lines_.emplace_back("ld.global." + t + " " + d.name() + ", [" + addr.name() + "];\n");
}

inline void Builder::st_global(const Reg &addr, const Reg &s, const std::string &t)
{
    lines_.emplace_back("st.global." + t + " [" + addr.name() + "], " + s.name() + ";");
}

inline void Builder::mov(const Reg &d, const Reg &s, const std::string &t)
{
    lines_.emplace_back("mov." + t + " " + d.name() + ", " + s.name() + ";");
}

inline void Builder::ret()
{
    lines_.emplace_back("ret;");
}

inline std::string Builder::str() const
{
    std::ostringstream oss;
    for (const auto &l : lines_)
        oss << "    " << l << '\n';
    return oss.str();
}

} // namespace ptx

#endif // PTX_BUILDER_H
