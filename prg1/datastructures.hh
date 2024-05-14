// Datastructures.hh
//
// Student name:
// Student email:
// Student number:

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <deque>

// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;
// Useful definitions for some pair types
using idDistPair = std::pair< TownID, Distance >;
using idNamePair = std::pair< TownID, Name >;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    //!!! In the following estimations of asymptotic complexity n is always the number of towns stored
    // in the towndata_ database !!!

    // Estimate of performance: O(1), constant
    // Short rationale for estimate: Only using the size-method of unordered_map, which is O(1)
    unsigned int town_count();

    // Estimate of performance: O(n), linear
    // Short rationale for estimate: clear-method of unordered_map
    void clear_all();

    // Estimate of performance: Average constant, worst-case scenario O(n)
    // Short rationale for estimate: using insert-method of unordered_map
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance: average constant, worst-case linear O(n)
    // Short rationale for estimate: using find and at -methods for unordered map
    Name get_town_name(TownID id);

    // Estimate of performance: average constant, worst-case linear O(n)
    // Short rationale for estimate: using find and at -methods for unordered map
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: average constant, worst-case linear O(n)
    // Short rationale for estimate: using find and at -methods for unordered map
    int get_town_tax(TownID id);

    // Estimate of performance: linear O(n)
    // Short rationale for estimate: using push_back-method (constant) for n times
    std::vector<TownID> all_towns();

    // Estimate of performance: linear O(n)
    // Short rationale for estimate: using push_back-method (constant) for n times
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance: constant on average, worst-case linear O(n)
    // Short rationale for estimate: indexing with at-method, using find-method,
    // both linear on worst-case scenario
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: sorting with STL:s algorithm
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: same as towns_nearest
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: using towns_nearest, same complexity
    TownID min_distance();

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: using towns_nearest, same complexity
    TownID max_distance();

    // Estimate of performance: worst-case O(n), in most cases (almost always) constant
    // Short rationale for estimate: using find and at -methods of unordered_map
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: worst-case O(n), in most cases (almost always) constant
    // Short rationale for estimate: using find and at -methods of unordered_map
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance: worst-case O(m*n), almost always O(m), where m describes the "depth"
    // of the master chain
    // Short rationale for estimate: indexing with at-method of unordered_map inside a loop
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance: worst-case n squared, O(n**2), almost always O(n), though
    // Short rationale for estimate: indexing with at-method of unordered_map inside a loop
    bool remove_town(TownID id);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: using STL:s sort-algorithm
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance: O(m*k), where m describes the "width" of the town vassalships and
    // k is the "depth" of the vassal chain
    // Short rationale for estimate: using private method vassals_recursive;
    // recursively looping through the vassals, only constant commands inside
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: O(m*n), where m describes the "width" of the town vassalships and
    // k is the "depth" of the vassal chain
    // Short rationale for estimate: using private recursive function taxFromVassal;
    // it has for loops and recursion combined
    int total_net_tax(TownID id);

private:
    struct Town{
        Name name_ = "";
        TownID id_ = "";
        Coord coord_ = NO_COORD;
        int tax_ = 0;    // tax from peasants, THIS IS NOT the net tax
        std::vector<TownID> vassals_ = {};
        TownID master_ = NO_TOWNID;
    };
    std::unordered_map <TownID, Town> towndata_ = {};

    // Used to find the longest vassal path
    std::deque<TownID> vassalsRecursive(TownID v);

    // Calculates tax which vassal id pays to master its recursively
    int taxFromVassal(TownID id);

    // Function calculates distances between 2 coords (rounded down to int)
    Distance dist(Coord const &c1, Coord const &c2);
};

#endif // DATASTRUCTURES_HH
