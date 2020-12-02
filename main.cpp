#include <iostream>
#include "embedding_mesh.h"



using namespace std;
using FLOAT = double;

int main (int argc, char *argv[])
{
  EmbeddingMesh<FLOAT> embedding_mesh(argv[1]);
  embedding_mesh.RemoveDuplicateVerts();
  embedding_mesh.SetVoxelDomainAndIndex();
  embedding_mesh.SetDomainCorner();
  embedding_mesh.SetDomainNeighbor();
  embedding_mesh.MergeDuplicateVerts();
  embedding_mesh.WriteMesh("out.vtk");
  
  cout << "cell num:" << embedding_mesh.cells_unique_.size() << endl;
  for (auto &v : embedding_mesh.idx_to_voxel_)
  {
    cerr << v.first[0] << " " << v.first[1] << " " << v.first[2] << "||" << v.second->domain_verts_.size();
    for (auto &c : v.second->domain_corner_)
    {
      cout << c[0] << " " << c[1] << " " << c[2] << " " << c[3] << " "
           << c[4] << " " << c[5] << " " << c[7] << " " << c[7] << endl;
    }

  }

  
  return 0;
}
