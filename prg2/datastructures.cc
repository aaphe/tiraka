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


//
// Phase 2 operations
//


void Datastructures::clear_roads()
{
    // Tyhjenetään jokainen tiemappi
    for (auto& datapair : towndata_) {
        datapair.second.roads_.clear();
    }
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{
    // Palautettava vektori
    std::vector<Road> roadsvec = {};
    Road idpair = {};
    TownID tid = "";
    TownID tid2 = "";
    for (const auto& i : towndata_) {
        tid = i.first;
        for (const auto& j : towndata_.at(tid).roads_) {
            tid2 = j.first;
            // Pienempi on ensin
            if (tid < tid2) {
                idpair = {tid, tid2};
            }
            else {
                idpair = {tid2, tid};
            }
            // Jos ei vielä ole
            if (std::find(roadsvec.begin(), roadsvec.end(), idpair) == roadsvec.end()) {
                roadsvec.push_back(idpair);
            }
        }
    }
    return roadsvec;
}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    // Jos jompaakumpaa ei ole
    if (towndata_.find(town1) == towndata_.end() or
            towndata_.find(town2) == towndata_.end()) {
        return false;
    }
    // Jos tie on jo
    if (towndata_.at(town1).roads_.find(town2) != towndata_.at(town1).roads_.end()) {
        return false;
    }
    // Lisäys
    Coord c1 = towndata_.at(town1).coord_;
    Coord c2 = towndata_.at(town2).coord_;
    towndata_.at(town1).roads_.insert({town2, dist(c1, c2)});
    towndata_.at(town2).roads_.insert({town1, dist(c1, c2)});
    return true;
}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
    // Jos ei ole
    if (towndata_.find(id) == towndata_.end()) {
        return {NO_TOWNID};
    }
    std::vector<TownID> roads = {};
    for (const auto& t : towndata_.at(id).roads_) {
        roads.push_back(t.first);
    }
    return roads;
}

std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{
    // Jos jompaakumpaa ei ole
    if (towndata_.find(toid) == towndata_.end() or
            towndata_.find(fromid) == towndata_.end()) {
        return {NO_TOWNID};
    }

    // Käytetään BFS:ää

    // Alustetaan solmut
    for (auto& datapair : towndata_) {
        datapair.second.isVisited_ = false;
        datapair.second.prevtown_ = NO_TOWNID;
    }

    // Jono
    std::deque<TownID> idsToLoop = {};
    // 1. alkio
    idsToLoop.push_back(fromid);
    towndata_.at(fromid).isVisited_ = true;
    TownID townid = fromid;
    TownID id = NO_TOWNID;

    // Kertoo löytyikö reitti
    bool routeFound = false;

    // Jatketaan, kunnes jompikumpi on tosi
    while ((not idsToLoop.empty()) and (not routeFound)) {
        // 1. käsittelyyn
        townid = idsToLoop.at(0);
        idsToLoop.pop_front();

        for (const auto& t : towndata_.at(townid).roads_) {
            // Jos ei käyty vielä
            id = t.first;
            if (not towndata_.at(id).isVisited_) {
                towndata_.at(id).isVisited_ = true;
                towndata_.at(id).prevtown_ = townid;
                idsToLoop.push_back(id);
            }
            // Jos tultiin maaliin
            if (id == toid) {
                // Tyhjentäminen epätehokasta (lineaarinen)
                routeFound = true;
                break;
            }
        }
    }
    // Jos ei löytynyt
    if (not routeFound) {
        return {};
    }

    // Nyt kaikilla on previous town määritelty
    // Käydään läpi alkaen maalista
    // Jos maalia ei löydetty, prev == NO_TOWNID ja palautuu tyhjä vektori,
    // whilelooppiin ei mennä ollenkaan
    // Palautettava vektori
    std::vector<TownID> pathvec = {};
    std::deque<TownID> pathdeq = {};    // Apudeque
    id = toid;
    while (id != NO_TOWNID) {
        pathdeq.push_front(id);
        id = towndata_.at(id).prevtown_;
    }
    // Ja kopiointi vektoriin oikeaan järjestykseen
    for (const TownID& t : pathdeq) {
        pathvec.push_back(t);
    }
    return pathvec;
}

