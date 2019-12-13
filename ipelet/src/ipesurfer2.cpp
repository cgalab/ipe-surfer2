
#include <iostream>

#include "ipesurfer2.h"

bool KGonIpelet::run(int, IpeletData *data, IpeletHelper *helper) {
	Page *page = data->iPage;
	int sel = page->primarySelection();
	if (sel < 0) {
		helper->message("No selection");
		return false;
	}

	const Path *p = page->object(sel)->asPath();

	std::string message("num sub paths: " + p->shape().countSubPaths());
	helper->message(message.c_str());


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
////	helper->message("Created regular k-gon");
	return true;
}
