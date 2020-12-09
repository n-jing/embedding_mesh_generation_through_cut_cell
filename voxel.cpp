#include "embedding_mesh.h"
#include <set>
#include <iostream>
#include <unordered_map>
#include "union_find_set.h"
using namespace std;


int Voxel::SetVoxelDomain()
{
  domain_verts_.clear();
  for (const auto &f : all_face_)
  {
    for (auto v : f)
    {
      voxel_verts_.insert(v);
    }
  }

  UnionFindSet union_find(voxel_verts_.size());
  unordered_map<int, int> v_to_idx;
  unordered_map<int, int> idx_to_v;
  for (auto v : voxel_verts_)
  {
    v_to_idx.emplace(v, v_to_idx.size());
    idx_to_v.emplace(idx_to_v.size(), v);
  }

  for (const auto &f : all_face_)
  {
    const int f_v_num = f.size();
    for (int i = 0; i < f_v_num; ++i)
    {
      int a = v_to_idx.at(f[i]);
      int b = v_to_idx.at(f[(i+1) % f_v_num]);
      union_find.set_union(a, b);
    }
  }

  unordered_map<size_t, vector<size_t>> group = union_find.get_group();
  for (const auto &g : group)
  {
    unordered_set<int> g_v;
    for (auto &idx : g.second)
    {
      g_v.insert(idx_to_v.at(idx));
    }
    domain_verts_.push_back(g_v);
  }

  UnionFindSet face_union_find(all_face_.size());
  const int face_num = all_face_.size();
  for (int fi = 0; fi < face_num; ++fi)
  {
    for (int fj = fi+1; fj < face_num; ++fj)
    {
      const std::vector<int> &fa = all_face_.at(fi);
      const std::vector<int> &fb = all_face_.at(fj);
      bool is_c = IsFaceConnect(fa, fb);
      if (is_c)
        face_union_find.set_union(fi, fj);
    }
  }

  unordered_map<size_t, vector<size_t>> face_group = face_union_find.get_group();
  for (auto &g : face_group)
  {
    std::vector<std::vector<int>> domain_f;
    for (auto &f : g.second)
    {
      domain_f.push_back(all_face_[f]);
    }
    domain_face_.push_back(domain_f);
  }

  return 0;
}

bool Voxel::IsFaceConnect(const std::vector<int> &fa, const std::vector<int> &fb)
{
  for (auto va : fa)
  {
    for (auto vb : fb)
    {
      for (auto &d : domain_verts_)
      {
        if (d.count(va) && d.count(vb))
          return true;
      }
    }
  }

  return false;
}


template<typename T>
int Voxel::SetVoxelIndex(const std::vector<Verts<T>> &verts, const std::array<std::vector<T>, 3> &grid_line)
{
  std::array<double, 3> center = {0, 0, 0};
  for (auto v : voxel_verts_)
  {
    for (int a = 0; a < 3; ++a)
      center[a] += verts.at(v).v_[a];
  }
  for (int a = 0; a < 3; ++a)
  {
    center[a] /= voxel_verts_.size();
    idx_[a] = lower_bound(grid_line[a].begin(), grid_line[a].end(), center[a]) - grid_line[a].begin();
  }

  return 0;
}

template int Voxel::SetVoxelIndex<float>(const std::vector<Verts<float>> &verts, const std::array<std::vector<float>, 3> &grid_line);
template int Voxel::SetVoxelIndex<double>(const std::vector<Verts<double>> &verts, const std::array<std::vector<double>, 3> &grid_line);
