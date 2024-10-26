#include "BooleanOperation.hh"
#include "OpenFlipper/BasePlugin/PluginFunctions.hh"
#include <ObjectTypes/PolyMesh/PolyMesh.hh>

#include <iostream>
#include <cassert>
#include <string>
#include <fstream>
#include <vector>
#include <set>

//#define QUICKLOAD

using namespace BoolOp;

void Boolean::initializePlugin()
{
	QWidget* toolBox = new QWidget();

	// Create the Toolbox Widget
	QWidget* toolBoxUnion = new QWidget();
	QWidget* toolBoxIntersection = new QWidget();
	QWidget* toolBoxDifference = new QWidget();
	QWidget* toolBoxRead = new QWidget();
	QWidget* toolBoxClear = new QWidget();

	QPushButton* UnionButton = new QPushButton("&Union", toolBoxUnion);
	QPushButton* IntersectionButton = new QPushButton("&Intersection", toolBoxIntersection);
	QPushButton* DifferenceButton = new QPushButton("&Difference", toolBoxDifference);
	QPushButton* ReadButton1 = new QPushButton("&Poly 1", toolBoxRead);
	QPushButton* ReadButton2 = new QPushButton("&Poly 2", toolBoxRead);
	QPushButton* ClearButton = new QPushButton("&Clear", toolBoxClear);

	QGridLayout* layout = new QGridLayout(toolBox);

	layout->addWidget(UnionButton, 0, 0);
	layout->addWidget(IntersectionButton, 0, 1);
	layout->addWidget(DifferenceButton, 0, 2);
	layout->addWidget(ReadButton1, 1, 0);
	layout->addWidget(ReadButton2, 1, 1);
	layout->addWidget(ClearButton, 1, 2);

	connect(UnionButton, SIGNAL(clicked()), this, SLOT(Union()));
	connect(IntersectionButton, SIGNAL(clicked()), this, SLOT(Intersection()));
	connect(DifferenceButton, SIGNAL(clicked()), this, SLOT(Difference()));
	connect(ReadButton1, SIGNAL(clicked()), this, SLOT(ReadPoly1()));
	connect(ReadButton2, SIGNAL(clicked()), this, SLOT(ReadPoly2()));
	connect(ClearButton, SIGNAL(clicked()), this, SLOT(Clear()));

	emit addToolbox(tr("Boolean"), toolBox);
}

void Boolean::ReadPoly1()
{
	//QString fileName = QFileDialog::getOpenFileName(nullptr, "open", "../", "txt(*.txt)");
	//if (!fileName.isEmpty()) {
	//	QFile file(fileName);
	//	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
	//		emit log(LOGERR, "Cannot open file");
	//		return;
	//	}

	//	PolyLineCollectionObject* poly1(0);
	//	emit addEmptyObject(DATA_POLY_LINE_COLLECTION, poly1_id);
	//	PluginFunctions::getObject(poly1_id, poly1);

	//	QTextStream in(&file);
	//	bool flag = false;
	//	PolyLine* polygon;
	//	while (!in.atEnd()) {
	//		QStringList line = in.readLine().split(" ");
	//		if (line[0] == "#loop" && !flag) {
	//			int id = poly1->new_poly_line();
	//			polygon = poly1->polyline(id);
	//			polygon->clear();
	//			flag = true;
	//			continue;
	//		}
	//		else if (line[0] == "#loop" && flag) {
	//			int id = poly1->new_poly_line();
	//			polygon = poly1->polyline(id);
	//			polygon->clear();
	//			flag = false;
	//			continue;
	//		}
	//				
	//		PolyLinePoint point(line[0].toDouble(), line[1].toDouble(), 0.0);
	//		polygon->add_point(point);
	//	}
	//	
	//	emit updatedObject(poly1_id, UPDATE_ALL);
	//	emit log(LOGERR, "Read Success");
	//	file.close();
	//}

	APolygon.clear();
#ifndef QUICKLOAD
	auto filePath = QFileDialog::getOpenFileName(nullptr,
		tr("Choose one file"),
		QString(),
		u8"Plain Text (*.txt *.ppp)");

	load(filePath.toStdString(), APolygon);
#else
	auto filePath = "E:/Course/Postgraduate/CAD/example/case1/01.txt";
	load(filePath, APolygon);
#endif
	for (auto i = 0; i < APolygon.size(); i++)
	{
		auto mesh = toMesh(APolygon[i]);
		addMesh(mesh, QString("Polygon A-") + QString::number(i+1));
		emit log(LOGERR, QString("Polygon A-") + QString::number(i+1));
	}

}


