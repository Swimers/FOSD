#include "disp3dmodel.h"

#include <QFile>
#include <QDebug>

#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>                  //照相机
#include <vtkAxesActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextActor.h>               //坐标系相关
#include <vtkLightCollection.h>
#include <vtkLight.h>                   //灯光相关
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkRendererCollection.h>
#include <vtkCollectionIterator.h>
#include <vtkLine.h>
#include <vtkTriangle.h>
#include <vtkQuad.h>
#include <vtkHexahedron.h>
#include <vtkHexagonalPrism.h>          //六棱柱
#include <vtkWedge.h>
#include <vtkSphereSource.h>
#include <vtkLookupTable.h>
#include <vtkRenderer.h>

#include <vtkSTLReader.h>

#include <disp3dpointpicker.h>
#include <disp3dinteractor.h>



disp3dmodel::disp3dmodel()
{
    renderWindow = vtkRenderWindow::New();
    renderer = vtkRenderer::New();
    renderWindow->AddRenderer(renderer);
<<<<<<< Updated upstream
    iren = vtkRenderWindowInteractor::New();
    iren->SetRenderWindow(renderWindow);
    iren->Initialize();
    disp3dinteractor* style = new disp3dinteractor();
    style->SetDefaultRenderer(renderer);
    style->GetDisplayModelPtr(this);
    iren->SetInteractorStyle(style);
=======

    iren = vtkRenderWindowInteractor::New();            //创建及交互器
    iren->SetRenderWindow(renderWindow);                //交互器添加窗口

    disp3dinteractor* style = disp3dinteractor::New();//创建交互器对象
    style->GetDisplayModelPtr(this);
    iren->SetInteractorStyle(style);                    //设置交互器类型
    style->SetDefaultRenderer(renderer);                //设置交互器的默认渲染器类型
>>>>>>> Stashed changes

    renderWindow->Render();
}

disp3dmodel::~disp3dmodel()
{
<<<<<<< Updated upstream
    ClearKeyPoint();           // 内部会把 keyPointEntity / keyPointText 从 renderer 移除并 Delete
                               // 并清空 SelectedPointMap
    rawPointMap.clear();
    pointDisplace.clear();
    pointpos.clear();
    keyPointSet.clear();
    SelectedPointMap.clear();

    if (renderer && modelActor) {
        renderer->RemoveActor(modelActor);
    }
    if (iren) {
        auto style = dynamic_cast<disp3dinteractor*>(iren->GetInteractorStyle());
        if (style) {
            iren->SetInteractorStyle(nullptr);
            delete style;
        }
        iren->Delete();
        iren = nullptr;
    }
    if (renderer) {
        if (renderWindow) {
            renderWindow->RemoveRenderer(renderer);
        }
        renderer->Delete();
        renderer = nullptr;
    }
=======
    if(this->points)            this->points->Delete();
    if(this->S_M_points)        this->S_M_points->Delete();
    if(this->cellarray)         this->cellarray->Delete();
    if(this->S_M_cellarray)     this->S_M_cellarray->Delete();
    if(this->cubeGrid)          this->cubeGrid->Delete();
    if(this->S_M_cubeGrid)      this->S_M_cubeGrid->Delete();
    if(this->GridMapper)        this->GridMapper->Delete();
    if(this->GridActor)         this->GridActor->Delete();
    if(this->axe)               this->axe->Delete();
    if(this->scalars_strain)    this->scalars_strain->Delete();
    if(this->scalars_temp)      this->scalars_temp->Delete();
    if(this->scalars_dispx)     this->scalars_dispx->Delete();
    if(this->scalars_dispy)     this->scalars_dispy->Delete();
    if(this->scalars_dispz)     this->scalars_dispz->Delete();

    //释放点的信息集合
    for(QMap<int,node*>::iterator it=nodes.begin();it!=nodes.end();)
    {
        delete (it.value())->cord;
        delete it.value();
        nodes.erase(it++);
    }
    //释放壳(三变形和四边形)单元的信息集合
    for(QMap<int,shell*>::iterator it=shells.begin();it!=shells.end();)
    {
        delete (it.value())->NodeNo;
        delete it.value();
        shells.erase(it++);
    }
    //释放梁单元信息集合
    for(QMap<int,beam*>::iterator it=beams.begin();it!=beams.end();)
    {
        delete (it.value())->NodeNo;
        delete it.value();
        beams.erase(it++);
    }
    //释放截面信息集合
    for(QMap<int,sectionInfo*>::iterator it=sectionInfos.begin();it!=sectionInfos.end();)
    {
        delete (it.value())->sectionGEo;
        delete it.value();
        sectionInfos.erase(it++);
    }
    //释放线面模型标记点到节点的序列关系
    for(QMap<int,int>::iterator it=LineModeFlagpointToNodePoint.begin();it!=LineModeFlagpointToNodePoint.end();)
    {
        LineModeFlagpointToNodePoint.erase(it++);
    }
    //释放实体模型标记点到节点的序列关系
    for(QMap<int,int>::iterator it=SolidModeFlagPointToNodePoint.begin();it!=SolidModeFlagPointToNodePoint.end();)
    {
        SolidModeFlagPointToNodePoint.erase(it++);
    }
    //释放实体模型节点到标记点的序列关系
    for(QMap<int,int>::iterator it=SolidModeNodePointToFlagPoint.begin();it!=SolidModeNodePointToFlagPoint.end();)
    {
        SolidModeNodePointToFlagPoint.erase(it++);
    }
    //释放所有逆有限单元中的差值点
    for(QMap<int,ReversibleElementPointSet*>::iterator it=ReversibleElementDvaluepointSet.begin();it!=ReversibleElementDvaluepointSet.end();)
    {
        delete (it.value())->Dvaluepoint;
        delete it.value();
        ReversibleElementDvaluepointSet.erase(it++);
    }
    //释放定义可逆单元坐标系的集合
    for(QMap<int,ReversibleElement*>::iterator it=ReversibleElementSet.begin();it!=ReversibleElementSet.end();)
    {
        delete (it.value())->ipoint;
        delete (it.value())->jpoint;
        delete (it.value())->kpoint;
        delete it.value();
        ReversibleElementSet.erase(it++);
    }
    //释放
    for(QMap<int,shell_len*>::iterator it=ReversibleLen.begin();it!=ReversibleLen.end();)
    {
        delete it.value();
        ReversibleLen.erase(it++);
    }
    //释放定义传感器数据与可逆单元特定位置应变数据的关联集合
    for(QMap<int,ChannalEle*>::iterator it=SensorANDInserverRelationSet.begin();it!=SensorANDInserverRelationSet.end();)
    {
        delete it.value();
        SensorANDInserverRelationSet.erase(it++);
    }
    //释放应变集合
    for(QMap<int,QVector<double>*>::iterator it=strainset.begin();it!=strainset.end();)
    {
        delete it.value();
        strainset.erase(it++);
    }
    for (QMap< int, surface* >::iterator it = new_element_info1.begin(); it != new_element_info1.end();) //释放单元集合
    {
        delete (*(it.value())).leftface;
        delete (*(it.value())).rightface;
        delete it.value();
        new_element_info1.erase(it++);
    }
    for (QMap< int, QVector<int>* >::iterator it = new_node_ex_node_info.begin(); it != new_node_ex_node_info.end();) //释放线面模型节点和实体模型点之间关系集合
    {
        delete it.value();
        new_node_ex_node_info.erase(it++);
    }
    for (QMap< int, struct nodeinfo*>::iterator it = new_node_info1.begin(); it != new_node_info1.end();)//释放点的位移数据
    {
        delete it.value();
        new_node_info1.erase(it++);
    }
    for (QMap< int, nodedisplace*>::iterator it = PointDisplaceSet.begin(); it != PointDisplaceSet.end();)//释放点的位移数据
    {
        delete it.value();
        PointDisplaceSet.erase(it++);
    }

    // 释放 TextActorLink 列表中的内存
    for (int i = 0; i < TextActorLink.size(); ++i) {
        delete TextActorLink[i];  // 删除对象
    }
    TextActorLink.clear();  // 清空列表

    // 释放 DisplayTextNodeFlagLink 列表中的内存
    for (int i = 0; i < DisplayTextNodeFlagLink.size(); ++i) {
        delete DisplayTextNodeFlagLink[i];  // 删除对象
    }
    DisplayTextNodeFlagLink.clear();  // 清空列表
    for (int i = 0; i < DisplayCord.size(); ++i) {
        delete DisplayCord[i];  // 删除对象
    }
    DisplayCord.clear();  // 清空列表
    for (int i = 0; i < CurrentDisplayCord.size(); ++i) {
        delete CurrentDisplayCord[i];  // 删除对象
    }
    CurrentDisplayCord.clear();  // 清空列表

    if(this->S_M_Type) delete[] S_M_Type;
    if(this->Type) delete[] Type;
>>>>>>> Stashed changes
}

void disp3dmodel::Disp3DModelDemo()
{
    vtkSmartPointer<vtkSTLReader> reader = vtkSTLReader::New();
    reader->SetFileName("C:\\Users\\Swimers\\Desktop\\Model1.stl");
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

<<<<<<< Updated upstream
void disp3dmodel::PickerClear()
{
    disp3dinteractor * style1 = dynamic_cast<disp3dinteractor *>(iren->GetInteractorStyle());
    style1->ClearPicker();
}

void disp3dmodel::LoadModel(const QString &filePath)
{
    modelScalars.Init();
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    std::string localPath = codec->fromUnicode(filePath).data();    //中文路径的兼容性
    reader->SetFileName(localPath.c_str());
    reader->Update();
    rawPolyData = reader->GetOutput();
    dynPolyData = vtkSmartPointer<vtkPolyData>::New();
    dynPolyData->DeepCopy(rawPolyData);
    mapper->SetInputData(dynPolyData);

    InitScalarsValues(modelScalars, dynPolyData);

    modelActor->SetMapper(mapper);
    modelActor->GetProperty()->SetEdgeVisibility(0);
    modelActor->GetProperty()->SetLineWidth(static_cast<float>(0.02));
    modelActor->GetProperty()->SetEdgeColor(0, 0.88, 0.88);
    modelActor->GetProperty()->SetRepresentationToSurface();
    modelActor->GetProperty()->SetColor(0.16, 0.582, 0.973);

    renderer->AddActor(modelActor);
=======
    mapper->SetInputConnection(reader->GetOutputPort());
    actor->SetMapper(mapper);
    renderer->AddActor(actor);
>>>>>>> Stashed changes
    renderer->ResetCamera();
    renderWindow->AddRenderer(renderer);
}

bool disp3dmodel::SetVtkRenderBackColor(double r, double g, double b)
{
<<<<<<< Updated upstream
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    std::string localPath = codec->fromUnicode(filePath).data();    //中文路径的兼容性
    vtkSmartPointer<vtkSTLWriter> writer = vtkSmartPointer<vtkSTLWriter>::New();
    writer->SetFileName(localPath.c_str());
    writer->SetInputData(rawPolyData);
    writer->Write();
    qDebug() << "模型成功导出到：" << filePath;
}

void disp3dmodel::UnLoadModel()
{
    if (Flag_Actor){
        PickerClear();
        renderer->RemoveActor(modelActor);
    }

    rawPointMap.clear();
    pointDisplace.clear();
    keyPointSet.clear();
    ClearKeyPoint();

    Flag_Actor = false;
}

void disp3dmodel::MergeModel(const QString &filePath)
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    std::string localPath = codec->fromUnicode(filePath).data();    //中文路径的兼容性
    vtkSmartPointer<vtkSTLReader> newReader = vtkSmartPointer<vtkSTLReader>::New();
    newReader->SetFileName(localPath.c_str());
    newReader->Update();
    vtkSmartPointer<vtkPolyData> newPolyData = newReader->GetOutput();
    vtkSmartPointer<vtkAppendPolyData> appender = vtkSmartPointer<vtkAppendPolyData>::New();
    appender->AddInputData(rawPolyData);
    appender->AddInputData(newPolyData);
    appender->Update();
    vtkSmartPointer<vtkCleanPolyData> cleaner = vtkSmartPointer<vtkCleanPolyData>::New();
    cleaner->SetInputConnection(appender->GetOutputPort());
    cleaner->Update();
    rawPolyData = cleaner->GetOutput();
    mapper->SetInputData(rawPolyData);
    renderer->ResetCamera();
    renderWindow->Render();

//    qDebug() << "合并新模型：";
//    qDebug() << "点数: " << newPolyData->GetNumberOfPoints()
//              << " 面数: " << newPolyData->GetNumberOfPolys();
//    qDebug() << "模型合并完成：";
//    qDebug() << "合并后点数: " << rawPolyData->GetNumberOfPoints()
    //              << " 面数: " << rawPolyData->GetNumberOfPolys();
}

