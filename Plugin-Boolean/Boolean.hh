#pragma once
#ifndef BOOLEANPLUGIN_HH
#define BOOLEANPLUGIN_HH
#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/common/Types.hh>

#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>

#include <memory>
#include <tuple>
#include <vector>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QSpinBox>
#include <array>


//struct Poly_2D {
//	std::vector<QVector2D> out_points;
//	std::vector<QVector2D> in_points;
//};

namespace Bool {
	using point = typename OpenMesh::Vec3d;
	using polygon = typename std::vector<OpenMesh::Vec3d>;
	using polygon_ptr = typename std::shared_ptr<polygon>;
	using segment = typename std::array<OpenMesh::Vec3d, 2>;

	constexpr double EPSILON = 1e-6;
	constexpr double SQUARE_EPSILON = 1e-12;
	constexpr double BLEND_RATIO = 0.5;

	class Boolean : public QObject, BaseInterface, ToolboxInterface, LoggingInterface, LoadSaveInterface
	{
		Q_OBJECT
			Q_INTERFACES(BaseInterface)
			Q_INTERFACES(ToolboxInterface)
			Q_INTERFACES(LoggingInterface)
			Q_INTERFACES(LoadSaveInterface)
			Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Boolean")

	signals:
		void updateView();
		void updatedObject(int _identifier, const UpdateType& _type);

		//LoggingInterface
		void log(Logtype _type, QString _message);
		void log(QString _message);
		// ToolboxInterface
		void addToolbox(QString _name, QWidget* _widget);

		void addEmptyObject(DataType _type, int& _id);
	public:

		~Boolean() {};

		QString name() { return QString("Boolean"); };

		QString description() { return QString("Does actually nothing but works!"); };


	private:
		//Poly_2D object1;
		//Poly_2D object2;
		//Poly_2D result;

		//int poly1_id, poly2_id, result_id;

		std::vector<polygon> APolygon;
		std::vector<polygon> BPolygon;
		std::vector<int> meshes;

		std::shared_ptr<PolyMesh> toMesh(const polygon& poly);
		void addMesh(std::shared_ptr<PolyMesh> polygon, const QString& name);
		void load(const std::string& filePath, std::vector<polygon>& polygons);

	private:
		void polygonToSegments(const polygon& poly, std::vector<segment>& segments, bool reverse = false);
		void segmentsToPolygon(std::vector<segment>& segments, polygon& poly);
		void getIntersectedSegments(const std::vector<segment>& s1, const std::vector<segment>& s2, std::vector<segment>& segments);
		bool isSegmentIntersect(const segment& s1, const segment& s2);
		void getIntersect(const segment& seg1, const segment& seg2, point& intersect);
		bool isPointInPolygon(const point& p, const std::vector<segment>& segs, bool& on_edge, segment& edge_segment);
		void getPolygon(const std::vector<segment>& segments, polygon& poly);
		bool pointCW(const point& a, const point& b, const point& center);

		bool isSegmentOverlap(const segment& s1, const segment& s2);
		bool isPointEqual(const point& p1, const point& p2);
		bool isPointOnSegment(const point& p, const segment& s);
		bool isSegmentSameDir(const segment& s1, const segment& s2);

	private slots:

		void initializePlugin();
		void ReadPoly1();
		void ReadPoly2();
		void Clear();
		void Union();
		void Intersection();
		void Difference();

	public slots:
		QString version() { return QString("1.0"); };
	};
}


#endif