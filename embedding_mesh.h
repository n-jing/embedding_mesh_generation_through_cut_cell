#ifndef EMBEDDING_MESH_JJ_H
#define EMBEDDING_MESH_JJ_H


#include <array>
#include <vector>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <memory>
#include "key_comparison.h"


template<typename T>
struct Verts
{
  // Verts(T a, T b, T c) : v_[0](a), v_[1](b), v_[2](c) { }
  Verts(const std::array<T, 3> &v) : v_(v) { }
  Verts() { }
  std::array<T, 3> v_;
};

struct Neighbor
{
  std::vector<std::array<int, 3>> idx_;
};

class Voxel
{
public:
  Voxel() { }
  void AddFace(const std::vector<int> &face) { all_face_.push_back(face);}
  int SetVoxelDomain();
  template<typename T>
  int SetVoxelIndex(const std::vector<Verts<T>> &verts, const std::array<std::vector<T>, 3> &grid_line);
  
  template<typename T>
  friend class EmbeddingMesh;
  
// private:
  std::vector<std::vector<int>> all_face_;
  std::vector<std::unordered_set<int>> domain_verts_;
  std::vector<std::array<int, 8>> domain_corner_;
  std::vector<std::vector<int>> neighbor_domain_;
  
  std::set<int> voxel_verts_;
  int copy_num_;
  std::array<int, 3> idx_;
};

template<typename T>
class EmbeddingMesh
{
public:
  EmbeddingMesh(const char *path);
  int GetGridLine(const char *path);
  
  int RemoveDuplicateVerts();
  int SetVoxelDomainAndIndex();
  int SetVoxelCorner();
  int SetVoxelNeighbor();
// private:

// private:
  std::vector<Verts<T>> verts_;
  std::vector<Voxel> cells_;
  
  std::vector<Verts<T>> verts_unique_;
  std::vector<std::shared_ptr<Voxel>> cells_unique_;
  std::unordered_map<int, std::pair<std::array<int, 3>, int>> id_to_domain_;
  
  std::unordered_map<std::array<int, 3>, std::shared_ptr<Voxel>,
                     Jing::KeyHash<std::array<int, 3>, 0>, Jing::KeyEqual<std::array<int, 3>, 0>> idx_to_voxel_;
  
  std::array<std::vector<T>, 3> grid_line_;
  std::unordered_map<int, std::array<T, 3>> corner_coordinates_;
};

template class Verts<float>;
template class Verts<double>;

template class EmbeddingMesh<float>;
template class EmbeddingMesh<double>;


#endif // EMBEDDING_MESH_JJ_H