void disp3dmodel::ResetModel()
{
    if (!Flag_Actor || !dynPolyData || !rawPolyData) {
        return;
    }

    vtkPoints* points = dynPolyData->GetPoints();
    if (!points) {
        return;
    }

    vtkIdType numPoints = points->GetNumberOfPoints();

    pointDisplace.clear();
    pointpos.clear();

    QMap<vtkIdType, QVector<double>>::const_iterator it;
    for (it = rawPointMap.constBegin(); it != rawPointMap.constEnd(); ++it) {
        vtkIdType id = it.key();
        const QVector<double> &coords = it.value();

        if (id >= 0 && id < numPoints && coords.size() >= 3) {
            points->SetPoint(id, coords[0], coords[1], coords[2]);
        }

        QVector<double> zero(3);
        zero[0] = 0.0;
        zero[1] = 0.0;
        zero[2] = 0.0;
        pointDisplace.insert(id, zero);

        QVector<double> pos(3);
        pos[0] = coords[0];
        pos[1] = coords[1];
        pos[2] = coords[2];
        pointpos.insert(id, pos);

        modelScalars.dispX->SetValue(id, 0);
        modelScalars.dispY->SetValue(id, 0);
        modelScalars.dispZ->SetValue(id, 0);
        modelScalars.temp->SetValue(id, 0.01);
        modelScalars.strain->SetValue(id, 0.01);
    }
    MaxDisplace = 0.0;

    dynPolyData->Modified();
    renderWindow->Render();
}


void disp3dmodel::InitScalarsValues(ModelScalars &scalars, vtkPolyData *poly)
{
    vtkIdType nPoints = poly->GetNumberOfPoints();
    scalars.dispX->SetNumberOfValues(0);
    scalars.dispY->SetNumberOfValues(0);
    scalars.dispZ->SetNumberOfValues(0);
    scalars.strain->SetNumberOfValues(0);
    scalars.temp->SetNumberOfValues(0);
    for (vtkIdType i = 0; i < nPoints; i++){
        scalars.dispX->InsertNextValue(0);
        scalars.dispY->InsertNextValue(0);
        scalars.dispZ->InsertNextValue(0);
        scalars.strain->InsertNextValue(0.1);
        scalars.temp->InsertNextValue(0.1);
    }
    vtkPointData* pd = poly->GetPointData();
    pd->AddArray(scalars.dispX);
    pd->AddArray(scalars.dispY);
    pd->AddArray(scalars.dispZ);
    pd->AddArray(scalars.strain);
    pd->AddArray(scalars.temp);

    vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfColors(256);
    lut->SetHueRange(0.6, 0.1);
    lut->Build();
    mapper->SetLookupTable(lut);
    mapper->SelectColorArray("Displacement_X");
    mapper->ScalarVisibilityOff();
    mapper->SetScalarRange(0,1);
    renderWindow->Render();
}

void disp3dmodel::SetupColorMapping(const std::string &scalarName)
{
    mapper->SetScalarRange(0,1);
    dynPolyData->GetPointData()->SetActiveScalars(scalarName.c_str());
    mapper->ScalarVisibilityOn();
    renderWindow->Render();
}

void disp3dmodel::OFFScalars()
{
    mapper->ScalarVisibilityOff();
    modelActor->GetProperty()->SetEdgeVisibility(0);
    modelActor->GetProperty()->SetLineWidth(static_cast<float>(0.02));
    modelActor->GetProperty()->SetEdgeColor(0, 0.88, 0.88);
    modelActor->GetProperty()->SetRepresentationToSurface();
    modelActor->GetProperty()->SetColor(0.16, 0.582, 0.973);
    renderWindow->Render();
}

void disp3dmodel::ReadPointQMap(vtkSmartPointer<vtkPolyData> PolyData)
{
    vtkSmartPointer<vtkPoints> points = PolyData->GetPoints();
    vtkIdType numPoints = points->GetNumberOfPoints();
    for (vtkIdType id= 0; id<numPoints; ++id) {
        double coords[3];
        points->GetPoint(id, coords);
        QVector<double> pointCoords = {0,0,0};
        pointDisplace.insert(id, pointCoords);
        pointCoords[0] = coords[0];  // x坐标
        pointCoords[1] = coords[1];  // y坐标
        pointCoords[2] = coords[2];  // z坐标
        //qDebug() << id << coords[0] << coords[1] << coords[2];
        rawPointMap.insert(id, pointCoords);
        pointpos.insert(id, pointCoords);
    }
}

