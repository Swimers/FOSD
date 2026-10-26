#pragma once
#include "visualization/VisualizationTypes.h"
#include <vtkSmartPointer.h>
#include <vtkWeakPointer.h>
class vtkRenderer;
class vtkAxesActor;
class vtkActor;
class vtkCamera;
class vtkProp3D;
class vtkCallbackCommand;
class vtkObject;
namespace fosd {
namespace visualization {
// GUI-thread scene props and camera settings. Rendering is requested by Control.
class SceneEnvironment final {
public:
    SceneEnvironment();
    ~SceneEnvironment();
    void setRenderer(vtkRenderer *renderer);
    void setModelActor(vtkProp3D *model);
    bool setAxesSettings(const AxesSettings &settings, QString *error = nullptr);
    bool setGridSettings(const GridSettings &settings, QString *error = nullptr);
    bool setBackgroundSettings(const BackgroundSettings &settings, QString *error = nullptr);
    bool setWorldSettings(const WorldSettings &settings, QString *error = nullptr);
    bool setCameraSettings(const CameraSettings &settings, QString *error = nullptr);
    bool setCameraPose(const CameraPose &pose, QString *error = nullptr);
    
    AxesSettings axesSettings() const {
        return m_axesSettings;
    }
    GridSettings gridSettings() const {
        return m_gridSettings;
    }
    BackgroundSettings backgroundSettings() const {
        return m_backgroundSettings;
    }
    WorldSettings worldSettings() const {
        return m_worldSettings;
    }
    CameraSettings cameraSettings() const;
    CameraPose cameraPose() const;
    void showAxes(bool visible);
    void showGrid(bool visible);
    bool showGrid(GridPlane plane, bool visible, QString *error = nullptr);
    bool setBackground(double red, double green, double blue, QString *error = nullptr);
    bool setCameraPreset(CameraPreset preset, QString *error = nullptr);
    void setCameraLocked(bool locked);
    bool resetCamera();
    void updateClippingRange();
    void clear();

private:
    static void onRender(vtkObject *, unsigned long, void *clientData, void *);
    void detachProps();
    void syncProps();
    void configureAxes();
    bool modelBounds(double bounds[6]) const;
    vtkCamera *camera() const;
    vtkRenderer *m_renderer = nullptr; // Borrowed from Control.
    vtkWeakPointer<vtkProp3D> m_model;
    vtkSmartPointer<vtkAxesActor> m_axes;
    std::array<vtkSmartPointer<vtkActor>, 3> m_grids;
    vtkSmartPointer<vtkActor> m_ground, m_sky;
    vtkSmartPointer<vtkCallbackCommand> m_renderObserver;
    unsigned long m_observerTag = 0;
    AxesSettings m_axesSettings;
    GridSettings m_gridSettings;
    BackgroundSettings m_backgroundSettings;
    CameraSettings m_cameraSettings;
    WorldSettings m_worldSettings;
    CameraPose m_savedPose;
    bool m_hasSavedPose = false;
};
} // namespace visualization
} // namespace fosd
