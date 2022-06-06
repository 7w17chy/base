#pragma once

#include <optional>
#include <string>
#include <type_traits>
#include <variant>

/* Copyright (c) 2021, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Claus
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE. */

namespace base
{

class Error
{
  public:
    static Error from_errno(int code) { return Error(code); }
    static Error from_string_literal(std::string &&str_literal)
    {
        return Error(std::move(str_literal));
    }

    bool is_errno() const { return m_code != 0; }

    int code() const { return m_code; }
    const std::string &string_literal() const { return m_string_literal; }

  protected:
    Error(int code) : m_code(code) {}

  private:
    Error(std::string &&str_literal) : m_string_literal(std::move(str_literal))
    {
    }

    int m_code{0};
    std::string m_string_literal;
};

template <typename T, typename ErrorType>
class [[nodiscard]] ErrorOr final : public std::variant<T, ErrorType>
{
  public:
    template <typename U>
    ErrorOr(U &&value) requires(
        !std::is_same<std::remove_reference<U>, ErrorOr<T, ErrorType>>::value)
        : std::variant<T, ErrorType>(std::forward<U>(value))
    {
    }

    T &value() { return std::get<T>(*this); }

    T const &value() const { return std::get<T>(*this); }

    ErrorType &error() { return std::get<ErrorType>(); }

    ErrorType const &error() const { return std::get<ErrorType>(*this); }

    bool is_error() const { return std::holds_alternative<ErrorType>(*this); }

    T release_value() { return std::move(value()); }
    ErrorType release_error() { return std::move(error()); }
};

// Partial specialization for void value type
template <typename ErrorType> class [[nodiscard]] ErrorOr<void, ErrorType>
{
  public:
    ErrorOr(ErrorType error) : m_error(std::move(error)) {}

    ErrorOr() = default;
    ErrorOr(ErrorOr &&other) = default;
    ErrorOr(ErrorOr const &other) = default;
    ~ErrorOr() = default;

    ErrorOr &operator=(ErrorOr &&other) = default;
    ErrorOr &operator=(ErrorOr const &other) = default;

    ErrorType &error() { return m_error.value(); }
    bool is_error() const { return m_error.has_value(); }
    ErrorType release_error() { return m_error.release_value(); }
    void release_value() {}

  private:
    std::optional<ErrorType> m_error;
};

// partial specialization for ErrorOr using Error as ErrorType
template <typename T> using Err = ErrorOr<T, Error>;
} // namespace base