bool Datastructures::remove_road(TownID town1, TownID town2)
{
    // Jos jompaakumpaa ei ole
    if (towndata_.find(town1) == towndata_.end() or
            towndata_.find(town2) == towndata_.end()) {
        return false;
    }
    // Jos tietä ei ole
    if (towndata_.at(town1).roads_.find(town2) == towndata_.at(town1).roads_.end()) {
        return false;
    }
    towndata_.at(town1).roads_.erase(town2);
    towndata_.at(town2).roads_.erase(town1);

    return true;
}

std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{
    // BFS-algoritmi löytää aina sen reitin, jolla on vähiten solmuja
    // Hyödynnetään any_routea
    return any_route(fromid, toid);
}

std::vector<TownID> Datastructures::road_cycle_route(TownID startid)
{
    // Jos ei ole
    if (towndata_.find(startid) == towndata_.end()) {
        return {NO_TOWNID};
    }

    // Alustetaan solmut alkuarvoihin
    for (auto& datapair : towndata_) {
        datapair.second.isVisited_ = false;
        // datapair.second.cyclehead_ = false;
        datapair.second.prevtown_ = NO_TOWNID;
        datapair.second.prev2_ = NO_TOWNID;
    }

    // Rekursio jauhamaan
    if (not hasCycle(startid)) {
        return {};
    }
    // Nyt on silmukan pää merkitty, etsitään se
    TownID cyclehead = NO_TOWNID;
    for (const auto& datapair : towndata_) {
        if (datapair.second.prev2_ != NO_TOWNID) {
            cyclehead = datapair.first;
        }
    }
    // Kerätään kaupungit dequeen
    std::deque<TownID> pathdeq = {cyclehead};
    std::vector<TownID> pathvec = {};
    TownID id = towndata_.at(cyclehead).prev2_;
    while (id != NO_TOWNID) {
        pathdeq.push_front(id);
        id = towndata_.at(id).prevtown_;
    }
    // Ja kopiointi vektoriin oikeaan järjestykseen
    for (const TownID& t : pathdeq) {
        pathvec.push_back(t);
    }
    return pathvec;
}

std::vector<TownID> Datastructures::shortest_route(TownID fromid, TownID toid)
{
    // Jos jompaakumpaa ei ole
    if (towndata_.find(toid) == towndata_.end() or
            towndata_.find(fromid) == towndata_.end()) {
        return {NO_TOWNID};
    }

    // Alustetaan solmut
    for (auto& datapair : towndata_) {
        datapair.second.isVisited_ = false;
        datapair.second.prevtown_ = NO_TOWNID;
    }

    // Hyödynnetään Dijkstran alogritmia, tässä
    // etäisyydet ovat kaarten painoja

    // Prioriteettijono
    std::priority_queue< distIdPair, std::vector<distIdPair>, std::greater<distIdPair> > prio = {};
    // Lyhimmät etäisyydet
    std::unordered_map< TownID, Distance > dists = {};

    TownID id =NO_TOWNID;
    Distance d = NO_DISTANCE;

    TownID childid = NO_TOWNID;
    Distance childdist = NO_DISTANCE;

    bool routeFound = false;

    // Lisätään 1. alkio
    prio.push({0, fromid});
    dists.insert({fromid, 0});
    towndata_.at(fromid).isVisited_ = true;

    while (not prio.empty()) {
        id = prio.top().second;
        d = prio.top().first;
        prio.pop();

        // Tarkistetaan, onko tämä käyty jo halvemmalla
        if (dists.at(id) < d) {
            continue;
        }

        for (const auto& datapair : towndata_.at(id).roads_) {
            childid = datapair.first;
            childdist = datapair.second;

            // Löytyikö maali
            if (id == toid) {
                routeFound = true;
            }

            // Jos ei vielä käyty
            if (not towndata_.at(childid).isVisited_) {
                // Edeltäjä muistiin, lisäys priojonoon, päivitys mappiin
                towndata_.at(childid).prevtown_ = id;
                towndata_.at(childid).isVisited_ = true;
                prio.push({d + childdist, childid});
                dists.insert({childid, d + childdist});
            }
            // Jos on käyty, tutkitaan päästäänkö tätä kautta halvemmalla
            else if (dists.at(childid) > d + childdist) {
                // Uusi edeltäjä muistiin, uudestaan priojonoon, päivitys mappiin
                prio.push({d + childdist, childid});
                dists.at(childid) = d + childdist;
                towndata_.at(childid).prevtown_ = id;
            }
        }
    }
    // Dijkstra valmis. Luodaan palautettava vektori
    if (not routeFound) {
        return {};
    }

    std::vector<TownID> pathvec = {};
    std::deque<TownID> pathdeq = {};    // Apudeque
    id = toid;
    while (id != NO_TOWNID) {
        pathdeq.push_front(id);
        id = towndata_.at(id).prevtown_;
    }
    // Ja kopiointi vektoriin oikeaan järjestykseen
    for (const TownID& t : pathdeq) {
        pathvec.push_back(t);
    }
    return pathvec;
}