void Boolean::ReadPoly2()
{
	//QString fileName = QFileDialog::getOpenFileName(nullptr, "open", "../", "txt(*.txt)");
	//if (!fileName.isEmpty()) {
	//	QFile file(fileName);
	//	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
	//		emit log(LOGERR, "Cannot open file");
	//		return;
	//	}

	//	PolyLineCollectionObject* poly2(0);
	//	emit addEmptyObject(DATA_POLY_LINE_COLLECTION, poly2_id);
	//	PluginFunctions::getObject(poly2_id, poly2);

	//	QTextStream in(&file);
	//	bool flag = false;
	//	PolyLine* polygon;
	//	while (!in.atEnd()) {
	//		QStringList line = in.readLine().split(" ");
	//		if (line[0] == "#loop" && !flag) {
	//			int id = poly2->new_poly_line();
	//			polygon = poly2->polyline(id);
	//			polygon->clear();
	//			flag = true;
	//			continue;
	//		}
	//		else if (line[0] == "#loop" && flag) {
	//			int id = poly2->new_poly_line();
	//			polygon = poly2->polyline(id);
	//			polygon->clear();
	//			flag = false;
	//			continue;
	//		}

	//		PolyLinePoint point(line[0].toDouble(), line[1].toDouble(), 0.0);
	//		polygon->add_point(point);
	//	}

	//	emit updatedObject(poly2_id, UPDATE_ALL);
	//	emit log(LOGERR, "Read Success");
	//	file.close();
	//}
	BPolygon.clear();
#ifndef QUICKLOAD
	auto filePath = QFileDialog::getOpenFileName(nullptr,
		tr("Choose one file"),
		QString(),
		u8"Plain Text (*.txt *.ppp)");
	load(filePath.toStdString(), BPolygon);
#else
	auto filePath = "E:/Course/Postgraduate/CAD/example/case1/02.txt";
	load(filePath, BPolygon);
#endif
	for (auto i = 0; i < BPolygon.size(); i++)
	{
		auto mesh = toMesh(BPolygon[i]);
		addMesh(mesh, QString("Polygon B-") + QString::number(i + 1));
		emit log(LOGERR, QString("Polygon B-") + QString::number(i + 1));
	}
}

void Boolean::Clear()
{
	for (auto i = 0; i < meshes.size(); i++)
	{
		PolyMesh* obj(0);
		PluginFunctions::getMesh(meshes[i], obj);
		obj->clear();
		emit updatedObject(meshes[i], UPDATE_ALL);
	}
	meshes.clear();
	emit log(LOGERR, "Clear Success");
}

