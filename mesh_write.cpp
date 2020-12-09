#include "embedding_mesh.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <cassert>
#include <unordered_map>
#include <fstream>
#include "key_comparison.h"

using namespace std;
using namespace Jing;



template<typename T>
int EmbeddingMesh<T>::WriteMesh(const char *path) const
{
  ofstream f_out(path);
  if (!f_out)
  {
    cout << "error in output file open" << endl;
    return 0;
  }

  f_out << "# vtk DataFile Version 2.0" << endl;
  f_out << "Unstructured Grid Example" << endl;
  f_out << "ASCII" << endl;
  f_out << "DATASET UNSTRUCTURED_GRID" << endl;
  f_out << "POINTS " << verts_group_.size() << " double" << endl;

  std::map<int, int> sort_from_one;

  for (const auto &g : verts_group_)
  {
    const std::array<T, 3> &v = corner_coordinates_.at(g.first);
    f_out << v[0] << " " << v[1] << " " << v[2] << endl;
    sort_from_one.insert(make_pair<int, int>(g.first, sort_from_one.size()));
  }
  f_out << "CELLS " << id_to_domain_.size() << " " << 9 * id_to_domain_.size() << endl;
  for (const auto &d : id_to_domain_)
  {
    const array<int, 8> &corner = idx_to_voxel_.at(d.second.first)->domain_corner_.at(d.second.second);
    f_out << 8;
    for (int v = 0; v < 8; ++v)
    {
      f_out << " " << sort_from_one.at(mesh_verts_union_.find(corner[v]));
    }
    f_out << endl;
  }
  f_out << "CELL_TYPES " << id_to_domain_.size() << endl;
  for (int i = 0; i < id_to_domain_.size(); ++i)
  {
    f_out << 12 << endl;
  }

  f_out.close();
  return 0;
}


template<typename T>
int EmbeddingMesh<T>::WriteCutCell(const char *path) const
{
  ofstream f_out(path);
   if (!f_out)
  {
    cout << "error in output file open" << endl;
    return 0;
  }
  std::cerr << "\033[1;37m" << __FILE__ << "__________________" << __LINE__ << "\033[0m" << std::endl;

  f_out << "# vtk DataFile Version 2.0" << endl;
  f_out << "Unstructured Grid Example" << endl;
  f_out << "ASCII" << endl;
  f_out << "DATASET UNSTRUCTURED_GRID" << endl;
  f_out << "POINTS " << verts_unique_.size() << " double" << endl;

  for (const auto &v : verts_unique_)
  {
    f_out << v.v_[0] << " " << v.v_[1] << " " << v.v_[2] << endl;
  }

  f_out << "CELLS " << id_to_domain_.size() << " ";
  size_t cell_pos = f_out.tellp();
  f_out << "                       " << endl;
  size_t cell_count = 0;
  for (const auto &d : id_to_domain_)
  {
    size_t line_pos = f_out.tellp();
    size_t line_count = 1;
    const std::shared_ptr<Voxel> &voxel_ptr = idx_to_voxel_.at(d.second.first);
    const std::vector<std::vector<int>> &cell_face = voxel_ptr->domain_face_.at(d.second.second);
    f_out << "                        " << cell_face.size();
    for (const auto &f : cell_face)
    {
      f_out << " " << f.size();
      line_count += f.size() + 1;
      for (auto v : f)
      {
        f_out << " " << v;
      }

    }
    f_out << endl;
    size_t line_end = f_out.tellp();
    f_out.seekp(line_pos);
    f_out << line_count;
    f_out.seekp(line_end);
    cell_count += (line_count + 1);
  }

  f_out << "CELL_TYPES " << id_to_domain_.size() << endl;
  for (int i = 0; i < id_to_domain_.size(); ++i)
  {
    f_out  << 42 << endl;
  }

  f_out << "CELL_DATA " << id_to_domain_.size() << endl;
  f_out << "SCALARS edge double 1" << endl;
  f_out << "LOOKUP_TABLE default" << endl;
  for (int i = 0; i < id_to_domain_.size(); ++i)
  {
    f_out << fmod(i/8.0, 1.0) << endl;
  }

  f_out << "LOOKUP_TABLE my_table 8" << endl;
  f_out << "0.407843 0.407843 0.407843 1" << endl;
  f_out << "0.407843 0.407843 0.8 1" << endl;
  f_out << "0.407843 0.8 0.407843 1" << endl;
  f_out << "0.407843 0.8 0.8 1" << endl;
  f_out << "0.8 0.407843 0.407843 1" << endl;
  f_out << "0.8 0.407843 0.8 1" << endl;
  f_out << "0.8 0.8 0.407843 1" << endl;
  f_out << "0.8 0.8 0.8 1" << endl;

  f_out.seekp(cell_pos);
  f_out << cell_count;
  f_out.close();
  return 0;
}
