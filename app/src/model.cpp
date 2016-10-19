#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            verts_.push_back(v);
			if (std::abs(v.x) > size) size = std::abs(v.x); // lazy quick fix
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
			if (line.find('/')==std::string::npos) {
				int fs[3];
				sscanf(line.c_str(), "%*s %d %d %d", &fs[0], &fs[1], &fs[2]);
				f.push_back(--fs[0]);
				f.push_back(--fs[1]);
				f.push_back(--fs[2]);
			} else {
				while (iss >> idx >> trash >> itrash >> trash >> itrash) {
					idx--; // in wavefront obj all indices start at 1, not zero
					f.push_back(idx);
				}
			}
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    return faces_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

