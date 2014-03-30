#pragma once

#include <vector>

class Edges {
public:
    virtual size_t GetNumberOfVertices() const = 0;

    virtual std::vector<size_t> GetIncidenceList(size_t vertex) const = 0;

    virtual ~Edges()
    {}
};

class ListOfEdges: public Edges {
    size_t number_of_vertices_;
    std::vector<std::vector<size_t> > list_of_edges_;

public:
    ListOfEdges(size_t number_of_vertices, const std::vector<size_t>& edge_begin,
        const std::vector<size_t>& edge_end)
    {
        number_of_vertices_ = number_of_vertices;
        list_of_edges_.resize(number_of_vertices);
        
        for (size_t i = 0; i < edge_begin.size(); ++i) {            
            list_of_edges_[edge_begin[i]].push_back(edge_end[i]);
        }
    }

    size_t GetNumberOfVertices() const
    {
        return number_of_vertices_;
    }

    std::vector<size_t> GetIncidenceList(size_t vertex) const
    {
        return list_of_edges_[vertex];
    }
};

class MatrixOfEdges: public Edges {
    size_t number_of_vertices_;
    std::vector<std::vector<bool> > matrix_of_edges_;

public:
    MatrixOfEdges(size_t number_of_vertices, const std::vector<size_t>& edge_begin,
        const std::vector<size_t>& edge_end)
    {
        number_of_vertices_ = number_of_vertices;
        std::vector<bool> bool_vector(number_of_vertices, false);
        matrix_of_edges_.resize(number_of_vertices, bool_vector);
        
        for (size_t i = 0; i < edge_begin.size(); ++i) {
            matrix_of_edges_[edge_begin[i]][edge_end[i]] = true;
        }
    }

    size_t GetNumberOfVertices() const
    {
        return number_of_vertices_;
    }

    std::vector<size_t> GetIncidenceList(size_t vertex) const
    {
        std::vector<size_t> incidence_list;
        for (size_t index = 0; index < number_of_vertices_; ++index) 
        if (matrix_of_edges_[vertex][index]) {
            incidence_list.push_back(index);
        }

        return incidence_list;
    }
};

class Graph {
    Edges * edges_;

    bool IsListBetterThanMatrix(size_t number_of_vertices, size_t number_of_edges)
    {
        long long list_memory = static_cast<long long>(number_of_edges);
        long long long_number_of_vertices = static_cast<long long>(number_of_vertices);
        long long matrix_memory = long_number_of_vertices * long_number_of_vertices / 8;

        return list_memory <= matrix_memory;
    }

    void MakeCompactGraph(size_t number_of_vertices, const std::vector<size_t>& edge_begin,
        const std::vector<size_t>& edge_end)
    {
        size_t numbers_of_edges = edge_begin.size();
        if (IsListBetterThanMatrix(number_of_vertices, numbers_of_edges))
            edges_ = new ListOfEdges(number_of_vertices, edge_begin, edge_end);
        else
            edges_ = new MatrixOfEdges(number_of_vertices, edge_begin, edge_end);            
    }

public:
    Graph(size_t number_of_vertices, const std::vector<size_t>& edge_begin,
        const std::vector<size_t>& edge_end)
    {
        if (edge_begin.size() != edge_end.size())
            throw std::runtime_error("Sizes of edge_begin and edge_end are different");

        MakeCompactGraph(number_of_vertices, edge_begin, edge_end);
    }

    Graph(Graph& graph)
    {
        if (edges_ != nullptr)
            MakeCompactGraph(0, {}, {});
        std::swap(edges_, graph.edges_);
    }

    size_t GetNumberOfVertices() const
    {
        return edges_->GetNumberOfVertices();
    }

    std::vector<size_t> GetIncidenceList(size_t vertex) const
    {
        return edges_->GetIncidenceList(vertex);
    }

    ~Graph() {
        delete edges_;
    }
};
