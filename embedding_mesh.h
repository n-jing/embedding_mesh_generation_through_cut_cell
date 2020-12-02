#ifndef EMBEDDING_MESH_JJ_H
#define EMBEDDING_MESH_JJ_H


#include <array>
#include <vector>
#include <unordered_set>
#include <set>


template<typename T>
struct Verts
{
  // Verts(T a, T b, T c) : v_[0](a), v_[1](b), v_[2](c) { }
  Verts(const std::array<T, 3> &v) : v_(v) { }
  Verts() { }
  std::array<T, 3> v_;
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
  // std::vector<std::array<int, 8>> voxel_conver_;
  // std::vector<>
  
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
// private:

  std::vector<Verts<T>> 

  
// private:
  std::vector<Verts<T>> verts_;
  std::vector<Voxel> cells_;
  
  std::vector<Verts<T>> verts_unique_;
  std::vector<Voxel> cells_unique_;
  
  
  std::array<std::vector<T>, 3> grid_line_;
};

template class Verts<float>;
template class Verts<double>;

template class EmbeddingMesh<float>;
template class EmbeddingMesh<double>;


#endif // EMBEDDING_MESH_JJ_H
