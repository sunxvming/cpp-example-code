

#include <iostream>
#include <iomanip>

#include <osg/PositionAttitudeTransform>
#include <osg/Notify>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>

#include <osgViewer/CompositeViewer>
#include <osgEarth/GeoTransform>
#include <osgEarth/ModelLayer>

#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>

#include <osgEarth/MapNode>
#include <osgEarth/GDAL>
#include <osgEarth/ExampleResources>
#include <osgEarth/EarthManipulator>
#include <osgEarth/AutoScaleCallback>

#include <osgEarth/Style>
#include <osgEarth/OGRFeatureSource>
#include <osgEarth/FeatureModelLayer>
#include <osgEarth/FeatureImageLayer>
#include <osgEarth/MeasureTool>
#include <osgEarth/AutoClipPlaneHandler>
#include <osgEarth/DebugImageLayer>
#include <osgEarth/TerrainLayer>
#include <osgEarth/ElevationQuery>

#include <future>
#include <thread>
#include <chrono> 
#include "cppnet.h"


using namespace osgEarth;
using namespace osgEarth::Util;
using namespace cppnet;

const int offsetX = 100;
const int offsetY = 100;
const int windowWidth = 960;
const int windowHeight = 540;
const double camera_lon = 122.172289;
const double camera_lat =  37.653006;
const double camera_height =  5.37;

const double pitch_offset = -7;
const double heading_offset = -10; 
const double roll_offset = 3;


osgViewer::CompositeViewer viewer;
MapNode *mapNode;
osg::ref_ptr<Map> map;
osgViewer::View* view;
osg::ref_ptr<osg::GraphicsContext> gc;
EarthManipulator *manip;

class MyCameraUpdateCallback : public osgEarth::Util::EarthManipulator::UpdateCameraCallback
{
public:
    virtual void onUpdateCamera(const osg::Camera* camera)
    {
        // 在相机更新时执行自定义的操作
        osg::Vec3d cameraPosition = camera->getViewMatrix().getTrans();
        // std::cout << "Camera Position: " << cameraPosition.x() << ", " << cameraPosition.y() << ", " << cameraPosition.z() << std::endl;

        // get camera heading、pitch、range
        osg::Vec3d eye, center, up;
        camera->getViewMatrixAsLookAt(eye, center, up);
        osg::Vec3d lookVector = center - eye;
        double heading = atan2(lookVector.y(), lookVector.x());
        double pitch = atan2(lookVector.z(), sqrt(lookVector.x() * lookVector.x() + lookVector.y() * lookVector.y()));
        double range = lookVector.length();
        // std::cout << "Camera Heading: " << heading <<  ", Pitch: " << pitch << ", Range: " << range << std::endl;
    }
};


static Viewpoint VPs[] = {
    Viewpoint("Africa", 0.0, 0.0, 0.0, 0.0, -90.0, 10e6),
    Viewpoint("California", -121.0, 34.0, 0.0, 0.0, -90.0, 6e6),
    Viewpoint("Europe", 0.0, 45.0, 0.0, 0.0, -90.0, 4e6),
    Viewpoint("Washington DC", -77.0, 38.0, 0.0, 0.0, -90.0, 1e6),
    Viewpoint("Australia", 135.0, -20.0, 0.0, 0.0, -90.0, 2e6),
    Viewpoint("Boston", -71.096936, 42.332771, 0, 0.0, -90, 1e5)};

struct FlyToViewpointHandler : public osgGA::GUIEventHandler
{
    FlyToViewpointHandler(EarthManipulator *manip) : _manip(manip) {}

    bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
    {
        if (ea.getEventType() == ea.KEYDOWN && ea.getKey() >= '1' && ea.getKey() <= '6')
        {
            _manip->setViewpoint(VPs[ea.getKey() - '1'], 0.0);
            aa.requestRedraw();
        }
        return false;
    }

    osg::observer_ptr<EarthManipulator> _manip;
};


struct MouseIntersectionHandler : public osgGA::GUIEventHandler
{
    MouseIntersectionHandler( MapNode* mapNode) : _mapNode(mapNode) {}

    bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
    {
        if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH)
        {        
            double lon = 0, lat = 0, h = 0;
            osgViewer::View* view = static_cast<osgViewer::View*>(aa.asView());                
            getLocationAt(view, ea.getX(), ea.getY(), lon, lat, h);
            std::cout << "\t mouse picked screen x=" << ea.getX() << ", y=" << ea.getY() << std::endl;
            std::cout << std::fixed << std::setprecision(6) << "\t pick lon=" << lon << ", lat=" << lat << ", h=" << h << std::endl;
      
             

            osg::Vec3d eye, center, up;
            view->getCamera()->getViewMatrixAsLookAt(eye, center, up);


            // 获取地形图层
            osgEarth::TerrainLayer* terrainLayer = mapNode->getTerrain()->getLayerByName("elevation");
            if (terrainLayer)
            {
                // 创建地形查询对象
                osgEarth::ElevationQuery query(mapNode->getMapSRS(), terrainLayer);
                // 查询地形高度
                double horizonHeight = query.getHeight(eye.x(), eye.y(), true);
                // 计算地平线位置
                osg::Vec3d horizonPoint(eye.x(), eye.y(), horizonHeight);
                std::cout << "Horizon point (local coordinates): " << horizonPoint << std::endl;

                // 如果需要，将地平线位置转换为地理坐标
                osg::Vec3d horizonGeoPoint;
                mapNode->getMapSRS()->transformFromWorld(horizonPoint, horizonGeoPoint);
                std::cout << "Horizon point (geographic coordinates): " << horizonGeoPoint << std::endl;
            }
            else
            {
                std::cerr << "Elevation layer not found." << std::endl;
            }


            /*
            Viewpoint vp;
            vp.focalPoint() = GeoPoint(map->getSRS(),
                119.717, 40.041, 3.0,
                ALTMODE_ABSOLUTE);

            vp.heading()->set(0.0, Units::DEGREES);
            vp.pitch()->set(-0.0, Units::DEGREES);
            vp.range()->set(10.0, Units::METERS);
            manip->setViewpoint(vp);


        
            osg::Matrixd matirx = manip->getMatrix();


            static int  ii = 0;
            double d = 0.0;
            if (ii%2 == 0){
                d = 30.0;
            }else{
                d = -30.0;
            }
            ii++;
            osg::Matrixd rotation2;
            rotation2.makeRotate(osg::DegreesToRadians(0.0),osg::X_AXIS
                                ,osg::DegreesToRadians(d),osg::Z_AXIS
                                ,osg::DegreesToRadians(0.0),osg::Y_AXIS);  
                                      
            osg::Matrixd i = rotation2 * matirx;
            manip->setByMatrix(i);
            */

            auto vp = manip->getViewpoint();
            std::cout << "vp's info: " << std::endl;
            std::cout << "\t focalPoint: " << vp.focalPoint().value().x() << ", " << vp.focalPoint().value().y() << ", " << vp.focalPoint().value().z() << std::endl;
            std::cout << "\t heading: " << vp.heading()->as(Units::DEGREES) << std::endl;
            std::cout << "\t pitch: " << vp.pitch()->as(Units::DEGREES) << std::endl;
            std::cout << "\t range: " << vp.range()->as(Units::METERS) << std::endl;

 

        }
        return false;
    }

    bool getLocationAt(osgViewer::View* view, double x, double y, double &lon, double &lat, double &h)
    {
        osgUtil::LineSegmentIntersector::Intersections results;            
        if ( getMapNode() &&  view->computeIntersections( x, y, results, _intersectionMask ) )
        {
            // find the first hit under the mouse:
            osgUtil::LineSegmentIntersector::Intersection first = *(results.begin());
            osg::Vec3d point = first.getWorldIntersectPoint();

            osg::Vec3d lon_lat_h =
                getMapNode()->getMap()->getProfile()->getSRS()->getEllipsoid().geocentricToGeodetic(point);

            lat = lon_lat_h.y();
            lon = lon_lat_h.x();
            h = lon_lat_h.z();
            return true;
        }
        return false;
    }

    virtual MapNode* getMapNode() { return _mapNode.get(); }

    osg::observer_ptr<MapNode> _mapNode;
    unsigned int _intersectionMask = 0xffffffff;
};


