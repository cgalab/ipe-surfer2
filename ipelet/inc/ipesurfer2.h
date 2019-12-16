#pragma once

#include <ipelet.h>
#include <ipepath.h>
#include <ipepage.h>

#include "BasicInput.h"

using namespace ipe;

IPELET_DECLARE Ipelet *newIpelet();

class KGonIpelet : public Ipelet {
public:
	virtual int ipelibVersion() const { return IPELIB_VERSION; }
	virtual bool run(int, IpeletData *data, IpeletHelper *helper);
};

IPELET_DECLARE Ipelet *newIpelet() {
	return new KGonIpelet;
}

class ISBasicInput : public BasicInput {
public:
	ISBasicInput(
			std::vector<std::pair<double,double>> p_points,
			std::vector<std::pair<int,int>> m_edges,
			std::vector<double> m_edgeWeights,
			std::vector<unsigned> m_degree
	) {
		for(size_t i = 0; i < p_points.size(); ++i) {
			auto p = p_points[i];
			add_vertex(Vertex(Point_2(p.first,p.second),m_degree[i],i));
		}
		for(size_t i = 0; i < m_edges.size(); ++i) {
			auto e = m_edges[i];
			add_edge(e.first,e.second,m_edgeWeights[i]);
		}
	}
};
