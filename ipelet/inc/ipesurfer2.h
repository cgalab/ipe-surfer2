/* ipe-surfer2 is an ipelet written in C++ and Lua. It is a wrapper
 * for surfer2, a weighted straight skeleton implementation based
 * exact arithmetic.
 *
 * Copyright 2019 Günther Eder - geder@cs.sbg.ac.at
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <ipelet.h>
#include <ipepath.h>
#include <ipepage.h>

#include <vector>
#include <string>

#include "SkeletonDCEL.h"
#include "BasicInput.h"

using namespace ipe;

IPELET_DECLARE Ipelet *newIpelet();

class SurfIpelet : public Ipelet {
public:
	virtual int ipelibVersion() const { return IPELIB_VERSION; }
	virtual bool run(int method, IpeletData *data, IpeletHelper *helper);
	~SurfIpelet() {}

private:
	bool separateLayers    = false;
	bool computeOffset     = false;
	bool computeSkeleton   = true;
	int restrict_component = -1;

	void parseParameters(int kind);

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

	Curve* getCurveFromSegment2(const Segment_2& s) const {
		Curve* sp = new Curve;
		auto v0 = getVector(s.source());
		auto v1 = getVector(s.target());
		sp->appendSegment(v0, v1);
		return sp;
	}

	double getWeightFromString(const Attribute& pen) const;

	/* Attribute Color has a rgb range from 0 to 1000 for each value instead of 255 */
	inline int cc(int c) const { return (int)(c*1000/255); }

	/* DEBUG OUTPUT */
	void printTuple(const std::tuple<Vector,Vector,double>& edge) const {
		std::cerr << "in vertexpair : "
				<< std::get<0>(edge).x  << "," << std::get<0>(edge).y
				<< " and "
				<< std::get<1>(edge).x  << "," << std::get<1>(edge).y
				<< " weight: "
				<< std::get<2>(edge)
				<< std::endl;
	}
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

		finalize();
	}
};