void Boolean::Union()
{
	Clear();

	std::vector<segment> segmentA;
	std::vector<segment> segmentB;

	for (auto i = 0; i < APolygon.size(); i++)
	{
		polygonToSegments(APolygon[i], segmentA);
	}
	for (auto i = 0; i < BPolygon.size(); i++)
	{
		polygonToSegments(BPolygon[i], segmentB);
	}

	std::vector<segment> insted_segA;
	std::vector<segment> insted_segB;

	getIntersectedSegments(segmentA, segmentB, insted_segA);
	getIntersectedSegments(segmentB, segmentA, insted_segB);

	//for (auto i = 0; i < insted_segA.size(); i++)
	//{
	//	emit log(LOGERR, "Intersect SegmentA: " + QString::number(insted_segA[i][0][0]) + "," + QString::number(insted_segA[i][0][1]) + " " + QString::number(insted_segA[i][1][0]) + "," + QString::number(insted_segA[i][1][1]));
	//}
	//for (auto i = 0; i < insted_segB.size(); i++)
	//{
	//	emit log(LOGERR, "Intersect SegmentB: " + QString::number(insted_segB[i][0][0]) + "," + QString::number(insted_segB[i][0][1]) + " " + QString::number(insted_segB[i][1][0]) + "," + QString::number(insted_segB[i][1][1]));
	//}

	std::vector<segment> save_seg;
	for (auto i = 0; i < insted_segA.size(); i++)
	{
		bool on_edge = false;
		segment temp_seg;
		point mid_point = insted_segA[i][0] * (1 - BLEND_RATIO) + insted_segA[i][1] * BLEND_RATIO;
		//emit log(LOGERR, "Mid Point: " + QString::number(mid_point[0]) + " " + QString::number(mid_point[1]));
		if (!isPointInPolygon(mid_point, segmentB, on_edge, temp_seg))
		{
			if(!on_edge)
				save_seg.emplace_back(insted_segA[i]);
			else if(isSegmentSameDir(temp_seg, insted_segA[i]))
				save_seg.emplace_back(temp_seg);
		}
			
	}
	for (auto i = 0; i < insted_segB.size(); i++)
	{
		bool on_edge = false;
		segment temp_seg;
		point mid_point = insted_segB[i][0] * (1 - BLEND_RATIO) + insted_segB[i][1] * BLEND_RATIO;
		//emit log(LOGERR, "Mid Point: " + QString::number(mid_point[0]) + " " + QString::number(mid_point[1]));
		if (!isPointInPolygon(mid_point, segmentA, on_edge, temp_seg))
		{
			if (!on_edge)
				save_seg.emplace_back(insted_segB[i]);
			else if (isSegmentSameDir(temp_seg, insted_segB[i]))
				save_seg.emplace_back(temp_seg);
		}
	}

	//for (auto i = 0; i < save_seg.size(); i++)
	//{
	//	emit log(LOGERR, "Save Segment: " + QString::number(save_seg[i][0][0]) + "," + QString::number(save_seg[i][0][1]) + " " + QString::number(save_seg[i][1][0]) + "," + QString::number(save_seg[i][1][1]));
	//}
	std::vector<polygon> result;
	while (!save_seg.empty())
	{
		polygon temp_poly;
		segmentsToPolygon(save_seg, temp_poly);
		result.push_back(temp_poly);
	}

	for (auto i = 0; i < result.size(); i++)
	{
		auto mesh = toMesh(result[i]);
		addMesh(mesh, QString("Union-") + QString::number(i + 1));
		emit log(LOGERR, QString("Union-") + QString::number(i + 1));

	}
	emit log(LOGERR, "Union Success");

	//for (auto i = 0; i < segmentA[0].size(); i++)
	//{
	//	bool is_intersect = false;
	//	for (auto j = 0; j < segmentB[0].size(); j++)
	//	{
	//		if (isSegmentIntersect(segmentA[0][i], segmentB[0][j]))
	//		{
	//			point intersect;
	//			if (getIntersect(segmentA[0][i], segmentB[0][j], intersect))
	//			{
	//				//emit log(LOGERR, "Intersect Point: " + QString::number(intersect[0]) + " " + QString::number(intersect[1]));
	//				auto p1 = segmentA[0][i][0];
	//				auto p2 = segmentA[0][i][1];
	//				auto q1 = segmentB[0][j][0];
	//				auto q2 = segmentB[0][j][1];

	//				//if (!isPointInPolygon(p1, BPolygon[0]) && p1 != intersect)
	//				//	save_seg.push_back(segment(p1, intersect));
	//				//if (!isPointInPolygon(p2, BPolygon[0]) && p2 != intersect)
	//				//	save_seg.push_back(segment(p2, intersect));
	//				//if (!isPointInPolygon(q1, APolygon[0]) && q1 != intersect)
	//				//	save_seg.push_back(segment(q1, intersect));
	//				//if (!isPointInPolygon(q2, APolygon[0]) && q2 != intersect)
	//				//	save_seg.push_back(segment(q2, intersect));

	//				if (!isPointInPolygon(p1, segmentB[0]) && p1 != intersect)
	//					save_seg.push_back(segment(p1, intersect));
	//				if (!isPointInPolygon(p2, segmentB[0]) && p2 != intersect)
	//					save_seg.push_back(segment(p2, intersect));
	//				if (!isPointInPolygon(q1, segmentA[0]) && q1 != intersect)
	//					save_seg.push_back(segment(q1, intersect));
	//				if (!isPointInPolygon(q2, segmentA[0]) && q2 != intersect)
	//					save_seg.push_back(segment(q2, intersect));
	//				
	//				is_intersect = true;
	//			}
	//			else
	//				emit log(LOGERR, "Overlap");
	//		}
	//	}
	//	if (!is_intersect)
	//		save_seg.push_back(segmentA[0][i]);
	//}

	//for (auto i = 0; i < segmentB[0].size(); i++)
	//{
	//	bool is_intersect = false;
	//	for (auto j = 0; j < segmentA[0].size(); j++)
	//	{
	//		if (isSegmentIntersect(segmentB[0][i], segmentA[0][j]))
	//		{
	//			is_intersect = true;
	//			break;
	//		}
	//	}
	//	if (!is_intersect)
	//		save_seg.push_back(segmentB[0][i]);
	//}
	//
	//getPolygon(save_seg, result[0]);

	//if (polygon_union(APolygon, BPolygon, result)) {
	//	log("Success to calculate union!");

	//	for (auto i = 0; i < result.size(); i++)
	//	{
	//		auto mesh = toMesh(result[i]);
	//		addMesh(mesh, QString("Union-") + QString::number(i + 1));
	//		emit log(LOGERR, QString("Union-") + QString::number(i + 1));
	//	}
	//	emit log(LOGERR, "Union Success");
	//}
	//else {
	//	log("Fail to calculate union!");
	//}


}

