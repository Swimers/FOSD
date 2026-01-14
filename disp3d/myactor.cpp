#include "myactor.h"


#include <QFile>
#include <QDebug>

#pragma execution_character_set("utf-8")


MyActor::MyActor()
{

}

void MyActor::CreatePointSet()
{
    points->SetNumberOfPoints(node_num);
    int count=0;
    for(int i=1;i<=node_num;i++)
    {
//        qDebug()<<"nodesize:"<<nodes.size()<<(*nodes[i]).cord[0];
        points->SetPoint(count++,(*nodes[i]).cord[0],(*nodes[i]).cord[1],(*nodes[i]).cord[2]);
    }
}
void MyActor::CreateCellSet()
{

}


bool MyActor::readdata(QString path)
{
    if(path.isEmpty()==false)   //path内容不为空
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))  //打开成功
        {
            while(!file.atEnd())
            {
                QString s=file.readLine().simplified();   //readline读取一行；simplified去除首尾空格，中间多个空格变为1个
                QString value=s.section(" ",0,0,QString::SectionSkipEmpty).toLower();
                QString line;
                QStringList list;
                if(value == "node")     //读取节点坐标
                {
                    QString node_n=s.section(" ",1,1,QString::SectionSkipEmpty);
                    QString serial_node;
                    QString location;
                    int i=0;
                    node_num = node_n.toInt();  //node_num为总的节点数
                    while(i<node_num)
                    {
                        if(!file.atEnd())
                        {
                            node *point=new node;
                            line = file.readLine().simplified();
                            serial_node=line.section(" ",0,0,QString::SectionSkipEmpty);
                            (*point).NodeNo=serial_node.toInt();    //这个NodeNo就是节点编号
                            double * cord = new double[3];//定义存储点坐标信息的数组
                            for (int j = 0; j < 3; j++)
                            {
                                location=line.section(" ",j+1,j+1,QString::SectionSkipEmpty);

                                cord[j]=location.toDouble();
                                cord[j] /= 1000.0; //直接从文件中读取的点的坐标信息为毫米,读取之后点的坐标信息为米。
                            }
                            (*point).cord = cord; //存储节点坐标
                            nodes[(*point).NodeNo]=point;
                            i++;
                        }
                    }
                }
                else if(value == "element")
                {
                    QString element_n = s.section(" ",1,1,QString::SectionSkipEmpty);
                    element_num=element_n.toInt();
                }
                else if(value == "shell")   //壳单元
                {
                    QString shell_n=s.section(" ",1,1,QString::SectionSkipEmpty);
                    QString serial_shell;
                    shell_num=shell_n.toInt();
                    for(int i=0;i<shell_num;i++)
                    {
                        shell *shell1=new shell;
                        line=file.readLine().simplified();
                        serial_shell=line.section(" ",0,0,QString::SectionSkipEmpty);
                        (*shell1).ElementNo=serial_shell.toInt();
                        serial_shell=line.section(" ",1,1,QString::SectionSkipEmpty);
                        (*shell1).ElementType=serial_shell.toInt();
                        if (!((*shell1).ElementType - 2)) //判断当前单元是不是三角形单元
                        {
                            (*shell1).NodeNo = new int[3];
                            for (int j = 0; j < 3; j++)
                            {
                                serial_shell=line.section(" ",2+j,2+j,QString::SectionSkipEmpty).trimmed();
                                (*shell1).NodeNo[j]=serial_shell.toInt();
                            }
                            triangle_number++;
                        }
                        else        //判断当前单元是不是四边形单元
                        {
                            (*shell1).NodeNo = new int[4];
                            for (int j = 0; j < 4; j++)
                            {
                                serial_shell=line.section(" ",2+j,2+j,QString::SectionSkipEmpty).trimmed();
                                (*shell1).NodeNo[j]=serial_shell.toInt();
                            }
                            quad_number++;
                        }
                        serial_shell=line.section(" ",3+(*shell1).ElementType,3+(*shell1).ElementType,QString::SectionSkipEmpty);
                        (*shell1).ElementThick=serial_shell.toDouble();
                        shells[(*shell1).ElementNo]=shell1;
                    }
                }
                else if(value == "beam")    //梁单元
                {
                    QString beam_n=s.section(" ",1,1,QString::SectionSkipEmpty);
                    beam_number=beam_n.toInt();
                    QString serial_beam;
                    for(int j=0;j<beam_number;j++)
                    {
                        beam *beam1 = new beam;
                        line=file.readLine().simplified();
                        serial_beam=line.section(" ",0,0,QString::SectionSkipEmpty);
                        (*beam1).ElementNo=serial_beam.toInt();
                        (*beam1).NodeNo=new int[2];
                        for(int j=0;j<2;j++)
                        {
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
                else if(value=="section")   //截面
                {
                    QString section_n=s.section(" ",1,1,QString::SectionSkipEmpty);
                    section_number=section_n.toInt();
                    QString serial_section;
                    for(int j=0;j<section_number;j++)
                    {
                        sectionInfo *sectionInfo1=new sectionInfo;
                        line=file.readLine().simplified();
                        serial_section=line.section(" ",0,0,QString::SectionSkipEmpty);
                        (*sectionInfo1).sectionNo=serial_section.toInt();
                        serial_section=line.section(" ",1,1,QString::SectionSkipEmpty);
                        (*sectionInfo1).sectionshape=serial_section.toInt();
                        if((*sectionInfo1).sectionshape==1)
                        {
                            (*sectionInfo1).sectionGEo=new double;
                            serial_section=line.section(" ",2,2,QString::SectionSkipEmpty);
                            (*sectionInfo1).sectionGEo[0]=serial_section.toDouble();
                        }
                        else
                        {
                            (*sectionInfo1).sectionGEo=new double[2];
                            for(int j=0;j<2;j++)
                            {
                                serial_section=line.section(" ",2+j,2+j,QString::SectionSkipEmpty);
                                (*sectionInfo1).sectionGEo[j]=serial_section.toDouble();
                            }
                        }
                        sectionInfos[(*sectionInfo1).sectionNo]=sectionInfo1;
                    }
                }
                else if (value=="flagpoint")
                {
                    QString flag_n=s.section(" ",1,1,QString::SectionSkipEmpty);
                    flag_number=flag_n.toInt();
                    QString serial_flagpoint;
                    line=file.readLine().simplified();
                    int len;
                    len=line.count(" ")+1;
                    int j=0;
                    while(j<flag_number)
                    {
                        for(int i=0;i<len&&j<flag_number;i++)
                        {
                            serial_flagpoint=line.section(" ",i,i,QString::SectionSkipEmpty);
                            LineModeFlagpointToNodePoint[j+1]=serial_flagpoint.toInt();
                            ++j;
                        }

                        if(j<flag_number)
                        {
                            line=file.readLine().simplified();
                            len=line.count(" ")+1;
                        }
                    }
                }
                else if(value=="reversibleele")
                {
                    QString reversibleelenum; //逆有限单元个数
                    QString serial_reversible;
                    int reversibleeleNO; //逆有限单元编号
                    int elepointnum;//逆有限单元
                    reversibleelenum=s.section(" ",1,1,QString::SectionSkipEmpty).simplified();
                    reversibleele_num=reversibleelenum.toInt();
                    for(int i=0;i<reversibleele_num;i++)
                    {
                        line=file.readLine().simplified();
                        serial_reversible=line.section(" ",0,0,QString::SectionSkipEmpty).trimmed();
                        reversibleeleNO=serial_reversible.toInt();
                        serial_reversible=line.section(" ",1,1,QString::SectionSkipEmpty).trimmed();
                        elepointnum=serial_reversible.toInt();
                        ReversibleElementPointSet *PointSet = new  ReversibleElementPointSet;
                        QVector <int> * pointlink = new QVector <int>;
                        (*pointlink).resize(elepointnum);
                        PointSet->elementNo = reversibleeleNO;
                        PointSet->DvaluePointNum = elepointnum;
                        PointSet->Dvaluepoint = pointlink;
                        line=file.readLine().simplified();
                        int len;
                        len=line.count(" ")+1;
                        int j=0;
                        while(j<elepointnum)
                        {
                            for(int i=0;i<len&&j<elepointnum;i++)
                            {
                                serial_reversible=line.section(" ",i,i,QString::SectionSkipEmpty);
                                (*pointlink)[j]=serial_reversible.toInt();
                                ++j;
                            }
                            if(j<elepointnum)
                            {
                                line=file.readLine().simplified();
                                len=line.count(" ")+1;
                            }
                        }
                        ReversibleElementDvaluepointSet[reversibleeleNO] = PointSet; //存储分配给每个逆有限单元的点的个数
                    }
                }
                else if(value=="reversibleelementset")
                {
                    QString serial_reverEle;
                    serial_reverEle=s.section(" ",1,1,QString::SectionSkipEmpty);
                    int reversibleele_numshell=serial_reverEle.toInt();
                    for(int j=0;j<reversibleele_numshell;j++)
                    {
                        ReversibleElement *reverele = new ReversibleElement; //存储逆有限单元坐标系
                        line=file.readLine().simplified();
                        serial_reverEle=line.section(" ",0,0,QString::SectionSkipEmpty);
                        reverele->eleNO=serial_reverEle.toInt();
                        reverele->ipoint=new double[3];
                        reverele->jpoint=new double[3];
                        reverele->kpoint=new double[3];
                        for(int i=0;i<9;i++)
                        {
                            serial_reverEle=line.section(" ",i+1,i+1,QString::SectionSkipEmpty);
                            if(i<3)
                            {
                                reverele->ipoint[i]=serial_reverEle.toDouble();
                            }
                            else if(i>=3&&i<6)
                            {
                                reverele->jpoint[i%3]=serial_reverEle.toDouble();
                            }
                            else if(i>=6&&i<9)
                            {
                                reverele->kpoint[i%3]=serial_reverEle.toDouble();
                            }
                        }
                        ReversibleElementSet[reverele->eleNO]=reverele;
                    }
                }
                else if(value=="reversiblelen")
                {
                    int shell_ReverNum;
                    QString serial_ReversibleLen;
                    serial_ReversibleLen=s.section(" ",1,1,QString::SectionSkipEmpty);
                    int shell_ReversibleNum=serial_ReversibleLen.toInt();//逆有限元板的个数
                    for(int i=0;i<shell_ReversibleNum;i++)
                    {
                        shell_len *shell_len1=new shell_len;
                        line=file.readLine().simplified();
                        serial_ReversibleLen=line.section(" ",0,0,QString::SectionSkipEmpty);
                        shell_ReverNum=serial_ReversibleLen.toInt();
                        serial_ReversibleLen=line.section(" ",1,1,QString::SectionSkipEmpty);
                        (*shell_len1).length=serial_ReversibleLen.toDouble();
                        serial_ReversibleLen=line.section(" ",2,2,QString::SectionSkipEmpty);
                        (*shell_len1).width=serial_ReversibleLen.toDouble();
                        ReversibleLen[shell_ReverNum]=shell_len1;
                    }
                }
                else if(value=="sensorandinserverrelationset")
                {
                    QString serial_sensor;
                    int channalnum;
                    serial_sensor=s.section(" ",1,1,QString::SectionSkipEmpty);
                    channalnum=serial_sensor.toInt(); //通道个数
                    int channalNo; //通道编号
                    int channalbitnum; //通道位数
                    int channalbit;
                    qDebug()<<"111";
                    for (int i = 0; i < channalnum; i++)
                    {
                        line=file.readLine().simplified();
                        serial_sensor=line.section(" ",0,0,QString::SectionSkipEmpty).trimmed();
                        channalNo=serial_sensor.toInt();
                        serial_sensor=line.section(" ",1,1,QString::SectionSkipEmpty).trimmed();
                        channalbitnum=serial_sensor.toInt();
                        for (int j = 0; j < channalbitnum; j++)
                        {
                            serial_sensor=line.section(" ",2+j,2+j,QString::SectionSkipEmpty);
                            channalbit=serial_sensor.toInt();
                            ChannalEle * chanele = new ChannalEle;
                            chanele->ChannalNO = channalNo;
                            chanele->Channalbit = j;
                            SensorANDInserverRelationSet[channalbit] = chanele;//下标索引从0开始
                        }
                        QVector<double>* currentstrain = new QVector<double>;
                        if (strainset[channalNo]!=NULL)
                        {
                            delete strainset[channalNo];
                        }
                        currentstrain->resize(channalbitnum);
                        strainset[channalNo] = currentstrain; //初始化存储应变的数据的集合
                        //delete currentstrain;     我觉得这里不应该删除内存
                        currentstrain=nullptr;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

MyActor::~MyActor()
{
    //释放点的信息集合
    for(QMap<int,node*>::iterator it=nodes.begin();it!=nodes.end();)
    {
        delete (it.value())->cord;
        delete it.value();
        it=nodes.erase(it);
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
}
