#include <map>
#include <set>
#include <list>
#include <tuple>
#include <queue>
#include <cstdint>
#include <optional>
#include <iostream>
#include <algorithm>
#include <initializer_list>

class weighted_graph {
private:
    uint32_t highest_id{1};

    struct edge
    {
        uint32_t destination;
        int32_t weight;
    };

    std::map<uint32_t, std::list<edge>>   adjacency_lists;   // (id, [edge1, edge2, ...])
    std::map<uint32_t, std::string>       vertices;          // (id, name)

    bool is_vertex_unique (uint32_t vertex_id, const std::string& vertex_name) const {
        // We need to check whether any of existing vertices has id
        // or name passed as an argument, we want to avoid duplicates.
        for (const auto& [id, name] : vertices) {
            if (id == vertex_id) {
                std::cout << "Vertex with id " << vertex_id << " already exists!" << std::endl;
                return false;
            }
            if (name == vertex_name) {
                std::cout << "Vertex with name " << vertex_name << " already exists!" << std::endl;
                return false;
            }
        }
            
        return true;
    }

    bool does_vertex_id_exist (uint32_t vertex_id) const {
        if (vertices.find(vertex_id) != vertices.end())
            return true;

        return false;
    }

    bool does_vertex_name_exist (const std::string& vertex_name) const {
        for (const auto& [id, name] : vertices)
            if (name == vertex_name)
                return true;
            
        return false;
    }

    bool does_edge_exist (uint32_t vertex1_id, uint32_t vertex2_id) const {
        // Check if the edge comes out of each vertex to the other:
        const auto& edges_from_vertex1 = adjacency_lists.find(vertex1_id)->second;
        const auto& edges_from_vertex2 = adjacency_lists.find(vertex2_id)->second;

        bool vertex1_edge = std::any_of(
            edges_from_vertex1.begin(), edges_from_vertex1.end(),
            [&](edge e) -> bool { return e.destination == vertex2_id; }
        );

        bool vertex2_edge = std::any_of(
            edges_from_vertex2.begin(), edges_from_vertex2.end(),
            [&](edge e) -> bool { return e.destination == vertex1_id; }
        );

        return (vertex1_edge && vertex2_edge);
    }

    // These functions (add/remove edge, change weight) are used in all
    // corresponding functions, as they can take name(s) or id(s) as
    // arguments in any order, so (name, name), (id, name), (name, id)
    // or (id, id). They all do the same, so we can pass their arguments
    // after making small adjustments here.
    void add_edge_ (uint32_t vertex1_id, uint32_t vertex2_id, int32_t weight) {
        if (does_edge_exist(vertex1_id, vertex2_id)) {
            std::cout << "Edge [" << vertex1_id << ", " << vertex2_id << "] already exists!" << std::endl; 
            return;
        }

        adjacency_lists[vertex1_id].push_front({vertex2_id, weight});
        adjacency_lists[vertex2_id].push_front({vertex1_id, weight});
    }

    void remove_edge_ (uint32_t vertex1_id, uint32_t vertex2_id) {
        if (!does_edge_exist(vertex1_id, vertex2_id))
            return;

        adjacency_lists[vertex1_id].remove_if([&](edge e) { return e.destination == vertex2_id; });
        adjacency_lists[vertex2_id].remove_if([&](edge e) { return e.destination == vertex1_id; });
    }

    void change_weight_ (uint32_t vertex1_id, uint32_t vertex2_id, int32_t weight) {
        if (!does_edge_exist(vertex1_id, vertex2_id))
            return;

        for (auto& neighbour : adjacency_lists[vertex1_id]) {
            if (neighbour.destination == vertex2_id) {
                neighbour.weight = weight;
                break;
            }
        }

        for (auto& neighbour : adjacency_lists[vertex2_id]) {
            if (neighbour.destination == vertex1_id) {
                neighbour.weight = weight;
                break;
            }
        }
    }

public:
    using vertex = std::pair<uint32_t, const std::string&>;

    weighted_graph ()                         = default;
    weighted_graph (weighted_graph&& wg)      = default;
    weighted_graph (const weighted_graph& wg) = default;
    
    weighted_graph (std::initializer_list<vertex> vertices,
                    std::initializer_list<std::tuple<uint32_t, uint32_t, int32_t>> edges) {
        
        for (auto& [id, name] : vertices) {
            //this->vertices[id] = name;
            //this->adjacency_lists.insert({id, {}});
            add_vertex(id, name);

            if (id > highest_id)
                highest_id = id;
        }

        for (auto& [source, destination, weight] : edges)
            add_edge(source, destination, weight);
    }