bool getLocationAt(MapNode* mapNode, osgViewer::View* view, double x, double y, double &lon, double &lat, double &h)
{
    osg::ref_ptr<osgGA::GUIEventAdapter> ea = new osgGA::GUIEventAdapter;
    // ea->setEventType(osgGA::GUIEventAdapter::PUSH); // 指定事件类型为鼠标按下事件
    // ea->setButtonMask(osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON); // 设置鼠标按键
    // ea->setX(x); // 设置鼠标点击的 x 坐标
    // ea->setY(y); // 设置鼠标点击的 y 坐标
    ea->setWindowRectangle(offsetX, offsetY, windowWidth, windowHeight);
    ea->setMouseYOrientation(osgGA::GUIEventAdapter::Y_INCREASING_UPWARDS);
    ea->setGraphicsContext(gc.get());
    ea->setInputRange(0, 0, windowWidth, windowHeight);
    view->getEventQueue()->setCurrentEventState(ea);    
    
    osgUtil::LineSegmentIntersector::Intersections results;            
    if ( mapNode &&  view->computeIntersections( x, y, results ) )
    {
        // find the first hit under the mouse:
        auto it = results.begin(); 
        // std::advance(it, 1);
        osgUtil::LineSegmentIntersector::Intersection first = *(it);
        osg::Vec3d point = first.getWorldIntersectPoint();
        osg::Vec3d lon_lat_h =
            mapNode->getMap()->getProfile()->getSRS()->getEllipsoid().geocentricToGeodetic(point);

        lat = lon_lat_h.y();
        lon = lon_lat_h.x();
        h = lon_lat_h.z();
        return true;
    }

    return false;
}



void add_mark(double lon, double lat)
{

    // printf("add_mark lon:%f lat:%f\n", lon, lat);
    osg::ref_ptr<osg::Node> model = osgDB::readRefNodeFile("./data/red_flag.osg.osgearth_shadergen");
    if (model.valid())
    {
        osg::PositionAttitudeTransform* pat = new osg::PositionAttitudeTransform();
        pat->addCullCallback(new AutoScaleCallback<osg::PositionAttitudeTransform>(100));
        pat->addChild(model.get());

        GeoTransform* xform = new GeoTransform();
        xform->setPosition(GeoPoint(SpatialReference::get("wgs84"), lon, lat));
        xform->addChild(pat);

        ModelLayer* layer = new ModelLayer();
        layer->setNode(xform);
        map->addLayer(layer);
    }
}