Distance Datastructures::trim_road_network()
{
    // Haetaan kaikki tiet
    std::vector< Road > allroads = all_roads();
    // Kerätään tiet vektoriin etäisyyksineen
    std::vector< std::pair< Distance, Road > > roads_wdists = {};
    Distance d = NO_DISTANCE;
    for (const auto &road : allroads) {
        d = dist(towndata_.at(road.first).coord_, towndata_.at(road.second).coord_);
        roads_wdists.push_back({d, road});
    }
    // Järkätään vektori matkan mukaan
    std::sort(roads_wdists.begin(), roads_wdists.end(), [](const std::pair<Distance, Road> road1,
              const std::pair<Distance, Road> road2) {return road1.first < road2.first;});

    // Käytetään Kruskalin algoritmia

    // Mappi jossa on kunkin kaupungin joukko
    std::unordered_map< TownID, int > sets = {};
    // Alussa jokaisella omansa
    int i = 1;
    for (const auto &datapair : towndata_) {
        sets.insert({datapair.first, i});
        i += 1;
    }
    int set1 = 0;
    int set2 = 0;
    Distance sumdist = 0;
    // Loopataan tiet läpi
    for (const auto &datapair : roads_wdists) {
        // Jos id :den joukot on samat, poistetaan
        if (sets.at(datapair.second.first) == sets.at(datapair.second.second)) {
            // toRemove.push_back(datapair.second);
            remove_road(datapair.second.first, datapair.second.second);
        }
        else {
            // Jos eri, yhdistetään joukot
            set1 = sets.at(datapair.second.first);
            set2 = sets.at(datapair.second.second);
            for (auto &dpair : sets) {
                // Joukkoon set2 kuuluvat yhdistyvät joukkoon set1
                if (dpair.second == set2) {
                    dpair.second = set1;
                }
            }
            // Kasvatetaan tieverkoston pituutta
            sumdist += datapair.first;
        }
    }
    return sumdist;
}

// Private methods

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

bool Datastructures::hasCycle(const TownID &id)
{
    TownID neighid = NO_TOWNID;
    // Väritys
    towndata_.at(id).isVisited_ = true;
    // Loopataan naapurit
    for (const auto& datapair : towndata_.at(id).roads_) {
        neighid = datapair.first;
        // Jos ei vielä käyty, tutkitaan rekursiivisesti naapurit
        if (not towndata_.at(neighid).isVisited_) {
            towndata_.at(neighid).prevtown_ = id;
            if (hasCycle(neighid)) {
                return true;
            }
        }
        // Jos on jo käyty, omaa edeltäjää ei lasketa silmukaksi
        else if (towndata_.at(neighid).isVisited_ and towndata_.at(id).prevtown_ != neighid) {
            // Silmukka löytyi, merkitään katkaisija ja lopetetaan
            // Eli nyt tällä on 2 edeltäjää
            towndata_.at(neighid).prev2_ = id;
            return true;
        }
    }
    // Jos päästiin tänne, ei silmukkaa ole
    return false;
}