void Boolean::Intersection()
{
	Clear();

	std::vector<segment> segmentA;
	std::vector<segment> segmentB;

	for (auto i = 0; i < APolygon.size(); i++)
	{
		polygonToSegments(APolygon[i], segmentA);
	}
	for (auto i = 0; i < BPolygon.size(); i++)
	{
		polygonToSegments(BPolygon[i], segmentB);
	}

	std::vector<segment> insted_segA;
	std::vector<segment> insted_segB;

	getIntersectedSegments(segmentA, segmentB, insted_segA);
	getIntersectedSegments(segmentB, segmentA, insted_segB);

	std::vector<segment> save_seg;
	for (auto i = 0; i < insted_segA.size(); i++)
	{
		bool on_edge = false;
		segment temp_seg;
		point mid_point = insted_segA[i][0] * (1 - BLEND_RATIO) + insted_segA[i][1] * BLEND_RATIO;
		//emit log(LOGERR, "Mid Point: " + QString::number(mid_point[0]) + " " + QString::number(mid_point[1]));
		if (isPointInPolygon(mid_point, segmentB, on_edge, temp_seg))
			save_seg.emplace_back(insted_segA[i]);
		if(on_edge && isSegmentSameDir(temp_seg, insted_segA[i]))
			save_seg.emplace_back(insted_segA[i]);
	}
	for (auto i = 0; i < insted_segB.size(); i++)
	{
		bool on_edge = false;
		segment temp_seg;
		point mid_point = insted_segB[i][0] * (1 - BLEND_RATIO) + insted_segB[i][1] * BLEND_RATIO;
		//emit log(LOGERR, "Mid Point: " + QString::number(mid_point[0]) + " " + QString::number(mid_point[1]));
		if (isPointInPolygon(mid_point, segmentA, on_edge, temp_seg))
			save_seg.emplace_back(insted_segB[i]);
		if(on_edge && isSegmentSameDir(temp_seg, insted_segB[i]))
			save_seg.emplace_back(insted_segB[i]);
	}

	std::vector<polygon> result;
	while (!save_seg.empty())
	{
		polygon temp_poly;
		segmentsToPolygon(save_seg, temp_poly);
		result.push_back(temp_poly);
	}

	for (auto i = 0; i < result.size(); i++)
	{
		auto mesh = toMesh(result[i]);
		addMesh(mesh, QString("Intersection-") + QString::number(i + 1));
		emit log(LOGERR, QString("Intersection-") + QString::number(i + 1));

	}
	emit log(LOGERR, "Intersection Success");

	//if (polygon_intersection(APolygon, BPolygon, result)) {
	//	log("Success to calculate Intersect!");

	//	for (auto i = 0; i < result.size(); i++)
	//	{
	//		auto mesh = toMesh(result[i]);
	//		addMesh(mesh, QString("Intersection-") + QString::number(i + 1));
	//		emit log(LOGERR, QString("Intersection-") + QString::number(i + 1));
	//	}
	//	emit log(LOGERR, "Intersection Success");
	//}
	//else {
	//	log("Fail to calculate Intersection!");
	//}

}
void Boolean::Difference()
{
	Clear();

	std::vector<segment> segmentA;
	std::vector<segment> segmentB;

	for (auto i = 0; i < APolygon.size(); i++)
	{
		polygonToSegments(APolygon[i], segmentA);
	}
	for (auto i = 0; i < BPolygon.size(); i++)
	{
		polygonToSegments(BPolygon[i], segmentB, true);
	}

	std::vector<segment> insted_segA;
	std::vector<segment> insted_segB;

	getIntersectedSegments(segmentA, segmentB, insted_segA);
	getIntersectedSegments(segmentB, segmentA, insted_segB);

	std::vector<segment> save_seg;
	for (auto i = 0; i < insted_segA.size(); i++)
	{
		bool on_edge = false;
		segment temp_seg;
		point mid_point = insted_segA[i][0] * (1 - BLEND_RATIO) + insted_segA[i][1] * BLEND_RATIO;
		//emit log(LOGERR, "Mid Point: " + QString::number(mid_point[0]) + " " + QString::number(mid_point[1]));
		if (!isPointInPolygon(mid_point, segmentB, on_edge, temp_seg))
		{
			if(on_edge && isSegmentSameDir(temp_seg, insted_segA[i]))
				save_seg.emplace_back(insted_segA[i]);
			else if(!on_edge)
				save_seg.emplace_back(insted_segA[i]);
		}

	}
	for (auto i = 0; i < insted_segB.size(); i++)
	{
		bool on_edge = false;
		segment temp_seg;
		point mid_point = insted_segB[i][0] * (1 - BLEND_RATIO) + insted_segB[i][1] * BLEND_RATIO;
		//emit log(LOGERR, "Mid Point: " + QString::number(mid_point[0]) + " " + QString::number(mid_point[1]));
		if (isPointInPolygon(mid_point, segmentA, on_edge, temp_seg))
			save_seg.emplace_back(insted_segB[i]);
	}

	std::vector<polygon> result;
	while (!save_seg.empty())
	{
		polygon temp_poly;
		segmentsToPolygon(save_seg, temp_poly);
		result.push_back(temp_poly);

		for(auto& p : temp_poly)
			emit log(LOGERR, QString::number(p[0]) + " " + QString::number(p[1]));
	}

	for (auto i = 0; i < result.size(); i++)
	{
		auto mesh = toMesh(result[i]);
		addMesh(mesh, QString("Difference-") + QString::number(i + 1));
		emit log(LOGERR, QString("Difference-") + QString::number(i + 1));

	}
	emit log(LOGERR, "Difference Success");

	//if (polygon_difference(APolygon, BPolygon, result)) {
	//	log("Success to calculate Diffence!");

	//	for (auto i = 0; i < result.size(); i++)
	//	{
	//		auto mesh = toMesh(result[i]);
	//		addMesh(mesh, QString("Diffence-") + QString::number(i + 1));
	//		emit log(LOGERR, QString("Diffence-") + QString::number(i + 1));
	//	}
	//	emit log(LOGERR, "Diffence Success");
	//}
	//else {
	//	log("Fail to calculate Diffence!");
	//}
}

