// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
    towndata_ = {};
}

unsigned int Datastructures::town_count()
{
    return towndata_.size();
}

void Datastructures::clear_all()
{
    towndata_.clear();
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    unsigned int num = towndata_.size(); // Tallenetaan koko
    Town t;
    t.id_ = id;
    t.name_ = name;
    t.coord_ = coord;
    t.tax_ = tax;
    t.master_ = NO_TOWNID;
    t.vassals_ = {};
    towndata_.insert({id, t});
    // Examining, if the dataset size has got bigger. If id already exists,
    // insert-method doesnt do anything
    return towndata_.size() > num;
}

Name Datastructures::get_town_name(TownID id)
{
    if (towndata_.find(id) == towndata_.end()) {
        return NO_NAME;
    }
    return towndata_.at(id).name_;
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    if (towndata_.find(id) == towndata_.end()) {
        return NO_COORD;
    }
    return towndata_.at(id).coord_;
}

int Datastructures::get_town_tax(TownID id)
{
    if (towndata_.find(id) == towndata_.end()) {
        return NO_VALUE;
    }
    return towndata_.at(id).tax_;
}

std::vector<TownID> Datastructures::all_towns()
{
    std::vector<TownID> towns = {};
    std::unordered_map <TownID, Town>::iterator it;
    for (it = towndata_.begin(); it != towndata_.end(); it++) {
        towns.push_back(it->first);
    }
    return towns;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::vector<TownID> towns = {};
    std::unordered_map <TownID, Town>::iterator it;
    for (it = towndata_.begin(); it != towndata_.end(); it++) {
        if (it->second.name_ == name) {
            towns.push_back(it->first);
        }
    }
    return towns;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    if (towndata_.find(id) == towndata_.end()) {
        return false;
    }
    towndata_.at(id).name_ = newname;
    return true;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    // Adding id,name -pairs to vector
    std::vector<idNamePair> townnames = {};
    idNamePair p;
    // Adding town names
    for (auto datapair : towndata_) {
        p.first = datapair.first;
        p.second = datapair.second.name_;
        townnames.push_back(p);
    }
    // Sorting with lambda
    sort(townnames.begin(), townnames.end(), [](const idNamePair &a,
         const idNamePair &b) {return a.second < b.second;});

    // Creating corresponding id-vector
    std::vector<TownID> ids = {};
    int s = townnames.size();
    for (int i = 0; i < s; i++) {
        ids.push_back(townnames.at(i).first);
    }
    return ids;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    // Using more general method towns_nearest
    Coord c0;
    c0.x = 0;
    c0.y = 0;
    return towns_nearest(c0);
}

TownID Datastructures::min_distance()
{
    if (towndata_.empty()) {
        return NO_TOWNID;
    }
    // Getting towns in increasing distance order
    std::vector<TownID> ids =  towns_distance_increasing();
    return ids.at(0);
}

TownID Datastructures::max_distance()
{
    if (towndata_.empty()) {
        return NO_TOWNID;
    }
    // Getting towns in increasing distance order
    std::vector<TownID> ids =  towns_distance_increasing();
    return ids.at(ids.size() - 1);
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    // No town(s)
    if ((towndata_.find(vassalid) == towndata_.end())
            or towndata_.find(masterid) == towndata_.end()) {
        return false;
    }
    // If already has master
    if (towndata_.at(vassalid).master_ != NO_TOWNID) {
        return false;
    }
    towndata_.at(vassalid).master_ = masterid;
    towndata_.at(masterid).vassals_.push_back(vassalid);
    return true;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    if (towndata_.find(id) == towndata_.end()) {
        return {NO_TOWNID};
    }
    return towndata_.at(id).vassals_;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    if (towndata_.find(id) == towndata_.end()) {
        return {NO_TOWNID};
    }
    std::vector< TownID > masterids = {id};
    TownID master = towndata_.at(id).master_;
    while (master != NO_TOWNID) {
        masterids.push_back(master);
        master = towndata_.at(master).master_;
    }
    return masterids;
}