void disp3dmodel::FreshKeyPoint()
{
    if (SelectedPointMap.isEmpty()) {
        return;
    }

    QMap<vtkIdType, SelectedPointEntity>::const_iterator it;
    for (it = SelectedPointMap.constBegin(); it != SelectedPointMap.constEnd(); ++it) {
        const SelectedPointEntity &ent = it.value();
        if (!ent.keyPointEntity || !ent.keyPointText) {
            continue;
        }
        double worldPos[3];
        ent.keyPointEntity->GetPosition(worldPos);
        renderer->SetWorldPoint(worldPos[0], worldPos[1], worldPos[2], 1.0);
        renderer->WorldToDisplay();
        double displayPos[3];
        renderer->GetDisplayPoint(displayPos);
        ent.keyPointText->SetDisplayPosition(static_cast<int>(std::round(displayPos[0])),
                                             static_cast<int>(std::round(displayPos[1])));
        char buf[64];
        sprintf(buf, "(%0.2f,%0.2f,%0.2f)", worldPos[0], worldPos[1], worldPos[2]);
        ent.keyPointText->SetInput(buf);
    }

    // 最后触发一次渲染更新
    if (renderWindow) {
=======
    if(renderer){
        renderer->SetBackground(r,g,b);
        renderer->SetGradientBackground(0);               //关闭渐变色背景设置
>>>>>>> Stashed changes
        renderWindow->Render();
        return true;
    }
    else{
        qDebug()<<"SetvtkRenderBackColor函数中"<<"renderer"<<"未设置,导致设置颜色失败"<<"\n";
        return false;
    }
}

bool disp3dmodel::SetVtkRenderBackGradient(double r1, double g1, double b1, double r2, double g2, double b2)
{
    if(renderer){
        renderer->SetBackground(r1,g1,b1);             //设置页面底部颜色值
        renderer->SetBackground2(r2, g2, b2);          //设置页面顶部颜色值
        renderer->SetGradientBackground(1);            //开启渐变色背景设置
        return true;
    }
    else{
        qDebug()<<"SetvtkRenderBackColor2函数中"<<"renderer"<<"未设置,导致设置颜色失败"<<"\n";
        return false;
    }
}

bool disp3dmodel::readData(QString path)
{
    if(path.isEmpty()==false)   //path内容不为空
    {
        QFile file(path);
        if(!file.exists()){
            return false;
        }
        else{
            if(file.open(QIODevice::ReadOnly)){  //打开成功
                while(!file.atEnd()){
                    QString s = file.readLine().simplified();   //readline读取一行；simplified去除首尾空格，中间多个空格变为1个
                    QString value = s.section(" ",0,0,QString::SectionSkipEmpty).toLower();     //section提取的一个单词；.toLower全部转换为小写
                    QString line;
                    QStringList list;
                    if(value == "node"){                            //读取节点坐标
                        QString node_n = s.section(" ",1,1,QString::SectionSkipEmpty);    //section提取的二个单词；
                        QString serial_node;
                        QString location;
                        int i = 0;
                        node_num = node_n.toInt();  //node_num为总的节点数
                        while(i < node_num){
                            if(!file.atEnd()){
                                node *point = new node;
                                line = file.readLine().simplified();
                                serial_node = line.section(" ",0,0,QString::SectionSkipEmpty);
                                (*point).NodeNo=serial_node.toInt();    //这个NodeNo就是节点编号
                                double * cord = new double[3];          //定义存储点坐标信息的数组
                                for (int j = 0; j < 3; j++){
                                    location = line.section(" ",j+1,j+1,QString::SectionSkipEmpty);
                                    cord[j] = location.toDouble();
                                    cord[j] /= 1000.0;                  //直接从文件中读取的点的坐标信息为毫米,读取之后点的坐标信息为米。
                                }
                                nodes[(*point).NodeNo] = point;     //存储节点编号
                                (*point).cord = cord;               //存储节点坐标
                                i++;
                            }
                        }
                    }
                    else if(value == "element"){                    //单元数
                        QString element_n = s.section(" ",1,1,QString::SectionSkipEmpty);
                        element_num = element_n.toInt();
                    }
                    else if(value == "shell"){                      //壳单元
                        QString shell_n = s.section(" ",1,1,QString::SectionSkipEmpty);
                        shell_num = shell_n.toInt();
                        QString serial_shell;
                        for(int i=0;i<shell_num;i++){
                            shell *shell1 = new shell;
                            line = file.readLine().simplified();
                            serial_shell = line.section(" ",0,0,QString::SectionSkipEmpty);
                            (*shell1).ElementNo = serial_shell.toInt();
                            serial_shell = line.section(" ",1,1,QString::SectionSkipEmpty);
                            (*shell1).ElementType = serial_shell.toInt();
                            if (!((*shell1).ElementType - 2)){  //三角形单元
                                (*shell1).NodeNo = new int[3];
                                for (int j = 0; j < 3; j++){
                                    serial_shell=line.section(" ",2+j,2+j,QString::SectionSkipEmpty).trimmed();
                                    (*shell1).NodeNo[j]=serial_shell.toInt();
                                }
                                triangle_number++;
                            }
                            else{                               //四边形单元
                                (*shell1).NodeNo = new int[4];
                                for (int j = 0; j < 4; j++){
                                    serial_shell=line.section(" ",2+j,2+j,QString::SectionSkipEmpty).trimmed();
                                    (*shell1).NodeNo[j]=serial_shell.toInt();
                                }
                                quad_number++;
                            }
                            serial_shell = line.section(" ",3+(*shell1).ElementType,3+(*shell1).ElementType,QString::SectionSkipEmpty);
                            (*shell1).ElementThick = serial_shell.toDouble();
                            shells[(*shell1).ElementNo] = shell1;
                        }
                    }
                    else if(value == "beam"){                       //梁单元
                        QString beam_n=s.section(" ",1,1,QString::SectionSkipEmpty);
                        beam_number=beam_n.toInt();
                        QString serial_beam;
                        for(int j=0;j<beam_number;j++){
                            beam *beam1 = new beam;
                            line=file.readLine().simplified();
                            serial_beam=line.section(" ",0,0,QString::SectionSkipEmpty);
                            (*beam1).ElementNo=serial_beam.toInt();
                            (*beam1).NodeNo=new int[2];
                            for(int j=0;j<2;j++){
                               serial_beam=line.section(" ",1+j,1+j,QString::SectionSkipEmpty);
                               (*beam1).NodeNo[j]=serial_beam.toInt();
                            }
                            serial_beam=line.section(" ",3,3,QString::SectionSkipEmpty);
                            (*beam1).ElementType=serial_beam.toInt();
                            serial_beam=line.section(" ",4,4,QString::SectionSkipEmpty);
                            (*beam1).sectionNo=serial_beam.toInt();
                            beams[(*beam1).ElementNo]=beam1;
                        }
                    }
                    else if(value == "section"){                    //截面
                        QString section_n=s.section(" ",1,1,QString::SectionSkipEmpty);
                        section_number=section_n.toInt();
                        QString serial_section;
                        for(int j=0;j<section_number;j++){
                            sectionInfo *sectionInfo1=new sectionInfo;
                            line=file.readLine().simplified();
                            serial_section=line.section(" ",0,0,QString::SectionSkipEmpty);
                            (*sectionInfo1).sectionNo=serial_section.toInt();
                            serial_section=line.section(" ",1,1,QString::SectionSkipEmpty);
                            (*sectionInfo1).sectionshape=serial_section.toInt();
                            if((*sectionInfo1).sectionshape==1){
                                (*sectionInfo1).sectionGEo=new double;
                                serial_section=line.section(" ",2,2,QString::SectionSkipEmpty);
                                (*sectionInfo1).sectionGEo[0]=serial_section.toDouble();
                            }
                            else{
                                (*sectionInfo1).sectionGEo=new double[2];
                                for(int j=0;j<2;j++){
                                    serial_section=line.section(" ",2+j,2+j,QString::SectionSkipEmpty);
                                    (*sectionInfo1).sectionGEo[j]=serial_section.toDouble();
                                }
                            }
                            sectionInfos[(*sectionInfo1).sectionNo]=sectionInfo1;
                        }
                    }
                    else if(value == "flagpoint"){                  //关键点
                        QString flag_n=s.section(" ",1,1,QString::SectionSkipEmpty);
                        flag_number = flag_n.toInt();
                        line = file.readLine().simplified();
                        int len = line.count(" ")+1;    //行的数据个数

                        QString serial_flagpoint;
                        int j=0;

                        while(j < flag_number){
                            for(int i=0; i<len && j<flag_number; i++){
                                serial_flagpoint = line.section(" ",i,i,QString::SectionSkipEmpty);
                                LineModeFlagpointToNodePoint[j+1] = serial_flagpoint.toInt();
                                ++j;
                            }
                            if(j < flag_number){      //如果当前行解析完但未达到 flag_number，继续读取下一行
                                line = file.readLine().simplified();
                                len = line.count(" ")+1;
                            }
                        }
                    }
                    else if(value == "reversibleele"){              //逆有限单元数据块
                        QString reversibleelenum;   //逆有限单元个数
                        QString serial_reversible;  //临时存储解析出的字符串数据
                        int reversibleeleNO;        //逆有限单元编号
                        int elepointnum;            //每个逆有限单元包含的差值点数量

                        reversibleelenum = s.section(" ",1,1,QString::SectionSkipEmpty).simplified();
                        reversibleele_num = reversibleelenum.toInt();

                        for(int i=0;i<reversibleele_num;i++){
                            line = file.readLine().simplified();

                            serial_reversible = line.section(" ",0,0,QString::SectionSkipEmpty).trimmed();
                            reversibleeleNO = serial_reversible.toInt();

                            serial_reversible = line.section(" ",1,1,QString::SectionSkipEmpty).trimmed();
                            elepointnum = serial_reversible.toInt();


                            ReversibleElementPointSet *PointSet = new  ReversibleElementPointSet;   //创建新的点集对象存储单元信息
                            QVector <int> * pointlink = new QVector <int>;                          //存储差值点编号的向量
                            (*pointlink).resize(elepointnum);
                            PointSet->elementNo = reversibleeleNO;
                            PointSet->DvaluePointNum = elepointnum;
                            PointSet->Dvaluepoint = pointlink;

                            line = file.readLine().simplified();    // 读取差值点编号行
                            int len = line.count(" ")+1;            // 计算当前行包含的数值数量
                            int j = 0;
                            while(j < elepointnum){
                                for(int i=0; i<len && j<elepointnum; i++){
                                    serial_reversible = line.section(" ",i,i,QString::SectionSkipEmpty);
                                    (*pointlink)[j] = serial_reversible.toInt();
                                    ++j;
                                }
                                if(j < elepointnum){    //读取下一行
                                     line = file.readLine().simplified();
                                     len = line.count(" ")+1;
                                }
                            }
                            ReversibleElementDvaluepointSet[reversibleeleNO] = PointSet; //存储分配给每个逆有限单元的点的个数
                        }
                    }
                    else if(value == "reversibleelementset"){       //可逆单元坐标系的集合
                        QString serial_reverEle;
                        serial_reverEle = s.section(" ",1,1,QString::SectionSkipEmpty);
                        int reversibleele_numshell = serial_reverEle.toInt();

                        for(int j=0; j<reversibleele_numshell; j++){
                            line = file.readLine().simplified();
                            serial_reverEle = line.section(" ",0,0,QString::SectionSkipEmpty);

                            ReversibleElement *reverele = new ReversibleElement; //存储逆有限单元坐标系
                            reverele->eleNO = serial_reverEle.toInt();
                            reverele->ipoint=new double[3];
                            reverele->jpoint=new double[3];
                            reverele->kpoint=new double[3];

                            for(int i=0;i<9;i++){
                                serial_reverEle=line.section(" ",i+1,i+1,QString::SectionSkipEmpty);
                                if(i<3){
                                    reverele->ipoint[i]=serial_reverEle.toDouble();
                                }
                                else if(i>=3&&i<6){
                                    reverele->jpoint[i%3]=serial_reverEle.toDouble();
                                }
                                else if(i>=6&&i<9){
                                    reverele->kpoint[i%3]=serial_reverEle.toDouble();
                                }
                            }
                            ReversibleElementSet[reverele->eleNO]=reverele;
                        }
                    }
                    else if(value == "reversiblelen"){              //逆有限元板的几何尺寸数据
                        int shell_ReverNum;      // 存储当前逆有限元板的编号
                        QString serial_ReversibleLen = s.section(" ",1,1,QString::SectionSkipEmpty);
                        int shell_ReversibleNum = serial_ReversibleLen.toInt();//逆有限元板的个数
                        for(int i=0;i<shell_ReversibleNum;i++){
                            shell_len *shell_len1 = new shell_len;
                            line=file.readLine().simplified();

                            serial_ReversibleLen = line.section(" ",0,0,QString::SectionSkipEmpty);
                            shell_ReverNum = serial_ReversibleLen.toInt();
                            serial_ReversibleLen = line.section(" ",1,1,QString::SectionSkipEmpty);
                            (*shell_len1).length= serial_ReversibleLen.toDouble();
                            serial_ReversibleLen = line.section(" ",2,2,QString::SectionSkipEmpty);
                            (*shell_len1).width= serial_ReversibleLen.toDouble();

                            ReversibleLen[shell_ReverNum]=shell_len1;
                        }
                    }
                    else if(value == "sensorandinserverrelationset")//传感器编号和通道数据
                    {
                        QString serial_sensor;
                        serial_sensor=s.section(" ",1,1,QString::SectionSkipEmpty);
                        int channalnum = serial_sensor.toInt(); //通道个数
                        int channalNo;                          //通道编号
                        int channalbitnum;                      //通道位数
                        int channalbit;                         //传感器编号
                        for (int i = 0; i < channalnum; i++){

                            line=file.readLine().simplified();
                            serial_sensor = line.section(" ",0,0,QString::SectionSkipEmpty).trimmed();
                            channalNo = serial_sensor.toInt();      // 解析通道编号
                            serial_sensor=line.section(" ",1,1,QString::SectionSkipEmpty).trimmed();
                            channalbitnum = serial_sensor.toInt();  // 解析通道位数

                            for (int j = 0; j < channalbitnum; j++){//处理每个通道的每个位
                                serial_sensor = line.section(" ",2+j,2+j,QString::SectionSkipEmpty);
                                channalbit = serial_sensor.toInt();
                                ChannalEle * chanele = new ChannalEle;
                                chanele->ChannalNO = channalNo;
                                chanele->Channalbit = j;
                                SensorANDInserverRelationSet[channalbit] = chanele;//下标索引从0开始
                            }

                            QVector<double>* currentstrain = new QVector<double>;   //为当前通道创建应变数据存储向量
                            if (strainset[channalNo] != nullptr){
                                delete strainset[channalNo];
                            }
                            currentstrain->resize(channalbitnum);
                            strainset[channalNo] = currentstrain; //初始化存储应变的数据的集合
                            //delete currentstrain;     我觉得这里不应该删除内存
                            currentstrain = nullptr;
                        }
                    }
                    else{
                        break;
                    }
                }
                file.close();
                return true;
            }
            else {
                return false;
            }
        }
    }
    else{
        return false;
    }
}

void disp3dmodel::CreatePointSet()  //创建点集合
{
    this->points = vtkPoints::New();
    points->SetNumberOfPoints(node_num);
    int count = 0;
    for(int i=1; i<=node_num; i++){
        points->SetPoint(count++,(*nodes[i]).cord[0],(*nodes[i]).cord[1],(*nodes[i]).cord[2]);
    }
}

void disp3dmodel::CreateCellSet()   //创建单元集合
{
    this->S_M_points = vtkPoints::New();
    this->cellarray = vtkCellArray::New();
    this->S_M_cellarray = vtkCellArray::New();

    Type = new int[static_cast<size_t>(element_num)];                        //创建线面模型单元类型
    S_M_Type = new int[static_cast<size_t>(element_num)];                    //创建实体模型单元类型
    lines = new vtkLine *[static_cast<size_t>(beam_number)];                 //线单元数组
    triangles = new  vtkTriangle*[static_cast<size_t>(triangle_number)];     //三角形单元数组
    quads = new vtkQuad*[static_cast<size_t>(quad_number)];                  //四边形单元数组
    wedge = new vtkWedge*[static_cast<size_t>(triangle_number)];             //三棱柱单元数组
    //Hexahdron = new vtkHexahedron*[quad_number];                           //四棱柱单元数组
    Hexahdron1 = new vtkHexahedron*[static_cast<size_t>(quad_number)];       //四棱柱单元数组
    liulengzhu = new vtkHexagonalPrism*[static_cast<size_t>(beam_number)];   //只确定一种六棱柱梁单元
    LineNumber = TriangleNumber = QuadNumber = 0;                       //给拓扑单元附初始值
    Beam_Number = Shell_Triangle_Number = Shell_Quad_Number = 0;

    int element_serial=0;
    int newnode_serial=0;

    for(int i=1;i<=element_num;i++){
        if(shells.find(i)!=shells.end()){  //开始板单元
            if(!((*shells[i]).ElementType-2)){//判断三角形单元
               triangles[TriangleNumber]=vtkTriangle::New();    //创建一个三角形单元
               for(int j=0;j<3;j++){
                   triangles[TriangleNumber]->GetPointIds()->SetId(j,(*shells[i]).NodeNo[j]-1);
               }
               Type[i-1]=5;
               cellarray->InsertNextCell(triangles[TriangleNumber]);
               TriangleNumber++;
               double x1=(*nodes[(*shells[i]).NodeNo[0]]).cord[0];  //获取三角形面板上三个点的坐标
               double y1=(*nodes[(*shells[i]).NodeNo[0]]).cord[1];
               double z1=(*nodes[(*shells[i]).NodeNo[0]]).cord[2];
               double x2=(*nodes[(*shells[i]).NodeNo[1]]).cord[0];
               double y2=(*nodes[(*shells[i]).NodeNo[1]]).cord[1];
               double z2=(*nodes[(*shells[i]).NodeNo[1]]).cord[2];
               double x3=(*nodes[(*shells[i]).NodeNo[2]]).cord[0];
               double y3=(*nodes[(*shells[i]).NodeNo[2]]).cord[1];
               double z3=(*nodes[(*shells[i]).NodeNo[2]]).cord[2];
               double dec_x1 = x2 - x1; double dec_y1 = y2 - y1; double dec_z1 = z2 - z1; //第一个方向向量
               double dec_x2 = x3 - x1; double dec_y2 = y3 - y1; double dec_z2 = z3 - z1; //第二个方向向量
               double dec_x3 = x1 - x3; double dec_y3 = y1 - y3; double dec_z3 = z1 - z3; //第三个方向向量
               double shell_thick =(*shells[i]).ElementThick; //壳的厚度
               double *f_vector1 = caculatevectorji(dec_x1, dec_y1, dec_z1, dec_x2, dec_y2, dec_z2);
               double *f_vector2 = caculatevectorji(dec_x1, dec_y1, dec_z1, dec_x3, dec_y3, dec_z3);
               double new_point[6][3] = {{0}};
               new_point[0][0] = f_vector1[0] * shell_thick / 2 + x1; //新的产生的板单元的两个面上点的坐标
               new_point[0][1] = f_vector1[1] * shell_thick / 2 + y1;
               new_point[0][2] = f_vector1[2] * shell_thick / 2 + z1;
               new_point[1][0] = f_vector1[0] * shell_thick / 2 + x2;
               new_point[1][1] = f_vector1[1] * shell_thick / 2 + y2;
               new_point[1][2] = f_vector1[2] * shell_thick / 2 + z2;
               new_point[2][0] = f_vector1[0] * shell_thick / 2 + x3;
               new_point[2][1] = f_vector1[1] * shell_thick / 2 + y3;
               new_point[2][2] = f_vector1[2] * shell_thick / 2 + z3;
               new_point[3][0] = f_vector2[0] * shell_thick / 2 + x1;
               new_point[3][1] = f_vector2[1] * shell_thick / 2 + y1;
               new_point[3][2] = f_vector2[2] * shell_thick / 2 + z1;
               new_point[4][0] = f_vector2[0] * shell_thick / 2 + x2;
               new_point[4][1] = f_vector2[1] * shell_thick / 2 + y2;
               new_point[4][2] = f_vector2[2] * shell_thick / 2 + z2;
               new_point[5][0] = f_vector2[0] * shell_thick / 2 + x3;
               new_point[5][1] = f_vector2[1] * shell_thick / 2 + y3;
               new_point[5][2] = f_vector2[2] * shell_thick / 2 + z3;
               delete[] f_vector1;
               delete[] f_vector2;
               wedge[Shell_Triangle_Number] = vtkWedge::New();
               surface *face = new surface;
               int *leftface = new int[3];
               int *rightface = new int[3];
                   (*face).elementtype = 2;
                   (*face).leftface = leftface;
                   (*face).rightface = rightface;
               new_element_info1[++element_serial] = face; //记录三棱柱单元的拓扑结构：
               for(int m=0;m<3;m++){    /*第一个面上点的编号*/
                   if(new_node_ex_node_info.find((*shells[i]).NodeNo[m])==new_node_ex_node_info.end()){ //情况1：原始节点没有对应的新节点映射（find没找到会指向end）
                       new_node_ex_node_info[(*shells[i]).NodeNo[m]] = new QVector<int >;  //创建面单元的当前节点对应的实体单元节点的存储集合
                       ++newnode_serial;  //创建新的编号
                       nodeinfo  *newnode = new nodeinfo; //创建新的节点的存储集合
                       (*newnode).bianhao = newnode_serial;
                       (*newnode).x_cord = new_point[m][0];
                       (*newnode).y_cord = new_point[m][1];
                       (*newnode).z_cord = new_point[m][2];
                       leftface[m] = newnode_serial; //新创建板单元的第一个面的M+1个点编号
                       S_M_points->InsertNextPoint(new_point[m][0], new_point[m][1], new_point[m][2]);  //给实体模型的VTK点集中插入新创建的点
                       new_node_info1[newnode_serial] = newnode;                                        //将新创建的点插入到实体模型对应的点集中
                       wedge[Shell_Triangle_Number]->GetPointIds()->SetId(m, newnode_serial - 1);       //三角形实体板单元插入包括的点的编号
                       (*new_node_ex_node_info[(*shells[i]).NodeNo[m]]).push_back(newnode_serial);      //已存在点的集合中当前新创建的点
                   }
                   else{                                                                                //情况2：原始节点已有对应的新节点映射；
                       QVector<int> *nodegaunxi = (new_node_ex_node_info[(*shells[i]).NodeNo[m]]); //当前已存在点的编号
                       int node_number1;
                       double point_x, point_y, point_z;
                       bool found = false;
                       for (QVector<int>::iterator t = (*nodegaunxi).begin(); t != (*nodegaunxi).end() && !found; t++){ //循环判断您当前已经计算的点在已存在点的集合中是否存在
                           node_number1 = *t;
                           point_x = (*(new_node_info1[node_number1])).x_cord;
                           point_y = (*(new_node_info1[node_number1])).y_cord;
                           point_z = (*(new_node_info1[node_number1])).z_cord;
                           if (!(point_x - new_point[m][0]) && !(point_x - new_point[m][0]) && !(point_z - new_point[m][2])){
                               found = true;
                           }
                       }
                       if (found){              //存在已经计算过的点
                           wedge[Shell_Triangle_Number]->GetPointIds()->SetId(m, node_number1 - 1);
                           leftface[m] = node_number1;
                       }
                       else{                    //不存在已经计算过的点，重新给已经计算过的点编号
                           ++newnode_serial;//创建新的编号
                           int number = S_M_points->InsertNextPoint(new_point[m][0], new_point[m][1], new_point[m][2]);//给实体模型的VTK点集中插入新创建的点
                           wedge[Shell_Triangle_Number]->GetPointIds()->SetId(m, number);//三角形实体板单元插入包括的点的编号
                           nodeinfo *newnode = new nodeinfo; //创建新的节点的存储集合
                           (*newnode).bianhao = newnode_serial;
                           (*newnode).x_cord = new_point[m][0];
                           (*newnode).y_cord = new_point[m][1];
                           (*newnode).z_cord = new_point[m][2];
                           new_node_info1[newnode_serial] = newnode;//将新创建的点插入到实体模型对应的点集中
                           leftface[m] = newnode_serial;//新创建板单元的第一个面的M+1个点编号
                           (*new_node_ex_node_info[(*shells[i]).NodeNo[m]]).push_back(newnode_serial);//已存在点的集合中当前新创建的点
                       }
                   }
               }

               for (int m = 0; m < 3; m++){/*第二个面点的编号，过程元第一个面基本相同*/
                   if (new_node_ex_node_info.find((*shells[i]).NodeNo[m]) == new_node_ex_node_info.end()){
                       new_node_ex_node_info[(*shells[i]).NodeNo[m]] = new QVector<int >;
                       ++newnode_serial;
                       nodeinfo *newnode = new nodeinfo;
                       (*newnode).bianhao = newnode_serial;
                       (*newnode).x_cord = new_point[m + 3][0];
                       (*newnode).y_cord = new_point[m + 3][1];
                       (*newnode).z_cord = new_point[m + 3][2];
                       S_M_points->InsertNextPoint(new_point[m + 3][0], new_point[m + 3][1], new_point[m + 3][2]);
                       new_node_info1[newnode_serial] = newnode;
                       wedge[Shell_Triangle_Number]->GetPointIds()->SetId(m + 3, newnode_serial - 1);
                       rightface[m] = newnode_serial;
                       (*new_node_ex_node_info[(*shells[i]).NodeNo[m]]).push_back(newnode_serial);
                   }
                   else{
                       QVector<int> * nodegaunxi = (new_node_ex_node_info[(*shells[i]).NodeNo[m]]);
                       int node_number1;
                       double point_x, point_y, point_z;
                       bool found = false;
                       for (QVector<int>::iterator t = (*nodegaunxi).begin(); t != (*nodegaunxi).end() && !found; t++){
                           node_number1 = *t;
                           point_x = (*(new_node_info1[node_number1])).x_cord;
                           point_y = (*(new_node_info1[node_number1])).y_cord;
                           point_z = (*(new_node_info1[node_number1])).z_cord;
                           if (!(point_x - new_point[m + 3][0]) && !(point_y - new_point[m + 3][1]) && !(point_z - new_point[m + 3][2])){
                               found = true;
                           }
                       }
                       if (found){
                           wedge[Shell_Triangle_Number]->GetPointIds()->SetId(m + 3, node_number1 - 1);
                           rightface[m] = node_number1;
                       }
                       else{
                           ++newnode_serial;
                           int number = S_M_points->InsertNextPoint(new_point[m + 3][0], new_point[m + 3][1], new_point[m + 3][2]);
                           wedge[Shell_Triangle_Number]->GetPointIds()->SetId(m + 3, number);
                           nodeinfo *newnode = new nodeinfo;
                           (*newnode).bianhao = newnode_serial;
                           (*newnode).x_cord = new_point[m + 3][0];
                           (*newnode).y_cord = new_point[m + 3][1];
                           (*newnode).z_cord = new_point[m + 3][2];
                           new_node_info1[newnode_serial] = newnode;
                           rightface[m] = newnode_serial;
                           (*new_node_ex_node_info[(*shells[i]).NodeNo[m]]).push_back(newnode_serial);
                       }
                   }
            }
           S_M_cellarray->InsertNextCell(wedge[Shell_Triangle_Number++]); //当前创建的三角形板单元插入到VTK单元集合中
           S_M_Type[i - 1] = 13; //确定当前单元类型
        }
            else if(!((*shells[i]).ElementType - 3)){
                //创建一个四边形单元
                quads[QuadNumber]=vtkQuad::New();
                for(int j=0;j<4;j++){
                    quads[QuadNumber]->GetPointIds()->SetId(j,(*shells[i]).NodeNo[j]-1);
                }
                Type[i-1]=9;
                cellarray->InsertNextCell(quads[QuadNumber]);
                QuadNumber++;

                //获取四边形面板上四个点的坐标
                double x1 = (*nodes[(*shells[i]).NodeNo[0]]).cord[0];
                double y1 = (*nodes[(*shells[i]).NodeNo[0]]).cord[1];
                double z1 = (*nodes[(*shells[i]).NodeNo[0]]).cord[2];
                double x2 = (*nodes[(*shells[i]).NodeNo[1]]).cord[0];
                double y2 = (*nodes[(*shells[i]).NodeNo[1]]).cord[1];
                double z2 = (*nodes[(*shells[i]).NodeNo[1]]).cord[2];
                double x3 = (*nodes[(*shells[i]).NodeNo[2]]).cord[0];
                double y3 = (*nodes[(*shells[i]).NodeNo[2]]).cord[1];
                double z3 = (*nodes[(*shells[i]).NodeNo[2]]).cord[2];
                double x4 = (*nodes[(*shells[i]).NodeNo[3]]).cord[0];
                double y4 = (*nodes[(*shells[i]).NodeNo[3]]).cord[1];
                double z4 = (*nodes[(*shells[i]).NodeNo[3]]).cord[2];
                //计算四边形单元的法向量
                //第一个方向向量
                double dec_x1 = x1 - x2;
                double dec_y1 = y1 - y2;
                double dec_z1 = z1 - z2;
                //第二个方向向量
                double dec_x2 = x3 - x2;
                double dec_y2 = y3 - y2;
                double dec_z2 = z3 - z2;
                //第三个方向向量
                double dec_x3 = x3 - x4;
                double dec_y3 = y3 - y4;
                double dec_z3 = z3 - z4;
                //第四个方向向量
                double dec_x4 = x1 - x4;
                double dec_y4 = y1 - y4;
                double dec_z4 = z1 - z4;
                //第五个方向向量
                double dec_x5 = x2 - x3;
                double dec_y5 = y2 - y3;
                double dec_z5 = z2 - z3;
                //第六个方向向量
                double dec_x6 = x4 - x1;
                double dec_y6 = y4 - y1;
                double dec_z6 = z4 - z1;
                //壳的厚度
                double shell_thick = (*shells[i]).ElementThick;
                double * f_vector11 = caculatevectorji(dec_x2, dec_y2, dec_z2, dec_x1, dec_y1, dec_z1);
                double * f_vector12 = caculatevectorji(dec_x4, dec_y4, dec_z4, dec_x3, dec_y3, dec_z3);
                double * f_vector21 = caculatevectorji(dec_x5, dec_y5, dec_z5, dec_x1, dec_y1, dec_z1);
                double * f_vector22 = caculatevectorji(dec_x6, dec_y6, dec_z6, dec_x3, dec_y3, dec_z3);
                double  f_vector1[3] = { 0 };
                double  f_vector2[3] = { 0 };
                for (int d = 0; d < 3; d++){
                    f_vector1[d] = (f_vector11[d] + f_vector12[d]) / 2;
                    f_vector2[d] = (f_vector21[d] + f_vector22[d]) / 2;
                }
                delete[] f_vector11;
                delete[] f_vector12;
                delete[] f_vector21;
                delete[] f_vector22;

                //新的单元的各个结点的坐标
                double new_point[8][3] = { 0.0 };
                new_point[0][0] = f_vector1[0] * shell_thick / 2 + x1;
                new_point[0][1] = f_vector1[1] * shell_thick / 2 + y1;
                new_point[0][2] = f_vector1[2] * shell_thick / 2 + z1;
                new_point[1][0] = f_vector1[0] * shell_thick / 2 + x2;
                new_point[1][1] = f_vector1[1] * shell_thick / 2 + y2;
                new_point[1][2] = f_vector1[2] * shell_thick / 2 + z2;
                new_point[2][0] = f_vector1[0] * shell_thick / 2 + x3;
                new_point[2][1] = f_vector1[1] * shell_thick / 2 + y3;
                new_point[2][2] = f_vector1[2] * shell_thick / 2 + z3;
                new_point[3][0] = f_vector1[0] * shell_thick / 2 + x4;
                new_point[3][1] = f_vector1[1] * shell_thick / 2 + y4;
                new_point[3][2] = f_vector1[2] * shell_thick / 2 + z4;
                new_point[4][0] = f_vector2[0] * shell_thick / 2 + x1;
                new_point[4][1] = f_vector2[1] * shell_thick / 2 + y1;
                new_point[4][2] = f_vector2[2] * shell_thick / 2 + z1;
                new_point[5][0] = f_vector2[0] * shell_thick / 2 + x2;
                new_point[5][1] = f_vector2[1] * shell_thick / 2 + y2;
                new_point[5][2] = f_vector2[2] * shell_thick / 2 + z2;
                new_point[6][0] = f_vector2[0] * shell_thick / 2 + x3;
                new_point[6][1] = f_vector2[1] * shell_thick / 2 + y3;
                new_point[6][2] = f_vector2[2] * shell_thick / 2 + z3;
                new_point[7][0] = f_vector2[0] * shell_thick / 2 + x4;
                new_point[7][1] = f_vector2[1] * shell_thick / 2 + y4;
                new_point[7][2] = f_vector2[2] * shell_thick / 2 + z4;
                //创建实体单元
//              QVector<int> * element_newnode = new QVector<int>;
                Hexahdron1[Shell_Quad_Number] = vtkHexahedron::New();
                surface *face = new surface;
                int *leftface = new int[4];
                int *rightface = new int[4];
                (*face).elementtype = 3;//四边形壳单元
                (*face).leftface = leftface;
                (*face).rightface = rightface;
                new_element_info1[++element_serial] = face;
                for(int m=0;m<4;m++){//创建的第一个面左边四个点编号的确定
                   if(new_node_ex_node_info.find((*shells[i]).NodeNo[m])==new_node_ex_node_info.end()){
                       new_node_ex_node_info[(*shells[i]).NodeNo[m]] = new QVector<int >;
                       ++newnode_serial;
                       nodeinfo  *newnode = new nodeinfo;
                       (*newnode).bianhao = newnode_serial;
                       (*newnode).x_cord = new_point[m][0];
                       (*newnode).y_cord = new_point[m][1];
                       (*newnode).z_cord = new_point[m][2];
                       S_M_points->InsertNextPoint(new_point[m][0], new_point[m][1], new_point[m][2]);
                       new_node_info1[newnode_serial] = newnode;
                       Hexahdron1[Shell_Quad_Number]->GetPointIds()->SetId(m, newnode_serial - 1);
                       leftface[m] = newnode_serial;
                       (*new_node_ex_node_info[(*shells[i]).NodeNo[m]]).push_back(newnode_serial);
                   }
                   else{
                       QVector <int> * nodegaunxi = (new_node_ex_node_info[(*shells[i]).NodeNo[m]]);
                       int node_number1;
                       double point_x, point_y, point_z;
                       bool found = false;
                       for (QVector<int>::iterator t = (*nodegaunxi).begin(); t != (*nodegaunxi).end() && !found; t++){
                           node_number1 = *t;
                           point_x = (*(new_node_info1[node_number1])).x_cord;
                           point_y = (*(new_node_info1[node_number1])).y_cord;
                           point_z = (*(new_node_info1[node_number1])).z_cord;
                           if (!(point_x - new_point[m][0]) && !(point_y - new_point[m][1]) && !(point_z - new_point[m][2])){
                               found = true;
                           }
                       }
                       if (found){
                           Hexahdron1[Shell_Quad_Number]->GetPointIds()->SetId(m, node_number1 - 1);
                           leftface[m] = node_number1;
                       }
                       else{
                           ++newnode_serial;
                           int number = S_M_points->InsertNextPoint(new_point[m][0], new_point[m][1], new_point[m][2]);
                           Hexahdron1[Shell_Quad_Number]->GetPointIds()->SetId(m, number);
                           nodeinfo *new_node = new nodeinfo;
                           (*new_node).bianhao = newnode_serial;
                           (*new_node).x_cord = new_point[m][0];
                           (*new_node).y_cord = new_point[m][1];
                           (*new_node).z_cord = new_point[m][2];
                           new_node_info1[newnode_serial] = new_node;
                           leftface[m] = newnode_serial;
                           (*new_node_ex_node_info[(*shells[i]).NodeNo[m]]).push_back(newnode_serial);
                       }
                   }
                }
                for(int m=0;m<4;m++){//创建的第二个面右边四个点编号的确定
                    if (new_node_ex_node_info.find((*shells[i]).NodeNo[m]) == new_node_ex_node_info.end()){
                        new_node_ex_node_info[(*shells[i]).NodeNo[m]] = new QVector<int >;
                        ++newnode_serial;
                        nodeinfo  *newnode = new nodeinfo;
                        (*newnode).bianhao = newnode_serial;
                        (*newnode).x_cord = new_point[m + 4][0];
                        (*newnode).y_cord = new_point[m + 4][1];
                        (*newnode).z_cord = new_point[m + 4][2];
                        S_M_points->InsertNextPoint(new_point[m + 4][0], new_point[m + 4][1], new_point[m + 4][2]);
                        new_node_info1[newnode_serial] = newnode;
                        Hexahdron1[Shell_Quad_Number]->GetPointIds()->SetId(m + 4, newnode_serial - 1);
                        rightface[m] = newnode_serial;
                        (*new_node_ex_node_info[(*shells[i]).NodeNo[m]]).push_back(newnode_serial);
                    }
                    else{
                        QVector<int> * nodegaunxi = (new_node_ex_node_info[(*shells[i]).NodeNo[m]]);
                        int node_number1;
                        double point_x, point_y, point_z;
                        bool found = false;
                        for (QVector<int>::iterator t = (*nodegaunxi).begin(); t != (*nodegaunxi).end() && !found; t++){
                            node_number1 = *t;
                            point_x = (*(new_node_info1[node_number1])).x_cord;
                            point_y = (*(new_node_info1[node_number1])).y_cord;
                            point_z = (*(new_node_info1[node_number1])).z_cord;
                            if (!(point_x - new_point[m + 4][0]) && !(point_y - new_point[m + 4][1]) && !(point_z - new_point[m + 4][2])){
                                found = true;
                            }
                        }
                        if (found){
                            Hexahdron1[Shell_Quad_Number]->GetPointIds()->SetId(m + 4, node_number1 - 1);
                            rightface[m] = node_number1;
                        }
                        else{
                            ++newnode_serial;
                            int number = S_M_points->InsertNextPoint(new_point[m + 4][0], new_point[m + 4][1], new_point[m + 4][2]);
                            Hexahdron1[Shell_Quad_Number]->GetPointIds()->SetId(m + 4, number);
                            nodeinfo  *newnode = new nodeinfo;
                            (*newnode).bianhao = newnode_serial;
                            (*newnode).x_cord = new_point[m + 4][0];
                            (*newnode).y_cord = new_point[m + 4][1];
                            (*newnode).z_cord = new_point[m + 4][2];
                            new_node_info1[newnode_serial] = newnode;
                            rightface[m] = newnode_serial;
                            (*new_node_ex_node_info[(*shells[i]).NodeNo[m]]).push_back(newnode_serial);
                        }
                    }
                }
                S_M_cellarray->InsertNextCell(Hexahdron1[Shell_Quad_Number++]); //当前创建的四边形板单元插入到VTK单元集合中
                S_M_Type[i - 1] = 12; //确定当前单元的类型
            }
        }
        else if(beams.find(i)!=beams.end())  //开始梁单元
        {
            //创建一个线单元
            lines[LineNumber] = vtkLine::New();
            for (int j = 0; j < 2; j++)
                lines[LineNumber]->GetPointIds()->SetId(j, (*(beams[i])).NodeNo[j] - 1);
            Type[i - 1] = 3;
            cellarray->InsertNextCell(lines[LineNumber]);
            LineNumber++;
            //计算对应截面上点的坐标
            //分别创建一根梁截面的两个截面，然后组织相关的拓扑关系
            //梁的两个点坐标
            double first_point[3], second_point[3];
            for (int point_d = 0; point_d < 3; point_d++)

            {
                first_point[point_d] = (*nodes[(*(beams[i])).NodeNo[0]]).cord[point_d];
                second_point[point_d] = (*nodes[(*(beams[i])).NodeNo[1]]).cord[point_d];
            }
            //梁的长度
            double liang_length = 0.0;
            for (int point_d = 0; point_d < 3; point_d++)

            {
                liang_length += (first_point[point_d] - second_point[point_d]) * (first_point[point_d] - second_point[point_d]);
            }
            liang_length = sqrt(liang_length);
            //梁的方向向量
            double direction_vec[3];
            for (int point_d = 0; point_d < 3; point_d++)
            {
                direction_vec[point_d] = (first_point[point_d] - second_point[point_d]) / liang_length;
            }
            //建立梁单元的局部坐标系
            /*采用默认的方式建立，默认方式是
            当梁的方向向量与全局坐标系的z轴共线时，直接以z轴作为y轴,以两者的向量积作为z轴，计算局部坐标系在全局坐标系的各轴的方向向量
            当梁的方向向量与全局坐标系的z轴不共线时，直接以z轴作为M轴，然后以两者的向量积作为z轴，然后再求y轴
            */
            double  y_direction[3] = { 0.0 };
            double  M1[3] = { 0, 0, 1.0 };
            double *z_direction = nullptr;
            if (!(direction_vec[2] - 1.0) || !(direction_vec[2] + 1.0))
            {
                y_direction[1] = 1.0;
                z_direction = caculatevectorji(direction_vec[0], direction_vec[1], direction_vec[2], y_direction[0], y_direction[1], y_direction[2]);

            }
            else
            {
                //单位向量
                z_direction = caculatevectorji(direction_vec[0], direction_vec[1], direction_vec[2], M1[0], M1[1], M1[2]);
                double *diretion_y = caculatevectorji(z_direction[0], z_direction[1], z_direction[2], direction_vec[0], direction_vec[1], direction_vec[2]);
                y_direction[0] = diretion_y[0];
                y_direction[1] = diretion_y[1];
                y_direction[2] = diretion_y[2];
                delete[] diretion_y;
//                delete[] z_direction;
            }
            //求解计算坐标变换矩阵
            double tranfrom[4][4] = { 0.0 };
            for (int m = 0; m < 4; m++)
            {
                tranfrom[0][m] = direction_vec[m];

            }
            for (int m = 0; m < 4; m++)
            {
                tranfrom[1][m] = y_direction[m];

            }
            for (int m = 0; m < 4; m++)
            {
                tranfrom[2][m] = z_direction[m];

            }
            for (int m = 0; m < 4; m++)
            {
                tranfrom[3][m] = first_point[m];
                if (m == 3)
                    tranfrom[3][m] = 1.0;
            }
            double cord1[6][4] = { 0.0 };
            double cord2[6][4] = { 0.0 };

            delete[] z_direction;

            double r = (*sectionInfos[(*(beams[i])).sectionNo]).sectionGEo[0];
            babianxingpoint cord_b = Getzhengliubianxing(r);
            /*计算两个截面上点在全局坐标系下的坐标*/
            for (int m = 0; m < 6; m++)

            {
                double *cord11 = tran_cord(tranfrom, cord_b[m]);
                cord1[m][0] = cord11[0];
                cord1[m][1] = cord11[1];
                cord1[m][2] = cord11[2];
                delete[] cord11;
            }
            for (int m = 0; m < 4; m++)
            {
                tranfrom[3][m] = second_point[m];
                if (m == 3)
                    tranfrom[3][m] = 1.0;
            }
            for (int m = 0; m < 6; m++)
            {
                double *cord12 = tran_cord(tranfrom, cord_b[m]);
                cord2[m][0] = cord12[0];
                cord2[m][1] = cord12[1];
                cord2[m][2] = cord12[2];
                delete[] cord12;
            }
            for (int m = 0; m < 4; m++)
            {
                tranfrom[3][m] = second_point[m];
                if (m == 3)
                    tranfrom[3][m] = 1.0;
            }
            for (int m = 0; m < 6; m++)
            {
                double *cord11 = tran_cord(tranfrom, cord_b[m]);
                cord2[m][0] = cord11[0];
                cord2[m][1] = cord11[1];
                cord2[m][2] = cord11[2];
                delete[] cord11;
            }

            delete[] cord_b;
            //开始创建单元
//            QVector<int> * element_newnode = new QVector<int>;
            liulengzhu[Beam_Number] = vtkHexagonalPrism::New();
            surface *face = new surface;
            int *leftface = new int[6];
            int *rightface = new int[6];
            (*face).elementtype = 1;//梁单元
            (*face).leftface = leftface; //第一个截面上点的编号
            (*face).rightface = rightface;//第二个截面上点的编号
            new_element_info1[++element_serial] = face;
            /*第一个截面上节点的编号的确定*/
            if (new_node_ex_node_info.find((*(beams[i])).NodeNo[0]) == new_node_ex_node_info.end())
            {
                new_node_ex_node_info[(*(beams[i])).NodeNo[0]] = new QVector<int>;
                for (int j = 0; j < 6; j++)
                {
                    ++newnode_serial; //重新给点编号
                    //给点集中插入一点
                    int number = S_M_points->InsertNextPoint(cord1[j][0], cord1[j][1], cord1[j][2]);
                    liulengzhu[Beam_Number]->GetPointIds()->SetId(j, number); //六棱柱梁单元插入点的编号
                    nodeinfo *newnode = new nodeinfo; //创建存储点的变量
                    (*newnode).bianhao = newnode_serial;
                    (*newnode).x_cord = cord1[j][0];
                    (*newnode).y_cord = cord1[j][1];
                    (*newnode).z_cord = cord1[j][2];
                    new_node_info1[newnode_serial] = newnode;	// 插入新创建的结点
                    leftface[j] = newnode_serial;	//为新创建的单元插入结点
                    (*new_node_ex_node_info[(*(beams[i])).NodeNo[0]]).push_back(newnode_serial);	//  为已存在点集合中的旧的结点加入关联结点
                }
            }
            else
            {
                for (int j = 0; j < 6; j++)
                {
                    QVector<int> * nodegaunxi = (new_node_ex_node_info[(*(beams[i])).NodeNo[0]]);//目前线单元第一个点对应的实体梁单元上点的集合
                    int  node_number1;
                    double 	point_x, point_y, point_z;
                    bool found = false;
                    for (QVector<int>::iterator t = (*nodegaunxi).begin(); t != (*nodegaunxi).end() && !found; t++) //遍历已存在点的集合
                    {
                        node_number1 = *t;
                        point_x = (*(new_node_info1[node_number1])).x_cord;
                        point_y = (*(new_node_info1[node_number1])).y_cord;
                        point_z = (*(new_node_info1[node_number1])).z_cord;
                        if (!(point_x - cord1[j][0]) && !(point_y - cord1[j][1]) && !(point_z - cord1[j][2])) //判断当前的点在以存在点集合中是存在
                        {
                            found = true;
                        }

                    }
                    if (found)
                    {
                        liulengzhu[Beam_Number]->GetPointIds()->SetId(j, node_number1 - 1); //六棱柱单元插入点的编号
                        leftface[j] = node_number1; //定一个截面插入点的编号
                    }
                    else
                    {
                        ++newnode_serial; //重新给点编号
                        int number = S_M_points->InsertNextPoint(cord1[j][0], cord1[j][1], cord1[j][2]); //实体显示模型中插入新的点
                        liulengzhu[Beam_Number]->GetPointIds()->SetId(j, number); //六棱柱单元插入点的编号
                        nodeinfo *newnode = new nodeinfo; //创建新的点的变量
                        (*newnode).bianhao = newnode_serial;
                        (*newnode).x_cord = cord1[j][0];
                        (*newnode).y_cord = cord1[j][1];
                        (*newnode).z_cord = cord1[j][2];
                        new_node_info1[newnode_serial] = newnode;  //新的点插入点集中
                        leftface[j] = newnode_serial;//新的单元中插入点的编号
                        (*new_node_ex_node_info[(*(beams[i])).NodeNo[0]]).push_back(newnode_serial); //已存在点集合中插入新的点
                    }
                }
            }
            /*第二个截面上节点的编号的确定，过程与第一个截面上点的编号相同*/
            if (new_node_ex_node_info.find((*(beams[i])).NodeNo[1]) == new_node_ex_node_info.end())
            {
                new_node_ex_node_info[(*(beams[i])).NodeNo[1]] = new QVector<int>;
                for (int j = 0; j < 6; j++)
                {
                    ++newnode_serial;
                    cord2[j][3] = newnode_serial;
                    int number = S_M_points->InsertNextPoint(cord2[j][0], cord2[j][1], cord2[j][2]);
                    liulengzhu[Beam_Number]->GetPointIds()->SetId(j + 6, number);
                    nodeinfo *newnode = new nodeinfo;
                    (*newnode).bianhao = newnode_serial;
                    (*newnode).x_cord = cord2[j][0];
                    (*newnode).y_cord = cord2[j][1];
                    (*newnode).z_cord = cord2[j][2];
                    new_node_info1[newnode_serial] = newnode;
                    rightface[j] = newnode_serial;
                    (*new_node_ex_node_info[(*(beams[i])).NodeNo[1]]).push_back(newnode_serial);
                }
            }
            else
            {
                for (int j = 0; j < 6; j++)
                {
                    QVector<int> * nodegaunxi = (new_node_ex_node_info[(*(beams[i])).NodeNo[1]]);
                    int node_number1;
                    double point_x, point_y, point_z;
                    bool found = false;
                    for (QVector<int>::iterator t = (*nodegaunxi).begin(); t != (*nodegaunxi).end() && !found; t++)
                    {
                        node_number1 = *t;
                        point_x = (*(new_node_info1[node_number1])).x_cord;
                        point_y = (*(new_node_info1[node_number1])).y_cord;
                        point_z = (*(new_node_info1[node_number1])).z_cord;
                        if (!(point_x - cord2[j][0]) && !(point_y - cord2[j][1]) && !(point_z - cord2[j][2]))
                        {
                            found = true;
                        }
                    }
                    if (found)
                    {
                        liulengzhu[Beam_Number]->GetPointIds()->SetId(j + 6, node_number1 - 1);
                        rightface[j] = node_number1;
                    }
                    else
                    {
                        ++newnode_serial;
                        int number = S_M_points->InsertNextPoint(cord2[j][0], cord2[j][1], cord2[j][2]);
                        liulengzhu[Beam_Number]->GetPointIds()->SetId(j + 6, number);
                        nodeinfo *newnode = new nodeinfo;
                        (*newnode).bianhao = newnode_serial;
                        (*newnode).x_cord = cord2[j][0];
                        (*newnode).y_cord = cord2[j][1];
                        (*newnode).z_cord = cord2[j][2];
                        new_node_info1[newnode_serial] = newnode;
                        rightface[j] = newnode_serial;
                        (*new_node_ex_node_info[(*(beams[i])).NodeNo[1]]).push_back(newnode_serial);
                    }
                }
            }
            S_M_Type[i - 1] = 16;//VTK单元的类型
            S_M_cellarray->InsertNextCell(liulengzhu[Beam_Number++]); //当前创建的六棱柱梁单元插入到VTK单元集合中

        }
  }

    /*初始化实体模型位移集合*/
   for (QMap< int, struct nodeinfo*>::iterator it = new_node_info1.begin(); it != new_node_info1.end(); it++)
   {
       nodedisplace * displace = new  nodedisplace;
       displace->xdisplace = 0.0;
       displace->ydisplace = 0.0;
       displace->zdisplace = 0.0;
       int number = (it.key());
       PointDisplaceSet[number] = displace;
   }

   /*需要对实体模型上测量点进行编号*/
   int solidnum = 0;
   int length = LineModeFlagpointToNodePoint.size();
   for (int i = 1; i <= length; i++)
   {
       QVector<int>* s_node = (new_node_ex_node_info[LineModeFlagpointToNodePoint[i]]);
       for (QVector<int>::iterator it2 = (*s_node).begin(); it2 != (*s_node).end(); it2++)
       {
           //solidnum新的关键点的编号，鼠标点击出来的标号。SolidModeFlagPointToNodePoint[solidnum]是实际的标号
           solidnum++;
           SolidModeFlagPointToNodePoint[solidnum] = *it2;
       }
   }
   for (QMap<int, int>::iterator it = SolidModeFlagPointToNodePoint.begin(); it != SolidModeFlagPointToNodePoint.end(); it++)
   {
       SolidModeNodePointToFlagPoint[it.value()] = it.key();
   }
}

void disp3dmodel::CreateDataSet()
{
    this->cubeGrid = vtkUnstructuredGrid::New();
    this->S_M_cubeGrid = vtkUnstructuredGrid::New();
    /*创建线框模型数据集*/
    cubeGrid->Allocate();
    cubeGrid->SetPoints(points);
    cubeGrid->SetCells(Type, cellarray);
    /*创建实体模型数据*/
    S_M_cubeGrid->Allocate();
    S_M_cubeGrid->SetPoints(S_M_points);
    S_M_cubeGrid->SetCells(S_M_Type, S_M_cellarray);
}

void disp3dmodel::AddAxesActor()
{
    this->axe = vtkAxesActor::New();
    axe->SetAxisLabels(1);      //设置lable是否可见
    axe->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);
    axe->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 2, 0);
    axe->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 3);
    axe->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    axe->GetYAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    axe->GetZAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    axe->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
    axe->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
    axe->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
    //axe->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->set(70);
    axe->SetTotalLength(0.2,0.2,0.2);
    axe->SetVisibility(false);           //true为可见，false为不可见
    renderer->AddActor(axe);
}