std::shared_ptr<PolyMesh> Boolean::toMesh(const polygon& poly)
{
	std::shared_ptr<PolyMesh> mesh = std::make_shared<PolyMesh>();

	std::vector<OpenMesh::SmartVertexHandle> v;
	for (auto it = poly.begin(); it != poly.end(); it++)
	{
		v.push_back(mesh->add_vertex(*it));
	}
	mesh->add_face(v);

	return mesh;
}

void Boolean::addMesh(std::shared_ptr<PolyMesh> polygon, const QString& name)
{
	int newObjectId = -1;
	emit addEmptyObject(DATA_POLY_MESH, newObjectId);
	meshes.push_back(newObjectId);

	PolyMeshObject* result = PluginFunctions::polyMeshObject(newObjectId);
	result->setName(name);
	result->mesh()->assign(*polygon, true);

	result->mesh()->request_vertex_status();
	result->mesh()->request_vertex_normals();
	result->mesh()->request_vertex_colors();
	result->mesh()->request_halfedge_status();
	result->mesh()->request_edge_status();
	result->mesh()->request_face_normals();
	result->mesh()->request_face_colors();
	result->mesh()->request_face_status();

	emit updatedObject(newObjectId, UPDATE_GEOMETRY);
}

void Boolean::load(const std::string& filePath, std::vector<polygon>& polygons)
{
	using namespace std;
	auto f = fstream(filePath, ios::in);

	if (!f.is_open())
		return;

	string line;
	istringstream linestream;

	polygon_ptr r = std::make_shared<polygon>();
	bool flag = false;
	while (getline(f, line))
	{
		//emit log(LOGERR, line.c_str());
		if (line == "#loop")
		{
			if (flag)
				polygons.push_back(*r);
			else
				flag = true;

			r = std::make_shared<polygon>();
			continue;
		}

		linestream = istringstream(line);
		double x, y;
		const double z = 0;
		linestream >> x >> y;

		r->emplace_back(OpenMesh::Vec3d(x, y, z));
	}
	polygons.push_back(*r);
	f.close();
	emit log(LOGERR, "Read Success");
}

