#pragma once

#include "polo.hpp"
#include <concepts>

namespace cr
{
    template <typename T>
    struct polo<T>::impl
    {
        template <typename U>
        struct model;

      public:
        virtual ~impl() = default;

      public:
        [[nodiscard]] virtual T *value()                   = 0;
        [[nodiscard]] virtual std::unique_ptr<impl> copy() = 0;
    };

    template <typename T>
    template <typename U>
    struct polo<T>::impl::model : polo<T>::impl
    {
        U m_value;

      public:
        template <typename... Us>
        model(std::in_place_t, Us &&...);

      public:
        T *value() override;
        std::unique_ptr<impl> copy() override;
    };

    template <typename T>
    template <typename U>
    template <typename... Us>
    polo<T>::impl::model<U>::model(std::in_place_t, Us &&...args) : m_value(std::forward<Us>(args)...)
    {
    }

    template <typename T>
    template <typename U>
    T *polo<T>::impl::model<U>::value()
    {
        return std::addressof(m_value);
    }

    template <typename T>
    template <typename U>
    std::unique_ptr<typename polo<T>::impl> polo<T>::impl::model<U>::copy()
    {
        return std::make_unique<model<U>>(std::in_place_t{}, m_value);
    }

    template <typename T>
    polo<T>::polo() : m_impl(std::make_unique<typename impl::template model<T>>(std::in_place_t{}))
    {
    }

    template <typename T>
    template <typename U>
        requires std::derived_from<U, T>
    polo<T>::polo(U &&value) : m_impl(std::make_unique<typename impl::template model<U>>(std::in_place_t{}, std::forward<U>(value)))
    {
    }

    template <typename T>
    template <typename U, typename... Us>
        requires(std::derived_from<U, T> and std::constructible_from<U, Us...>)
    polo<T>::polo(std::in_place_type_t<U>, Us &&...values)
        : m_impl(std::make_unique<typename impl::template model<U>>(std::in_place_t{}, std::forward<Us>(values)...))
    {
    }

    template <typename T>
    polo<T>::polo(const polo &other) : m_impl(other.m_impl->copy())
    {
    }

    template <typename T>
    polo<T>::polo(polo &&other) noexcept : m_impl(std::move(other.m_impl))
    {
    }

    template <typename T>
    polo<T>::~polo() = default;

    template <typename T>
    polo<T> &polo<T>::operator=(polo other) noexcept
    {
        swap(*this, other);
        return *this;
    }

    template <typename T>
    void swap(polo<T> &first, polo<T> &second) noexcept
    {
        using std::swap;
        swap(first.m_impl, second.m_impl);
    }

    template <typename T>
    template <typename Self>
    decltype(auto) polo<T>::operator*(this Self &&self)
    {
        using type = std::conditional_t<std::is_const_v<Self>, const T &, T &>;
        return static_cast<type>(*std::forward<Self>(self).m_impl->value());
    }

    template <typename T>
    template <typename Self>
    decltype(auto) polo<T>::operator->(this Self &&self)
    {
        using type = std::conditional_t<std::is_const_v<Self>, const T *, T *>;
        return static_cast<type>(std::forward<Self>(self).m_impl->value());
    }

    template <typename T>
    bool polo<T>::valueless_after_move() const
    {
        return !m_impl;
    }
} // namespace cr
