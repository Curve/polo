#pragma once

#include <memory>
#include <utility>
#include <concepts>

namespace cr
{
    template <typename T>
    struct polo;

    template <typename T>
    void swap(polo<T> &, polo<T> &) noexcept;

    template <typename T>
    struct polo
    {
        struct impl;

      public:
        std::unique_ptr<impl> m_impl;

      public:
        explicit polo();

      public:
        template <typename U = T>
            requires std::derived_from<U, T>
        explicit polo(U &&);

        template <typename U, typename... Us>
            requires(std::derived_from<U, T> and std::constructible_from<U, Us...>)
        explicit polo(std::in_place_type_t<U>, Us &&...);

      public:
        polo(const polo &);
        polo(polo &&) noexcept;

      public:
        ~polo();

      public:
        polo &operator=(polo) noexcept;
        friend void swap<>(polo<T> &, polo<T> &) noexcept;

      public:
        template <typename Self>
        [[nodiscard]] decltype(auto) operator*(this Self &&);

        template <typename Self>
        [[nodiscard]] decltype(auto) operator->(this Self &&);

      public:
        [[nodiscard]] bool valueless_after_move() const;
    };
} // namespace cr

#include "polo.inl"