void Boolean::polygonToSegments(const polygon& poly, std::vector<segment>& segments, bool reverse)
{
	for (auto i = 0; i < poly.size() - 1; i++) {
		auto p1 = poly[i];
		auto p2 = poly[(i + 1) % poly.size()];
		if (reverse)
			segments.emplace_back(segment({ p2, p1 }));
		else
			segments.emplace_back(segment({ p1, p2 }));
	}
}

void Bool::Boolean::segmentsToPolygon(std::vector<segment>& segments, polygon& poly)
{
	if (segments.empty())
		return;

	poly.push_back(segments[0][0]);
	poly.push_back(segments[0][1]);
	segments.erase(segments.begin());
	while (!segments.empty())
	{
		bool found = false;
		for (auto i = 0; i < segments.size(); i++)
		{
			if (isPointEqual(poly.back(), segments[i][0]))
			{
				poly.push_back(segments[i][1]);
				segments.erase(segments.begin() + i);
				found = true;
				break;
			}
		}

		if (!found)
		{
			return;
		}
	}

	poly.push_back(poly[0]);

}

void Bool::Boolean::getIntersectedSegments(const std::vector<segment>& s1, const std::vector<segment>& s2, std::vector<segment>& segments)
{
	for (auto& seg1 : s1)
	{
		std::vector<point> intersect_points;
		intersect_points.push_back(seg1[0]);
		for (auto& seg2 : s2)
		{
			if (isSegmentIntersect(seg1, seg2))
			{
				if (isSegmentOverlap(seg1, seg2))
				{
					intersect_points.push_back(seg2[0]);
					intersect_points.push_back(seg2[1]);
				}
				else
				{
					point intersect;
					getIntersect(seg1, seg2, intersect);
					intersect_points.push_back(intersect);
				}
			}

		}
		intersect_points.push_back(seg1[1]);
		if (seg1[0][0] < seg1[1][0])
			std::sort(intersect_points.begin(), intersect_points.end(), [](const point& p1, const point& p2) { return p1[0] < p2[0]; });
		else if (seg1[0][0] > seg1[1][0])
			std::sort(intersect_points.begin(), intersect_points.end(), [](const point& p1, const point& p2) { return p1[0] > p2[0]; });
		else
		{
			if (seg1[0][1] < seg1[1][1])
				std::sort(intersect_points.begin(), intersect_points.end(), [](const point& p1, const point& p2) { return p1[1] < p2[1]; });
			else if (seg1[0][1] > seg1[1][1])
				std::sort(intersect_points.begin(), intersect_points.end(), [](const point& p1, const point& p2) { return p1[1] > p2[1]; });
		}

		for (auto i = 0; i < intersect_points.size() - 1; i++)
			if (!isPointEqual(intersect_points[i], intersect_points[i + 1]))
				segments.emplace_back(segment({ intersect_points[i], intersect_points[i + 1] }));

	}

}

