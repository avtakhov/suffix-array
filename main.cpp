//
// Created by avtakhov on 20.12.2020.
//

#include "suffix-array.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <utility>
#include <vector>
#include <cstring>
#include <functional>
#include <memory>

ss_iterator::ss_iterator(shifted_string const& ss, size_t index)
    : ss_ref(ss)
    , index(index)
{}

ss_iterator& ss_iterator::operator++()
{
    index++;
    return *this;
}

ss_iterator ss_iterator::operator++(int)
{
    ss_iterator res = *this;
    index++;
    return res;
}

bool ss_iterator::operator==(ss_iterator const& other) const
{
    return this->index == other.index;
}

bool ss_iterator::operator!=(ss_iterator const& other) const
{
    return !(*this == other);
}

ss_iterator ss_iterator::operator+(difference_type d)
{
    auto res(*this);
    return res += d;
}

ss_iterator& ss_iterator::operator+=(difference_type d)
{
    index += d;
    return *this;
}

shifted_string::shifted_string(std::shared_ptr<std::string const> ptr, size_t start)
    : ptr(std::move(ptr))
    , start(start)
{}

char shifted_string::operator[](size_t index) const
{
    if (index >= ptr->size())
    {
        return 0;
    }
    return ptr->operator[]((index + start) % ptr->size());
}

[[nodiscard]] ss_iterator shifted_string::begin() const
{
    return ss_iterator(*this, 0);
}

[[nodiscard]] ss_iterator shifted_string::end() const
{
    return ss_iterator(*this, ptr->size());
}

bool shifted_string::operator<(shifted_string const& other) const
{
    return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
}

std::ostream& operator<<(std::ostream& os, shifted_string const& s)
{
    for (auto i : s)
    {
        os << i;
    }
    return os;
}

char ss_iterator::operator*() const
{
    return ss_ref.get()[index];
}

std::vector<shifted_string> suffix_array(std::string const& s)
{
    auto ptr = std::make_shared<std::string const>(s);
    std::vector<shifted_string> a;
    a.reserve(s.size());
    for (int i = 0; i < s.size(); ++i)
    {
        a.emplace_back(ptr, i);
    }
    std::vector<int> compare(a.size());
    std::vector<int> help(a.size());

    for (auto const& i : a)
    {
        compare[i.start] = static_cast<unsigned char>(i[0]);
    }

    std::function<bool(shifted_string const&, shifted_string const&)> comp;

    for (int cur = 1; (1 << cur) < 2 * s.size(); ++cur)
    {
        int prev = cur - 1;
        comp = [&compare, &prev, &s](shifted_string const& i, shifted_string const& j)
        {
            if (compare[i.start] != compare[j.start])
                return compare[i.start] < compare[j.start];
            else
                return compare[(i.start + (1 << prev)) % s.size()] < compare[(j.start + (1 << prev)) % s.size()];
        };
        std::sort(a.begin(), a.end(), comp);
        help[a[0].start] = 0;
        for (int j = 1; j < help.size(); ++j)
        {
            help[a[j].start] = help[a[j - 1].start] + (comp(a[j - 1], a[j]) ? 1 : 0);
        }
        std::copy(help.begin(), help.end(), compare.begin());
    }
    a.resize(std::unique(a.begin(), a.end(),
                         [&comp](shifted_string const& a, shifted_string const& b)
                         {
                             return !comp(a, b) && !comp(b, a);
                         }) - a.begin());
    return a;
}

void solve(std::istream& is, std::ostream& os)
{
    std::string s;
    int x;
    is >> s >> x;
    x--;
    auto suf = suffix_array(s);
    os << (x < suf.size() ? std::string(suf[x].begin(), suf[x].end()) : "IMPOSSIBLE");
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    solve(std::cin, std::cout);
}