void disp3dmodel::DynamicFresh()
{
    //更改显示文本的位置
    char  text[100] = { '\0' };
    double worldCord[3];
    double screenCord[3];
    QList<TextActor*>::iterator t;
    for (t = TextActorLink.begin(); t != TextActorLink.end(); t++){
        //结点编号
        int nodeNO = (*(t))->nodeNo;
        worldCord[0] = new_node_info1[nodeNO]->x_cord + Displacefactor*PointDisplaceSet[nodeNO]->xdisplace;
        worldCord[1] = new_node_info1[nodeNO]->y_cord + Displacefactor*PointDisplaceSet[nodeNO]->ydisplace;
        worldCord[2] = new_node_info1[nodeNO]->z_cord + Displacefactor*PointDisplaceSet[nodeNO]->zdisplace;
        bool found = false;
        for (QList<struct cordinfo*>::iterator it = CurrentDisplayCord.begin(); it != CurrentDisplayCord.end() && !found; it++){
            if ((*(it))->bianhao == nodeNO){
                (*(it))->x_cord = worldCord[0];
                (*(it))->y_cord = worldCord[1];
                (*(it))->z_cord = worldCord[2];
                found = true;
            }
        }
        //两步： 1、先获取当前文本在屏幕上的位置
        GetvtkRenderer()->SetWorldPoint(worldCord[0], worldCord[1], worldCord[2], 1.0);
        GetvtkRenderer()->WorldToDisplay();
        GetvtkRenderer()->GetDisplayPoint(screenCord);
        //阈值判断，超过设定阈值颜色变化
        if (PointDisplaceSet[nodeNO]->zdisplace >= 0.08 || PointDisplaceSet[nodeNO]->zdisplace <= -0.08){
            (*(t))->Actor->GetTextProperty()->SetColor(1, 0, 0);
        }
        else{
            (*(t))->Actor->GetTextProperty()->SetColor(0, 47/256.0, 167/256.0);
        }
        CordToCordCHAR(PointDisplaceSet[nodeNO]->xdisplace, PointDisplaceSet[nodeNO]->ydisplace, PointDisplaceSet[nodeNO]->zdisplace, text);//此时不乘factor单位已经是毫米了
        //获取文本
        // int position[2];
        (*(t))->Actor->SetInput(text);
        //TextActorLink[pos]->SetPosition(position);
        (*(t))->Actor->SetDisplayPosition(screenCord[0], screenCord[1]);
        (*(t))->Actor->Modified();
        (*(t))->Actor->SetPosition(screenCord[0], screenCord[1]);
        //更改显示坐标标记的位置
        found = false;
        for (QList<CordDisplayFlag*>::iterator it2 = DisplayTextNodeFlagLink.begin(); it2 != DisplayTextNodeFlagLink.end() && !found; it2++){
            if ((*(it2))->nodeNo == nodeNO){
                (*(it2))->Actor->SetPosition(worldCord[0], worldCord[1], worldCord[2]);
                found = true;
            }
        }
    }
    disp3dinteractor *style1 = dynamic_cast<disp3dinteractor *>(iren->GetInteractorStyle());
    style1->refreshPickPointText();
    renderWindow->Render();
}