bool Boolean::isSegmentIntersect(const segment& seg1, const segment& seg2)
{
	if (
		(std::min(seg2[0][0], seg2[1][0]) - std::max(seg1[0][0], seg1[1][0]) > EPSILON) ||
		(std::min(seg1[0][0], seg1[1][0]) - std::max(seg2[0][0], seg2[1][0]) > EPSILON) ||
		(std::min(seg2[0][1], seg2[1][1]) - std::max(seg1[0][1], seg1[1][1]) > EPSILON) ||
		(std::min(seg1[0][1], seg1[1][1]) - std::max(seg2[0][1], seg2[1][1]) > EPSILON)
		) {
		return false;
	}

	auto p1 = seg1[0];
	auto p2 = seg1[1];
	auto q1 = seg2[0];
	auto q2 = seg2[1];


	auto p1q1 = q1 - p1;
	auto p2q2 = q2 - p2;
	auto p1q2 = q2 - p1;
	auto p2q1 = q1 - p2;

	auto q1p1 = -p1q1;
	auto q2p2 = -p2q2;
	auto q1p2 = -p2q1;
	auto q2p1 = -p1q2;

	auto cross1 = p1q1.cross(p1q2);
	auto cross2 = p2q1.cross(p2q2);
	auto cross3 = q1p1.cross(q1p2);
	auto cross4 = q2p1.cross(q2p2);

	if (cross1.dot(cross2) <= 0 && cross3.dot(cross4) <= 0)
		return true;
	return false;
}

void Boolean::getIntersect(const segment& seg1, const segment& seg2, point& intersect)
{
	auto p1 = seg1[0];
	auto p2 = seg1[1];
	auto q1 = seg2[0];
	auto q2 = seg2[1];

	//emit log(LOGERR, "Point1: " + QString::number(p1[0]) + " " + QString::number(p1[1]));
	//emit log(LOGERR, "Point2: " + QString::number(p2[0]) + " " + QString::number(p2[1]));
	//emit log(LOGERR, "Point3: " + QString::number(q1[0]) + " " + QString::number(q1[1]));
	//emit log(LOGERR, "Point4: " + QString::number(q2[0]) + " " + QString::number(q2[1]));

    double a1 = p2[1] - p1[1], b1 = p1[0] - p2[0];
    double c1 = p1[0] * p2[1] - p2[0] * p1[1];
    double a2 = q2[1] - q1[1], b2 = q1[0] - q2[0];
    double c2 = q1[0] * q2[1] - q2[0] * q1[1];
    double c = a1 * b2 - b1 * a2;
    if (std::abs(c) < EPSILON) {
		if(isPointOnSegment(seg2[0], seg1))
			intersect = seg2[0];
		else if(isPointOnSegment(seg2[1], seg1))
			intersect = seg2[1];
    }
    else {
		intersect[0] = (c1 * b2 - b1 * c2) / c;
		intersect[1] = (a1 * c2 - c1 * a2) / c;
		intersect[2] = 0.0;
    }

}

bool Boolean::isPointInPolygon(const point& p, const std::vector<segment>& segs, bool& on_edge, segment& edge_segment)
{
	double x_min = p[0];
	for (auto seg : segs) { if (seg[0][0]< x_min) { x_min = seg[0][0]; } }
	x_min -= 10.0;
	point tmp_p(x_min, p[1], 0.0);
	segment radial_seg{ tmp_p, p }; // É¨ÃèÏß

	int n_intersect = 0;
	
	for (auto& seg : segs) {
		if (isPointOnSegment(p, seg))
		{
			on_edge = true;
			edge_segment = seg;
			return false;
		}

		if (isSegmentIntersect(radial_seg, seg)) {
			if(!isSegmentOverlap(radial_seg, seg))
				n_intersect++;
		}
	}
	//emit log(LOGERR, +"Point: " + QString::number(p[0]) + " " + QString::number(p[1]));
	//emit log(LOGERR, +"Intersect: " + QString::number(n_intersect));

	if (n_intersect % 2 == 1) 
	{ 
		return true; 
	}

	return false;
}

