#include <iostream>
#include "embedding_mesh.h"



using namespace std;
using FLOAT = double;

int main (int argc, char *argv[])
{
  EmbeddingMesh<FLOAT> embedding_mesh(argv[1]);
  embedding_mesh.RemoveDuplicateVerts();
  embedding_mesh.SetVoxelDomainAndIndex();

  cout << "cell num:" << embedding_mesh.cells_unique_.size() << endl;
  for (auto &v : embedding_mesh.cells_unique_)
  {
    cerr << v.domain_verts_.size() << " " << v.idx_[0] << " " << v.idx_[1] << " " << v.idx_[2] << endl;
  }

  
  return 0;
}
