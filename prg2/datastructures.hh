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
#include <unordered_set>
#include <queue>

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
using distIdPair = std::pair< Distance, TownID >;
using Road = std::pair< TownID, TownID >;

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

    // Estimate of performance:
    // Short rationale for estimate:
    unsigned int town_count();

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_all();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_town_name(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Coord get_town_coordinates(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int get_town_tax(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> all_towns();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID min_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID max_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_town(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int total_net_tax(TownID id);


    // Phase 2 operations

    // In the following estimations of performance n,
    // if not defined otherwise, is the amount of towns stored
    // And m is the amount of roads stored

    // Estimate of performance: O(n*m)
    // Short rationale for estimate: using clear for each town
    void clear_roads();

    // Estimate of performance: O(n*m²)
    // Short rationale for estimate: Looping through all roads and
    // using find for each
    std::vector<std::pair<TownID, TownID>> all_roads();

    // Estimate of performance: O(m)
    // Short rationale for estimate: using find the roads of a town
    bool add_road(TownID town1, TownID town2);

    // Estimate of performance: O(m)
    // Short rationale for estimate: adding each road of town to vector
    std::vector<TownID> get_roads_from(TownID id);

    // Estimate of performance: O(n+m)
    // Short rationale for estimate: using BFS
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // Non-compulsory phase 2 operations

    // Estimate of performance: worst case O(m), average constant
    // Short rationale for estimate: using erase for unordered_map
    bool remove_road(TownID town1, TownID town2);

    // Estimate of performance: O(n+m)
    // Short rationale for estimate: same as any_route
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // Estimate of performance: O(n+m)
    // Short rationale for estimate: using DFS
    std::vector<TownID> road_cycle_route(TownID startid);

    // Estimate of performance: O((n+k)*log(n+k))
    // Short rationale for estimate: using Djikstra
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // Estimate of performance: O(k*log(k))
    // Short rationale for estimate: sorting vector by distance value
    Distance trim_road_network();

private:
    // Add stuff needed for your class implementation here

    enum Color {BLACK, GRAY, WHITE, RED};

    struct Town{
        Name name_ = "";
        TownID id_ = "";
        Coord coord_ = NO_COORD;
        int tax_ = 0;    // tax from peasants, THIS IS NOT the net tax
        std::vector<TownID> vassals_ = {};
        TownID master_ = NO_TOWNID;
        // Phase 2:
        // set of townid:s that are connected by road
        //std::unordered_set<TownID> roads_ = {};
        // dist is the "weight" of edge (road)
        std::unordered_map<TownID, Distance> roads_ = {};
        TownID prevtown_ = NO_TOWNID;  // Previous town when going through graph
        TownID prev2_ = NO_TOWNID;   // Used in finding cycle
        bool isVisited_ = false;    // If visited, true
    };
    
    std::unordered_map <TownID, Town> towndata_ = {};

    // Used to find the longest vassal path
    std::deque<TownID> vassalsRecursive(TownID v);

    // Calculates tax which vassal id pays to master its recursively
    int taxFromVassal(TownID id);

    // Function calculates distances between 2 coords (rounded down to int)
    Distance dist(Coord const &c1, Coord const &c2);

    // Recursive help function for cycle finding
    bool hasCycle(const TownID &id);

};

#endif // DATASTRUCTURES_HH
