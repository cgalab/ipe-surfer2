#pragma once

#include <ipelet.h>
#include <ipepath.h>
#include <ipepage.h>

#include <vector>

#include "SkeletonDCEL.h"
#include "BasicInput.h"

using namespace ipe;

IPELET_DECLARE Ipelet *newIpelet();

class SurfIpelet : public Ipelet {
public:
	virtual int ipelibVersion() const { return IPELIB_VERSION; }
	virtual bool run(int, IpeletData *data, IpeletHelper *helper);

private:
	bool separateLayers  = false;
	bool computeOffset   = false;
	bool computeSkeleton = true;

	void parseParameters(IpeletHelper* helper);

	std::vector<const Path*> getSelectedPaths(Page* page) const;

	void convertToAlmostBasicInput(
			const std::vector<const Path*> selection,
			std::vector<std::pair<double,double>>& points,
			std::vector<std::pair<int,int>>& edges,
				std::vector<double>& edgeWeights,
				std::vector<unsigned>& vertexDegree
	) const;

	std::vector<Segment_2> getSKSegments(const SkeletonDCEL& sk) const;

	inline Vector getVector(const Point_2& p) const {
		Vector v;
		v.x = p.x().doubleValue();
		v.y = p.y().doubleValue();
		return v;
	}

	size_t getMarkedView(const Page* page) {
		for(size_t i = 0; i < page->countViews(); ++i) {
			if(page->markedView(i)) {
				return i;
			}
		}
		return 0;
	}

	Curve** getCurveFromSegment2(const Segment_2& s) const {
		Curve *sp = new Curve;
		auto v0 = getVector(s.source());
		auto v1 = getVector(s.target());
		sp->appendSegment(v0, v1);
		return &sp;
	}
	inline int cc(int c) const { return (int)(c*1000/255); }
};

IPELET_DECLARE Ipelet *newIpelet() {
	return new SurfIpelet;
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
