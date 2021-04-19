#ifndef PLATFORM_HASHING_BASE
#define PLATFORM_HASHING_BASE

namespace Platform::Hashing
{
    template<typename TValue>
    using HasherFunctionType = std::function<std::size_t(const TValue&)>
}

#endif //PLATFORM_HASHING_BASE
