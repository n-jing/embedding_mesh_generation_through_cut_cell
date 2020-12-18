#include <iostream>
#include "embedding_mesh.h"



using namespace std;
using FLOAT = double;

int main (int argc, char *argv[])
{
  EmbeddingMesh<FLOAT> embedding_mesh(argv[1]);
  
  embedding_mesh.RemoveDuplicateVerts();
  embedding_mesh.SetVoxelDomainAndIndex();
#ifdef SEPERATE_CELL
  embedding_mesh.WriteCutCell(argv[3]);
#endif
  embedding_mesh.SetDomainCorner();
  embedding_mesh.SetDomainNeighbor();
  embedding_mesh.MergeDuplicateVerts();

  string out(argv[1]);
  if (argc == 1)
    out = out.substr(0, out.rfind(".")) + "_hex.vtk";
  else
    out = string(argv[2]);

  embedding_mesh.WriteMesh(out.c_str());

  cerr << "******************************" << endl;
  const vector<int> &surface_v_domain_id = embedding_mesh.GetSurfaceVertsDomainId();
  for (auto d : surface_v_domain_id)
  {
    cerr << d << ", ";
  }
  cerr << endl;
  return 0;
}
