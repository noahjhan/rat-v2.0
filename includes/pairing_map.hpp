#ifndef PAIRING_MAP_HPP
#define PAIRING_MAP_HPP

#include <initializer_list>
#include <unordered_map>
#include <utility>

template <typename T1, typename T2> class PairingMap
{
    public:
    PairingMap(std::initializer_list<std::pair<T1, T2>> init)
    : forward_map_(init), reverse_map_(reverse(init))
    {
    }

    const T2 &forward_at(const T1 &key) const
    {
        return forward_map_.at(key);
    }
    const T1 &reverse_at(const T2 &key) const
    {
        return reverse_map_.at(key);
    }

    private:
    static std::unordered_map<T2, T1>
    reverse(const std::initializer_list<std::pair<T1, T2>> &init)
    {
        std::unordered_map<T2, T1> reverse_map;
        for (const auto &pair : init) {
            reverse_map[pair.second] = pair.first;
        }
        return reverse_map;
    }

    const std::unordered_map<T1, T2> forward_map_;
    const std::unordered_map<T2, T1> reverse_map_;
};

#endif // PAIRING_MAP_HPP