//bool Boolean::isPointInPolygon(const point& p, const polygon& poly)
//{
//	bool isInside = false;
//	for (int i = 0, j = poly.size() - 1; i < poly.size(); j = i++) {
//		if ((poly[i][1] <= p[1] && poly[j][1] >= p[1])
//			|| (poly[i][1] >= p[1] && poly[j][1] <= p[1])) {
//			if ((p[0] - poly[i][0]) * (poly[j][1] - poly[i][1]) - (poly[j][0] - poly[i][0]) * (p[1] - poly[i][1]) < 0)
//			{
//				isInside =!isInside;
//			}
//		}
//	}
//	return isInside;
//}





//void Boolean::getPolygon(const std::vector<segment>& segments, polygon& poly)
//{
//	std::set<point> points;
//	for (auto& s : segments) {
//		points.insert(s[0]);
//		points.insert(s[1]);
//	}
//
//	double sumX = 0, sumY = 0;
//	for (const point& p : points) {
//		sumX += p[0];
//		sumY += p[1];
//		poly.push_back(p);
//	}
//	
//	point center(sumX / poly.size(), sumY / poly.size(), 0.0);
//
//	for (int i = 0; i < poly.size() - 1; i++) {
//		for (int j = 0; j < poly.size() - i - 1; j++) {
//			if (pointCW(poly[j], poly[j + 1], center)) {
//				point tmp = poly[j];
//				poly[j] = poly[j + 1];
//				poly[j + 1] = tmp;
//			}
//		}
//	}
//
//	for (auto it = poly.begin(); it!= poly.end(); it++)
//		emit log(LOGERR, QString::number((*it)[0]) + " " + QString::number((*it)[1]));
//
//	poly.push_back(poly[0]);
//}

//bool Boolean::pointCW(const point& a, const point& b, const point& center)
//{
//	int det = (a[0] - center[0]) * (b[1] - center[1]) - (b[0] - center[0]) * (a[1] - center[1]);
//	if (det < 0)
//		return true;
//	if (det > 0)
//		return false;
//	int d1 = (a[0] - center[0]) * (a[0] - center[0]) + (a[1] - center[1]) * (a[1] - center[1]);
//	int d2 = (b[0] - center[0]) * (b[0] - center[0]) + (b[1] - center[1]) * (b[1] - center[1]);
//	return d1 > d2;
//}

bool Bool::Boolean::isSegmentOverlap(const segment& s1, const segment& s2)
{
	if (isPointOnSegment(s2[0], s1) && isPointOnSegment(s2[1], s1))
		return true;
	return false;
}

bool Bool::Boolean::isPointEqual(const point& p1, const point& p2)
{
	return (p1 - p2).sqrnorm() < SQUARE_EPSILON;
}

bool Bool::Boolean::isPointOnSegment(const point& p, const segment& s)
{
	if (isPointEqual(p, s[0]) || isPointEqual(p, s[1]))
		return true;
	if((p - s[0]).cross(p - s[1]).sqrnorm() < SQUARE_EPSILON
		&& p[0] >= std::min(s[0][0], s[1][0]) && p[0] <= std::max(s[0][0], s[1][0])
		&& p[1] >= std::min(s[0][1], s[1][1]) && p[1] <= std::max(s[0][1], s[1][1]))
		return true;

	return false;
}

bool Bool::Boolean::isSegmentSameDir(const segment& s1, const segment& s2)
{
	if ((s1[1] - s1[0]).cross(s2[1] - s2[0]).sqrnorm() < SQUARE_EPSILON
		&& (s1[1] - s1[0]).dot(s2[1] - s2[0]) > SQUARE_EPSILON)
		return true;
	return false;
}