void disp3dmodel::ExecutePipeline()
{
    //初始化标量数据
    scalars_dispx = vtkFloatArray::New();
    scalars_dispy = vtkFloatArray::New();
    scalars_dispz = vtkFloatArray::New();
    scalars_strain = vtkFloatArray::New();
    scalars_temp = vtkFloatArray::New();

    scalars_dispx->SetName("x方向变形");
    scalars_dispy->SetName("y方向变形");
    scalars_dispz->SetName("z方向变形");
    scalars_strain->SetName("应变");
    scalars_temp->SetName("温度");

    for (int i = 0; i < S_M_points->GetNumberOfPoints(); ++i)
    {
        scalars_strain->InsertNextValue(0.1);
        scalars_temp->InsertNextValue(0.1);
        scalars_dispx->InsertNextValue(0);
        scalars_dispy->InsertNextValue(0);
        scalars_dispz->InsertNextValue(0);
    }
    S_M_cubeGrid->GetPointData()->AddArray(scalars_strain);
    S_M_cubeGrid->GetPointData()->AddArray(scalars_temp);
    S_M_cubeGrid->GetPointData()->AddArray(scalars_dispx);
    S_M_cubeGrid->GetPointData()->AddArray(scalars_dispy);
    S_M_cubeGrid->GetPointData()->AddArray(scalars_dispz);
    vtkSmartPointer<vtkLookupTable> pColorTable = vtkSmartPointer<vtkLookupTable>::New();

    //设置颜色表中的颜色
    pColorTable->SetNumberOfColors(256);
    pColorTable->SetHueRange(0.6, 0.1);
    pColorTable->Build();

    GridMapper = vtkDataSetMapper::New();
    GridActor = vtkActor::New();

    GridMapper->SetInputData(S_M_cubeGrid);
    GridMapper->SetLookupTable(pColorTable);
    GridActor->SetMapper(GridMapper);
    renderer->AddActor(GridActor);

    Flag_Actor = true;//机翼存在
    /*设置显示对象的属性*/
    GridActor->GetProperty()->SetEdgeVisibility(0);
    GridActor->GetProperty()->SetLineWidth(0.02);
    GridActor->GetProperty()->SetEdgeColor(0, 0.88, 0.88);
    GridActor->GetProperty()->SetRepresentationToSurface();/*设置显示对象的以面的方式显示，这一步很关键*/
    GridActor->GetProperty()->SetColor(0.16, 0.582, 0.973);
}