    void add_vertex (uint32_t vertex_id, const std::string& vertex_name) {
        if (!is_vertex_unique(vertex_id, vertex_name)) 
            return;

        vertices[vertex_id] = vertex_name;

        if (vertex_id > highest_id)
            highest_id = vertex_id;

        adjacency_lists.insert({vertex_id, {}});
    }

    void add_vertex (const std::string& vertex_name) {
        add_vertex(++highest_id, vertex_name);
    }

    void remove_vertex (uint32_t vertex_id) {
        if (!does_vertex_id_exist(vertex_id))
            return;

        vertices.erase(vertex_id);
        adjacency_lists.erase(vertex_id);

        // We have to delete all connected edges to that vertex, which
        // means we remove e.g. edge [1, 5] and [5, 1].
        for (auto& vert_id : adjacency_lists) {
            vert_id.second.erase(
                std::remove_if(
                    vert_id.second.begin(),
                    vert_id.second.end(),
                    [&](edge e) -> bool { return e.destination == vertex_id; }
                ),
                vert_id.second.end()
            );
        }
    }

    void remove_vertex (const std::string& vertex_name) {
        if (!does_vertex_name_exist(vertex_name))
            return;

        for (const auto& [id, name] : vertices)
            if (name == vertex_name)
                remove_vertex(id);
    }

    std::optional<uint32_t> get_vertex_id (const std::string& vertex_name) const {
        for (const auto& [id, name] : vertices)
            if (name == vertex_name)
                return id;
        
        return std::nullopt;
    }

    std::optional<std::string> get_vertex_name (uint32_t vertex_id) const {
        auto it = vertices.find(vertex_id);
        if (it == vertices.end())
            return std::nullopt;

        return it->second;
    }

    std::list<vertex> get_vertices() const {
        // vertices are saved as std::map, but it is better to have a list
        // when we want to print everything later
        std::list<vertex> list_vertices;

        for (auto& vertex : vertices) {
            auto& id   = vertex.first;
            auto& name = vertex.second;
            list_vertices.push_back({id, name});
        }

        return list_vertices;
    }

    std::list<std::pair<uint32_t, int32_t>> get_neighbours (uint32_t vertex_id) const {
        auto& edges = adjacency_lists.find(vertex_id)->second; // second = list<edge>
        std::list<std::pair<uint32_t, int32_t>> neighbours;
        for (auto& [dest, weight] : edges) {
            neighbours.push_back({dest, weight});
        }

        return neighbours;
    }

    void add_edge (uint32_t vertex1_id, uint32_t vertex2_id, int32_t weight) {
        if (does_vertex_id_exist(vertex1_id) && does_vertex_id_exist(vertex2_id))
            add_edge_(vertex1_id, vertex2_id, weight);
    }

    void add_edge (uint32_t vertex1_id, const std::string& vertex2_name, int32_t weight) {
        if (does_vertex_id_exist(vertex1_id) && does_vertex_name_exist(vertex2_name))
            add_edge_(vertex1_id, get_vertex_id(vertex2_name).value(), weight);
    }

    void add_edge (const std::string& vertex1_name, uint32_t vertex2_id, int32_t weight) {
        if (does_vertex_name_exist(vertex1_name) && does_vertex_id_exist(vertex2_id))
            add_edge_(get_vertex_id(vertex1_name).value(), vertex2_id, weight);
    }

    void add_edge (const std::string& vertex1_name, const std::string& vertex2_name, int32_t weight) {
        if (does_vertex_name_exist(vertex1_name) && does_vertex_name_exist(vertex2_name))
            add_edge_(get_vertex_id(vertex1_name).value(), get_vertex_id(vertex2_name).value(), weight);
    }

    void remove_edge (uint32_t vertex1_id, uint32_t vertex2_id) {
        if (does_vertex_id_exist(vertex1_id) && does_vertex_id_exist(vertex2_id))
            remove_edge_(vertex1_id, vertex2_id);
    }

    void remove_edge (uint32_t vertex1_id, const std::string& vertex2_name) {
        if (does_vertex_id_exist(vertex1_id) && does_vertex_name_exist(vertex2_name))
            remove_edge_(vertex1_id, get_vertex_id(vertex2_name).value());
    }

    void remove_edge (const std::string& vertex1_name, uint32_t vertex2_id) {
        if (does_vertex_name_exist(vertex1_name) && does_vertex_id_exist(vertex2_id))
            remove_edge_(get_vertex_id(vertex1_name).value(), vertex2_id);
    }

