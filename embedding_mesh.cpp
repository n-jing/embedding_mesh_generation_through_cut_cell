#include "embedding_mesh.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <cassert>
#include <unordered_map>
#include "key_comparison.h"

using namespace std;
using namespace Jing;


template<typename T>
EmbeddingMesh<T>::EmbeddingMesh(const char *path)
{
  if (string(path).substr(string(path).rfind(".")) != ".vtk")
  {
    cerr << "[  \033[1;31merror\033[0m  ] " << "only vtk format are supported!" << endl;
    exit(1);
  }
  ifstream f_in(path);
  if (!f_in)
  {
    cerr << "[  \033[1;31merror\033[0m  ] " << "error in file open!" << endl;
    exit(1);
  }
  string str_line;
  while (!f_in.eof())
  {
    getline(f_in, str_line);
    if (str_line.substr(0, str_line.find(" ")) == "POINTS")
    {
      int verts_num = 0;
      sscanf(str_line.c_str(), "%*s%d%*s", &verts_num);
      verts_ = vector<Verts<T>>(verts_num);
      for (int i = 0; i < verts_num; ++i)
      {
        getline(f_in, str_line);
        sscanf(str_line.c_str(), "%lf%lf%lf", &verts_[i].v_[0], &verts_[i].v_[1], &verts_[i].v_[2]);
      }
    }
    if (str_line.substr(0, str_line.find(" ")) == "CELLS")
    {
      int cells_num = 0;
      sscanf(str_line.c_str(), "%*s%d%*s", &cells_num);
      cells_ = std::vector<Voxel>(cells_num);
      for (int i = 0; i < cells_num; ++i)
      {
        getline(f_in, str_line);
        stringstream s_str(str_line);
        int line_num;
        int cell_face_num;
        s_str >> line_num >> cell_face_num;
        for (int cf = 0; cf < cell_face_num; ++cf)
        {
          vector<int> face;
          int v_num;
          s_str >> v_num;
          for (int j = 0; j < v_num; ++j)
          {
            int v_idx;
            s_str >> v_idx;
            face.push_back(v_idx);
          }
          cells_[i].AddFace(face);
        }
      }
    }
  }
  f_in.close();

  string grid_file(path);
  grid_file = grid_file.substr(0, grid_file.rfind(".")) + ".line";
  GetGridLine(grid_file.c_str());
}

template<typename T>
int EmbeddingMesh<T>::RemoveDuplicateVerts()
{
  map<array<T, 3>, int, KeyCompare<array<T, 3>, 1000000>> verts_to_id;
  unordered_map<int, int> old_to_new_id;
  for (int v = 0; v < verts_.size(); ++v)
  {
    if (!verts_to_id.count(verts_[v].v_))
    {
      old_to_new_id.emplace(v, verts_to_id.size());
      verts_to_id.emplace(verts_[v].v_, verts_to_id.size());
      verts_unique_.push_back(verts_[v].v_);
    }
    else
      old_to_new_id.emplace(v, verts_to_id.at(verts_[v].v_));
  }

  cells_unique_ = cells_;
  for (int c = 0; c < cells_.size(); ++c)
  {
    for (int p = 0; p < cells_[c].all_face_.size(); ++p)
    {
      const vector<int> &face_v = cells_[c].all_face_[p];
      for (int q = 0; q < face_v.size(); ++q)
      {
        assert(old_to_new_id.count(face_v[q]));
        cells_unique_[c].all_face_[p][q] = old_to_new_id.at(face_v[q]);
      }
    }
  }

  return 0;
}

template<typename T>
int EmbeddingMesh<T>::SetVoxelDomainAndIndex()
{
  for (int i = 0; i < cells_unique_.size(); ++i)
  {
    cells_unique_[i].SetVoxelDomain();
    cells_unique_[i].SetVoxelIndex<T>(verts_unique_, grid_line_);
  }

  return 0;
}

template<typename T>
int EmbeddingMesh<T>::GetGridLine(const char *path)
{
  ifstream f_in(path);
  if (!f_in)
  {
    cerr << "[  \033[1;31merror\033[0m  ] " << "error in grid_line file open" << endl;
    exit(1);
  }

  string str_line;
  for (int axis = 0; axis < 3; ++axis)
  {
    grid_line_[axis].clear();
    getline(f_in, str_line);
    int idx = 0;
    while (idx < str_line.size())
    {
      int next_idx = str_line.find(" ", idx);
      if (next_idx == string::npos)
        next_idx = str_line.size();
      string g = str_line.substr(idx, next_idx-idx);
      grid_line_[axis].push_back(atof(g.c_str()));
      idx = next_idx + 1;
      while (idx < str_line.size() && str_line[idx] == ' ')
        ++idx;
    }
  }

  f_in.close();
  return 0;
}