void disp3dmodel::AxesShow(bool flag)
{
    axe->SetVisibility(flag);           //true为可见，false为不可见
    renderWindow->Render();
}

float disp3dmodel::GetSphereSize()
{
    return SphereSize;
}

void disp3dmodel::Resetcamera(int DisplayFalg)
{
    double *x = this->GridActor->GetCenter();
    vtkCamera* camra = renderer->GetActiveCamera();
    camra->SetClippingRange(0.1, 2000);//设置裁剪范围
    //新加的
    if (DisplayFalg == 1){
        // 设置相机在网格中心点附近，相机看向网格中心，视角平行于 Y 轴
        camra->SetFocalPoint(x[0], x[1], x[2]);
        camra->SetPosition(x[0] , x[1]-4, x[2]);
        camra->SetViewUp(0, 0, 1);
    }
    else if (DisplayFalg == 2){
        // 设置相机在网格中心点附近，相机看向网格中心，视角平行于 Z 轴
        camra->SetFocalPoint(x[0], x[1], x[2]);
        camra->SetPosition(x[0]-4, x[1] , x[2]);
        camra->SetViewUp(0, 0, 1);

    }
    else if (DisplayFalg == 3){
        // 设置相机在网格中心点附近，相机看向网格中心，视角平行于 X 轴
        camra->SetFocalPoint(x[0], x[1], x[2]);
        camra->SetPosition(x[0], x[1]+4, x[2] );
        camra->ComputeViewPlaneNormal();
        camra->SetViewUp(0, 0, 1);
    }
    else if (DisplayFalg == 4){
        // 设置相机在网格中心点附近，相机看向网格中心，视角平行于负 X 轴
        camra->SetFocalPoint(x[0], x[1], x[2]);
        camra->SetPosition(x[0] + 4, x[1], x[2]);
        camra->SetViewUp(0, 0, 1);
    }
    else  if (DisplayFalg == 5){
        // 设置相机在网格中心点附近，相机看向网格中心，视角平行于负 Z 轴
        camra->SetFocalPoint(x[0], x[1], x[2]);
        camra->SetPosition(x[0], x[1], x[2] - 4);
        camra->SetViewUp(0, 1, 0);
    }
    else if (DisplayFalg == 6){
        // 设置相机在网格中心点附近，相机看向网格中心，视角平行于负 Y 轴
        camra->SetFocalPoint(x[0], x[1], x[2]);
        camra->SetPosition(x[0], x[1] , x[2]+4);
        camra->SetViewUp(0, 1, 0);
    }
    else if (DisplayFalg == 7){
        // 设置相机在网格中心点附近，相机看向网格中心，视角是正轴测图（45 度俯视）
        camra->SetFocalPoint(x[0], x[1], x[2]);
        camra->SetPosition(x[0], x[1], x[2] + 4);
        camra->SetViewUp(0, 1, 0);
        camra->Elevation(-60);
    }
}

