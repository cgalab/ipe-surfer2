
#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <unordered_set>
#include <boost/functional/hash.hpp>

#include "SkeletonStructure.h"

#include "ipesurfer2.h"

bool KGonIpelet::run(int, IpeletData *data, IpeletHelper *helper) {
	std::stringstream ss;

	/* check if something is selected */
	Page *page = data->iPage;
	int sel = page->primarySelection();
	if (sel < 0) {helper->message("No selection");return false;}

	/* get selected paths from page */
	std::vector<const Path*> selection;
	for(size_t i = 0; i < page->count(); ++i) {
		if(page->select(i) > 0 && page->object(i)->type() == Object::Type::EPath) {
			selection.emplace_back(page->object(i)->asPath());
		}
	}

//	ss << " Selected paths: " << selection.size() << " ! ";
//	helper->message(ss.str().c_str());
//	ss.clear();

	std::vector<std::tuple<Vector,Vector,double>> vertexPairs;

	std::vector<std::pair<CurveSegment,Attribute>> segments;
	/* get segments from selection */
	for(auto p : selection) {
		for(int sub = 0; sub < p->shape().countSubPaths(); ++sub) {

			if(p->shape().subPath(sub)->type() == SubPath::Type::ECurve) {
				for(int i = 0; i < p->shape().subPath(sub)->asCurve()->countSegments(); ++i) {
					segments.push_back({p->shape().subPath(sub)->asCurve()->segment(i),p->pen()});
				}
				if(p->shape().subPath(sub)->asCurve()->closed()) {
					auto firstSeg = p->shape().subPath(sub)->asCurve()->segment(0);
					auto lastSeg = p->shape().subPath(sub)->asCurve()->segment(p->shape().subPath(sub)->asCurve()->countSegments()-1);

					Vector vA = firstSeg.cp(0); Vector vB = lastSeg.last();

					if(p->pen().isNumber()) {
						vertexPairs.push_back({vA,vB,p->pen().number().toDouble()});
					} else {
						vertexPairs.push_back({vA,vB,1.0});
					}
				}
			}

		}
	}

	std::unordered_map<std::pair<double, double>, int, boost::hash<std::pair<double, double>>> pointsMap;

	for(auto s : segments) {
		if(s.first.type() == CurveSegment::Type::EArc) {
			std::cerr << "TODO: an EArc not added!" << std::endl;
		} else if(s.first.type() == CurveSegment::Type::ESegment) {
			std::cerr << "added ESegment" << std::endl;
			Vector vA = s.first.cp(0); Vector vB = s.first.last();
			pointsMap.insert({{vA.x,vA.y},-1});
			pointsMap.insert({{vB.x,vB.y},-1});

			if(s.second.isNumber()) {
				vertexPairs.push_back({vA,vB,s.second.number().toDouble()});
			} else {
				vertexPairs.push_back({vA,vB,1.0});
			}
		}
	}

	std::vector<std::pair<double,double>> points;

	for(auto& p : pointsMap) {
		p.second = points.size();
		points.push_back(p.first);
	}

	std::vector<std::pair<int,int>> edges;
	std::vector<double> edgeWeights;
	std::vector<unsigned> vertexDegree(points.size(),0);


	for(auto& edge : vertexPairs) {
		auto itA = pointsMap.find({std::get<0>(edge).x,std::get<0>(edge).y});
		auto itB = pointsMap.find({std::get<1>(edge).x,std::get<1>(edge).y});
		edges.push_back({itA->second,itB->second});
		edgeWeights.push_back(std::get<2>(edge));
		++vertexDegree[itA->second];
		++vertexDegree[itB->second];
	}


	ISBasicInput input(points,edges,edgeWeights,vertexDegree);

	SkeletonStructure s(input);

	s.initialize(-1);
	s.wp.advance_to_end();


//	  if (write_ipe) {
//	      s.get_skeleton().write_ipe(os, skoffset);
//	    } else {
//	      s.get_skeleton().write_obj(os);
//	    }


//	const Path *p = page->object(sel)->asPath();
//	if (p == 0 || p->shape().countSubPaths() != 1 ||
//			p->shape().subPath(0)->type() != SubPath::EEllipse) {
//		helper->message("Primary selection is not a circle");
//		return false;
//	}
//	String str = helper->getParameter("n");   // get default value from Lua wrapper
//	if (!helper->getString("Enter k (number of corners)", str))
//		return false;
//	int k = Lex(str).getInt();
//	if (k < 3 || k > 1000)
//		return false;
//	const Ellipse *e = p->shape().subPath(0)->asEllipse();
//	Matrix m = p->matrix() * e->matrix();
//
//	Vector center = m.translation();
//	Vector v = m * Vector(1,0);
//	double radius = (v - center).len();
//	Curve *sp = new Curve;
//	double alpha = 2.0 * IpePi / k;
//	Vector v0 = center + radius * Vector(1,0);
//	for (int i = 1; i < k; ++i) {
//		Vector v1 = center + radius * Vector(Angle(i * alpha));
//		sp->appendSegment(v0, v1);
//		v0 = v1;
//	}
//	sp->setClosed(true);
//	Shape shape;
//	shape.appendSubPath(sp);
//	Path *obj = new Path(data->iAttributes, shape);
//	page->append(ESecondarySelected, data->iLayer, obj);
//	helper->message("Created regular k-gon");
	return true;
}