void osg_loop()
{
    osgEarth::initialize();

    map = new Map();
    GDALImageLayer *basemap = new GDALImageLayer();
    basemap->setURL("./data/world.tif");
    map->addLayer(basemap);
    osgEarth::DebugImageLayer* debugLayer = new osgEarth::DebugImageLayer();
    map->addLayer(debugLayer);
    
    OGRFeatureSource *features = new OGRFeatureSource();
    features->setName("vector-data");

    Ring *line = new Ring();    
    line->push_back(osg::Vec3d(119.717, 40.041, 0.0));
    line->push_back(osg::Vec3d(119.717, 40.041, 2.0));
    line->push_back(osg::Vec3d(119.717, 40.041, 3.0));

    features->setGeometry(line);
    map->addLayer(features);

    Style style;
    LineSymbol *ls = style.getOrCreateSymbol<LineSymbol>();
    ls->stroke()->color() = Color::Yellow;
    ls->stroke()->width() = 10.0f;
    ls->tessellationSize()->set(100, Units::KILOMETERS);
    FeatureModelLayer* layer = new FeatureModelLayer();
    layer->setFeatureSource(features);
    StyleSheet* styleSheet = new StyleSheet();
    styleSheet->addStyle(style);
    layer->setStyleSheet(styleSheet);
    map->addLayer(layer);


    LayerVector layers;
    map->getLayers(layers);
    for (LayerVector::const_iterator i = layers.begin(); i != layers.end(); ++i)
    {
        Layer *layer = i->get();
        if (layer->getOpenAutomatically() && !layer->isOpen())
        {
            OE_WARN << layer->getName() << " : " << layer->getStatus().toString() << std::endl;
        }
    }

    osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
    if (!wsi)
    {
        osg::notify(osg::NOTICE)<<"Error, no WindowSystemInterface available, cannot create windows."<<std::endl;
        return;
    }

    unsigned int width, height;
    osg::GraphicsContext::ScreenIdentifier main_screen_id;
    main_screen_id.readDISPLAY();
    main_screen_id.setUndefinedScreenDetailsToDefaultScreen();
    wsi->getScreenResolution(main_screen_id, width, height);

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = offsetX;
    traits->y = offsetY;
    traits->width = windowWidth;
    traits->height = windowHeight;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->sharedContext = 0;
    traits->readDISPLAY();
    traits->setUndefinedScreenDetailsToDefaultScreen();

    gc = osg::GraphicsContext::createGraphicsContext(traits.get());
    if (gc.valid())
    {
        osg::notify(osg::INFO)<<"  GraphicsWindow has been created successfully."<<std::endl;

        // need to ensure that the window is cleared make sure that the complete window is set the correct colour
        // rather than just the parts of the window that are under the camera's viewports
        gc->setClearColor(osg::Vec4f(0.2f,0.2f,0.6f,1.0f));
        gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else
    {
        osg::notify(osg::NOTICE)<<"  GraphicsWindow has not been created successfully."<<std::endl;
    }


    // ====== 48帧位置 ==========
    // 设置摄像机的初始位置
    manip = new EarthManipulator();
    Viewpoint vp;
    vp.focalPoint() = GeoPoint(map->getSRS(),
        camera_lon,camera_lat,camera_height,
        ALTMODE_ABSOLUTE);
    vp.heading()->set(43.08 + heading_offset, Units::DEGREES);
    vp.pitch()->set(1.29 + pitch_offset, Units::DEGREES);
    vp.range()->set(0.0, Units::METERS);
    manip->setHomeViewpoint(vp);

    // 摄像机的位置
    add_mark(camera_lon, camera_lat);

    // 两条船的位置 
    add_mark(122.174,37.6705);  // ship1
    add_mark(122.179,37.6589);  // ship2


    mapNode = new MapNode(map.get());
    view = new osgViewer::View;
    view->setName("View one");
    viewer.addView(view);

    view->setSceneData(mapNode);
    view->getCamera()->setName("Cam one");
    view->getCamera()->setViewport(new osg::Viewport(0,0, windowWidth, windowHeight));
    view->getCamera()->setGraphicsContext(gc.get());
    view->setCameraManipulator(manip);

    view->getCamera()->addCullCallback(new osgEarth::Util::AutoClipPlaneCullCallback(mapNode));
    view->getCamera()->setSmallFeatureCullingPixelSize(-1.0f);


    double fovy, ar, n, f;
    view->getCamera()->getProjectionMatrixAsPerspective(fovy, ar, n, f);
    std::cout << "before fovy: " << fovy << " ar: " << ar << " n: " << n << " f: " << f << std::endl;
    view->getCamera()->setProjectionMatrixAsPerspective(47.5, 88/47.5, n, 50*1000);
    view->getCamera()->getProjectionMatrixAsPerspective(fovy, ar, n, f);
    std::cout << "after fovy: " << fovy << " ar: " << ar << " n: " << n << " f: " << f << std::endl;


    // add some stock OSG handlers:
    MapNodeHelper().configureView(view);

    view->addEventHandler(new FlyToViewpointHandler(manip));
    // view->addEventHandler(new osgEarth::Contrib::MeasureToolHandler(mapNode));
    view->addEventHandler(new MouseIntersectionHandler(mapNode));

    MyCameraUpdateCallback* callback = new MyCameraUpdateCallback;
    manip->setUpdateCameraCallback(callback);

    viewer.realize();
    while (!viewer.done())
    {
        // viewer.frame();
        viewer.advance();
        viewer.eventTraversal();
        viewer.updateTraversal();
        viewer.renderingTraversals();
        // std::this_thread::sleep_for(std::chrono::seconds(1));

        
        auto i = viewer.getFrameStamp()->getFrameNumber(); 
        // if( i % 100 == 0){
        if(0){
            auto t1 = viewer.getFrameStamp()->getReferenceTime();
            // auto t2 = viewer.getFrameStamp()->getSimulationTime();
            std::cout << "Frame number: " << i  << " time:" << t1 << std::endl;
            double lon, lat, h;           
            double x = 100; 
            double y = 200;  
            getLocationAt(mapNode, view, x, y, lon, lat, h);
            std::cout << "\t function picked screen x=" << x << ", y=" << y << std::endl;
            std::cout << std::fixed << std::setprecision(6) << "\t function pick lon=" << lon << ", lat=" << lat << ", h=" << h << std::endl;
            
            // osg::Vec3d cameraPosition = manip->getMatrix().getTrans();
            // std::cout << "cameraPosition: " << cameraPosition.x() << ", " << cameraPosition.y() << ", " << cameraPosition.z() << std::endl;

            // 模拟鼠标按下事件
            // osg::ref_ptr<osgGA::EventQueue> eventQueue = view->getEventQueue();
            // eventQueue->mouseButtonPress(x, y, osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON); 
        }
    }
}

std::vector<std::string> splitString(const std::string &input, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}
std::string trim(const std::string &str) {
    size_t start = 0;
    size_t end = str.length();

    while (start < end && std::isspace(str[start])) {
        ++start;
    }

    while (end > start && std::isspace(str[end - 1])) {
        --end;
    }

    return str.substr(start, end - start);
}


static const int __buf_len = 2048;
static const char* __buf_spilt = "\n";


struct pos{
    int id;
    double x;
    double y;
};

void ReadFunc(Handle handle, cppnet::BufferPtr data, uint32_t len) {
    char msg_buf[__buf_len] = {0};
    uint32_t need_len = 0;
    uint32_t find_len = (uint32_t)strlen(__buf_spilt);
    // get recv data to send back.
    uint32_t size = data->ReadUntil(msg_buf, __buf_len, __buf_spilt, find_len, need_len);
    std::string msg(msg_buf, size - 1);
    // std::cout << "recv msg : " << msg << std::endl;
     char delimiter = ',';
    std::vector<std::string> result = splitString(msg, delimiter);
    
    double lon = std::stod(result[1]);
    double lat = std::stod(result[2]);
    double h = std::stod(result[3]);
    double heading = std::stod(result[4]);
    double pitch = std::stod(result[5]);
    double roll = std::stod(result[6]);
    double hfov = std::stod(result[7]);
    double vfov = std::stod(result[8]);
    int pos_num = std::stoi(result[9]);

    std::vector<pos> pos_list;
    for(int i = 0; i < pos_num; i++){
        pos p;
        p.id = std::stoi(result[10 + i * 3]);
        double x = std::stod(result[10 + i * 3 + 1]);
        double y = std::stod(result[10 + i * 3 + 2]);
        p.x = windowWidth * x;
        p.y = windowHeight - windowHeight * y;
        pos_list.push_back(p);
    }


    // std::cout << "lon:" << lon << " lat:" << lat << " h:" << h  
    //     << " heading:" << heading << " pitch:" << pitch << " roll:" << roll 
    //     << " hfov:" << hfov << " vfov:" << vfov <<  std::endl;
    std::cout << "pos num:" << pos_num << std::endl;
    for(const auto &p : pos_list){
        std::cout << "\tid:" << p.id << " x:" << p.x << " y:" << p.y << std::endl;
    }

    {
        double range = 0;
        Viewpoint vp;
        vp.focalPoint() = GeoPoint(map->getSRS(),
            lon, lat, h,
            ALTMODE_ABSOLUTE);
        vp.heading()->set(heading + heading_offset, Units::DEGREES);
        vp.pitch()->set(pitch + pitch_offset, Units::DEGREES);
        vp.range()->set(range, Units::METERS);
        manip->setViewpoint(vp);

        osg::Matrixd matirx = manip->getMatrix();
        osg::Matrixd rotation;
        rotation.makeRotate(osg::DegreesToRadians(0.0),osg::X_AXIS
                            ,osg::DegreesToRadians(roll + roll_offset),osg::Z_AXIS
                            ,osg::DegreesToRadians(0.0),osg::Y_AXIS);  
                                    
        osg::Matrixd i = rotation * matirx;
        manip->setByMatrix(i);

        viewer.advance();
        viewer.eventTraversal();
        viewer.updateTraversal();
        viewer.renderingTraversals();

        std::vector<pos> res_list;
        for(const auto &p : pos_list){
            double lon = 0;
            double lat = 0;
            double h = 0;           
            getLocationAt(mapNode, view, p.x, p.y, lon, lat, h);
            pos res;
            res.id = p.id;
            if(std::abs(h) > 1)
            {
                res.x = 0;
                res.y = 0;
            }else{
                res.x = lon;
                res.y = lat;
            }
            res_list.push_back(res);
        }

        std::cout << "real pos:" << std::endl;
        for(const auto &p : res_list){
            std::cout << "\tid:" << p.id << " x:" << p.x << " y:" << p.y << std::endl;
        }

        std::string res = std::to_string(res_list.size());
        for(const auto &p : res_list){
            res += "," + std::to_string(p.id) + "," + std::to_string(p.x) + "," + std::to_string(p.y);
        }
        res += "\n";
        handle->Write(res.c_str(), res.length());
    }
}

void ConnectFunc(Handle handle, uint32_t error) {
    static std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);
    std::cout << "has connect : " << handle->GetSocket() << std::endl;
    if (error == CEC_CLOSED) {
        std::cout << "remote closed connect : " << handle->GetSocket() << std::endl;
    } else if (error != CEC_SUCCESS) {
        std::cout << "something err while connect : " << error << std::endl;
    }
}


int main(int argc, char **argv)
{
    auto task = [&]() 
    {
        cppnet::CppNet net;
        net.Init(1);
        net.SetAcceptCallback(ConnectFunc);
        net.SetReadCallback(ReadFunc);
        net.SetDisconnectionCallback(ConnectFunc);
        net.ListenAndAccept("0.0.0.0", 8090);
        net.Join();
    };
    auto f = std::async(task); 

    osg_loop();

    return 0;
}