void disp3dmodel::LineShowPipeline(bool flag)
{
    GridActor->GetProperty()->SetEdgeVisibility(flag);
    renderWindow->Render();
}

void disp3dmodel::SetScalarMapping(disp3dmodel::ScalarTable &scalarindex, double range)
{
    if(scalarindex == disp3dmodel::dispx)
        S_M_cubeGrid->GetPointData()->SetScalars(scalars_dispx);
    else if(scalarindex == disp3dmodel::dispy)
        S_M_cubeGrid->GetPointData()->SetScalars(scalars_dispy);
    else if(scalarindex == disp3dmodel::dispz)
        S_M_cubeGrid->GetPointData()->SetScalars(scalars_dispz);
    else
        qDebug()<<"Error: Scalar array not found at index"<<scalarindex<<"\n";

    GridMapper->SetScalarRange(0,range);
    renderWindow->Render();
}

void disp3dmodel::refreshFlagPointTextPoisiton()
{
    double worldCord[3];
    double screenCord[3];
    QList<TextActor*>::iterator t;
    for (t = TextActorLink.begin(); t != TextActorLink.end(); t++)
    {
        //结点编号
        int nodeNO = (*t)->nodeNo;
        worldCord[0] = new_node_info1[nodeNO]->x_cord+ Displacefactor*PointDisplaceSet[nodeNO]->xdisplace;
        worldCord[1] = new_node_info1[nodeNO]->y_cord+ Displacefactor*PointDisplaceSet[nodeNO]->ydisplace;
        worldCord[2] = new_node_info1[nodeNO]->z_cord+ Displacefactor*PointDisplaceSet[nodeNO]->zdisplace;
        //文本的序列的位置
        //更改当前文本的显示位置
        //两步： 1、先获取当前文本在屏幕上的位置
         this->renderer->SetWorldPoint(worldCord[0], worldCord[1], worldCord[2], 1.0);
         this->renderer->WorldToDisplay();
         this->renderer->GetDisplayPoint(screenCord);
        //       2、更新文本在屏幕上的位置
        (*t)->Actor->SetDisplayPosition(screenCord[0], screenCord[1]);
    }
}

