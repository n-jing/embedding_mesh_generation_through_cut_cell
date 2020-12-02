#include "embedding_mesh.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
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
