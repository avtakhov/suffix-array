//
// Created by avtakhov on 20.12.2020.
//

#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H

#include <iostream>
#include <functional>
#include <memory>
#include <vector>

struct shifted_string;

struct ss_iterator
{
    using difference_type = std::ptrdiff_t;
    using value_type = char const;
    using pointer = value_type *;
    using reference = value_type &;
    using iterator_category = std::forward_iterator_tag;

    ss_iterator(shifted_string const& ss, size_t index);

    ss_iterator(ss_iterator const& other) = default;

    ss_iterator& operator++();

    ss_iterator operator++(int);

    bool operator==(ss_iterator const& other) const;

    bool operator!=(ss_iterator const& other) const;

    char operator*() const;

    ss_iterator operator+(difference_type d);
    ss_iterator& operator+=(difference_type d);

private:
    std::reference_wrapper<shifted_string const> ss_ref;
    size_t index;
};

std::ostream& operator<<(std::ostream& os, shifted_string const& s);

struct shifted_string
{
    std::shared_ptr<std::string const> ptr;
    size_t start = 0;

    shifted_string(std::shared_ptr<std::string const> ptr, size_t start);

    shifted_string() = default;

    char operator[](size_t index) const;

    [[nodiscard]] ss_iterator begin() const;

    [[nodiscard]] ss_iterator end() const;

    bool operator<(shifted_string const& other) const;

};

std::vector<shifted_string> suffix_array(std::string const& s);

#endif //SUFFIX_ARRAY_H
