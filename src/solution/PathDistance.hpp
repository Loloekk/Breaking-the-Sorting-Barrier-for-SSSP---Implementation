#ifndef PATH_DISTANCE_H
#define PATH_DISTANCE_H

#include <string>

template <typename T>
struct PathDistance
{
    T dis;
    int count;
    int me;
    int last;

    PathDistance() : dis(0), count(0), me(0), last(0) {}
    PathDistance(const PathDistance &other) = default;

    PathDistance(T d, int c, int m, int l) : dis(d), count(c), me(m), last(l) {}

    bool operator<(const PathDistance &other) const
    {
        if (dis != other.dis)
            return dis < other.dis;
        if (count != other.count)
            return count < other.count;
        if (me != other.me)
            return me < other.me;
        return last < other.last;
    }

    bool operator>(const PathDistance &other) const
    {
        return other < *this;
    }

    bool operator==(const PathDistance &other) const
    {
        return (dis == other.dis && count == other.count &&
                me == other.me && last == other.last);
    }

    bool operator!=(const PathDistance &other) const
    {
        return !(*this == other);
    }

    bool operator<=(const PathDistance &other) const
    {
        return !(other < *this);
    }

    PathDistance &operator=(const PathDistance &other)
    {
        if (this != &other)
        {
            dis = other.dis;
            count = other.count;
            me = other.me;
            last = other.last;
        }
        return *this;
    }

    PathDistance operator+(const std::pair<int, T> &edge) const
    {
        return PathDistance(this->dis + edge.second, this->count + 1, edge.first, this->me);
    }

    std::string to_string() const
    {
        std::string output = "(";
        output += std::to_string(dis);
        output += ", ";
        output += std::to_string(count);
        output += ", ";
        output += std::to_string(me);
        output += ", ";
        output += std::to_string(last);
        output += ")";
        return output;
    }
};

#endif /*PATH_DISTANCE_H*/