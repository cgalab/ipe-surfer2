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

#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <unordered_set>
#include <boost/functional/hash.hpp>

#include "SkeletonStructure.h"

#include "ipesurfer2.h"

#include <ipegroup.h>

bool SurfIpelet::run(int method, IpeletData *data, IpeletHelper *helper) {
	/* check if something is selected */
	Page *page = data->iPage;
	int sel = page->primarySelection();
	if (sel < 0) {helper->message("No selection");return false;}

	auto selection = getSelectedPaths(page);

	/* parse parameter from 'lua' what we have to do */
	parseParameters(method);

	/* convert selection and run surfer2 */
	std::vector<std::pair<double,double>> points;
	std::vector<std::pair<int,int>> edges;
	std::vector<double> edgeWeights;
	std::vector<unsigned> vertexDegree;

	convertToAlmostBasicInput(selection,points,edges,edgeWeights,vertexDegree);
	if(edges.size() < 2) {
		helper->message("surfer 2 -- more than one segment please!"); return false;
	}

	auto s = std::make_unique<SkeletonStructure>( ISBasicInput(points,edges,edgeWeights,vertexDegree) );
	s->initialize(restrict_component);
	s->wp.advance_to_end();
	auto& sk = s->get_skeleton();
	auto sk_segments = getSKSegments(sk);

	auto viewId = getMarkedView(page);

	/* compute offset */
	if(computeOffset) {
		String str = "5,10,15";
		if (!helper->getString("Enter offset-spec (e.g., 5,10,15)", str)) {
			str = "5,10,15";
		}

		std::string offset_spec(str.z());
		std::vector<SkeletonDCEL::OffsetFamily> offsets;
		for (const NT& offset_distance : sk.parse_offset_spec( offset_spec )) {
			offsets.emplace_back(sk.make_offset(offset_distance));
		}

		if (offsets.size() > 0) {
			AllAttributes allAttr = data->iAttributes;
			allAttr.iStroke  = Attribute(Color(cc(189), cc(195), cc(199)));
			allAttr.iDashStyle = Attribute(allAttr.iDashStyle.makeDashStyle("dashed-narrow"));
			allAttr.iPen = Attribute(Fixed::fromDouble(0.3));

			auto offLayer = data->iLayer;
			if(separateLayers) {
				page->addLayer("offset");
				offLayer = page->findLayer("offset");
				page->setVisible(viewId,"offset",true);
			}

			Group* grp = new ipe::Group();
			for (const auto& family : offsets) {
				Group* family_group = new ipe::Group();
				for (const Segment_2& segment : family) {
					Shape shape;
					Curve* sp = getCurveFromSegment2(segment);
					shape.appendSubPath(sp);
					Path *obj = new Path(allAttr, shape);
					family_group->push_back(obj);
				}
				grp->push_back(family_group);
			}
			page->append(ESecondarySelected, offLayer, grp);

		}
		helper->message("Offset Computed.");
	}

	/* compute skeleton */
	if(computeSkeleton) {
		auto skLayer = data->iLayer;
		if(separateLayers) {
			page->addLayer("sk");
			skLayer = page->findLayer("sk");
			page->setVisible(viewId,"sk",true);
		}

		{
			AllAttributes allAttr = data->iAttributes;
			allAttr.iStroke  = Attribute(Color((int)(85*1000/255),(int)(170*1000/255),(int)(255*1000/255)));
			allAttr.iLineCap = TLineCap::ERoundCap;

			Group* sk_group = new ipe::Group();
			for(auto s : sk_segments) {
				Shape shape;
				Curve* sp = getCurveFromSegment2(s);
				shape.appendSubPath(sp);
				Path *obj = new Path(allAttr, shape);
				sk_group->push_back(obj);
			}

			page->append(ESecondarySelected, skLayer, sk_group);
		}
		helper->message("Skeleton Computed.");
	}

	return true;
}

std::vector<const Path*> SurfIpelet::getSelectedPaths(Page* page) const {
	/* get selected paths from page */
	std::vector<const Path*> selection;
	for(size_t i = 0; i < page->count(); ++i) {
		if(page->select(i) > 0 && page->object(i)->type() == Object::Type::EPath) {
			selection.emplace_back(page->object(i)->asPath());
		}
	}
	return selection;
}