    void remove_edge (const std::string& vertex1_name, const std::string& vertex2_name) {
        if (does_vertex_name_exist(vertex1_name) && does_vertex_name_exist(vertex2_name))
            remove_edge_(get_vertex_id(vertex1_name).value(), get_vertex_id(vertex2_name).value());
    }

    void change_weight (uint32_t vertex1_id, uint32_t vertex2_id, int32_t weight) {
        if (does_vertex_id_exist(vertex1_id) && does_vertex_id_exist(vertex2_id))
            change_weight_(vertex1_id, vertex2_id, weight);
    }

    void change_weight (uint32_t vertex1_id, const std::string& vertex2_name, int32_t weight) {
        if (does_vertex_id_exist(vertex1_id) && does_vertex_name_exist(vertex2_name))
            change_weight_(vertex1_id, get_vertex_id(vertex2_name).value(), weight);
    }

    void change_weight (const std::string& vertex1_name, uint32_t vertex2_id, int32_t weight) {
        if (does_vertex_name_exist(vertex1_name) && does_vertex_id_exist(vertex2_id))
            change_weight_(get_vertex_id(vertex1_name).value(), vertex2_id, weight);
    }

    void change_weight (const std::string& vertex1_name, const std::string& vertex2_name, int32_t weight) {
        if (does_vertex_name_exist(vertex1_name) && does_vertex_name_exist(vertex2_name))
            change_weight_(get_vertex_id(vertex1_name).value(), get_vertex_id(vertex2_name).value(), weight);
    }


    std::list<uint32_t> find_path (uint32_t source, uint32_t destination) const {
        if (does_vertex_id_exist(source) && does_vertex_id_exist(destination)) {
            std::queue<uint32_t>         queue;
            std::set<uint32_t>           visited;
            std::map<uint32_t, uint32_t> parents;

            visited.insert(source);
            queue.push(source);

            const auto backtrack = [&](uint32_t child) -> std::list<uint32_t> {
                std::list<uint32_t> path;

                while (child != parents[child]) {
                    path.push_front(child);
                    child = parents[child];
                }

                path.push_front(source);
                return path;
            };

            parents[source] = source;

            while (!queue.empty()) {
                uint32_t vert = queue.front();
                queue.pop();

                if (vert == destination)
                    return backtrack(vert);

                auto& neighbours = adjacency_lists.find(vert)->second;
                for (auto& n : neighbours) {
                    auto vert_id = n.destination;

                    if (!visited.count(vert_id)) {
                        visited.insert(vert_id);
                        parents[vert_id] = vert;
                        queue.push(vert_id);
                    }
                }
            }

            return {};
        }

        return {};
    }

    void print() {
        for (auto& [id, name] : get_vertices()) {
            std::cout << "[" << id << " : " << name << "]: " << std::endl;

            auto neighbours = get_neighbours(id);
            if (neighbours.empty())
                std::cout << "\tNo neighbours!" << std::endl;
            else {
                for (auto& [dest_vertex, dest_weight] : neighbours) {
                    std::cout << "\t[" << id << ", " << dest_vertex << ", " << dest_weight << "]" << std::endl;
                }
            }
        }
    }

    void print_path (uint32_t source, uint32_t destination) {
        std::list<uint32_t> path = find_path(source, destination);

        if (path.empty())
            std::cout << "There is no path from " << source << " to " << destination << "!" << std::endl;
        else {
            for (auto& vertex_id : path)
                std::cout << vertex_id << " ";

            std::cout << std::endl;
        }
    }
};

int main() {
    auto graph = weighted_graph(
        { {1, std::string("first")}, {2, std::string("second")}, {3, std::string("third")}, 
          {4, std::string("fourth")}, {5, std::string("fifth")}, {6, std::string("sixth")} },
        { {1, 2, 1}, {1, 5, 17}, {2, 3, 2}, {2, 4, 15}, {3, 4, 3}, {3, 5, 10} }
    );

    std::cout << "All vertices and edges: " << std::endl;
    graph.print();
    
    std::cout << "Print path between vertex 1 and 6: " << std::endl;
    graph.print_path(1, 6);

    std::cout << "Print path between vertex 1 and 4: " << std::endl;
    graph.print_path(1, 4);

    // Adding vertices and an edge:
    graph.add_vertex(7, "seventh!");
    graph.add_vertex("eighth"); // should have id 8
    graph.add_edge(7, 8, 10);

    graph.change_weight("first", 2, 777);

    std::cout << "All vertices and edges after changes: " << std::endl;
    graph.print();

    // Removing edges and vertices:
    graph.remove_edge(3, 5); // ordinary edge
    graph.remove_vertex(1);  // vertex connected with 2, 5

    std::cout << "All vertices and edges after removals: " << std::endl;
    graph.print();
}