void disp3dmodel::ModifySphereSize(float size)
{
    SphereSize = size;
    QList <CordDisplayFlag*> ::iterator it;
    for (it = DisplayTextNodeFlagLink.begin(); it != DisplayTextNodeFlagLink.end() ;it++){
        (*it)->Actor->SetScale(static_cast<double>(SphereSize));
    }
    renderWindow->Render();//刷新显示
}

void disp3dmodel::Addsphere(vtkActor *actor, int Node)
{
    vtkSmartPointer<vtkRendererCollection> renderers = renderWindow->GetRenderers();
    vtkSmartPointer<vtkRenderer> renderer = vtkRenderer::SafeDownCast(renderers->GetItemAsObject(0));

    CordDisplayFlag* CordDisPlayflag = new CordDisplayFlag; //创建重要点显示标记对象的结构体
    CordDisPlayflag->nodeNo = Node;
    CordDisPlayflag->Actor = actor;
    DisplayTextNodeFlagLink.push_back(CordDisPlayflag); //将创建的重要点显示标记对象的结构体对象添加到重要点标记集合

    renderer->AddActor(actor);
    renderWindow->Render();
}

void disp3dmodel::DeleteDisplayCordText(int nodeNO)
{
    vtkSmartPointer<vtkRendererCollection> renderers = renderWindow->GetRenderers();
    vtkSmartPointer<vtkRenderer> renderer = vtkRenderer::SafeDownCast(renderers->GetItemAsObject(0));

    bool found = false;
    QList<TextActor*> ::iterator it;
    for (it = TextActorLink.begin(); it != TextActorLink.end() && !found;){
        if ((*it)->nodeNo == nodeNO){
            //从图形上删除
            renderer->RemoveActor((*it)->Actor);//从图形显示窗口中删除重要点位移文本
            //从文本列表中内存结构中删除
            it = TextActorLink.erase(it);//从位移显示文本集合中删除重要点为位移文本
            found = true;
        }
        else{
            it++;
        }
    }
    renderWindow->Render();//刷新显示
}

void disp3dmodel::Deletesphere(int Node)
{
    vtkSmartPointer<vtkRendererCollection> renderers = renderWindow->GetRenderers();
    vtkSmartPointer<vtkRenderer> renderer = vtkRenderer::SafeDownCast(renderers->GetItemAsObject(0));

    int found = false;
    QList <CordDisplayFlag*> ::iterator it;
    for (it = DisplayTextNodeFlagLink.begin(); it != DisplayTextNodeFlagLink.end() && !found;){
        if ((*(it))->nodeNo == Node){
            renderer->RemoveActor((*(it))->Actor);//从显示窗口中删除重要点标记
            found = true;
            it = DisplayTextNodeFlagLink.erase(it);//从重要点显示标记集合中删除要删除的重要点标记
        }
        else{
            it++;
        }
    }
    renderWindow->Render();//刷新显示
}

void disp3dmodel::EditDisplayCordText(int BFnodeNo, int AFnodeNo, double x_cord, double y_cord, double z_cord)
{
    vtkSmartPointer<vtkRendererCollection> renderers =
            renderWindow->GetRenderers();
    vtkSmartPointer<vtkRenderer> renderer = vtkRenderer::SafeDownCast(renderers->GetItemAsObject(0));

    double WorldCord[3];//世界坐标系
    double DisplayCord[3];//显示坐标系
    char DisplayCordText[100] = { '\0' };
    WorldCord[0] = x_cord;
    WorldCord[1] = y_cord;
    WorldCord[2] = z_cord;
    WorldCordToDisplayCord(WorldCord, DisplayCord);//世界坐标系转化为屏幕坐标系

    CordToCordCHAR(PointDisplaceSet[AFnodeNo]->xdisplace, PointDisplaceSet[AFnodeNo]->ydisplace, PointDisplaceSet[AFnodeNo]->zdisplace, DisplayCordText);
    //生成显示文本的VTK对象
    QList<TextActor*> ::iterator it;
    bool found = false;
    for (it = TextActorLink.begin(); it != TextActorLink.end() && !found;){
        if ((*it)->nodeNo == BFnodeNo){
            (*(it))->nodeNo = AFnodeNo;
            (*(it))->Actor->SetInput(DisplayCordText);//重新设置显示文本
            (*(it))->Actor->SetDisplayPosition(DisplayCord[0], DisplayCord[1]);//设置文本的位置
            (*(it))->Actor->Modified();
            found = true;
        }
        else{
                   it++;
        }
    }
}

void disp3dmodel::EditDisplayCordFlagInfo(int BFnodeNO, int AFnodeNo, double x_cord, double y_cord, double z_cord)
{
    Deletesphere(BFnodeNO);//删除之前重要点显示标记
    vtkActor *actor;
    actor = CreatesphereSource(x_cord, y_cord, z_cord);//重新创建显示点标记
    Addsphere(actor, AFnodeNo);//添加重要点显示标记
    renderWindow->Render();//刷新显示
}

void disp3dmodel::createDisplayPointDisplaceText(int nodeNo, double x_cord, double y_cord, double z_cord)
{
    double WorldCord[3];        //世界坐标系
    double DisplayCord[3];      //屏幕坐标系
    char DisplayCordText[100] = { '\0' };
    WorldCord[0] = x_cord;
    WorldCord[1] = y_cord;
    WorldCord[2] = z_cord;
    double x_displace;
    double y_displace;
    double z_displace;
    x_displace = PointDisplaceSet[nodeNo]->xdisplace;   //编号为nodeNO的x向位移
    y_displace = PointDisplaceSet[nodeNo]->ydisplace;   //编号为nodeNO的y向位移
    z_displace = PointDisplaceSet[nodeNo]->zdisplace;   //编号为nodeNO的z向位移
    WorldCordToDisplayCord(WorldCord, DisplayCord);     //世界坐标系到屏幕坐标系
    CordToCordCHAR(x_displace, y_displace, z_displace, DisplayCordText);//产生显示文本

    vtkTextActor* temp = CreateCordTextobject(DisplayCordText, DisplayCord[0], DisplayCord[1]);	//生成显示位移的VTK对象

    TextActor * textactor = new TextActor;
    textactor->nodeNo = nodeNo;
    textactor->Actor = temp;
    TextActorLink.push_back(textactor);

    renderer->AddActor(temp);
    renderWindow->Render();
}

vtkActor *disp3dmodel::CreatesphereSource(double x, double y, double z)
{
    vtkSphereSource* sphereSource = vtkSphereSource::New();             //创建vtkSphereSource对象
    sphereSource->Update();                                             //请求数据
    vtkPolyDataMapper* sphereMapper = vtkPolyDataMapper::New();         //创建映射器对象
    sphereMapper->SetInputConnection(sphereSource->GetOutputPort());    //输入vtkSphereSource对象数据
    vtkActor* sphereActor = vtkActor::New();                            //创建小球对象
    sphereActor->SetMapper(sphereMapper);
    sphereActor->SetPosition(x, y, z);                                  //设置小球的位置为世界坐标
    sphereActor->GetProperty()->SetColor(1, 0, 0);                      //设置小球对象为红色
    sphereActor->SetScale(static_cast<double>(SphereSize));                                  //设置小球的大小
    return sphereActor;                                                 //返回红色小球对象
}

void disp3dmodel::CordToCordCHAR(double x, double y, double z, char *str)   //将坐标信息转换为文本
{
    for (int j = 0; j <100; j++)
        str[j] = '\0'; //初始化字符串数组str
    double cord[3] = {x, y, z};
    char string[100];
    int count = 0;
    str[0] = '(';
    for (int i = 0; i < 3; i++){
        for (int j = 0; j <100; j++)
            string[j] = '\0';
        sprintf(string, "%.3lf", cord[i] * 1000); //将位移转化为字符串，位移单位为毫米
        strcat_s(str, 100, string);
        count = static_cast<int>(strlen(str));
        if (i < 2)
            str[count] = ',';
        else
            str[count] = ')';
    }
}

double *disp3dmodel::caculatevectorji(double x1, double y1, double z1, double x2, double y2, double z2)
{
    double x = y1*z2 - y2*z1;
    double y = x2*z1 - x1*z2;
    double z = x1*y2 - x2*y1;
    double * vector = new double[3];
    vector[0] = x / sqrt(x*x + y*y + z*z);
    vector[1] = y / sqrt(x*x + y*y + z*z);
    vector[2] = z / sqrt(x*x + y*y + z*z);
    return vector;
}

babianxingpoint disp3dmodel::Getzhengliubianxing(double r)
{
    double(*babianxing)[3] = new double[6][3];
    for (int i = 0; i <6; i++){
        babianxing[i][0] = 0;
        babianxing[i][1] = r*cos((i * 60)*PI / 180);
        babianxing[i][2] = r*sin((i * 60)*PI / 180);
    }
    return  babianxing;
}

double *disp3dmodel::tran_cord(double tran_Max[4][4], double l_cord[])
{
    double * g_c = new double[3];
    for (int i = 0; i < 3; i++){
        g_c[i] = tran_Max[3][i] + tran_Max[0][i] * l_cord[0] + tran_Max[1][i] * l_cord[1] + tran_Max[2][i] * l_cord[2];
    }
    return g_c;
}

void disp3dmodel::WorldCordToDisplayCord(double *WorldCord, double *DisplayCord)
{
    renderer->SetWorldPoint(WorldCord[0], WorldCord[1], WorldCord[2], 1.0);
    renderer->WorldToDisplay();
    renderer->GetDisplayPoint(DisplayCord);
}

vtkTextActor *disp3dmodel::CreateCordTextobject(const char *CordInfo, double x, double y)
{
    vtkTextActor *temp = vtkTextActor::New();
    temp->SetDisplayPosition(static_cast<int>(x), static_cast<int>(y));
    temp->SetInput(CordInfo);
    temp->GetTextProperty()->SetFontSize(18);
    temp->GetTextProperty()->SetColor(0, 1, 1);
    return temp;
}

void disp3dmodel::PickerGetQLabelPtr(std::tuple<QLabel *, QLabel *, QLabel *, QLabel *>labels)
{
    disp3dinteractor * style1 = dynamic_cast<disp3dinteractor *>(iren->GetInteractorStyle());
    style1->label0 = std::get<0>(labels);
    style1->label1 = std::get<1>(labels);
    style1->label2 = std::get<2>(labels);
    style1->label3 = std::get<3>(labels);
}

void disp3dmodel::CameraLock(bool index)
{
    disp3dinteractor * style1 = dynamic_cast<disp3dinteractor *>(iren->GetInteractorStyle());
    style1->Flag_CameraLock = index;
}

void disp3dmodel::CameraFellow(bool index)
{
    disp3dinteractor * style1 = dynamic_cast<disp3dinteractor *>(iren->GetInteractorStyle());
    style1->Flag_CameraFellow = index;
}