void SurfIpelet::convertToAlmostBasicInput(
			const std::vector<const Path*> selection,
			std::vector<std::pair<double,double>>& points,
			std::vector<std::pair<int,int>>& edges,
			std::vector<double>& edgeWeights,
			std::vector<unsigned>& vertexDegree
	) const {

	std::vector<std::tuple<Vector,Vector,double>> vertexPairs;
	std::unordered_map<std::pair<double, double>, int, boost::hash<std::pair<double, double>>> pointsMap;

	/* get segments from selection */
	for(auto p : selection) {
		for(int sub = 0; sub < p->shape().countSubPaths(); ++sub) {
			double weight = getWeightFromString(p->pen());

			if(p->shape().subPath(sub)->type() == SubPath::Type::ECurve) {

				for(int i = 0; i < p->shape().subPath(sub)->asCurve()->countSegments(); ++i) {
					auto seg = p->shape().subPath(sub)->asCurve()->segment(i);

					if(seg.type() == CurveSegment::Type::EArc) {
						std::cerr << "TODO: an EArc not yet handled!" << std::endl;
					} else if(seg.type() == CurveSegment::Type::ESegment) {
						Vector vA = seg.cp(0); Vector vB = seg.last();

						pointsMap.insert({{vA.x,vA.y},-1});
						pointsMap.insert({{vB.x,vB.y},-1});
						vertexPairs.push_back({vA,vB,weight});
					}
				}
				if(p->shape().subPath(sub)->asCurve()->closed()) {
					/* we need to add the closing edge if the curve is closed (not contained path) */
					auto firstSeg = p->shape().subPath(sub)->asCurve()->segment(0);
					auto lastSeg  = p->shape().subPath(sub)->asCurve()->segment(p->shape().subPath(sub)->asCurve()->countSegments()-1);

					Vector vA = firstSeg.cp(0); Vector vB = lastSeg.last();
					vertexPairs.push_back({vA,vB,weight});
				}
			}
		}
	}

	for(auto& p : pointsMap) {
		p.second = points.size();
		points.push_back(p.first);
	}

	vertexDegree.resize(points.size(),0);

	for(auto& edge : vertexPairs) {
//		printTuple(edge);
		auto itA = pointsMap.find({std::get<0>(edge).x,std::get<0>(edge).y});
		auto itB = pointsMap.find({std::get<1>(edge).x,std::get<1>(edge).y});
		assert(itA != pointsMap.end());
		assert(itB != pointsMap.end());
		edges.push_back({itA->second,itB->second});
		edgeWeights.push_back(std::get<2>(edge));
		++vertexDegree[itA->second];
		++vertexDegree[itB->second];
	}
}

std::vector<Segment_2> SurfIpelet::getSKSegments(const SkeletonDCEL& sk) const {
	std::vector<Segment_2> sk_segments;
	const int ray_length = 50;
	for (auto hit = sk.halfedges_begin(); hit != sk.halfedges_end(); ++hit) {
		if (hit > hit->opposite()) continue; /* Only paint one of every halfedge pair */
		if (hit->is_input()) continue;

		const auto& arc = hit->curve();
		Segment_2 s;
		if (arc.type() == typeid(Segment_3)) {
			const Segment_3& s3 = boost::get<Segment_3>(arc);
			s = project_plane(s3);
		} else {
			assert(arc.type() == typeid(Ray_3));
			const Ray_3& r = boost::get<Ray_3>(arc);
			const Ray_2 r2 = project_plane(r);
			s =  Segment_2(r2.source(), r2.point(ray_length));
		}
		sk_segments.emplace_back(s);
	}

	return sk_segments;
}

void SurfIpelet::parseParameters(int kind) {
	switch(kind) {
	case 0:	computeSkeleton = true;
			computeOffset   = false;
			break;

	case 1:	computeSkeleton = true;
			computeOffset   = false;
			restrict_component = 0;
			break;

	case 2:	computeSkeleton = true;
			computeOffset   = false;
			restrict_component = 1;
			break;

	case 3: computeSkeleton = false;
			computeOffset   = true;
			break;

	case 4: computeSkeleton = true;
			computeOffset   = true;
			break;

	case 5: computeSkeleton = true;
			computeOffset   = true;
			separateLayers  = true;
			break;
	}
}

double SurfIpelet::getWeightFromString(const Attribute& pen) const {
	std::string str(pen.string().z());
	if(str.compare("normal") == 0)   {return 0.4;}
	if(str.compare("heavier") == 0)  {return 0.8;}
	if(str.compare("fat") == 0) 	 {return 1.2;}
	if(str.compare("ultrafat") == 0) {return 2.0;}
	return pen.number().toDouble();
}
