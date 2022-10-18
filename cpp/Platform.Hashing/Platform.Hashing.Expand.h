#pragma once

// Based on https://security.stackexchange.com/a/209892

#include "Platform.Hashing.Combine.h"

namespace Platform::Hashing
{
    [[deprecated("it's identity - later will be removed")]] std::size_t Expand(std::size_t originalHash)
    {
        return originalHash;
    }
}