bool Datastructures::remove_town(TownID id)
{
    if (towndata_.find(id) == towndata_.end()) {
        return false;
    }

    std::vector<TownID>::iterator iter = towndata_.at(id).vassals_.begin();
    TownID masterid = towndata_.at(id).master_;
    bool hasMaster = towndata_.at(id).master_ != NO_TOWNID;
    if (hasMaster) {
        iter = find(towndata_.at(masterid).vassals_.begin(), towndata_.at(masterid).vassals_.end(), id);
        // Removing from masters vassallist
        towndata_.at(masterid).vassals_.erase(iter);
    }
    // Changing the masters of own vassals. If not master => NO_TOWNID
    for (TownID v : towndata_.at(id).vassals_) {
        towndata_.at(v).master_ = masterid;
        if (hasMaster) {
            towndata_.at(masterid).vassals_.push_back(v);
        }
    }
    // Removing
    towndata_.erase(id);
    return true;
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    std::map<TownID, Distance> dists = {};
    idDistPair p;
    // Creating vector including (id, distance) -pairs
    std::vector< idDistPair > pairs;
    for (auto datapair : towndata_) {
        p.first = datapair.first;
        p.second = dist(datapair.second.coord_, coord);
        pairs.push_back(p);
    }
    // Sorting with lambda
    std::sort(pairs.begin(), pairs.end(),
              [](const idDistPair &a, const idDistPair &b) {
        if (a.second != b.second) {return a.second < b.second;}
        return a.first < b.first;});

    // Copying to vector
    std::vector<TownID> ids = {};
    for (idDistPair p : pairs) {
        ids.push_back(p.first);
    }
    return ids;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    if (towndata_.find(id) == towndata_.end()) {
        return {NO_TOWNID};
    }
    std::deque<TownID> maxvassalpath = vassalsRecursive(id);
    std::vector<TownID> maxvassalpathvec = {};
    for (auto i : maxvassalpath) {
        maxvassalpathvec.push_back(i);
    }
    return maxvassalpathvec;
}

int Datastructures::total_net_tax(TownID id)
{
    if (towndata_.find(id) == towndata_.end()) {
        return NO_VALUE;
    }
    Town town = towndata_.at(id);
    int net_tax = town.tax_;
    // Tax from vassals
    for (TownID v : town.vassals_) {
        net_tax += taxFromVassal(v);
    }
    // If not master
    if (town.master_ == NO_TOWNID) {
        return net_tax;
    }
    return (net_tax - trunc(0.1*net_tax));
}

std::deque<TownID> Datastructures::vassalsRecursive(TownID id)
{
    std::deque<TownID>::size_type  maxlen = 0;
    std::deque<TownID>::size_type pathlen = 0;
    std::deque<TownID> maxvassalpath = {};
    std::deque<TownID> vassalpath = {};

    // Looping through vassals and finding the longest path
    for (TownID v : towndata_.at(id).vassals_) {
        vassalpath = vassalsRecursive(v);
        pathlen = vassalpath.size();
        if (pathlen > maxlen) {
            maxlen = pathlen;
            maxvassalpath = vassalpath;
        }
    }
    maxvassalpath.push_front(id);
    return maxvassalpath;
}

int Datastructures::taxFromVassal(TownID id)
{
    Town town = towndata_.at(id);
    int net_tax = 0;

    // Tax from poor peasants
    net_tax += towndata_.at(id).tax_;
    // Tax from own vassals recursively
    for (TownID v : town.vassals_) {
        net_tax += taxFromVassal(v);
    }
    // Tax to master
    return trunc(0.1*net_tax);
}

Distance Datastructures::dist(const Coord &c1, const Coord &c2)
{
    double d = sqrt((std::pow((c1.x - c2.x),2) + (std::pow((c1.y - c2.y),2))));
    return trunc(d);
}


